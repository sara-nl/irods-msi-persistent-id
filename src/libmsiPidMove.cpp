#include "libmsi_pid_common.h"
 
extern "C"
{
  double get_plugin_interface_version()
  {
    return 1.0;
  }

  int msiPidMove(msParam_t* _inPathOld,
                 msParam_t* _inPathNew,
                 msParam_t* _outHandle,
                 ruleExecInfo_t* rei);

  irods::ms_table_entry* plugin_factory()
  {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(3);
#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
    msvc->add_operation("msiPidMove", "msiPidMove");
#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2
    msvc->add_operation("msiPidMove", std::function<int(msParam_t*,
                                                        msParam_t*,
                                                        msParam_t*,
                                                        ruleExecInfo_t*)>(msiPidMove));
#endif
    return msvc;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Implemenation
//
////////////////////////////////////////////////////////////////////////////////
int msiPidMove(msParam_t* _inPathOld,
               msParam_t* _inPathNew,
               msParam_t* _outHandle, ruleExecInfo_t* rei)
{
  using Object = surfsara::ast::Object;
  using String = surfsara::ast::String;
  if (rei == NULL || rei->rsComm == NULL)
  {
    return (SYS_INTERNAL_NULL_INPUT_ERR);
  }
  if (_inPathOld == NULL || _inPathOld->type == NULL || strcmp(_inPathOld->type, STR_MS_T) != 0)
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
  auto client = cfg.makeIRodsHandleClient();
  char * pathOld = (char*)(_inPathOld->inOutStruct);
  char * pathNew = (char*)(_inPathNew->inOutStruct);
  try
  {
    auto res = client->move(pathOld, pathNew);
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
        rodsLog(LOG_ERROR, "failed to move PID for iRods path from %s to %s", pathOld, pathNew);
        rodsLog(LOG_ERROR, "%s", tmp.c_str());
        return UNMATCHED_KEY_OR_INDEX;
      }
    }
    else
    {
      std::stringstream ss;
      ss << res;
      rodsLog(LOG_ERROR, "failed to move PID for iRods path from %s to %s", pathOld, pathNew);
      rodsLog(LOG_ERROR, "%s", ss.str().c_str());
      return ACTION_FAILED_ERR;
    }
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to move PID for iRods path from %s to %s", pathOld, pathNew);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}
