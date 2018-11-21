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

  int msiPidLookupKey(msParam_t* _inKey,
                      msParam_t* _inValue,
                      msParam_t* _outHandles,
                      ruleExecInfo_t* rei);

  irods::ms_table_entry* plugin_factory()
  {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(3);
#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
    msvc->add_operation("msiPidLookupKey", "msiPidLookupKey");
#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2
    msvc->add_operation("msiPidLookupKey", std::function<int(msParam_t*,
                                                             msParam_t*,
                                                             msParam_t*,
                                                             ruleExecInfo_t*)>(msiPidLookupKey));
#endif
    return msvc;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Implemenation
//
////////////////////////////////////////////////////////////////////////////////
int msiPidLookupKey(msParam_t* _inKey,
                    msParam_t* _inValue,
                    msParam_t* _outHandles,
                    ruleExecInfo_t* rei)
{
  using Object = surfsara::ast::Object;
  using String = surfsara::ast::String;
  if (rei == NULL || rei->rsComm == NULL)
  {
    return (SYS_INTERNAL_NULL_INPUT_ERR);
  }
  if (_inKey == NULL || _inKey->type == NULL || strcmp(_inKey->type, STR_MS_T) != 0)
  {
    return (USER_PARAM_TYPE_ERR);
  }
  if (_inValue == NULL || _inValue->type == NULL || strcmp(_inValue->type, STR_MS_T) != 0)
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
  auto client = cfg.makeReverseLookupClient();
  char * key = (char*)(_inKey->inOutStruct);
  char * value = (char*)(_inValue->inOutStruct);
  try
  {
    std::vector<std::string> result = client->lookup({{key, value}});
    strArray_t * strArray = (strArray_t *)malloc( sizeof( strArray_t ) );
    memset(strArray, 0, sizeof(strArray_t));
    for(auto & h : result)
    {
      std::cout << h << std::endl;
      addStrArray(strArray, strdup(h.c_str()));
    }
    _outHandles->inOutStruct = ( void * ) strArray;
    _outHandles->type = strdup( StrArray_MS_T );
    return 0;
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to lookup handle for iRods path %s", key);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}
