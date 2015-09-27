#Etext (Engine Test)
cmake_minimum_required(VERSION 2.6)

FILE(GLOB SRC "*.cpp")
FILE(GLOB HEADER "include/*.h")

SET(COMPLETE_SRC ${SRC} ${HEADER})

if(MSVC)

# src
source_group("src" FILES ${SRC})
# include
source_group("include" FILES ${HEADER})
#source_group("include" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/${REC2}" )
endif(MSVC)

#ENABLE_EXPORTS()
#DEFINE_SYMBOL(__TEST)


if(WIN32)
SET(LIBS DRCore2 DRUniversumLib)
else(UNIX)
SET(LIBS DRCore2 DRUniversumLib)
endif(WIN32)

ADD_EXECUTABLE(Test ${COMPLETE_SRC})
TARGET_LINK_LIBRARIES(Test ${LIBS})

SET_TARGET_PROPERTIES(Test PROPERTIES EXECUTABLE_OUTPUT_PATH ../import/bin)

