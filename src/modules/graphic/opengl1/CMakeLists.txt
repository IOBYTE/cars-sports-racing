INCLUDE(../../../../cmake/macros.cmake)

SET(OPENGL1_HEADERS  grMoonpos.h grSky.h
                     grSphere.h #grStars.h
					 #grwavesystem.h
                     grboard.h grcam.h grcar.h
                     grcarlight.h grmain.h grsimplestate.h
                     grmultitexstate.h grloadac.h grssgext.h
                     grscene.h grbackground.h grscreen.h
                     grshadow.h
                     #grshadowvolume.h
                     grskidmarks.h
                     grsmoke.h grtexture.h
                     #grRenderTexture.h
                     loadsgi.h
                     grtracklight.h grtrackmap.h grutil.h
                     grvtxtable.h grrain.h
                     #grSharedPtr.h grReferenced.h
                     opengl1.h)

SET(OPENGL1_SOURCES  grMoonpos.cpp
                     grMoon.cpp grSun.cpp grCloudLayer.cpp grSky.cpp
                     grSkyDome.cpp grSphere.cpp grStars.cpp
					 grboard.cpp grcam.cpp grcar.cpp
                     grcarlight.cpp grmain.cpp
                     grsimplestate.cpp grmultitexstate.cpp grloadac.cpp
                     grscene.cpp grbackground.cpp grscreen.cpp
                     grshadow.cpp
                     #grshadowvolume.cpp
                     grskidmarks.cpp
                     grsmoke.cpp grtexture.cpp
                     #grRenderTexture.cpp
                     grtracklight.cpp grtrackmap.cpp grutil.cpp
                     grvtxtable.cpp grrain.cpp
                     opengl1.cpp)

SET(OPENGL1_OTHER_SOURCES )

IF(NOT APPLE AND NOT HAIKU)
    SET(OPENGL1_HEADERS ${OPENGL1_HEADERS} grshader.h)
	SET(OPENGL1_SOURCES ${OPENGL1_SOURCES} grshader.cpp)
ENDIF(NOT APPLE AND NOT HAIKU)

ADD_INTERFACE_INCLUDEDIR()
ADD_CSRLIB_INCLUDEDIR(portability math tgf tgfclient robottools)

ADD_PLIB_INCLUDEDIR()

ADD_SDL2_INCLUDEDIR()

IF(COMMAND CMAKE_POLICY)
    CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

IF(WIN32)
    # DLL export stuff under Windows (to avoid .def file)
    ADD_DEFINITIONS(-DOPENGL1_DLL)
ENDIF(WIN32)

IF(MSVC)
    # Ignore some run-time libs to avoid link time warnings and sometimes even crashes.
    SET(CMAKE_MODULE_LINKER_FLAGS_DEBUG
        "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:libcmtd.lib")
    SET(CMAKE_MODULE_LINKER_FLAGS_RELEASE
        "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:libcmt.lib")
ENDIF(MSVC)


# Note: Other sources and headers needed for having them available in IDEs.
CSR_ADD_LIBRARY(opengl1 MODULE ${OPENGL1_SOURCES} ${OPENGL1_HEADERS} ${OPENGL1_OTHER_SOURCES})

# Might not work with GCC 4.5 or + (non-robot modules crash at 1st reload = after 1 dlclose)
#SET_TARGET_PROPERTIES(ssggraph PROPERTIES VERSION ${VERSION} SOVERSION 0.0.0)

ADD_PLIB_LIBRARY(opengl1 ul sg ssg ssgaux)

ADD_SDL2_LIBRARY(opengl1)

ADD_OPENGL_LIBRARY(opengl1)

ADD_CSRLIB_LIBRARY(opengl1 portability tgf tgfclient robottools)

CSR_INSTALL_FILES(LIB modules/graphic TARGETS opengl1)
