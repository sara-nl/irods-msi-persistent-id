PREFIX=../
include ../config.mk
include ./unit_test.mk

LIBS= libmsi_pid_create${IRODS_SUFFIX}.so \
      libmsi_pid_delete${IRODS_SUFFIX}.so \
      libmsi_pid_lookup${IRODS_SUFFIX}.so \
      libmsi_pid_update${IRODS_SUFFIX}.so \
      libmsi_pid_get${IRODS_SUFFIX}.so

all: ${LIBS} test_pid
.PONY: all

-include $(patsubst %.so,../obj/%.dep, ${LIBS})

${LIBS}:
	${CXX} ${INC} -DRODS_SERVER ${CXX_FLAGS} -o $@ $(patsubst %${IRODS_SUFFIX}.so, %.cpp, $@) -lcurl
	${CXX} ${INC} -MM -MT $@ -MF $(patsubst %.so,../obj/%.dep, $@) -DRODS_SERVER ${CXX_FLAGS} $(patsubst %${IRODS_SUFFIX}.so, %.cpp, $@)

test_pid: ${OBJ_TEST}
	${CXX_TEST_PREFIX} ${CXX_TEST} ${INC_TEST} ${CXX_TEST_LIB_DIR} ${OBJ_TEST} ${CXX_MAIN_SRC} ${CXX_TEST_LIBS} -o test_pid

${OBJ_TEST}:
	mkdir -p $(dir $@)
	${CXX_TEST} ${INC_TEST}	-c $(patsubst ${OBJECT_DIR}/%.o, ${PREFIX}/%.cpp, $@) -o $@

test: test_pid
	./test_pid
.PONY: test


install:
	cp libmsi_pid_create${IRODS_SUFFIX}.so ${INSTALL_DIR}/libmsi_pid_create.so
	cp libmsi_pid_delete${IRODS_SUFFIX}.so ${INSTALL_DIR}/libmsi_pid_delete.so
	cp libmsi_pid_lookup${IRODS_SUFFIX}.so ${INSTALL_DIR}/libmsi_pid_lookup.so
	cp libmsi_pid_update${IRODS_SUFFIX}.so ${INSTALL_DIR}/libmsi_pid_update.so
	cp libmsi_pid_get${IRODS_SUFFIX}.so ${INSTALL_DIR}/libmsi_pid_get.so


functional_test:
	echo "todo"

package:
	echo "todo"

clean:
	rm -f ${LIBS}
	rm -f ${OBJ_TEST}
	rm -f test_pid
	rm -f $(patsubst %.so,../obj/%.dep, ${LIBS})

allclean:
	rm -f libmsi_pid*.so
