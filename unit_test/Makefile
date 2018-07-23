PREFIX=../
OBJ_TEST=

include ../config.mk
include ../msi_wordcount/unit_test.mk
include ../msi_pid/unit_test.mk

OBJ_TEST+= ${OBJECT_DIR}/unit_test/unit_test.o

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
