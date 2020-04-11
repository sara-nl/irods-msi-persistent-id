/*****************************************************************
Copyright 2018-2019, SURFsara
Author Stefan Wolfsheimer


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
********************************************************************/
#pragma once
#include "libmsi_pid_common.h"
#include <surfsara/handle_result.h>
#include <surfsara/handle_permissions.h>
#include <functional>
#include <memory>

typedef std::vector<std::pair<std::string, std::string>> KeyValuePairs;

using GetterFunction =
  std::function<surfsara::handle::Result(std::shared_ptr<surfsara::handle::IRodsHandleClient> client,
                                         const std::string & path)>;

using UnsetterFunction =
  std::function<surfsara::handle::Result(std::shared_ptr<surfsara::handle::IRodsHandleClient> client,
                                         const std::string & path,
                                         const std::vector<std::string> & key)>;

using SetterFunction =
  std::function<surfsara::handle::Result(std::shared_ptr<surfsara::handle::IRodsHandleClient> client,
                                         const std::string & path,
                                         const std::vector<std::pair<std::string, std::string>> & kvp)>;

using Permissions = surfsara::handle::Permissions;

/**
 * Checks the permission for current user / group against permission object.
 */
inline bool checkPermissions(std::shared_ptr<Permissions> perm,
                             ruleExecInfo_t* rei,
                             const char * path = nullptr,
                             const std::string & path_based_perm = "read");

inline int msiPidGetAction(GetterFunction getter,
                           msParam_t* _inPath,
                           msParam_t* _inType,
                           msParam_t* _outValue,
                           ruleExecInfo_t* rei,
                           bool isRodsPath);

inline int msiPidSetAction(SetterFunction setter,
                           msParam_t* _inStr,
                           msParam_t* _inKey,
                           msParam_t* _inValue,
                           msParam_t* _outHandle,
                           ruleExecInfo_t* rei,
                           bool isRodsPath);

inline int msiPidUnsetAction(UnsetterFunction unsetter,
                             msParam_t* _inStr, /* iRODS path or handle */
                             msParam_t* _inKey,
                             msParam_t* _outHandle,
                             ruleExecInfo_t* rei,
                             bool isRodsPath);

////////////////////////////////////////////////////////////////////////////////
//
// Implemenation
//
////////////////////////////////////////////////////////////////////////////////

// helpers
static std::string joinKeys(const KeyValuePairs & kvPairs)
{
  std::string ret;
  for(const auto & p : kvPairs)
  {
    if(!ret.empty())
    {
      ret.push_back(',');
    }
    ret += p.first;
  }
  return ret;
}

static KeyValuePairs parseInputArray(msParam_t* _inKey)
{
  strArray_t *strArr = (strArray_t *) _inKey->inOutStruct;
  if(strArr->len == 0 || (strArr->len % 2) != 0)
  {
    throw std::logic_error("input array must not be empty or "
                           "contain uneven number of elements");
  }
  KeyValuePairs ret;
  const char *val = strArr->value;
  int i = 0;
  while(i < strArr->len)
  {
    ret.push_back(std::make_pair(std::string(&val[i * strArr->size]),
                                 std::string(&val[(i + 1) * strArr->size])));
    i += 2;
  }
  return ret;
}

static KeyValuePairs parseInputParameters(msParam_t* _inKey, msParam_t* _inValue)
{
  if (_inKey == NULL || _inKey->type == NULL)
  {
    throw std::logic_error("invalid argument key");
  }
  else
  {
    if(strcmp(_inKey->type, STR_MS_T) == 0)
    {
      // key is string and value is string
      if (_inValue == NULL || _inValue->type == NULL || strcmp(_inValue->type, STR_MS_T) != 0)
      {
        throw std::logic_error("invalid argument value");
      }
      return KeyValuePairs({{
            std::string((char*)(_inKey->inOutStruct)),
            std::string((char*)(_inValue->inOutStruct))}});
    }
    else if(strcmp(_inKey->type, StrArray_MS_T) == 0)
    {
      return parseInputArray(_inKey);
    }
    else
    {
      throw std::logic_error("invalid argument type: key");
    }
  }
}


////////////////////////////////////////////////////////////////////////////////
//
// implmentation
//
////////////////////////////////////////////////////////////////////////////////
inline void addQueryCondition(genQueryInp_t * genQueryInp,
                              int field,
                              const char * value)
{
  char condstr[MAX_NAME_LEN];
  snprintf(condstr, MAX_NAME_LEN, "= '%s'", value);
  addInxVal(&genQueryInp->sqlCondInp, field, condstr);
}

