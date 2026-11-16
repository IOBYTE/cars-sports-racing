INCLUDE(../../cmake/macros.cmake)

# User settings.
SET(_CONFIG_USER_FILES logging.xml osghudconfig.xml raceengine.xml screen.xml graph.xml sound.xml)
SET(_CONFIG_USER_FILES ${_CONFIG_USER_FILES} webserver.xml)

# User settings.
   CSR_INSTALL_FILES(DATA config USER config FILES ${_CONFIG_USER_FILES})


# Other config files.
SET (_OTHER_CONFIG_FILES raceresults.xsl)
CSR_INSTALL_FILES(DATA config FILES ${_OTHER_CONFIG_FILES})

# Fictive = no-output target, for having source files available in IDEs.
ADD_CUSTOM_TARGET(config_xml SOURCES ${_CONFIG_USER_FILES} ${_OTHER_CONFIG_FILES})

# Race modes.
ADD_SUBDIRECTORY(raceman)
