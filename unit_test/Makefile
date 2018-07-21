PREFIX=../
include ../config.mk

OBJECT_DIR=${PREFIX}/obj/${IRODS_SUFFIX}

OBJ_TEST= ${OBJECT_DIR}/unit_test/unit_test.o \
	  ${OBJECT_DIR}/msi_wordcount/test_wordcount.o \
	  ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_flag.o \
	  ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_multiple_value.o \
	  ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_value.o \
	  ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_argument.o \
	  ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_multiple_flag.o \
	  ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_parser.o \
	  ${OBJECT_DIR}/handle-cpp-client/json-parser-cpp/test/ast.o \
	  ${OBJECT_DIR}/handle-cpp-client/json-parser-cpp/test/json_parser.o \
	  ${OBJECT_DIR}/handle-cpp-client/unit_test/test_handle.o  \
	  ${OBJECT_DIR}/handle-cpp-client/unit_test/test_util.o

all: runtests
.PHONY : all

${OBJ_TEST}:
	mkdir -p $(dir $@)
	${CXX_TEST} ${INC_TEST}	-c $(patsubst ${OBJECT_DIR}/%.o, ${PREFIX}/%.cpp, $@) -o $@

runtests: ${OBJ_TEST}
	${CXX_TEST_PREFIX} ${CXX_TEST} ${CXX_TEST_LIB_DIR} ${OBJ_TEST} ${CXX_TEST_LIBS} -o runtests

clean:
	rm -f runtests
	rm -f ${OBJ_TEST}
