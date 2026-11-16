INCLUDE(../../../cmake/macros.cmake)

FILE(GLOB png *.png)
FILE(GLOB bmp *.bmp)
FILE(GLOB ico *.ico)
FILE(GLOB svg *.svg)

CSR_INSTALL_FILES(DATA share/icons FILES ${png} ${bmp} ${ico} ${svg})
