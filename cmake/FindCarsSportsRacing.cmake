#This file can be distributed with packages such as cars, robots, tracks
#It tries to locate a installed SD version and includes the distributed macro's

SET(CSR_PREFIX CACHE PATH "Prefix where Cars Sports Racing is installed")
SET(CSR_DATADIR CACHE PATH "Place where the data is installed")
FIND_FILE(CSR_CMAKE_MACROS cars-sports-racing.cmake PATHS ${CSR_PREFIX} ${CSR_PREFIX}/${CSR_DATADIR} ${CSR_DATADIR} /usr /usr/local PATH_SUFFIXES cmake share/cmake share/games/cars-sports-racing/cmake DOC "Place where Cars Sports Racing is installed")
IF(NOT CSR_CMAKE_MACROS)
	MESSAGE(FATAL_ERROR "Didn't find Cars Sports Racing. Please specify the location with the CSR_PREFIX or CSR_DATADIR path.")
ENDIF(NOT CSR_CMAKE_MACROS)

INCLUDE(${CSR_CMAKE_MACROS})
