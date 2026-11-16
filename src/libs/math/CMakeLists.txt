INCLUDE(../../../cmake/macros.cmake)

SET(_SOURCES linalg_t.h straight2_t.h  v2_t.h v3_t.h  v4_t.h)

CSR_INSTALL_FILES(INCLUDE tmath FILES ${_SOURCES})

# Fictive = no-output target, for having source files available in IDEs.
ADD_CUSTOM_TARGET(math SOURCES ${_SOURCES})
