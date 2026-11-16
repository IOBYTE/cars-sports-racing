INCLUDE(../../../cmake/macros.cmake)

FILE(GLOB AC_FILES *.ac)
FILE(GLOB ACC_FILES *.acc)

CSR_INSTALL_FILES(DATA data/instruments FILES ${AC_FILES} ${ACC_FILES})
