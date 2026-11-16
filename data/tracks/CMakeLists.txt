INCLUDE(../../cmake/macros.cmake)

# Official track sets.
CSR_ADD_SUBDIRECTORY(circuit)
CSR_ADD_SUBDIRECTORY(dirt)
CSR_ADD_SUBDIRECTORY(grandprix)
CSR_ADD_SUBDIRECTORY(road)
CSR_ADD_SUBDIRECTORY(speedway)
# Work-in-progress track sets.
IF(NOT OPTION_OFFICIAL_ONLY)
	CSR_ADD_SUBDIRECTORY(development)
ENDIF()
