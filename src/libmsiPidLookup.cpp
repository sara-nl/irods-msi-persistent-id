#include "libmsi_pid_common.h"
 
extern "C"
{
  double get_plugin_interface_version()
  {
    return 1.0;
  }

  int msiPidLookup(msParam_t* _inPath, msParam_t* _outHandle, ruleExecInfo_t* rei);

  irods::ms_table_entry* plugin_factory()
  {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
    msvc->add_operation("msiPidLookup", "msiPidLookup");
#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2
    msvc->add_operation("msiPidLookup", std::function<int(msParam_t*,
                                                          msParam_t*,
                                                          ruleExecInfo_t*)>(msiPidLookup));
#endif
    return msvc;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Implemenation
//
////////////////////////////////////////////////////////////////////////////////
int msiPidLookup(msParam_t* _inPath, msParam_t* _outHandle, ruleExecInfo_t* rei)
{
  using Object = surfsara::ast::Object;
  using String = surfsara::ast::String;
  if (rei == NULL || rei->rsComm == NULL)
  {
    return (SYS_INTERNAL_NULL_INPUT_ERR);
  }
  if (_inPath == NULL || _inPath->type == NULL || strcmp(_inPath->type, STR_MS_T) != 0)
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
  auto client = cfg.makeIRodsHandleClient();
  char * path = (char*)(_inPath->inOutStruct);
  try
  {
    auto handle = client->lookup(path);
    fillStrInMsParam(_outHandle, handle.c_str());
    return 0;
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to lookup handle for iRods path %s", path);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}
