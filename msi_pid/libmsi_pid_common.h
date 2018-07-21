#pragma once

#include <irods_version.h>

#if IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 1
//4.1.x
#include <apiHeaderAll.h>
#include <msParam.h>
#include <irods_ms_plugin.hpp>
#include <dataObjOpen.h>
#include <dataObjRead.h>
#include <dataObjClose.h>

#include "reGlobalsExtern.hpp"
#include "rsGlobalExtern.hpp"
#include "rcGlobalExtern.h"

#elif IRODS_VERSION_MAJOR == 4 && IRODS_VERSION_MINOR == 2

#include <apiHeaderAll.h>
#include <msParam.h>
#include <irods_ms_plugin.hpp>
#include <rsDataObjOpen.hpp>
#include <rsDataObjRead.hpp>
#include <rsDataObjClose.hpp>

#else

#error "unsupported irods version"

#endif

#include <surfsara/handle_config.h>
#include <surfsara/handle_client.h>
#include <surfsara/reverse_lookup_client.h>
#include <surfsara/handle_util.h>
#include <surfsara/irods_handle_client.h>
#include <surfsara/ast.h>


#ifndef IRODS_PID_CONFIG_FILE
#define IRODS_PID_CONFIG_FILE "/etc/irods/irods_pid.json"
#endif
