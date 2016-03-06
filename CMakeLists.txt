# Engine2
cmake_minimum_required(VERSION 2.6)

PROJECT(DRUniversumLib CXX)

#build with multicore support
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)


SUBDIRS(src test dependencies/jsoncpp dependencies/Core2 dependencies/UniNetwork dependencies/Image)
#ADD_SUBDIRECTORY(dependencies/Core2 dependencies/Network)
INCLUDE_DIRECTORIES(import/include import/bin import/lib)
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(lib)
INCLUDE_DIRECTORIES(bin)

#CMAKE_LIBRARY_PATH()



#ADD_LIBRARY(Core2 SHARED core2main.cpp)
#ADD_EXECUTABLE(test ${testSrc})
