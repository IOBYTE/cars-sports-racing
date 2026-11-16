INCLUDE(../../../../cmake/macros.cmake)

ADD_SUBDIRECTORY(1x1)
ADD_SUBDIRECTORY(4x3)
ADD_SUBDIRECTORY(4x3png)

CSR_INSTALL_FILES(DATA data/img/flags/ FILES readme.txt)
