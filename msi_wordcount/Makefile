PREFIX=../
include ../config.mk
include ./unit_test.mk

all: libmsi_wordcount${IRODS_SUFFIX}.so test_wordcount${IRODS_SUFFIX}
.PONY: all

libmsi_wordcount${IRODS_SUFFIX}.so: libmsi_wordcount.cpp libmsi_wordcount.h
	${CXX} ${INC} -DRODS_SERVER ${CXX_FLAGS} -o $@ libmsi_wordcount.cpp -Wno-deprecated

test_wordcount${IRODS_SUFFIX}: ${OBJ_TEST}
	${CXX_TEST_PREFIX} ${CXX_TEST} ${INC_TEST} ${CXX_TEST_LIB_DIR} ${OBJ_TEST} ${CXX_MAIN_SRC} ${CXX_TEST_LIBS} -o test_wordcount${IRODS_SUFFIX}

${OBJ_TEST}:
	mkdir -p $(dir $@)
	${CXX_TEST} ${INC_TEST}	-c $(patsubst ${OBJECT_DIR}/%.o, ${PREFIX}/%.cpp, $@) -o $@

test: test_wordcount${IRODS_SUFFIX}
	./test_wordcount${IRODS_SUFFIX}
.PONY: test

install:
	cp libmsi_wordcount${IRODS_SUFFIX}.so ${INSTALL_DIR}/libmsi_wordcount.so

clean:
	rm -f libmsi_wordcount${IRODS_SUFFIX}.so

allclean:
	rm -f libmsi_wordcount*.so



