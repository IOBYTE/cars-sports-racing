INCLUDE(../../../cmake/macros.cmake)

FILE(GLOB png *.png)
FILE(GLOB jpg *.jpg)
FILE(GLOB bmp *.bmp)

ADD_SUBDIRECTORY(flags)

IF(OPTION_OPENGL3)
  ADD_SUBDIRECTORY(osg-hud)
ENDIF(OPTION_OPENGL3)

CSR_INSTALL_FILES(DATA data/img FILES ${png} ${jpg} ${bmp})
