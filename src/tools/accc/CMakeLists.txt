INCLUDE(../../../cmake/macros.cmake)

PROJECT(accc)

SET(_TARGET_NAME ${CSR_TOOLS_EXECPREFIX}accc)

SET(ACCC_SOURCES ac3dgroup.cpp ac3dload.cpp mainaccc.cpp)
SET(ACCC_HEADERS accc.h)

ADD_CSRLIB_INCLUDEDIR(portability)

# Note: Headers needed for having them available in IDEs.
CSR_ADD_EXECUTABLE(${_TARGET_NAME} ${ACCC_SOURCES} ${ACCC_HEADERS})

ADD_CSRLIB_LIBRARY(${_TARGET_NAME} portability)

IF(UNIX)
    CSR_INSTALL_FILES(MAN man6 PREFIX ${SOURCE_DIR}/doc/man FILES ${_TARGET_NAME}.6)
ENDIF(UNIX)

CSR_INSTALL_FILES(BIN TARGETS ${_TARGET_NAME})
