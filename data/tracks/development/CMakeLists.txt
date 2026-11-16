INCLUDE(../../../cmake/macros.cmake)

# Work-in-progress tracks.
IF(NOT OPTION_OFFICIAL_ONLY)
	CSR_ADD_SUBDIRECTORY(garage)
ENDIF()
