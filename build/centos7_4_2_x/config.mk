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

CXX = /opt/irods-externals/clang3.8-0/bin/clang++ -std=c++14
CXX_TEST = /opt/irods-externals/clang3.8-0/bin/clang++ -std=c++14 -stdlib=libc++
CXX_TEST_LIB_DIR+= -L/opt/irods-externals/clang3.8-0/lib/
CXX_TEST_PREFIX = LD_LIBRARY_PATH=/opt/irods-externals/clang3.8-0/lib/
CXX_FLAGS = -fPIC -shared -nostdinc++ -Wno-write-strings
CXX_BIN_FLAGS = -nostdinc++ -Wall -Wextra -Werror -Wno-unused-parameter -Wno-write-strings
BASE_INSTALL_DIR=/usr/lib/irods/plugins/
INSTALL_DIR=/usr/lib/irods/plugins/microservices/
INC_TEST+=-I/opt/irods-externals/clang3.8-0/include/c++/v1/
INC+=	-I/usr/include/irods
INC+=	-I/opt/irods-externals/clang3.8-0/include/c++/v1
INC+=	-I/opt/irods-externals/qpid-with-proton0.34-0/include
INC+=	-I/opt/irods-externals/boost1.60.0-0/include
INC+=	-I/opt/irods-externals/jansson2.7-0/include
INC+=	-I/opt/irods-externals/libarchive3.3.2-0/include
INC+=	-I/opt/irods-externals/boost1.60.0-0/include
INC+=	-I/opt/Catch2-2.9.2/single_include

OBJECT_DIR=${PREFIX}/obj/${IRODS_SUFFIX}
LIBS_DIR=${PREFIX}/lib/${IRODS_SUFFIX}


# /opt/irods-externals/clang3.8-0/bin/clang++   -DBOOST_SYSTEM_NO_DEPRECATED -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -D_LARGE_FILES -Dlinux_platform -I/opt/irods-externals/clang3.8-0/include/c++/v1 -I//usr/include/irods -I/opt/irods-externals/avro1.7.7-0/include -I/opt/irods-externals/boost1.60.0-0/include -I/opt/irods-externals/jansson2.7-0/include -I/opt/irods-externals/libarchive3.3.2-0/include -I/opt/irods-externals/cppzmq4.1-0/include -I/opt/irods-externals/zeromq4-14.1.3-0/include  -O3 -DNDEBUG   -nostdinc++ -Wall -Wextra -Werror -Wno-unused-parameter -Wno-write-strings -std=gnu++14 -o CMakeFiles/iinit.dir/src/iinit.cpp.o -c /build/irods_client_icommands/src/iinit.cpp
