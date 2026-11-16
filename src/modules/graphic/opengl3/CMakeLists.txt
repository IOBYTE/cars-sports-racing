INCLUDE(../../../../cmake/macros.cmake)

SET(OPENGL3_HEADERS  Utils/OsgVectorArrayAdapter.h
                     Utils/OsgAtomic.h
                     Utils/OsgColor.h
                     Utils/OsgMath.h
                     Utils/OsgNodeMask.h
                     Utils/OsgLightTransform.h
                     #Utils/OsgPrecipitation.h
                     #Utils/OsgOptions.h

                     Sky/OsgSphere.h
                     Sky/OsgSun.h
                     Sky/OsgMoon.h
                     Sky/OsgStars.h
                     Sky/OsgDome.h
                     Sky/OsgCloud.h
                     Sky/OsgSky.h

                     Loader/AccGeode.h
                     Loader/AccException.h
                     Loader/ReaderWriterACC.h
                     Loader/OsgLoader.h

                     Scenery/OsgScenery.h

                     Render/OsgRender.h
                     Render/OsgReflectionMapping.h
                     Render/OsgShader.h
                     Render/OsgParticles.h

                     Viewer/OsgView.h
                     Viewer/OsgScreens.h
                     Viewer/OsgCamera.h
                     Viewer/OsgHUD.h
                     Viewer/OsgDebugHUD.h
                     Viewer/OsgGraphicsWindow.h

                     Car/OsgCar.h
                     Car/OsgCarLight.h
                     Car/OsgWheel.h
                     Car/OsgBrake.h
                     #Car/OsgCarShadow.h

                     OpenGL3.h
                     OsgMain.h)

SET(OPENGL3_SOURCES  Utils/OsgMath.cpp
                     Utils/OsgLightTransform.cpp
                     #Utils/OsgPrecipitation.cpp
                     #Utils/OsgOptions.cpp

                     Sky/OsgSphere.cpp
                     Sky/OsgSun.cpp
                     Sky/OsgMoon.cpp
                     Sky/OsgStars.cpp
                     Sky/OsgDome.cpp
                     Sky/OsgCloud.cpp
                     Sky/OsgSky.cpp

                     Loader/AccGeode.cpp
                     Loader/AccException.cpp
                     Loader/ReaderWriterACC.cpp
                     Loader/OsgLoader.cpp

                     Scenery/OsgBackground.cpp
                     Scenery/OsgPit.cpp
                     Scenery/OsgTrackLight.cpp
                     Scenery/OsgScenery.cpp

                     Render/OsgRender.cpp
                     Render/OsgReflectionMapping.cpp
                     Render/OsgShader.cpp
                     Render/OsgParticles.cpp

                     Viewer/OsgView.cpp
                     Viewer/OsgScreens.cpp
                     Viewer/OsgCamera.cpp
                     Viewer/OsgHUD.cpp
                     Viewer/OsgDebugHUD.cpp
                     Viewer/OsgGraphicsWindow.cpp

                     Car/OsgCar.cpp
                     Car/OsgCarLight.cpp
                     Car/OsgWheel.cpp
                     Car/OsgBrake.cpp
                     #Car/OsgCarShadow.cpp

                     OpenGL3.cpp
                     OsgMain.cpp)

INCLUDE_DIRECTORIES(Utils Sky Loader Scenery Render Viewer Car .)

ADD_INTERFACE_INCLUDEDIR()
ADD_CSRLIB_INCLUDEDIR(portability math tgf tgfclient robottools)

ADD_SDL2_INCLUDEDIR()

ADD_OSG_INCLUDEDIR()

# Temporary, but should not remain !
ADD_PLIB_INCLUDEDIR()

IF(COMMAND CMAKE_POLICY)
    CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

IF(WIN32)
    # DLL export stuff under Windows (to avoid .def file)
    ADD_DEFINITIONS(-DOPENGL3_DLL)
ENDIF(WIN32)

IF(MSVC)
    # Ignore some run-time libs to avoid link time warnings and sometimes even crashes.
    SET(CMAKE_MODULE_LINKER_FLAGS_DEBUG
        "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:libcmtd.lib")
    SET(CMAKE_MODULE_LINKER_FLAGS_RELEASE
        "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:libcmt.lib")
ENDIF(MSVC)

# Note: Other sources and headers needed for having them available in IDEs.
CSR_ADD_LIBRARY(opengl3 MODULE ${OPENGL3_SOURCES} ${OPENGL3_HEADERS})

# Might not work with GCC 4.5 or + (non-robot modules crash at 1st reload = after 1 dlclose)
#SET_TARGET_PROPERTIES(osggraph PROPERTIES VERSION ${VERSION} SOVERSION 0.0.0)

ADD_OSG_LIBRARY(opengl3)

ADD_SDL2_LIBRARY(opengl3)

ADD_OPENGL_LIBRARY(opengl3)

# Temporary, but should not remain !
ADD_PLIB_LIBRARY(opengl3 ul sg)

ADD_CSRLIB_LIBRARY(opengl3 tgf tgfclient robottools)

CSR_INSTALL_FILES(LIB modules/graphic TARGETS opengl3)
