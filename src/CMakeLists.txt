# UniversumLib src
cmake_minimum_required(VERSION 2.6)


FILE(GLOB HEADER "../include/*.h")
FILE(GLOB MODEL_HEADER "../include/model/*.h")
FILE(GLOB MODEL_GEOMETRIE_HEADER "../include/model/geometrie/*.h")
FILE(GLOB VIEW_HEADER "../include/view/*.h")
FILE(GLOB VIEW_GEOMETRIE_HEADER "../include/view/geometrie/*.h")
FILE(GLOB CONTROLLER_HEADER "../include/controller/*.h")
FILE(GLOB GENERATOR_HEADER "../include/generator/*.h")
FILE(GLOB SERVER_HEADER "../include/server/*.h")
FILE(GLOB LIB_HEADER "../include/lib/*.h")

FILE(GLOB SRC "*.cpp")
FILE(GLOB MODEL_SRC "model/*.cpp")
FILE(GLOB MODEL_GEOMETRIE_SRC "model/geometrie/*.cpp")
FILE(GLOB VIEW_SRC "view/*.cpp")
FILE(GLOB VIEW_GEOMETRIE_SRC "view/geometrie/*.cpp")
FILE(GLOB CONTROLLER_SRC "controller/*.cpp")
FILE(GLOB GENERATOR_SRC "generator/*.cpp")
FILE(GLOB SERVER_SRC "server/*.cpp")
FILE(GLOB LIB_SRC "lib/*.cpp")

SET(DRUniversumLibSrc ${SRC} ${HEADER} ${MODEL_SRC} ${MODEL_HEADER} ${VIEW_SRC} ${VIEW_HEADER} ${CONTROLLER_SRC} ${CONTROLLER_HEADER}
				      ${GENERATOR_SRC} ${GENERATOR_HEADER} ${SERVER_SRC} ${SERVER_HEADER} ${LIB_HEADER} ${LIB_SRC} ${MODEL_GEOMETRIE_HEADER} ${MODEL_GEOMETRIE_SRC} ${VIEW_GEOMETRIE_HEADER} ${VIEW_GEOMETRIE_SRC})


if(MSVC)

# src
source_group("src\\model\\geometrie" FILES ${MODEL_GEOMETRIE_SRC})
source_group("src\\model" FILES ${MODEL_SRC} )
source_group("src\\view\\geometrie" FILES ${VIEW_GEOMETRIE_SRC})
source_group("src\\view" FILES ${VIEW_SRC} )
source_group("src\\controller" FILES ${CONTROLLER_SRC} )
source_group("src\\generator" FILES ${GENERATOR_SRC} )
source_group("src\\server" FILES ${SERVER_SRC} )
source_group("src\\lib" FILES ${LIB_SRC} )
source_group("src" FILES ${SRC})
# include
source_group("include\\model\\geometrie" FILES ${MODEL_GEOMETRIE_HEADER})
source_group("include\\model" FILES ${MODEL_HEADER} )
source_group("include\\view\\geometrie" FILES ${VIEW_GEOMETRIE_HEADER})
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

find_library(SDL2_LIB SDL2 PATHS ../import/lib REQUIRED)

if(WIN32)
SET(DRUniversumLibLibs json DRCore2 ${SDL2_LIB})
else(UNIX)
SET(DRUniversumLibLibs json DRCore2 SDL2)
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
