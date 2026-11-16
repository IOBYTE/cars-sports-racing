INCLUDE(../../../cmake/macros.cmake)

FILE(GLOB shaders *.glsl)
FILE(GLOB fragment *.frag)
FILE(GLOB vert *vert)

CSR_INSTALL_FILES(DATA data/shaders FILES ${shaders} ${fragment} ${vert})
