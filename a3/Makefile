OPT := # -O3
STYPE := int
SIMPL := CBEGIN
BIMPL := NOBUSY
BCHECK := NOBARGINGCHECK

CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wextra -multi ${OPT} -MMD -DSTYPE="${STYPE}" -D"${SIMPL}" \
		-D"${BIMPL}" -D"${BCHECK}"	# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = # object files forming 1st executable with prefix "q2"
EXEC1 = quicksort				# 1st executable name

OBJECTS2 = # object files forming 2nd executable with prefix "q3"
EXEC2 = buffer					# 2nd executable name

OBJECTS = ${OBJECTS1} ${OBJECTS2}		# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2}			# all executables

#############################################################

.PHONY : all clean
.ONESHELL :

all : ${EXECS}					# build all executables

-include MergeImpl

# same implementation concurrency/type as last time ?
ifeq (${shell if [ "${IMPLSTYPE}" = "${STYPE}" -a "${IMPLSIMPL}" = "${SIMPL}" ] ; \
		then echo true ; fi },true)
${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
else						# implementation type has changed => rebuilt
.PHONY : ${EXEC1}
${EXEC1} :
	rm -f MergeImpl
	touch q2quicksort.h
	${MAKE} ${EXEC1} STYPE="${STYPE}" SIMPL="${SIMPL}"
endif

MergeImpl :
	echo "IMPLSTYPE=${STYPE}\nIMPLSIMPL=${SIMPL}" > MergeImpl
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
	rm -f *.d *.o ${EXECS} MergeImpl BufImpl
