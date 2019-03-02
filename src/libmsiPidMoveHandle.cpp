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
#include "libmsi_pid_common.h"
#include "libmsi_pid_util.h"

extern "C"
{
  double get_plugin_interface_version()
  {
    return 1.0;
  }

  int msiPidMoveHandle(msParam_t* _inHandle,
                       msParam_t* _inPathNew,
                       ruleExecInfo_t* rei);

  irods::ms_table_entry* plugin_factory()
  {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
    msvc->add_operation("msiPidMoveHandle", "msiPidMoveHandle");
#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2
    msvc->add_operation("msiPidMoveHandle", std::function<int(msParam_t*,
                                                              msParam_t*,
                                                              ruleExecInfo_t*)>(msiPidMoveHandle));
#endif
    return msvc;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Implemenation
//
////////////////////////////////////////////////////////////////////////////////
int msiPidMoveHandle(msParam_t* _inHandle,
                     msParam_t* _inPathNew,
                     ruleExecInfo_t* rei)
{
  using Object = surfsara::ast::Object;
  using String = surfsara::ast::String;
  if (rei == NULL || rei->rsComm == NULL)
  {
    return (SYS_INTERNAL_NULL_INPUT_ERR);
  }
  if (_inHandle == NULL || _inHandle->type == NULL || strcmp(_inHandle->type, STR_MS_T) != 0)
  {
    return (USER_PARAM_TYPE_ERR);
  }
  if (_inPathNew == NULL || _inPathNew->type == NULL || strcmp(_inPathNew->type, STR_MS_T) != 0)
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
  if(!checkPermissions(cfg.getWritePermissions(), rei))
  {
    rodsLog(LOG_ERROR, "user %s#%s is not allowed to update handle",
            rei->rsComm->clientUser.userName,
            rei->rsComm->clientUser.rodsZone);
    return MSI_OPERATION_NOT_ALLOWED;
  }

  char * handle = (char*)(_inHandle->inOutStruct);
  char * pathNew = (char*)(_inPathNew->inOutStruct);
  try
  {
    auto client = cfg.makeIRodsHandleClient();
    auto res = client->moveHandle(handle, pathNew);
    if(res.success)
    {
      if(res.data.isA<Object>() &&
         res.data.as<Object>().has("handle") &&
         res.data.as<Object>()["handle"].isA<String>())
      {
        return 0;
      }
      else
      {
        auto tmp = surfsara::ast::formatJson(res.data, true);
        rodsLog(LOG_ERROR, "failed to move path for handle %s to new path %s", handle, pathNew);
        rodsLog(LOG_ERROR, "%s", tmp.c_str());
        return UNMATCHED_KEY_OR_INDEX;
      }
    }
    else
    {
      std::stringstream ss;
      ss << res;
      rodsLog(LOG_ERROR, "failed to move path for handle %s to new path %s", handle, pathNew);
      rodsLog(LOG_ERROR, "%s", ss.str().c_str());
      return ACTION_FAILED_ERR;
    }
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to move path for handle %s to new path %s", handle, pathNew);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}
