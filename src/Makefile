PREFIX=../
include ../irods-ci-server/config.mk

INC_TEST+= -I${PREFIX}/Catch2/single_include
INC_TEST+= -I${PREFIX}/handle-cpp-client/include
INC_TEST+= -I${PREFIX}/handle-cpp-client/json-parser-cpp/include
INC_TEST+= -I${PREFIX}/handle-cpp-client/CliArgs/include
INC+= -I${PREFIX}/handle-cpp-client/include
INC+= -I${PREFIX}/handle-cpp-client/json-parser-cpp/include
INC+= -I${PREFIX}/handle-cpp-client/CliArgs/include

LIBS= ${LIBS_DIR}/libmsiPidCreate.so \
      ${LIBS_DIR}/libmsiPidDelete.so \
      ${LIBS_DIR}/libmsiPidLookup.so \
      ${LIBS_DIR}/libmsiPidMove.so \
      ${LIBS_DIR}/libmsiPidGet.so

OBJ_TEST+= ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_flag.o \
	   ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_multiple_value.o \
	   ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_value.o \
	   ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_argument.o \
	   ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_multiple_flag.o \
	   ${OBJECT_DIR}/handle-cpp-client/CliArgs/tests/test_parser.o \
	   ${OBJECT_DIR}/handle-cpp-client/json-parser-cpp/test/ast.o \
	   ${OBJECT_DIR}/handle-cpp-client/json-parser-cpp/test/json_parser.o \
	   ${OBJECT_DIR}/handle-cpp-client/unit_test/test_handle.o  \
	   ${OBJECT_DIR}/handle-cpp-client/unit_test/test_util.o \
           ${OBJECT_DIR}/src/unit_test.o

all: ${LIBS} ${LIBS_DIR}/test_pid
.PONY: all

-include $(patsubst ${LIBS_DIR}/%.so,${OBJECT_DIR}/%.dep, ${LIBS})

${OBJ_TEST}:
	mkdir -p $(dir $@)
	${CXX_TEST} ${INC_TEST}	-c $(patsubst ${OBJECT_DIR}/%.o, ${PREFIX}/%.cpp, $@) -o $@

${LIBS}:
	mkdir -p $(dir $@)
	mkdir -p $(dir $(patsubst ${LIBS_DIR}/%.so,${OBJECT_DIR}/%.dep, $@))
	${CXX} ${INC} -DRODS_SERVER ${CXX_FLAGS} -o $@ $(patsubst ${LIBS_DIR}/%.so, %.cpp, $@) -lcurl
	${CXX} ${INC} -MM -MT $@ -MF $(patsubst ${LIBS_DIR}/%.so,${LIBS_DIR}/%.dep, $@) -DRODS_SERVER \
	${CXX_FLAGS} $(patsubst ${LIBS_DIR}/%.so, %.cpp, $@)

${LIBS_DIR}/test_pid: ${OBJ_TEST}
	${CXX_TEST_PREFIX} ${CXX_TEST} ${INC_TEST} ${CXX_TEST_LIB_DIR} ${OBJ_TEST} ${CXX_TEST_LIBS} \
	-o ${LIBS_DIR}/test_pid


test: ${LIBS_DIR}/test_pid
	mkdir -p ${LIBS_DIR}
	${LIBS_DIR}/test_pid
.PONY: test


install:
	cp ${LIBS_DIR}/libmsiPidCreate.so ${INSTALL_DIR}/libmsiPidCreate.so
	cp ${LIBS_DIR}/libmsiPidDelete.so ${INSTALL_DIR}/libmsiPidDelete.so
	cp ${LIBS_DIR}/libmsiPidLookup.so ${INSTALL_DIR}/libmsiPidLookup.so
	cp ${LIBS_DIR}/libmsiPidMove.so   ${INSTALL_DIR}/libmsiPidMove.so
	cp ${LIBS_DIR}/libmsiPidGet.so    ${INSTALL_DIR}/libmsiPidGet.so


functional_test:
	echo "todo"

package:
	echo "todo"

clean:
	rm -f ${LIBS}
	rm -f ${OBJ_TEST}
	rm -f test_pid
	rm -f $(patsubst %.so,../obj/%.dep, ${LIBS})
