# UniversumLib src
cmake_minimum_required(VERSION 2.6)

FILE(GLOB SRC "*.cpp")
FILE(GLOB HEADER "../include/*.h")
FILE(GLOB MODEL_SRC "model/*.cpp")
FILE(GLOB MODEL_HEADER "../include/model/*.h")

SET(DRUniversumLibSrc ${SRC} ${HEADER} ${MODEL_SRC} ${MODEL_HEADER})

if(MSVC)

# src
source_group("src\\model" FILES ${MODEL_SRC} )
source_group("src" FILES ${SRC})
# include
source_group("include\\model" FILES ${MODEL_HEADER} )
source_group("include" FILES ${HEADER})
#source_group("include" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/${REC2}" )
endif(MSVC)

#ENABLE_EXPORTS()
#DEFINE_SYMBOL(BUILD_DLL)

find_library(DR_CORE2_LIB DRCore2 PATHS ../import/lib REQUIRED)
if(WIN32)
SET(DRUniversumLibLibs ${DR_CORE2_LIB})
else(UNIX)
SET(DRUniversumLibLibs DRCore2)
endif(WIN32)


ADD_LIBRARY(DRUniversumLib SHARED ${DRUniversumLibSrc})
TARGET_LINK_LIBRARIES(DRUniversumLib ${DRUniversumLibLibs})

INSTALL(TARGETS DRUniversumLib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

set_target_properties(DRUniversumLib PROPERTIES DEFINE_SYMBOL BUILD_UNIVERSUM_LIB_DLL)
SET_TARGET_PROPERTIES(DRUniversumLib PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib)
SET_TARGET_PROPERTIES(DRUniversumLib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../lib)
