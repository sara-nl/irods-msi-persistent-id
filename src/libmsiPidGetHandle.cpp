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
 
extern "C"
{
  double get_plugin_interface_version()
  {
    return 1.0;
  }

  int msiPidGetHandle(msParam_t* _inPathOld,
                      msParam_t* _inPathNew,
                      msParam_t* _outHandle,
                      ruleExecInfo_t* rei);

  irods::ms_table_entry* plugin_factory()
  {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(3);
#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
    msvc->add_operation("msiPidGetHandle", "msiPidGetHandle");
#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2
    msvc->add_operation("msiPidGetHandle", std::function<int(msParam_t*,
                                                             msParam_t*,
                                                             msParam_t*,
                                                             ruleExecInfo_t*)>(msiPidGetHandle));
#endif
    return msvc;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Implemenation
//
////////////////////////////////////////////////////////////////////////////////
int msiPidGetHandle(msParam_t* _inHandle,
                    msParam_t* _inType,
                    msParam_t* _outValue, ruleExecInfo_t* rei)
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
  auto client = cfg.makeHandleClient();
  char * handle = (char*)(_inHandle->inOutStruct);
  std::string inType = std::string((char*)(_inType->inOutStruct));
  std::cout << handle << std::endl;
  try
  {
    auto res = client->get(handle);
    if(res.success)
    {
      if(inType.empty())
      {
        auto jsonString = surfsara::ast::formatJson(res.data, true);
        std::cout << jsonString << std::endl;
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
      rodsLog(LOG_ERROR, "failed to get details of PID for iRods path from %s", handle);
      rodsLog(LOG_ERROR, "%s", ss.str().c_str());
      return ACTION_FAILED_ERR;
    }
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to get details of PID for iRods path from %s", handle);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}