inline void initQuery(genQueryInp_t * genQueryInp,
                      const std::vector<int> & fields)
{
  memset(genQueryInp, 0, sizeof(genQueryInp_t));
  genQueryInp->maxRows = MAX_SQL_ROWS;
  for(auto f : fields)
  {
    addInxIval(&genQueryInp->selectInp, f, 1 );
  }
}

/////////////////////////////////////
// get user ids of current users
// including groups
/////////////////////////////////////
inline std::set<int> getUserIds(ruleExecInfo_t* rei)
{
  std::set<int> res;
  genQueryInp_t genQueryInp;
  genQueryOut_t *genQueryOut = NULL;
  const char * zone = rei->rsComm->clientUser.rodsZone;
  const char * user = rei->rsComm->clientUser.userName;
  initQuery(&genQueryInp, {COL_USER_GROUP_ID});
  addQueryCondition(&genQueryInp, COL_USER_NAME, user);
  addQueryCondition(&genQueryInp, COL_USER_ZONE, zone);
  int status = rsGenQuery( rei->rsComm, &genQueryInp, &genQueryOut );
  if(genQueryOut && status >= 0)
  {
    sqlResult_t *group_coll_id = getSqlResultByInx(genQueryOut, COL_USER_GROUP_ID);
    for(int j = 0; j < genQueryOut->rowCnt; j++)
    {
      res.insert(std::stoi(&group_coll_id->value[group_coll_id->len * j]));
    }
  }
  freeGenQueryOut( &genQueryOut );
  clearGenQueryInp( &genQueryInp );
  return res;
}

inline void getAccess(ruleExecInfo_t* rei,
                      const std::string & id,
                      std::set<std::string> & acl,
                      const std::set<int> & userIds)
{
  genQueryInp_t genQueryInp;
  genQueryOut_t *genQueryOut = NULL;
  initQuery(&genQueryInp, {COL_DATA_ACCESS_NAME,
                           COL_DATA_ACCESS_USER_ID});
  addQueryCondition(&genQueryInp, COL_DATA_ACCESS_DATA_ID, id.c_str());
  int status = rsGenQuery( rei->rsComm, &genQueryInp, &genQueryOut );
  if(genQueryOut && status >= 0)
  {
    sqlResult_t* group_access_name = getSqlResultByInx(genQueryOut, COL_DATA_ACCESS_NAME);
    sqlResult_t* group_access_user_id = getSqlResultByInx(genQueryOut, COL_DATA_ACCESS_USER_ID);
    for(int j = 0; j < genQueryOut->rowCnt; j++)
    {
      int userId = std::stoi(&group_access_user_id->value[group_access_user_id->len * j]);
      if(userIds.find(userId) != userIds.end())
      {
        std::string accName(&group_access_name->value[group_access_name->len * j]);
        if(accName == "modify object") accName = "write";
        else if(accName == "read object") accName = "read";
        acl.insert(accName);
      }
    }
  }
  freeGenQueryOut( &genQueryOut );
  clearGenQueryInp( &genQueryInp );
}

inline bool getAclOfCollection(ruleExecInfo_t* rei,
                               const std::string & _path,
                               std::set<std::string> & acl,
                               const std::set<int> & userIds)
{
  genQueryInp_t genQueryInp;
  genQueryOut_t *genQueryOut = NULL;
  std::string zone(rei->rsComm->clientUser.rodsZone);
  std::string user(rei->rsComm->clientUser.userName);
  std::string path(_path);
  if(path.size() && path.back() == '/')
  {
    path.erase(path.size() - 1);
  }
  initQuery(&genQueryInp, {COL_COLL_OWNER_NAME,
                           COL_COLL_OWNER_ZONE,
                           COL_COLL_ID});
  addQueryCondition(&genQueryInp, COL_COLL_NAME, path.c_str());
  int status = rsGenQuery( rei->rsComm, &genQueryInp, &genQueryOut );
  std::string collId;
  if(genQueryOut && status >= 0)
  {
    sqlResult_t* group_owner_name = getSqlResultByInx(genQueryOut, COL_COLL_OWNER_NAME);
    sqlResult_t* group_owner_zone = getSqlResultByInx(genQueryOut, COL_COLL_OWNER_ZONE);
    sqlResult_t* group_id = getSqlResultByInx(genQueryOut, COL_COLL_ID);
    for(int j = 0; j < genQueryOut->rowCnt; j++)
    {
      collId = std::string(&group_id->value[group_id->len * j]);
      if(user == &group_owner_name->value[group_owner_name->len * j] &&
         zone == &group_owner_zone->value[group_owner_zone->len * j])
      {
        acl.insert("own");
      }
    }
  }
  freeGenQueryOut( &genQueryOut );
  clearGenQueryInp( &genQueryInp );
  if(collId.empty())
  {
    return false;
  }
  else
  {
    getAccess(rei, collId, acl, userIds);
    return true;
  }
}

