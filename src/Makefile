PREFIX=../
include ../build/config.mk

CXX_FLAGS+= -O2

INC_TEST+= -I${PREFIX}/surfsara-handle-client-cpp/Catch2/single_include
INC_TEST+= -I${PREFIX}/surfsara-handle-client-cpp/include
INC_TEST+= -I${PREFIX}/surfsara-handle-client-cpp/surfsara-json-parser-cpp/include
INC_TEST+= -I${PREFIX}/surfsara-handle-client-cpp/CliArgs/include
INC+= -I${PREFIX}/surfsara-handle-client-cpp/include
INC+= -I${PREFIX}/surfsara-handle-client-cpp/surfsara-json-parser-cpp/include
INC+= -I${PREFIX}/surfsara-handle-client-cpp/CliArgs/include

LIBS= ${LIBS_DIR}/libmsiPidCreate.so \
      ${LIBS_DIR}/libmsiPidDelete.so \
      ${LIBS_DIR}/libmsiPidDeleteHandle.so \
      ${LIBS_DIR}/libmsiPidLookup.so \
      ${LIBS_DIR}/libmsiPidLookupOne.so \
      ${LIBS_DIR}/libmsiPidLookupKey.so \
      ${LIBS_DIR}/libmsiPidMove.so \
      ${LIBS_DIR}/libmsiPidMoveHandle.so \
      ${LIBS_DIR}/libmsiPidGet.so \
      ${LIBS_DIR}/libmsiPidGetHandle.so \
      ${LIBS_DIR}/libmsiPidSet.so \
      ${LIBS_DIR}/libmsiPidSetHandle.so \
      ${LIBS_DIR}/libmsiPidUnset.so \
      ${LIBS_DIR}/libmsiPidUnsetHandle.so

OBJ_TEST+= ${OBJECT_DIR}/surfsara-handle-client-cpp/CliArgs/tests/test_flag.o \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/CliArgs/tests/test_multiple_value.o \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/CliArgs/tests/test_value.o \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/CliArgs/tests/test_argument.o \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/CliArgs/tests/test_multiple_flag.o \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/CliArgs/tests/test_parser.o \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/surfsara-json-parser-cpp/test/ast.o \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/surfsara-json-parser-cpp/test/json_parser.o \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/unit_test/test_handle.o  \
	   ${OBJECT_DIR}/surfsara-handle-client-cpp/unit_test/test_util.o \
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
	${CXX} ${INC} -MM -MT $@ -MF $(patsubst ${LIBS_DIR}/%.so,${OBJECT_DIR}/%.dep, $@) -DRODS_SERVER \
	${CXX_FLAGS} $(patsubst ${LIBS_DIR}/%.so, %.cpp, $@)

${LIBS_DIR}/test_pid: ${OBJ_TEST}
	${CXX_TEST_PREFIX} ${CXX_TEST} ${INC_TEST} ${CXX_TEST_LIB_DIR} ${OBJ_TEST} ${CXX_TEST_LIBS} \
	-o ${LIBS_DIR}/test_pid


test: ${LIBS_DIR}/test_pid
	mkdir -p ${LIBS_DIR}
	${CXX_TEST_PREFIX} ${LIBS_DIR}/test_pid
.PONY: test


install:
	cp ${LIBS_DIR}/libmsiPidCreate.so ${INSTALL_DIR}/libmsiPidCreate.so
	cp ${LIBS_DIR}/libmsiPidDelete.so ${INSTALL_DIR}/libmsiPidDelete.so
	cp ${LIBS_DIR}/libmsiPidDeleteHandle.so ${INSTALL_DIR}/libmsiPidDeleteHandle.so
	cp ${LIBS_DIR}/libmsiPidLookup.so ${INSTALL_DIR}/libmsiPidLookup.so
	cp ${LIBS_DIR}/libmsiPidLookupOne.so ${INSTALL_DIR}/libmsiPidLookupOne.so
	cp ${LIBS_DIR}/libmsiPidLookupKey.so ${INSTALL_DIR}/libmsiPidLookupKey.so
	cp ${LIBS_DIR}/libmsiPidMove.so   ${INSTALL_DIR}/libmsiPidMove.so
	cp ${LIBS_DIR}/libmsiPidMoveHandle.so   ${INSTALL_DIR}/libmsiPidMoveHandle.so
	cp ${LIBS_DIR}/libmsiPidGet.so    ${INSTALL_DIR}/libmsiPidGet.so
	cp ${LIBS_DIR}/libmsiPidGetHandle.so ${INSTALL_DIR}/libmsiPidGetHandle.so
	cp ${LIBS_DIR}/libmsiPidSet.so       ${INSTALL_DIR}/libmsiPidSet.so
	cp ${LIBS_DIR}/libmsiPidSetHandle.so ${INSTALL_DIR}/libmsiPidSetHandle.so
	cp ${LIBS_DIR}/libmsiPidUnset.so     ${INSTALL_DIR}/libmsiPidUnset.so
	cp ${LIBS_DIR}/libmsiPidUnsetHandle.so ${INSTALL_DIR}/libmsiPidUnsetHandle.so

clean:
	rm -f ${LIBS}
	rm -f ${OBJ_TEST}
	rm -f test_pid
	rm -f $(patsubst %.so,../obj/%.dep, ${LIBS})
