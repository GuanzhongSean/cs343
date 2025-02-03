OPT := # -O2

CXX = u++					# compiler
CXXFLAGS = -quiet -g -Wall -Wextra -MMD ${OPT}
Q3FLAGS = ${CXXFLAGS} -multi -O3 -nodebug
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = q1main.o q1filter.o q1reader.o q1writer.o
EXEC1 = filter

OBJECTS2 = q2cardgame.o q2player.o
EXEC2 = cardgame

OBJECTS3 = q3.o
EXEC3 = q3

OBJECTS = ${OBJECTS1} ${OBJECTS2}		# all object files
EXECS = ${EXEC1} ${EXEC2}			# all executables
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
DEPENDS += ${OBJECTS3:.o=.d}

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

${OBJECTS3} : q3.cc					# compile step 3rd object
	${CXX} ${Q3FLAGS} -c $< -o $@

${EXEC3} : ${OBJECTS3}				# link step 3rd executable
	${CXX} ${Q3FLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -rf ${DEPENDS} ${OBJECTS} ${OBJECTS3} ${EXECS} ${EXEC3} test_results/

submit:
	submit cs343 2 .
