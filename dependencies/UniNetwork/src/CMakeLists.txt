# Network src
FILE(GLOB SRC "*.cpp")
FILE(GLOB HEADER "../include/*.h")

SET(UniNetworkSrc ${SRC} ${HEADER})
# group
source_group("src" FILES ${SRC})
source_group("include" FILES ${HEADER})

#libs
SET(PA ../../../import/lib)
SET(PA2 ../dependencies/lib)
find_library(POCO_FOUNDATION_DEBUG PocoFoundationd PATHS ${PA2})
find_library(POCO_FOUNDATION PocoFoundation PATHS ${PA2})
find_library(POCO_NET_DEBUG PocoNetd PATHS ${PA2})
find_library(POCO_NET PocoNet PATHS ${PA2})
find_library(POCO_UTIL_DEBUG PocoUtild PATHS ${PA2})
find_library(POCO_UTIL PocoUtil PATHS ${PA2})
find_library(POCO_XML_DEBUG PocoXMLd PATHS ${PA2})
find_library(POCO_XML PocoXML PATHS ${PA2})
find_library(POCO_CRYPTO_DEBUG PocoCryptod PATHS ${PA2})
find_library(POCO_CRYPTO PocoCrypto PATHS ${PA2})
find_library(POCO_NET_SSL_DEBUG PocoNetSSLd PATHS ${PA2})
find_library(POCO_NET_SSL PocoNetSSL PATHS ${PA2})
find_library(POCO_ZIP_DEBUG PocoZipd PATHS ${PA2})
find_library(POCO_ZIP PocoZip PATHS ${PA2})
if(WIN32)
SET(UniNetworkLibs ${POCO_FOUNDATION_DEBUG} ${POCO_NET_DEBUG} DRUniversumLib ${POCO_UTIL_DEBUG} ${POCO_XML_DEBUG} ${POCO_CRYPTO_DEBUG} ${POCO_NET_SSL_DEBUG} ${POCO_ZIP_DEBUG})
else(UNIX)
SET(UniNetworkLibs PocoFoundation PocoUtil PocoXML PocoNet DRUniversumLib)
endif(WIN32)

ADD_LIBRARY(UniNetwork SHARED ${UniNetworkSrc})
TARGET_LINK_LIBRARIES(UniNetwork ${UniNetworkLibs})

set_target_properties(UniNetwork PROPERTIES DEFINE_SYMBOL BUILD_DLL_UNI_NETWORK)
SET_TARGET_PROPERTIES(UniNetwork PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../lib)
SET_TARGET_PROPERTIES(UniNetwork PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../bin)
