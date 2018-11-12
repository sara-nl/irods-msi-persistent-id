/*****************************************************************
Copyright 2018, SURFsara
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
#include "libmsi_pid_common.h"

typedef std::vector<std::pair<std::string, std::string>> KeyValuePairs;

extern "C"
{
  double get_plugin_interface_version()
  {
    return 1.0;
  }

  int msiPidSet(msParam_t* _inPath,
                msParam_t* _inKey,
                msParam_t* _inValue,
                msParam_t* _outHandle,
                ruleExecInfo_t* rei);

  irods::ms_table_entry* plugin_factory()
  {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(4);
#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
    msvc->add_operation("msiPidSet", "msiPidSet");
#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2
    msvc->add_operation("msiPidSet", std::function<int(msParam_t*,
                                                       msParam_t*,
                                                       msParam_t*,
                                                       msParam_t*,
                                                       ruleExecInfo_t*)>(msiPidSet));
#endif
    return msvc;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Implemenation
//
////////////////////////////////////////////////////////////////////////////////
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

int msiPidSet(msParam_t* _inPath,
              msParam_t* _inKey,
              msParam_t* _inValue,
              msParam_t* _outHandle,
              ruleExecInfo_t* rei)
{
  using Object = surfsara::ast::Object;
  using String = surfsara::ast::String;
  rodsLog(LOG_ERROR, "type %s", _inKey->type);
  if (rei == NULL || rei->rsComm == NULL)
  {
    return (SYS_INTERNAL_NULL_INPUT_ERR);
  }
  if (_inPath == NULL || _inPath->type == NULL || strcmp(_inPath->type, STR_MS_T) != 0)
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
  auto client = cfg.makeIRodsHandleClient();
  char * path = (char*)(_inPath->inOutStruct);
  try
  {
    auto res = client->set(path, kvPairs);
    if(res.success)
    {
      if(res.data.isA<Object>() &&
         res.data.as<Object>().has("handle") &&
         res.data.as<Object>()["handle"].isA<String>())
      {
        fillStrInMsParam(_outHandle, res.data.as<Object>()["handle"].as<String>().c_str());
        return 0;
      }
      else
      {
        auto tmp = surfsara::ast::formatJson(res.data, true);
        rodsLog(LOG_ERROR, "failed to update index %s for iRods path: %s",
                joinKeys(kvPairs).c_str(),
                path);
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
              path);
      rodsLog(LOG_ERROR, "%s", ss.str().c_str());
      return ACTION_FAILED_ERR;
    }
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to update index %s for iRods path: %s",
            joinKeys(kvPairs).c_str(),
            path);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}
