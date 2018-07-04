CXX= g++ -std=c++11
CXX_TEST= g++ -std=c++11
INC_TEST= -I../Catch2/single_include/
CXX_TEST_PREFIX=
CXX_TEST_LIBS=
CXX_FLAGS = -fPIC -shared -Wno-write-strings
INSTALL_DIR=/var/lib/irods/plugins/microservices/
INC=  -I/usr/include/irods/ 
INC+= -I/app/irods-4.1.11/iRODS/server/re/include/
INC+= -I/app/irods-4.1.11/iRODS/lib/core/include/
INC+= -I/app/irods-4.1.11/iRODS/lib/api/include/
INC+= -I/app/irods-4.1.11/iRODS/server/core/include/
INC+= -I/app/irods-4.1.11/iRODS/server/icat/include/
INC+= -I/app/irods-4.1.11/iRODS/server/drivers/include/
INC+= -I/app/irods-4.1.11/iRODS/lib/api/include/
