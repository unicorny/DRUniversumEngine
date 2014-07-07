# UniversumLib src
cmake_minimum_required(VERSION 2.6)

FILE(GLOB SRC "*.cpp")
FILE(GLOB HEADER "../include/*.h")
FILE(GLOB MODEL_SRC "model/*.cpp")
FILE(GLOB MODEL_HEADER "../include/model/*.h")
FILE(GLOB VIEW_SRC "view/*.cpp")
FILE(GLOB VIEW_HEADER "../include/view/*.h")
FILE(GLOB CONTROLLER_SRC "controller/*.cpp")
FILE(GLOB CONTROLLER_HEADER "../include/controller/*.h")
FILE(GLOB GENERATOR_SRC "generator/*.cpp")
FILE(GLOB GENERATOR_HEADER "../include/generator/*.h")
FILE(GLOB SERVER_SRC "server/*.cpp")
FILE(GLOB SERVER_HEADER "../include/server/*.h")
FILE(GLOB LIB_HEADER "../include/lib/*.h")
FILE(GLOB LIB_SRC "lib/*.cpp")

SET(DRUniversumLibSrc ${SRC} ${HEADER} ${MODEL_SRC} ${MODEL_HEADER} ${VIEW_SRC} ${VIEW_HEADER} ${CONTROLLER_SRC} ${CONTROLLER_HEADER}
				      ${GENERATOR_SRC} ${GENERATOR_HEADER} ${SERVER_SRC} ${SERVER_HEADER} ${LIB_HEADER} ${LIB_SRC})

if(MSVC)

# src
source_group("src\\model" FILES ${MODEL_SRC} )
source_group("src\\view" FILES ${VIEW_SRC} )
source_group("src\\controller" FILES ${CONTROLLER_SRC} )
source_group("src\\generator" FILES ${GENERATOR_SRC} )
source_group("src\\server" FILES ${SERVER_SRC} )
source_group("src\\lib" FILES ${LIB_SRC} )
source_group("src" FILES ${SRC})
# include
source_group("include\\model" FILES ${MODEL_HEADER} )
source_group("include\\view" FILES ${VIEW_HEADER} )
source_group("include\\controller" FILES ${CONTROLLER_HEADER} )
source_group("include\\generator" FILES ${GENERATOR_HEADER} )
source_group("include\\server" FILES ${SERVER_HEADER} )
source_group("include\\lib" FILES ${LIB_HEADER} )
source_group("include" FILES ${HEADER})
#source_group("include" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/${REC2}" )
endif(MSVC)

#ENABLE_EXPORTS()
#DEFINE_SYMBOL(BUILD_DLL)

find_library(DR_CORE2_LIB DRCore2 PATHS ../import/lib REQUIRED)
find_library(SDL2_LIB SDL2 PATHS ../import/lib REQUIRED)
find_library(GMP10_LIB "libgmp-10" PATHS ../import/lib REQUIRED)
find_library(MPFR4_LIB "libmpfr-4" PATHS ../import/lib REQUIRED)
if(WIN32)
SET(DRUniversumLibLibs json ${DR_CORE2_LIB} ${SDL2_LIB} ${GMP10_LIB} ${MPFR4_LIB})
else(UNIX)
SET(DRUniversumLibLibs json DRCore2 SDL2 gmp-10 mpfr-4)
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
