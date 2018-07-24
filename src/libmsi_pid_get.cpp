#include "libmsi_pid_common.h"
 
extern "C"
{
  double get_plugin_interface_version()
  {
    return 1.0;
  }

  int msi_pid_get(msParam_t* _inPathOld,
                  msParam_t* _inPathNew,
                  msParam_t* _outHandle,
                  ruleExecInfo_t* rei);

  irods::ms_table_entry* plugin_factory()
  {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(3);
#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
    msvc->add_operation("msi_pid_get", "msi_pid_get");
#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2
    msvc->add_operation("msi_pid_get", std::function<int(msParam_t*,
                                                         msParam_t*,
                                                         msParam_t*,
                                                         ruleExecInfo_t*)>(msi_pid_update));
#endif
    return msvc;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Implemenation
//
////////////////////////////////////////////////////////////////////////////////
inline std::string extractValue(const surfsara::ast::Node & node,
                                const std::string & type)
{
  using String = surfsara::ast::String;
  using Node = surfsara::ast::Node;
  auto obj = node.find("values/*/data/value", [type](const Node & root,
                                                     const std::vector<std::string> & _path)
                       {
                         std::vector<std::string> path(_path);
                         path.pop_back();
                         path.pop_back();
                         path.push_back("type");
                         auto n = root.find(path);
                         if(n.isA<String>() && n.as<String>() == type)
                         {
                           return true;
                         }
                         else
                         {
                           return false;
                         }
                       });
  if(obj.isA<String>())
  {
    return obj.as<String>();
  }
  else
  {
    return "[OBJECT]";
  }
}

int msi_pid_get(msParam_t* _inPath,
                msParam_t* _inType,
                msParam_t* _outValue, ruleExecInfo_t* rei)
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
  auto client = cfg.makeIRodsHandleClient();
  char * path = (char*)(_inPath->inOutStruct);
  std::string inType = std::string((char*)(_inType->inOutStruct));
  try
  {
    auto res = client->get(path);
    if(res.success)
    {
      if(inType.empty())
      {
        auto jsonString = surfsara::ast::formatJson(res.data, true);
        fillStrInMsParam(_outValue, jsonString.c_str());
      }
      else
      {
        auto result = extractValue(res.data, inType);
        fillStrInMsParam(_outValue, result.c_str());
      }
    }
    else
    {
      std::stringstream ss;
      ss << res;
      rodsLog(LOG_ERROR, "failed to get details of PID for iRods path from %s", path);
      rodsLog(LOG_ERROR, "%s", ss.str().c_str());
      return ACTION_FAILED_ERR;
    }
  }
  catch(std::exception & ex)
  {
    rodsLog(LOG_ERROR, "failed to get details of PID for iRods path from %s", path);
    rodsLog(LOG_ERROR, "exception: %s", ex.what());
    return ACTION_FAILED_ERR;
  }
}
