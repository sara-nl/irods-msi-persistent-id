PREFIX=../
include ../config.mk

LIBS= libmsi_pid_create${IRODS_SUFFIX}.so

# libmsi_pid_delete${IRODS_SUFFIX}.so \
#      libmsi_pid_update${IRODS_SUFFIX}.so \
#      libmsi_pid_lookup${IRODS_SUFFIX}.so \
#      libmsi_pid_get${IRODS_SUFFIX}.so

all: ${LIBS}
.PONY: all

-include $(patsubst %.so,../obj/%.dep, ${LIBS})

${LIBS}:
	${CXX} ${INC} -DRODS_SERVER ${CXX_FLAGS} -o $@ $(patsubst %${IRODS_SUFFIX}.so, %.cpp, $@) -lcurl
	${CXX} ${INC} -MM -MT $@ -MF $(patsubst %.so,../obj/%.dep, $@) -DRODS_SERVER ${CXX_FLAGS} $(patsubst %${IRODS_SUFFIX}.so, %.cpp, $@)

#-Wno-deprecated

install:
	cp libmsi_pid_create${IRODS_SUFFIX}.so ${INSTALL_DIR}/libmsi_pid_create.so
#	cp libmsi_pid_update${IRODS_VERSION}.so ${INSTALL_DIR}/libmsi_pid_update.so
#	cp libmsi_pid_get${IRODS_VERSION}.so ${INSTALL_DIR}/libmsi_pid_get.so
#	cp libmsi_pid_delete${IRODS_VERSION}.so ${INSTALL_DIR}/libmsi_pid_delete.so
#	cp libmsi_pid_lookup${IRODS_VERSION}.so ${INSTALL_DIR}/libmsi_pid_lookup.so

functional_test:
	echo "todo"

package:
	echo "todo"

clean:
	rm -f ${LIBS}
	rm -f $(patsubst %.so,../obj/%.dep, ${LIBS})

allclean:
	rm -f libmsi_pid*.so
