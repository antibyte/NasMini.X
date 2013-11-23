#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/NasMini.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/NasMini.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Z80/ConDebug.c Z80/Debug.c Z80/Z80.c Emulation.c Video/ps2.c fatfs/ff.c fatfs/mmcPIC32.c Video/video.c uart.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Z80/ConDebug.o ${OBJECTDIR}/Z80/Debug.o ${OBJECTDIR}/Z80/Z80.o ${OBJECTDIR}/Emulation.o ${OBJECTDIR}/Video/ps2.o ${OBJECTDIR}/fatfs/ff.o ${OBJECTDIR}/fatfs/mmcPIC32.o ${OBJECTDIR}/Video/video.o ${OBJECTDIR}/uart.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Z80/ConDebug.o.d ${OBJECTDIR}/Z80/Debug.o.d ${OBJECTDIR}/Z80/Z80.o.d ${OBJECTDIR}/Emulation.o.d ${OBJECTDIR}/Video/ps2.o.d ${OBJECTDIR}/fatfs/ff.o.d ${OBJECTDIR}/fatfs/mmcPIC32.o.d ${OBJECTDIR}/Video/video.o.d ${OBJECTDIR}/uart.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Z80/ConDebug.o ${OBJECTDIR}/Z80/Debug.o ${OBJECTDIR}/Z80/Z80.o ${OBJECTDIR}/Emulation.o ${OBJECTDIR}/Video/ps2.o ${OBJECTDIR}/fatfs/ff.o ${OBJECTDIR}/fatfs/mmcPIC32.o ${OBJECTDIR}/Video/video.o ${OBJECTDIR}/uart.o

# Source Files
SOURCEFILES=Z80/ConDebug.c Z80/Debug.c Z80/Z80.c Emulation.c Video/ps2.c fatfs/ff.c fatfs/mmcPIC32.c Video/video.c uart.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/NasMini.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX250F128B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Z80/ConDebug.o: Z80/ConDebug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Z80 
	@${RM} ${OBJECTDIR}/Z80/ConDebug.o.d 
	@${RM} ${OBJECTDIR}/Z80/ConDebug.o 
	@${FIXDEPS} "${OBJECTDIR}/Z80/ConDebug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Z80/ConDebug.o.d" -o ${OBJECTDIR}/Z80/ConDebug.o Z80/ConDebug.c    -finline-small-functions
	
${OBJECTDIR}/Z80/Debug.o: Z80/Debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Z80 
	@${RM} ${OBJECTDIR}/Z80/Debug.o.d 
	@${RM} ${OBJECTDIR}/Z80/Debug.o 
	@${FIXDEPS} "${OBJECTDIR}/Z80/Debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Z80/Debug.o.d" -o ${OBJECTDIR}/Z80/Debug.o Z80/Debug.c    -finline-small-functions
	
${OBJECTDIR}/Z80/Z80.o: Z80/Z80.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Z80 
	@${RM} ${OBJECTDIR}/Z80/Z80.o.d 
	@${RM} ${OBJECTDIR}/Z80/Z80.o 
	@${FIXDEPS} "${OBJECTDIR}/Z80/Z80.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Z80/Z80.o.d" -o ${OBJECTDIR}/Z80/Z80.o Z80/Z80.c    -finline-small-functions
	
${OBJECTDIR}/Emulation.o: Emulation.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Emulation.o.d 
	@${RM} ${OBJECTDIR}/Emulation.o 
	@${FIXDEPS} "${OBJECTDIR}/Emulation.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Emulation.o.d" -o ${OBJECTDIR}/Emulation.o Emulation.c    -finline-small-functions
	
${OBJECTDIR}/Video/ps2.o: Video/ps2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Video 
	@${RM} ${OBJECTDIR}/Video/ps2.o.d 
	@${RM} ${OBJECTDIR}/Video/ps2.o 
	@${FIXDEPS} "${OBJECTDIR}/Video/ps2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Video/ps2.o.d" -o ${OBJECTDIR}/Video/ps2.o Video/ps2.c    -finline-small-functions
	
${OBJECTDIR}/fatfs/ff.o: fatfs/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/fatfs 
	@${RM} ${OBJECTDIR}/fatfs/ff.o.d 
	@${RM} ${OBJECTDIR}/fatfs/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/fatfs/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/fatfs/ff.o.d" -o ${OBJECTDIR}/fatfs/ff.o fatfs/ff.c    -finline-small-functions
	
${OBJECTDIR}/fatfs/mmcPIC32.o: fatfs/mmcPIC32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/fatfs 
	@${RM} ${OBJECTDIR}/fatfs/mmcPIC32.o.d 
	@${RM} ${OBJECTDIR}/fatfs/mmcPIC32.o 
	@${FIXDEPS} "${OBJECTDIR}/fatfs/mmcPIC32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/fatfs/mmcPIC32.o.d" -o ${OBJECTDIR}/fatfs/mmcPIC32.o fatfs/mmcPIC32.c    -finline-small-functions
	
