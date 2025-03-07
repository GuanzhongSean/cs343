OPT := # -O2
Q1IMPL := ARRAY
VIMPL := MC
OUTPUT := OUTPUT
BCHECK := BARGINGCHECK

CXX = u++					# compiler
CXXFLAGS = -quiet  -g -multi -Wall -Wextra -MMD ${OPT} -D"${Q1IMPL}" -D"${VIMPL}"\
		-D"${OUTPUT}" -D"${BCHECK}" # compiler flags
ifeq ("${OUTPUT}","NOOUTPUT")
	CXXFLAGS += -O2 -nodebug
endif
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECT1 = q1.o
EXEC1 = q1

OBJECTS = q2tallyVotes${VIMPL}.o
EXEC = vote

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean
.ONESHELL :

all : ${EXEC}					# build all executables

${EXEC1} : ${OBJECT1}
	${CXX} ${CXXFLAGS} $^ -o $@
	/usr/bin/time -f "%Uu %Ss %Er %Mkb" ./q1 10000000 20

-include VoteImpl

# same implementation concurrency/type as last time ?
ifeq (${shell if [ "${LOCKVIMPL}" = "${VIMPL}" -a "${OUTPUTTYPE}" = "${OUTPUT}" \
		-a "${BCHECKIMPL}" = "${BCHECK}" ] ; then echo true ; fi },true)
${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@
else						# implementation type has changed => rebuilt
.PHONY : ${EXEC}
${EXEC} :
	rm -f VoteImpl
	touch q2tallyVotes.h
	${MAKE} ${EXEC} VIMPL="${VIMPL}" OUTPUT="${OUTPUT}" BCHECK="${BCHECK}"
endif

VoteImpl :
	echo "LOCKVIMPL=${VIMPL}\nOUTPUTTYPE=${OUTPUT}\nBCHECKIMPL=${BCHECK}" > VoteImpl
	sleep 1

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -rf *.d ${OBJECT1} ${OBJECTS} ${EXEC1} ${EXEC} VoteImpl test_results/

test:
	python q2test.py

submit:
	submit cs343 4 .
