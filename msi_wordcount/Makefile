IRODS_VERSION=$(shell echo -e "import json\nwith open('/var/lib/irods/VERSION.json') as f: print(str(json.load(f)['irods_version']))" | python )

include ../irods_${IRODS_VERSION}.mk

all: libmsi_wordcount.${IRODS_VERSION}.so  test_wordcount_${IRODS_VERSION}
.PONY: all

libmsi_wordcount.${IRODS_VERSION}.so: libmsi_wordcount.cpp libmsi_wordcount.h
	${CXX} ${INC} -DRODS_SERVER ${CXX_FLAGS} -o libmsi_wordcount.${IRODS_VERSION}.so libmsi_wordcount.cpp -Wno-deprecated

test_wordcount_${IRODS_VERSION}: test_wordcount.cpp libmsi_wordcount.h
	${CXX_TEST} ${INC_TEST} ${CXX_TEST_LIBS} test_wordcount.cpp -o test_wordcount_${IRODS_VERSION}

test:
	${CXX_TEST_PREFIX} ./test_wordcount_${IRODS_VERSION}

install:
	cp libmsi_wordcount.${IRODS_VERSION}.so ${INSTALL_DIR}/libmsi_wordcount.so

clean:
	rm -f test_wordcount_${IRODS_VERSION}
	rm -f libmsi_wordcount.${IRODS_VERSION}.so

allclean:
	rm -f test_wordcount_*
	rm -f libmsi_wordcount.*.so



