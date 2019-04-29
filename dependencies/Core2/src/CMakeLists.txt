
SET(CoreSrc 
	Core2Main.cpp DRCubicSpline.cpp DRFile.cpp DRFileManager.cpp DRHashList.cpp
	DRLogger.cpp DRPack2D DRUtils.cpp DRMatrix.cpp DRVector2.cpp DRVector2i DRDampingSpring.cpp DRIni.cpp
    DRVector3.cpp DRVector3i DRObjekt.cpp DRFrustumCulling DRPlane DRIndexReferenzHolder DRRandom
	DRResourcePtr DRResourcePtrHolder DRGameStateManager DRBoundingBox DRBoundingBoxi DRBezierCurve DRVirtualFile)


	
#IF(WIN32)
SET(CoreSrc ${CoreSrc} ../include/Core2Main.h ../include/DRFile.h ../include/DRFileManager.h ../include/DRHash.hpp ../include/DRHashList.hpp
                       ../include/DRLogger.h ../include/DRPack2D.h ../include/DRUtils.h 
					   ../include/DRVector2.h ../include/DRVector2i.h ../include/DRVector3.h ../include/DRVector3i.h ../include/DRMatrix.h
					   ../include/DRColor.h ../include/DRPlane.h ../include/DRDampingSpring.h ../include/DRIni.h
                       ../include/DRObjekt.h ../include/DRFrustumCulling.h ../include/DRIndexReferenzHolder.h
					   ../include/DRRandom.h ../include/DRGameStateManager.h
					   ../include/DRResourcePtr.h ../include/DRResourcePtrHolder.h ../include/DRIResource.h
					   ../include/DRCubicSpline ../include/DRBoundingBox ../include/DRBoundingBoxi ../include/DRBezierCurve
					   ../include/DRVirtualFile)
#ENDIF(WIN32)
ADD_LIBRARY(DRCore2 SHARED ${CoreSrc})

IF(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -mavx")
ENDIF(UNIX)

INSTALL(TARGETS DRCore2
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

set_target_properties(DRCore2 PROPERTIES DEFINE_SYMBOL BUILD_CORE2_DLL)
SET_TARGET_PROPERTIES(DRCore2 PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../bin)
SET_TARGET_PROPERTIES(DRCore2 PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../bin)
SET_TARGET_PROPERTIES(DRCore2 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../lib)
