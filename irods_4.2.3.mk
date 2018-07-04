CXX = /opt/irods-externals/clang3.8-0/bin/clang++ -std=c++14
CXX_TEST = /opt/irods-externals/clang3.8-0/bin/clang++ -std=c++14 -stdlib=libc++
CXX_TEST_LIBS = -L/opt/irods-externals/clang3.8-0/lib/
CXX_TEST_PREFIX = LD_LIBRARY_PATH=/opt/irods-externals/clang3.8-0/lib/

CXX_FLAGS = -fPIC -shared -nostdinc++ -Wno-write-strings

INSTALL_DIR=/usr/lib/irods/plugins/microservices/

# CXX_LINK_FLAGS=-fPIC -stdlib=libc++ -shared -lc++abi ${CXX_LIBS} -Wl,-rpath,/opt/irods-externals/boost1.60.0-0/lib:/opt/irods-externals/libarchive3.3.2-0/lib 

# CXX_DEFINES = -DBOOST_SYSTEM_NO_DEPRECATED -DENABLE_RE -DRODS_SERVER -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -D_LARGE_FILES

INC_TEST=-I/opt/irods-externals/clang3.8-0/include/c++/v1/
INC_TEST+= -I../Catch2/single_include/

INC=	-I/usr/include/irods
INC+=	-I/opt/irods-externals/clang3.8-0/include/c++/v1
INC+=	-I/opt/irods-externals/qpid-with-proton0.34-0/include
INC+=	-I/opt/irods-externals/boost1.60.0-0/include
INC+=	-I/opt/irods-externals/jansson2.7-0/include
INC+=	-I/opt/irods-externals/libarchive3.3.2-0/include
INC+=	-I/opt/irods-externals/boost1.60.0-0/include
INC+=	-I../Catch2/single_include
