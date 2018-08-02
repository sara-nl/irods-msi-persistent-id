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
