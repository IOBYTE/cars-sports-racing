include(${CMAKE_SOURCE_DIR}/cmake/macros.cmake)

#PROJECT(csnetworking)

find_package(SDL2 REQUIRED)
find_package(ENET REQUIRED)
SET(CSR_NET_SOURCES localtime.cpp netasync.cpp netclient.cpp netdriver.cpp netserver.cpp pack.cpp)
SET(CSR_NET_HEADERS localtime.h netasync.h netclient.h netdriver.h netplayer.h netserver.h network.h pack.h)

#disable developer warning
if (COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# Note: Headers needed for having them available in IDEs.
#TODO: Maje this a real (dynamically loadable) module, or move it to standard src/libs !
CSR_ADD_LIBRARY(net SHARED ${CSR_NET_SOURCES} ${CSR_NET_HEADERS})

# Might not work with GCC 4.5 or + (non-robot modules crash at 1st reload = after 1 dlclose)
#SET_TARGET_PROPERTIES(csnetworking PROPERTIES VERSION ${VERSION} SOVERSION 0.0.0)

ADD_INTERFACE_INCLUDEDIR()

ADD_SDL2_INCLUDEDIR()

ADD_CSRLIB_INCLUDEDIR(csnetworking portability tgf math)

ADD_PLIB_INCLUDEDIR()
ADD_ENET_INCLUDEDIR()

ADD_PLIB_LIBRARY(net ul sg)
ADD_ENET_LIBRARY(net)
ADD_SDL2_LIBRARY(net)

IF(WIN32)
	TARGET_LINK_LIBRARIES(net Winmm ws2_32)
ENDIF(WIN32)

IF(MSVC)
	# Ignore some run-time libs to avoid link time warnings and sometimes even crashes.
	SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG
		"${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:libcmt.lib")
	SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE
		"${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:libcmt.lib")
ENDIF(MSVC)


ADD_CSRLIB_LIBRARY(net portability tgf)

IF(WIN32)
	CSR_INSTALL_FILES(BIN TARGETS net)
ELSE(WIN32)
	CSR_INSTALL_FILES(LIB lib TARGETS net)
ENDIF(WIN32)
