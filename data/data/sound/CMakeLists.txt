INCLUDE(../../../cmake/macros.cmake)

FILE(GLOB sounds *.wav)

CSR_INSTALL_FILES(DATA data/sound FILES ${sounds})
