#==============================================================================
#
#   file        : prerelease.cmake
#   copyright   : (C) 2021 Joe Thompson
#   email       : beaglejoe@users.sourceforge.net
#   web         : www.cars-sports-racing.net
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
INCLUDE(cmake/prereleaseutils.cmake)

GET_PROPERTY(CSR_TRACK_LIST_ITEMS GLOBAL PROPERTY CSR_TRACK_LIST)
GET_PROPERTY(CSR_CAR_LIST_ITEMS GLOBAL PROPERTY CSR_CAR_LIST)
GET_PROPERTY(CSR_ROBOT_LIST_ITEMS GLOBAL PROPERTY CSR_ROBOT_LIST)

string(REGEX REPLACE ";" "\n" _tracks_contents "${CSR_TRACK_LIST_ITEMS}")
string(REGEX REPLACE ";" "\n" _cars_contents "${CSR_CAR_LIST_ITEMS}")
string(REGEX REPLACE ";" "\n" _robots_contents "${CSR_ROBOT_LIST_ITEMS}")


message("\n")
message("======= Pre-Release checks =======")
#message("\n")

if(OPTION_OFFICIAL_ONLY)
   set(PRE "official")
else()
   set(PRE "unofficial")
   message("OPTION_OFFICIAL_ONLY NOT set")
endif()

IF(VERSION_IS_TAG)
   message(STATUS "VERSION_IS_TAG = ${VERSION_IS_TAG}")
else()
   message("working copy is NOT from a tag")
endif()


message(STATUS "SVN_REV = ${SVN_REV}")
message(STATUS "VERSION = ${VERSION}")
#message(STATUS "NSIS_MAKE_EXE = ${NSIS_MAKE_EXE}")


FILE(WRITE "${PROJECT_BINARY_DIR}/${PRE}-tracklist.txt" ${_tracks_contents})
FILE(WRITE "${PROJECT_BINARY_DIR}/${PRE}-carlist.txt" ${_cars_contents})
FILE(WRITE "${PROJECT_BINARY_DIR}/${PRE}-robotlist.txt" ${_robots_contents})

message("==== Checking cars ====")
check_cars("${CSR_CAR_LIST_ITEMS}")

message("==== Checking tracks ====")
check_tracks("${CSR_TRACK_LIST_ITEMS}")

message("==== Checking robots ====")
check_robots("${CSR_ROBOT_LIST_ITEMS}")

