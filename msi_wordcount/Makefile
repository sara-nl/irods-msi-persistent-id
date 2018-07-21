PREFIX=../
include ../config.mk


all: libmsi_wordcount${IRODS_SUFFIX}.so
.PONY: all

libmsi_wordcount${IRODS_SUFFIX}.so: libmsi_wordcount.cpp libmsi_wordcount.h
	${CXX} ${INC} -DRODS_SERVER ${CXX_FLAGS} -o $@ libmsi_wordcount.cpp -Wno-deprecated

install:
	cp libmsi_wordcount${IRODS_SUFFIX}.so ${INSTALL_DIR}/libmsi_wordcount.so

clean:
	rm -f libmsi_wordcount${IRODS_SUFFIX}.so

allclean:
	rm -f libmsi_wordcount*.so



