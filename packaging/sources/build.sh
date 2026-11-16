#!/bin/sh
#
# Script for building source packages
# 
# Usage (example) :
#  cd my/svn/sandbox/tags/2.0.0-rc1
#  ./packaging/sources/build.sh 2.0.0-rc1-r4420
#
# Warning: The generated packages can't be used separately for building :
#          you need to get and extract all of them before building.
#
# copyright  : (C) 2011 onwards Jean-Philippe Meuret
# $Id: build.sh 7602 2021-07-14 14:28:17Z beaglejoe $

# This program is free software ; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation# either version 2 of the License, or
# (at your option) any later version.

# Check if we are on top of an SD source tree
if [ -f CMakeLists.txt -a -f data/data/credits.xml -a -d cmake -a -d data -a -d src ] ; then

  echo "Building $1 source packages (`date`) ..."
  
  specDir="./packaging/sources"

  echo "* 'Base' package ..."
  tar -c -J -X $specDir/csr-src-exclude.lst --exclude-vcs -T $specDir/csr-src-base.lst -f cars-sports-racing-src-base-$1.tar.xz --owner=root --group=root

  ls -l cars-sports-racing-src-base*.tar.xz

  echo "* 'SEASON 2 cars, tracks and AI drivers datas' package ..."
tar -c -J -X $specDir/csr-src-exclude.lst --exclude-vcs -T $specDir/csr-src-season2-cars-and-tracks.lst -f cars-sports-racing-src-season2-cars-and-tracks-$1.tar.xz --owner=root --group=root

  ls -l cars-sports-racing-src-season2*.tar.xz

  echo "* 'SEASON 3 cars, tracks and AI drivers datas' package ..."
tar -c -J -X $specDir/csr-src-exclude.lst --exclude-vcs -T $specDir/csr-src-season3-cars-and-tracks.lst -f cars-sports-racing-src-season3-cars-and-tracks-$1.tar.xz --owner=root --group=root

#  ls -l cars-sports-racing-src-season3*.tar.xz

#  echo "* 'SEASON 4 cars and tracks' package ..."
#  tar -c -J -X $specDir/csr-src-exclude.lst --exclude-vcs -T $specDir/csr-src-season4-cars-and-tracks.lst -f cars-sports-racing-src-season4-cars-and-tracks-$1.tar.xz --owner=root --group=root

#  ls -l cars-sports-racing-src-season4*.tar.xz

#  echo "* 'SEASON 5 cars and tracks' package ...' package ..."
#  tar -c -J -X $specDir/csr-src-exclude.lst --exclude-vcs -T $specDir/csr-src-season5-cars-and-tracks.lst -f cars-sports-racing-src-season5-cars-and-tracks-$1.tar.xz --owner=root --group=root

#  ls -l cars-sports-racing-src-season5*.tar.xz

#  echo "* 'SEASON 6 cars and tracks' package ...' package ..."
#  tar -c -J -X $specDir/csr-src-exclude.lst --exclude-vcs -T $specDir/csr-src-season6-cars-and-tracks.lst -f cars-sports-racing-src-season6-cars-and-tracks-$1.tar.xz --owner=root --group=root

#  ls -l cars-sports-racing-src-season6*.tar.xz

  echo "* 'All' package ..."
  tar -c -J -X $specDir/csr-src-exclude.lst --exclude-vcs -T $specDir/csr-src-all.lst -f cars-sports-racing-src-all-$1.tar.xz --owner=root --group=root

  ls -l cars-sports-racing-src-all*.tar.xz

  echo "Done (`date`)."
  
else

  echo "Bad current dir for that ; please run from the root folder of an CSR source tree."

fi
