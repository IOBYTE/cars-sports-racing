INCLUDE(../../../cmake/macros.cmake)

#PROJECT(engineopengl1)

SET(SG sg)
SET(SG_SOURCES   ${SG}/sg.cpp ${SG}/sgd.cpp ${SG}/sgisect.cpp  
							${SG}/sgdisect.cpp ${SG}/sgperlinnoise.cpp)
SET(SG_HEADERS	${SG}/sg.h)

SET(SKY sky)
SET(SKY_SOURCES ${SKY}/sgCloudLayer.cpp ${SKY}/sgMoon.cpp ${SKY}/sgMoonpos.cpp
				${SKY}/sgSky.cpp ${SKY}/sgSkyDome.cpp ${SKY}/sgSphere.cpp
				${SKY}/sgStars.cpp ${SKY}/sgSun.cpp)

SET(SKY_HEADERS ${SKY}/sgMoonpos.h ${SKY}/sgSphere.h ${SKY}/sgSky.h)

SET(UL utils)
SET(UL_SOURCES   ${UL}/ul.cpp ${UL}/ulclock.cpp ${UL}/ullinkedlist.cpp  
							${UL}/ullist.cpp ${UL}/ulrtti.cpp)

SET(UL_HEADERS   ${UL}/ul.h ${UL}/ullocal.h ${UL}/ulrtti.h)

SET(ENGINEOPENGL1_SOURCES ${SG_SOURCES} ${SKY_SOURCES} ${UL_SOURCES})

SET(ENGINEOPENGL1_HEADERS ${SG_HEADERS} ${SKY_HEADERS} ${UL_HEADERS})

INCLUDE_DIRECTORIES(. ${SG} ${SKY} ${UL})
                       
ADD_CSRLIB_INCLUDEDIR(portability tgf math)

IF(WIN32)
    # DLL export stuff under Windows (to avoid .def file)
    ADD_DEFINITIONS(-DENGINEOPENGL1_DLL)
ENDIF(WIN32)
    
IF(MSVC)
    # Ignore some run-time libs to avoid link time warnings and sometimes even crashes.
    SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt.lib")
ENDIF(MSVC)

# Disable developer warning
IF (COMMAND CMAKE_POLICY)
    CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

# Note: Headers needed for having them available in IDEs.
CSR_ADD_LIBRARY(engineopengl1 SHARED ${ENGINEOPENGL1_SOURCES} ${ENGINEOPENGL1_HEADERS})

# Might not work with GCC 4.5 or + (non-robot modules crash at 1st reload = after 1 dlclose) 
#SET_TARGET_PROPERTIES(engineopengl1 PROPERTIES VERSION ${VERSION} SOVERSION 0.0.0)

ADD_CSRLIB_LIBRARY(engineopengl1 portability tgf)

IF(WIN32)
    CSR_INSTALL_FILES(BIN TARGETS engineopengl1)
ELSE(WIN32)
    CSR_INSTALL_FILES(LIB lib TARGETS engineopengl1)
ENDIF(WIN32)

CSR_INSTALL_FILES(INCLUDE FILES ${ENGINEOPENGL1_HEADERS})
