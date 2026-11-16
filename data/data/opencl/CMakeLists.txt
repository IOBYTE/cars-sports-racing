INCLUDE(../../../cmake/macros.cmake)

FILE(GLOB cl *.cl)

CSR_INSTALL_FILES(DATA data/opencl FILES ${cl} )
