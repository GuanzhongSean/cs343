OPT := -O2
VIMPL := EXT
OUTPUT := OUTPUT
BCHECK := BARGINGCHECK

CXX = u++ # compiler
CXXFLAGS = -quiet  -g -multi -Wall -Wextra -MMD ${OPT} -D"${VIMPL}"\
		-D"${OUTPUT}" -D"${BCHECK}" -Wno-unused-parameter -Wno-comment # compiler flags
ifeq ("${OUTPUT}","NOOUTPUT")
	CXXFLAGS += -O2 -nodebug
endif
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = q3tallyVotes${VIMPL}.o q3main.o q3printer.o q3voter.o
EXEC = vote

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean
.ONESHELL :

all : ${EXEC}					# build all executables

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
	touch q3tallyVotes.h
	${MAKE} ${EXEC} VIMPL="${VIMPL}" OUTPUT="${OUTPUT}" BCHECK="${BCHECK}"
endif

VoteImpl :
	echo "LOCKVIMPL=${VIMPL}\nOUTPUTTYPE=${OUTPUT}\nBCHECKIMPL=${BCHECK}" > VoteImpl
	sleep 1

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -rf *.d q3tallyVotes*.o ${OBJECTS} ${EXEC} VoteImpl test_results/ a.out

test:
	python q3test.py

q3b: clean
	${MAKE} ${EXEC} VIMPL="${VIMPL}" OUTPUT=NOOUTPUT BCHECK=NOBARGINGCHECK
	/usr/bin/time -f "%Uu %Ss %Er %Mkb" ./vote 100 10 50000 1003
	/usr/bin/time -f "%Uu %Ss %Er %Mkb" ./vote 100 10 50000 1003 2

submit:
	submit cs343 5 .