inline bool getAclOfObject(ruleExecInfo_t* rei,
                           const std::string & path,
                           std::set<std::string> & acl,
                           const std::set<int> & userIds)
{
  genQueryInp_t genQueryInp;
  genQueryOut_t *genQueryOut = NULL;
  std::string zone(rei->rsComm->clientUser.rodsZone);
  std::string user(rei->rsComm->clientUser.userName);
  std::size_t found = path.find_last_of("/\\");
  std::string collname;
  std::string filename;
  if(found != std::string::npos)
  {
    collname = path.substr(0, found);
    filename = path.substr(found + 1);
  }
  initQuery(&genQueryInp, {COL_D_OWNER_NAME,
                           COL_D_OWNER_ZONE,
                           COL_D_DATA_ID});
  addQueryCondition(&genQueryInp, COL_DATA_NAME, filename.c_str());
  addQueryCondition(&genQueryInp, COL_COLL_NAME, collname.c_str());
  int status = rsGenQuery( rei->rsComm, &genQueryInp, &genQueryOut );
  std::string objectId;
  if(genQueryOut && status >= 0)
  {
    sqlResult_t* group_owner_name = getSqlResultByInx(genQueryOut, COL_D_OWNER_NAME);
    sqlResult_t* group_owner_zone = getSqlResultByInx(genQueryOut, COL_D_OWNER_ZONE);
    sqlResult_t* group_id = getSqlResultByInx(genQueryOut, COL_D_DATA_ID);
    for(int j = 0; j < genQueryOut->rowCnt; j++)
    {
      objectId = std::string(&group_id->value[group_id->len * j]);
      if(user == &group_owner_name->value[group_owner_name->len * j] &&
         zone == &group_owner_zone->value[group_owner_zone->len * j])
      {
        acl.insert("own");
      }
    }
  }
  freeGenQueryOut( &genQueryOut );
  clearGenQueryInp( &genQueryInp );
  if(objectId.empty())
  {
    return false;
  }
  else
  {
    getAccess(rei, objectId, acl, userIds);
    return  true;
  }
}

inline std::set<std::string> getAclOfPath(ruleExecInfo_t* rei, const char * path)
{
  std::set<std::string> acl;
  std::set<int> userIds(getUserIds(rei));
  bool ret = ( getAclOfCollection(rei, path, acl, userIds) ||
               getAclOfObject(rei, path, acl, userIds) );
  if(ret)
  {
    if(acl.find("own") != acl.end() ||
       acl.find("write") != acl.end())
    {
      acl.insert("read");
    }
    if(acl.find("own") != acl.end())
    {
      acl.insert("write");
    }
  }
  return acl;
}

inline bool checkPermissions(std::shared_ptr<Permissions> perm, ruleExecInfo_t* rei,
                             const char * path,
                             const std::string & path_based_perm)
{
  if(perm->checkAny())
  {
    return true;
  }
  else if(perm->checkSome())
  {
    const char * zone = rei->rsComm->clientUser.rodsZone;
    const char * user = rei->rsComm->clientUser.userName;
    if(perm->checkUser(std::string(user) + "#" + std::string(zone)))
    {
      return true;
    }
    genQueryInp_t genQueryInp;
    genQueryOut_t *genQueryOut = NULL;
    memset( &genQueryInp, 0, sizeof( genQueryInp_t ) );
    genQueryInp.maxRows = MAX_SQL_ROWS;
    addInxIval( &genQueryInp.selectInp, COL_USER_GROUP_NAME, 1 );
    char condstr[MAX_NAME_LEN];
    snprintf( condstr, MAX_NAME_LEN, "= '%s'", user);
    addInxVal( &genQueryInp.sqlCondInp, COL_USER_NAME, condstr);
    snprintf( condstr, MAX_NAME_LEN, "= '%s'", zone);
    addInxVal( &genQueryInp.sqlCondInp, COL_USER_ZONE, condstr);
    sqlResult_t *group_sql_result;
    int status = rsGenQuery( rei->rsComm, &genQueryInp, &genQueryOut );
    bool found = false;
    if(genQueryOut && status >= 0)
    {
      group_sql_result = getSqlResultByInx(genQueryOut, COL_USER_GROUP_NAME);
      if (group_sql_result != nullptr)
      {
        char *group_str;
        // iterating over groups
        for(int j = 0; j < genQueryOut->rowCnt; j++)
        {
          group_str = &group_sql_result->value[group_sql_result->len * j];
          if (strcmp(group_str, user) == 0)
          {
            continue;
          }
          if(perm->checkGroup(group_str))
          {
            found = true;
            break;
          }
        }
      }
    }
    freeGenQueryOut( &genQueryOut );
    clearGenQueryInp( &genQueryInp );
    if(!found && path)
    {
      std::set<std::string> acl(getAclOfPath(rei, path));
      found = (acl.find(path_based_perm) != acl.end());
    }
    return found;
  }
  else
  {
    return false;
  }
}

