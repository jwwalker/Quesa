#! /bin/sh

##################################################
# build unix lib
#
#  several pitfalls
#
# Be sure the files have unix line endings
#
# Be sure to have version 1.6 or later of the gnu build tools
#
# Be sure to be on the /Development/Projects/Unix/Scripts directory before run this
#

pushd ../../Unix || exit

aclocal
automake --add-missing
automake
autoconf
./configure
make

popd

