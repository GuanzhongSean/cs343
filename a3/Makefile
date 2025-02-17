OPT := # -O3
STYPE := int
SIMPL := CBEGIN
BIMPL := NOBUSY
BCHECK := NOBARGINGCHECK

CXX = u++					# compiler
CXXFLAGS = -quiet -g -Wall -Wextra -multi -MMD ${OPT} -DSTYPE="${STYPE}" -D"${SIMPL}" \
		-D"${BIMPL}" -D"${BCHECK}"	# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = q2main.o # object files forming 1st executable with prefix "q2"
EXEC1 = quicksort				# 1st executable name

OBJECTS2 = # object files forming 2nd executable with prefix "q3"
EXEC2 = buffer					# 2nd executable name

OBJECTS3 = q1.o                    # object files for q1
EXEC3 = q1               # 3rd executable name for q1

OBJECTS = ${OBJECTS1} ${OBJECTS2}		# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2}			# all executables

#############################################################

.PHONY : all clean q1stack q1darray q1vector1 q1vector2 submit
.ONESHELL :

all : ${EXECS}					# build all executables

q1stack: CXXFLAGS = -quiet -O2 -multi -nodebug -DSTACK
q1stack: ${EXEC3}

q1darray: CXXFLAGS = -quiet -O2 -multi -nodebug -DDARRAY
q1darray: ${EXEC3}

q1vector1: CXXFLAGS = -quiet -O2 -multi -nodebug -DVECTOR1
q1vector1: ${EXEC3}

q1vector2: CXXFLAGS = -quiet -O2 -multi -nodebug -DVECTOR2
q1vector2: ${EXEC3}

${EXEC3}: ${OBJECTS3}
	${CXX} ${CXXFLAGS} -o $@ $^

-include QuickImpl

# same implementation concurrency/type as last time ?
ifeq (${shell if [ "${IMPLSTYPE}" = "${STYPE}" -a "${IMPLSIMPL}" = "${SIMPL}" ] ; \
		then echo true ; fi },true)
${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
else						# implementation type has changed => rebuilt
.PHONY : ${EXEC1}
${EXEC1} :
	rm -f QuickImpl
	touch q2quicksort.h
	${MAKE} ${EXEC1} STYPE="${STYPE}" SIMPL="${SIMPL}"
endif

QuickImpl :
	echo "IMPLSTYPE=${STYPE}\nIMPLSIMPL=${SIMPL}" > QuickImpl
	sleep 1

-include BufImpl

# same implementation concurrency/type as last time ?
ifeq (${shell if [ "${BUFIMPL}" = "${BIMPL}" -a "${BCHECKIMPL}" = "${BCHECK}" ] ; \
		then echo true ; fi },true)
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@
else						# implementation type has changed => rebuilt
.PHONY : ${EXEC2}
${EXEC2} :
	rm -f BufImpl
	touch q3buffer.h
	sleep 1
	${MAKE} ${EXEC2} BIMPL="${BIMPL}" BCHECK="${BCHECK}"
endif

BufImpl :
	echo "BUFIMPL=${BIMPL}\nBCHECKIMPL=${BCHECK}" > BufImpl
	sleep 1

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS} ${EXEC3} QuickImpl BufImpl

submit:
	submit cs343 3 .
