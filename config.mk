ifneq ("$(wildcard /var/lib/irods/VERSION.json)", "")
	IRODS_VERSION=$(shell python -c "import json;print(str(json.load(open('/var/lib/irods/VERSION.json')).get('irods_version', '')))" )
	IRODS_SUFFIX=_${IRODS_VERSION}
else
	IRODS_VERSION=
	IRODS_SUFFIX=
endif

INC_TEST=
INC_TEST+= -I${PREFIX}/Catch2/single_include
INC_TEST+= -I${PREFIX}/handle-cpp-client/include
INC_TEST+= -I${PREFIX}/handle-cpp-client/json-parser-cpp/include
INC_TEST+= -I${PREFIX}/handle-cpp-client/CliArgs/include

INC=
INC+= -I${PREFIX}/handle-cpp-client/include
INC+= -I${PREFIX}/handle-cpp-client/json-parser-cpp/include
INC+= -I${PREFIX}/handle-cpp-client/CliArgs/include

CXX_TEST_LIBS=
CXX_TEST_LIBS+= -lcurl

CXX_TEST_LIB_DIR=
CXX_TEST_PREFIX=

CXX_MAIN_SRC= ${PREFIX}/unit_test/unit_test.cpp

ifeq (${IRODS_VERSION},4.2.3)
	CXX = /opt/irods-externals/clang3.8-0/bin/clang++ -std=c++14
	CXX_TEST = /opt/irods-externals/clang3.8-0/bin/clang++ -std=c++14 -stdlib=libc++
	CXX_TEST_LIB_DIR+= -L/opt/irods-externals/clang3.8-0/lib/
	CXX_TEST_PREFIX = LD_LIBRARY_PATH=/opt/irods-externals/clang3.8-0/lib/
	CXX_FLAGS = -fPIC -shared -nostdinc++ -Wno-write-strings
	INSTALL_DIR=/usr/lib/irods/plugins/microservices/
	INC_TEST+=-I/opt/irods-externals/clang3.8-0/include/c++/v1/
	INC+=	-I/usr/include/irods
	INC+=	-I/opt/irods-externals/clang3.8-0/include/c++/v1
	INC+=	-I/opt/irods-externals/qpid-with-proton0.34-0/include
	INC+=	-I/opt/irods-externals/boost1.60.0-0/include
	INC+=	-I/opt/irods-externals/jansson2.7-0/include
	INC+=	-I/opt/irods-externals/libarchive3.3.2-0/include
	INC+=	-I/opt/irods-externals/boost1.60.0-0/include
	INC+=	-I../Catch2/single_include
else ifeq (${IRODS_VERSION},4.1.11)
	CXX= g++ -std=c++11
	CXX_TEST= g++ -std=c++11
	CXX_FLAGS = -fPIC -shared -Wno-write-strings -Wno-deprecated
	INSTALL_DIR=/var/lib/irods/plugins/microservices/
	INC+=  -I/usr/include/irods/ 
	INC+= -I/app/irods-4.1.11/iRODS/server/re/include/
	INC+= -I/app/irods-4.1.11/iRODS/lib/core/include/
	INC+= -I/app/irods-4.1.11/iRODS/lib/api/include/
	INC+= -I/app/irods-4.1.11/iRODS/server/core/include/
	INC+= -I/app/irods-4.1.11/iRODS/server/icat/include/
	INC+= -I/app/irods-4.1.11/iRODS/server/drivers/include/
	INC+= -I/app/irods-4.1.11/iRODS/lib/api/include/
else
	CXX= g++ -std=c++11
	CXX_TEST= g++ -std=c++11
endif 

OBJECT_DIR=${PREFIX}/obj/${IRODS_SUFFIX}
