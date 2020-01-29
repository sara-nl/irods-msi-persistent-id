ifneq ("$(wildcard /var/lib/irods/VERSION.json)", "")
	IRODS_VERSION=$(shell python -c "import json;print(str(json.load(open('/var/lib/irods/VERSION.json')).get('irods_version', '')))" )
	IRODS_SUFFIX=_${IRODS_VERSION}
else
	IRODS_VERSION=
	IRODS_SUFFIX=
endif

INC_TEST=
INC=

CXX_TEST_LIBS=
CXX_TEST_LIBS+= -lcurl

CXX_TEST_LIB_DIR=
CXX_TEST_PREFIX=

CXX= g++ -std=c++11
CXX_TEST= g++ -std=c++11
CXX_FLAGS = -fPIC -shared -Wno-write-strings -Wno-deprecated
BASE_INSTALL_DIR=/var/lib/irods/plugins/
INSTALL_DIR=/var/lib/irods/plugins/microservices/
INC+=  -I/usr/include/irods/ 
INC+= -I/app/irods-4.1.11/iRODS/server/re/include/
INC+= -I/app/irods-4.1.11/iRODS/lib/core/include/
INC+= -I/app/irods-4.1.11/iRODS/lib/api/include/
INC+= -I/app/irods-4.1.11/iRODS/server/core/include/
INC+= -I/app/irods-4.1.11/iRODS/server/icat/include/
INC+= -I/app/irods-4.1.11/iRODS/server/drivers/include/
INC+= -I/app/irods-4.1.11/iRODS/lib/api/include/

OBJECT_DIR=${PREFIX}/obj/${IRODS_SUFFIX}
LIBS_DIR=${PREFIX}/lib/${IRODS_SUFFIX}
