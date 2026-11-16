INCLUDE(../../../cmake/macros.cmake)

FILE(GLOB OGG_FILES *.ogg)

CSR_INSTALL_FILES(DATA data/music FILES ${OGG_FILES})