${OBJECTDIR}/Video/video.o: Video/video.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Video 
	@${RM} ${OBJECTDIR}/Video/video.o.d 
	@${RM} ${OBJECTDIR}/Video/video.o 
	@${FIXDEPS} "${OBJECTDIR}/Video/video.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Video/video.o.d" -o ${OBJECTDIR}/Video/video.o Video/video.c    -finline-small-functions
	
${OBJECTDIR}/uart.o: uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/uart.o.d 
	@${RM} ${OBJECTDIR}/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/uart.o.d" -o ${OBJECTDIR}/uart.o uart.c    -finline-small-functions
	
else
${OBJECTDIR}/Z80/ConDebug.o: Z80/ConDebug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Z80 
	@${RM} ${OBJECTDIR}/Z80/ConDebug.o.d 
	@${RM} ${OBJECTDIR}/Z80/ConDebug.o 
	@${FIXDEPS} "${OBJECTDIR}/Z80/ConDebug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Z80/ConDebug.o.d" -o ${OBJECTDIR}/Z80/ConDebug.o Z80/ConDebug.c    -finline-small-functions
	
${OBJECTDIR}/Z80/Debug.o: Z80/Debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Z80 
	@${RM} ${OBJECTDIR}/Z80/Debug.o.d 
	@${RM} ${OBJECTDIR}/Z80/Debug.o 
	@${FIXDEPS} "${OBJECTDIR}/Z80/Debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Z80/Debug.o.d" -o ${OBJECTDIR}/Z80/Debug.o Z80/Debug.c    -finline-small-functions
	
${OBJECTDIR}/Z80/Z80.o: Z80/Z80.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Z80 
	@${RM} ${OBJECTDIR}/Z80/Z80.o.d 
	@${RM} ${OBJECTDIR}/Z80/Z80.o 
	@${FIXDEPS} "${OBJECTDIR}/Z80/Z80.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Z80/Z80.o.d" -o ${OBJECTDIR}/Z80/Z80.o Z80/Z80.c    -finline-small-functions
	
${OBJECTDIR}/Emulation.o: Emulation.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Emulation.o.d 
	@${RM} ${OBJECTDIR}/Emulation.o 
	@${FIXDEPS} "${OBJECTDIR}/Emulation.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Emulation.o.d" -o ${OBJECTDIR}/Emulation.o Emulation.c    -finline-small-functions
	
${OBJECTDIR}/Video/ps2.o: Video/ps2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Video 
	@${RM} ${OBJECTDIR}/Video/ps2.o.d 
	@${RM} ${OBJECTDIR}/Video/ps2.o 
	@${FIXDEPS} "${OBJECTDIR}/Video/ps2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Video/ps2.o.d" -o ${OBJECTDIR}/Video/ps2.o Video/ps2.c    -finline-small-functions
	
${OBJECTDIR}/fatfs/ff.o: fatfs/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/fatfs 
	@${RM} ${OBJECTDIR}/fatfs/ff.o.d 
	@${RM} ${OBJECTDIR}/fatfs/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/fatfs/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/fatfs/ff.o.d" -o ${OBJECTDIR}/fatfs/ff.o fatfs/ff.c    -finline-small-functions
	
${OBJECTDIR}/fatfs/mmcPIC32.o: fatfs/mmcPIC32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/fatfs 
	@${RM} ${OBJECTDIR}/fatfs/mmcPIC32.o.d 
	@${RM} ${OBJECTDIR}/fatfs/mmcPIC32.o 
	@${FIXDEPS} "${OBJECTDIR}/fatfs/mmcPIC32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/fatfs/mmcPIC32.o.d" -o ${OBJECTDIR}/fatfs/mmcPIC32.o fatfs/mmcPIC32.c    -finline-small-functions
	
${OBJECTDIR}/Video/video.o: Video/video.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Video 
	@${RM} ${OBJECTDIR}/Video/video.o.d 
	@${RM} ${OBJECTDIR}/Video/video.o 
	@${FIXDEPS} "${OBJECTDIR}/Video/video.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/Video/video.o.d" -o ${OBJECTDIR}/Video/video.o Video/video.c    -finline-small-functions
	
${OBJECTDIR}/uart.o: uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/uart.o.d 
	@${RM} ${OBJECTDIR}/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -O1 -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/uart.o.d" -o ${OBJECTDIR}/uart.o uart.c    -finline-small-functions
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/NasMini.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O2 -mno-float -o dist/${CND_CONF}/${IMAGE_TYPE}/NasMini.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=2048,--defsym=_min_stack_size=2048,--gc-sections,-s
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/NasMini.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O2 -mno-float -o dist/${CND_CONF}/${IMAGE_TYPE}/NasMini.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,--defsym=_min_stack_size=2048,--gc-sections,-s
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/NasMini.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
