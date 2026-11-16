#==============================================================================
#
#   file        : .cmake
#   copyright   : (C) 2019 Joe Thompson
#   email       : beaglejoe@users.sourceforge.net
#   web         : www.cars-sports-racing.org
#   version     : $Id:$
#
#==============================================================================
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#==============================================================================
FIND_PACKAGE(NSIS)

Message(STATUS "NSIS_FOUND = ${NSIS_FOUND}")
Message(STATUS "NSIS_MAKE_EXE = ${NSIS_MAKE_EXE}")
Message(STATUS "NSIS_INET_PLUGIN = ${NSIS_INET_PLUGIN}")


if(NSIS_FOUND AND NSIS_MAKE_EXE AND NSIS_INET_PLUGIN)
   FILE(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}" NSIS_INSTALL_DIR)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/readme_for_user.txt" 
                  "${CMAKE_BINARY_DIR}/packaging/readme_for_user.txt" COPYONLY)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/cars-sports-racing.ini" 
                  "${CMAKE_BINARY_DIR}/packaging/cars-sports-racing.ini" COPYONLY)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/cars-sports-racing.nsh.in" 
                  "${CMAKE_BINARY_DIR}/packaging/cars-sports-racing.nsh" @ONLY)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/cars-sports-racing-base.nsi" 
                  "${CMAKE_BINARY_DIR}/packaging/cars-sports-racing-base.nsi" @ONLY)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/cars-sports-racing-season2-cars-and-tracks.nsi" 
                  "${CMAKE_BINARY_DIR}/packaging/cars-sports-racing-season2-cars-and-tracks.nsi" @ONLY)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/cars-sports-racing-season3-cars-and-tracks.nsi" 
                  "${CMAKE_BINARY_DIR}/packaging/cars-sports-racing-season3-cars-and-tracks.nsi" @ONLY)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/cars-sports-racing-season4-cars-and-tracks.nsi" 
                  "${CMAKE_BINARY_DIR}/packaging/cars-sports-racing-season4-cars-and-tracks.nsi" @ONLY)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/cars-sports-racing-season5-cars-and-tracks.nsi" 
                  "${CMAKE_BINARY_DIR}/packaging/cars-sports-racing-season5-cars-and-tracks.nsi" @ONLY)

   CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/packaging/windows/cars-sports-racing-season6-cars-and-tracks.nsi" 
                  "${CMAKE_BINARY_DIR}/packaging/cars-sports-racing-season6-cars-and-tracks.nsi" @ONLY)

   #ADD_CUSTOM_TARGET(PACKAGE_BASE DEPENDS INSTALL)
      # can't depend on built-in target INSTALL, so we ADD_CUSTOM_COMMAND
      # below to make sure INSTALL is done
   ADD_CUSTOM_TARGET(PACKING_INSTALL)

   ADD_CUSTOM_TARGET(PACKAGE_ALL DEPENDS PACKAGE_BASE
                                         PACKAGE_SEASON2
                                         PACKAGE_SEASON3
                                         PACKAGE_SEASON4
                                         PACKAGE_SEASON5
										 PACKAGE_SEASON6)

   ADD_CUSTOM_TARGET(PACKAGE_BASE DEPENDS PACKING_INSTALL PACKAGE_MKDIR)
   ADD_CUSTOM_TARGET(PACKAGE_SEASON2 DEPENDS PACKING_INSTALL PACKAGE_MKDIR)
   ADD_CUSTOM_TARGET(PACKAGE_SEASON3 DEPENDS PACKING_INSTALL PACKAGE_MKDIR)
   ADD_CUSTOM_TARGET(PACKAGE_SEASON4 DEPENDS PACKING_INSTALL PACKAGE_MKDIR)
   ADD_CUSTOM_TARGET(PACKAGE_SEASON5 DEPENDS PACKING_INSTALL PACKAGE_MKDIR)
   ADD_CUSTOM_TARGET(PACKAGE_SEASON6 DEPENDS PACKING_INSTALL PACKAGE_MKDIR)
   ADD_CUSTOM_TARGET(PACKAGE_MKDIR)
   
   ADD_CUSTOM_COMMAND(TARGET PACKING_INSTALL
                      COMMAND "${CMAKE_COMMAND}" --build . --target INSTALL --config $<CONFIG>
                      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                      COMMENT "Building INSTALL...")

   ADD_CUSTOM_COMMAND(TARGET PACKAGE_BASE
                        COMMAND ${NSIS_MAKE_EXE} packaging/cars-sports-racing-base.nsi
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                        COMMENT "Building base package...")

   ADD_CUSTOM_COMMAND(TARGET PACKAGE_SEASON2
                        COMMAND ${NSIS_MAKE_EXE} packaging/cars-sports-racing-season2-cars-and-tracks.nsi
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                        COMMENT "Building season 2 package...")

   ADD_CUSTOM_COMMAND(TARGET PACKAGE_SEASON3
                        COMMAND ${NSIS_MAKE_EXE} packaging/cars-sports-racing-season3-cars-and-tracks.nsi
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                        COMMENT "Building season 3 package...")

   ADD_CUSTOM_COMMAND(TARGET PACKAGE_SEASON4
                        COMMAND ${NSIS_MAKE_EXE} packaging/cars-sports-racing-season4-cars-and-tracks.nsi
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                        COMMENT "Building season 4 package...")

   ADD_CUSTOM_COMMAND(TARGET PACKAGE_SEASON5
                        COMMAND ${NSIS_MAKE_EXE} packaging/cars-sports-racing-season5-cars-and-tracks.nsi
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                        COMMENT "Building season5 package...")

   ADD_CUSTOM_COMMAND(TARGET PACKAGE_SEASON6
                        COMMAND ${NSIS_MAKE_EXE} packaging/cars-sports-racing-season6-cars-and-tracks.nsi
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                        COMMENT "Building season6 package...")

   # Make sure that SD_PACKAGEDIR is not empty or PACKAGE_MKDIR will fail.
   # Treat empty string as current directory
   if(CSR_PACKAGEDIR STREQUAL "")
     set(CSR_PACKAGEDIR "." CACHE PATH "Location for the created installers" FORCE)
   endif(CSR_PACKAGEDIR STREQUAL "")

   ADD_CUSTOM_COMMAND(TARGET PACKAGE_MKDIR
                        COMMAND "${CMAKE_COMMAND}" -E make_directory ${CSR_PACKAGEDIR}
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                        COMMENT "Create ${CSR_PACKAGEDIR} directory for Packages dir")

else(NSIS_FOUND AND NSIS_MAKE_EXE AND NSIS_INET_PLUGIN)
   if(NOT NSIS_FOUND)
      Message(WARNING "NSIS NOT FOUND Packaging targets NOT added.")
   elseif(NOT NSIS_INET_PLUGIN)
      Message(WARNING "INetC.dll NOT FOUND Packaging targets NOT added.")
   endif(NOT NSIS_FOUND)
endif()