inline int msiPidGetAction(GetterFunction getter,
                           msParam_t* _inStr,
                           msParam_t* _inType,
                           msParam_t* _outValue,
                           ruleExecInfo_t* rei,
                           bool isRodsPath)
{
  using Object = surfsara::ast::Object;
  using String = surfsara::ast::String;
  if (rei == NULL || rei->rsComm == NULL)
  {
    return (SYS_INTERNAL_NULL_INPUT_ERR);
  }
  if (_inStr == NULL || _inStr->type == NULL || strcmp(_inStr->type, STR_MS_T) != 0)
  {
    return (USER_PARAM_TYPE_ERR);
  }
  if (_inType == NULL || _inType->type == NULL || strcmp(_inType->type, STR_MS_T) != 0)
  {
    return (USER_PARAM_TYPE_ERR);
  }
  surfsara::handle::Config cfg;
  try
  {
    cfg.parseJson(IRODS_PID_CONFIG_FILE);
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to read PID config file %s:\n%s", IRODS_PID_CONFIG_FILE, ex.what());
    return FILE_READ_ERR;
  }
  char * pathOrHandle = (char*)(_inStr->inOutStruct);
  if(!checkPermissions(cfg.getReadPermissions(), rei, (isRodsPath ? pathOrHandle : nullptr), "read"))
  {
    rodsLog(LOG_ERROR, "user %s#%s is not allowed to create the handle",
            rei->rsComm->clientUser.userName,
            rei->rsComm->clientUser.rodsZone);
    return MSI_OPERATION_NOT_ALLOWED;
  }

  auto client = cfg.makeIRodsHandleClient();
  std::string inType = std::string((char*)(_inType->inOutStruct));
  try
  {
    auto res = getter(client, pathOrHandle);
    if(res.success)
    {
      if(inType.empty())
      {
        auto jsonString = surfsara::ast::formatJson(res.data, true);
        fillStrInMsParam(_outValue, jsonString.c_str());
      }
      else
      {
        auto result = surfsara::handle::extractValueByType(res.data, inType);
        fillStrInMsParam(_outValue, result.c_str());
      }
      return 0;
    }
    else
    {
      std::stringstream ss;
      ss << res;
      rodsLog(LOG_ERROR, "failed to get details from %s", pathOrHandle);
      rodsLog(LOG_ERROR, "%s", ss.str().c_str());
      return ACTION_FAILED_ERR;
    }
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to get details from %s", pathOrHandle);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}

inline int msiPidSetAction(SetterFunction setter,
                           msParam_t* _inStr,
                           msParam_t* _inKey,
                           msParam_t* _inValue,
                           msParam_t* _outHandle,
                           ruleExecInfo_t* rei,
                           bool isRodsPath)
{
  using Object = surfsara::ast::Object;
  using String = surfsara::ast::String;
  if (rei == NULL || rei->rsComm == NULL)
  {
    return (SYS_INTERNAL_NULL_INPUT_ERR);
  }
  if (_inStr == NULL || _inStr->type == NULL || strcmp(_inStr->type, STR_MS_T) != 0)
  {
    return (USER_PARAM_TYPE_ERR);
  }

  KeyValuePairs kvPairs;
  try
  {
    kvPairs = parseInputParameters(_inKey, _inValue);
  }
  catch(std::exception ex)
  {
    rodsLog(LOG_ERROR, "exception: %s", ex.what());    
    return (USER_PARAM_TYPE_ERR);
  }

  surfsara::handle::Config cfg;
  try
  {
    cfg.parseJson(IRODS_PID_CONFIG_FILE);
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to read PID config file %s:\n%s", IRODS_PID_CONFIG_FILE, ex.what());
    return FILE_READ_ERR;
  }
  char * pathOrHandle = (char*)(_inStr->inOutStruct);
  if(!checkPermissions(cfg.getWritePermissions(), rei, (isRodsPath ? pathOrHandle : nullptr), "write"))
  {
    rodsLog(LOG_ERROR, "user %s#%s is not allowed to modify handle",
            rei->rsComm->clientUser.userName,
            rei->rsComm->clientUser.rodsZone);
    return MSI_OPERATION_NOT_ALLOWED;
  }
  try
  {
    auto client = cfg.makeIRodsHandleClient();
    auto res = setter(client, pathOrHandle, kvPairs);
    if(res.success)
    {
      if(res.data.isA<Object>() &&
         res.data.as<Object>().has("handle") &&
         res.data.as<Object>()["handle"].isA<String>())
      {
        if(_outHandle)
        {
          fillStrInMsParam(_outHandle, res.data.as<Object>()["handle"].as<String>().c_str());
        }
        return 0;
      }
      else
      {
        auto tmp = surfsara::ast::formatJson(res.data, true);
        rodsLog(LOG_ERROR, "failed to update index %s for iRods path: %s",
                joinKeys(kvPairs).c_str(),
                pathOrHandle);
        rodsLog(LOG_ERROR, "%s", tmp.c_str());
        return UNMATCHED_KEY_OR_INDEX;
      }
    }
    else
    {
      std::stringstream ss;
      ss << res;
      rodsLog(LOG_ERROR, "failed to update index %s for iRods path: %s",
              joinKeys(kvPairs).c_str(),
              pathOrHandle);
      rodsLog(LOG_ERROR, "%s", ss.str().c_str());
      return ACTION_FAILED_ERR;
    }
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to update index %s for iRods path: %s",
            joinKeys(kvPairs).c_str(),
            pathOrHandle);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}


inline int msiPidUnsetAction(UnsetterFunction unsetter,
                             msParam_t* _inStr, /* iRODS path or handle */
                             msParam_t* _inKey,
                             msParam_t* _outHandle,
                             ruleExecInfo_t* rei,
                             bool isRodsPath)
{
  using Object = surfsara::ast::Object;
  using String = surfsara::ast::String;
  if (rei == NULL || rei->rsComm == NULL)
  {
    return (SYS_INTERNAL_NULL_INPUT_ERR);
  }
  if (_inStr == NULL || _inStr->type == NULL || strcmp(_inStr->type, STR_MS_T) != 0)
  {
    return (USER_PARAM_TYPE_ERR);
  }
  if (_inKey == NULL || _inKey->type == NULL || strcmp(_inKey->type, STR_MS_T) != 0)
  {
    return (USER_PARAM_TYPE_ERR);
  }
  surfsara::handle::Config cfg;
  try
  {
    cfg.parseJson(IRODS_PID_CONFIG_FILE);
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to read PID config file %s:\n%s", IRODS_PID_CONFIG_FILE, ex.what());
    return FILE_READ_ERR;
  }
  char * pathOrHandle = (char*)(_inStr->inOutStruct);
  char * key = (char*)(_inKey->inOutStruct);
  if(!checkPermissions(cfg.getWritePermissions(), rei, (isRodsPath ? pathOrHandle : nullptr), "write"))
  {
    rodsLog(LOG_ERROR, "user %s#%s is not allowed to modify handle",
            rei->rsComm->clientUser.userName,
            rei->rsComm->clientUser.rodsZone);
    return MSI_OPERATION_NOT_ALLOWED;
  }
  try
  {
    auto client = cfg.makeIRodsHandleClient();
    auto res = unsetter(client, pathOrHandle, {key});
    if(res.success)
    {
      if(res.data.isA<Object>() &&
         res.data.as<Object>().has("handle") &&
         res.data.as<Object>()["handle"].isA<String>())
      {
        if(_outHandle)
        {
          fillStrInMsParam(_outHandle, res.data.as<Object>()["handle"].as<String>().c_str());
        }
        return 0;
      }
      else
      {
        auto tmp = surfsara::ast::formatJson(res.data, true);
        rodsLog(LOG_ERROR, "failed to remove index %s for iRods path: %s", key, pathOrHandle);
        rodsLog(LOG_ERROR, "%s", tmp.c_str());
        return UNMATCHED_KEY_OR_INDEX;
      }
    }
    else
    {
      std::stringstream ss;
      ss << res;
      rodsLog(LOG_ERROR, "failed to remove index %s for iRods path: %s", key, pathOrHandle);
      rodsLog(LOG_ERROR, "%s", ss.str().c_str());
      return ACTION_FAILED_ERR;
    }
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to remove index %s for iRods path: %s", key, pathOrHandle);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}
