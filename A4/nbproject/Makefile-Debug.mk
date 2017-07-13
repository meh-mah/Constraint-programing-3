#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=msvc_caller
CCC=msvc_caller
CXX=msvc_caller
FC=gfortran
AS=as

# Macros
CND_PLATFORM=VCC4N-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/SquarePacking.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-LIBPATH:/C/Program\ Files\ \(x86\)/Gecode/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/a4.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/a4.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -OUT:${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/a4 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/SquarePacking.o: SquarePacking.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -Od -W3 -I /C/Program\ Files\ \(x86\)/Gecode/include -Fo${OBJECTDIR}/SquarePacking.o SquarePacking.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/a4.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
