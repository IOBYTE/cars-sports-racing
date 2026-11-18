# OpenSceneGraph 3.6.5's osga plugin uses _FPOSOFF, which newer Visual Studio C
# runtimes no longer define. From VC++ 2017 15.8 on, a stream position is already
# a plain 64-bit offset, so _FPOSOFF(position) is just the position: use that.
# Run with -DSOURCE_DIR=<OpenSceneGraph source folder>.
set(_file "${SOURCE_DIR}/src/osgPlugins/osga/OSGA_Archive.cpp")
file(READ "${_file}" _text)
string(REPLACE "_FPOSOFF( position )" "(std::streamoff)( position )" _patched "${_text}")
if(NOT _patched STREQUAL _text)
   file(WRITE "${_file}" "${_patched}")
endif()
