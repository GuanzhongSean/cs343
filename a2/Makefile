OPT := # -O2

CXX = u++					# compiler
CXXFLAGS = -quiet -g -Wall -Wextra -MMD ${OPT}
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = q1main.o q1filter.o q1reader.o q1writer.o
EXEC1 = filter

OBJECTS2 = # object files forming 2nd executable with prefix "q2"
EXEC2 = cardgame

OBJECTS = ${OBJECTS1} ${OBJECTS2}		# all object files
EXECS = ${EXEC1} ${EXEC2}			# all executables
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

ifeq (${CXX},cfa)
# strip removes whitespace between value and comment
TOUCH := $(strip ${TOUCH})fa
.SUFFIXES :					# remove other default suffix rules
.SUFFIXES : .cfa .o				# add CFA default rules
.cfa.o :
	${CXX} ${CXXFLAGS} -c $<
endif

#############################################################

.PHONY : all clean submit
.ONESHELL :

all : ${EXECS}					# build all executables

${EXEC1} : ${OBJECTS1}				# link step 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

${EXEC2} : ${OBJECTS2}				# link step 2nd executable
	${CXX} ${CXXFLAGS} $^ -o $@ q2printer.o

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXECS} test_results/

submit:
	submit cs343 2 .
