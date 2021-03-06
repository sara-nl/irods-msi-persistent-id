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
#include "libmsi_pid_util.h"

typedef std::vector<std::pair<std::string, std::string>> KeyValuePairs;

extern "C"
{
  double get_plugin_interface_version()
  {
    return 1.0;
  }

  int msiPidSetHandle(msParam_t* _inHandle,
                      msParam_t* _inKey,
                      msParam_t* _inValue,
                      ruleExecInfo_t* rei)
  {
    return msiPidSetAction([](std::shared_ptr<surfsara::handle::IRodsHandleClient> client,
                              const std::string & handle,
                              const std::vector<std::pair<std::string, std::string>> & kvp) {
                             return client->setHandle(handle, kvp);
                           },
                           _inHandle, _inKey, _inValue, nullptr, rei, false);
  }

  irods::ms_table_entry* plugin_factory()
  {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(3);
#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
    msvc->add_operation("msiPidSetHandle", "msiPidSetHandle");
#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2
    msvc->add_operation("msiPidSetHandle", std::function<int(msParam_t*,
                                                             msParam_t*,
                                                             msParam_t*,
                                                             ruleExecInfo_t*)>(msiPidSetHandle));
#endif
    return msvc;
  }
}
