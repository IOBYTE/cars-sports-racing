INCLUDE(../../../cmake/macros.cmake)

# Using GLOB here because when building from less than the complete set of source tars,
# the build would fail because some of the files listed above will be missing.
FILE(GLOB _RACE_FILES *.xml)

CSR_INSTALL_FILES(DATA config/raceman USER config/raceman FILES ${_RACE_FILES})

SET(_EXTRA_FILES extra/skill.xml)

CSR_INSTALL_FILES(DATA config/raceman/extra USER config/raceman/extra
                 FILES ${_EXTRA_FILES})

# Fictive = no-output target, for having source files available in IDEs.
ADD_CUSTOM_TARGET(raceman SOURCES ${_RACE_FILES} ${_EXTRA_FILES})

