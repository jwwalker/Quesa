#! /bin/sh

##################################################
# create unix distribution
#
#  several pitfalls
#
# Be sure the files have unix line endings (otherwise it doesn't compile on old gcc)
#
# Be sure to have version 1.6 or later of the gnu build tools
#
# Be sure to be on the /Development/Projects/Unix/Scripts directory before run this
#

pushd ../../Unix || exit

aclocal
autoreconf -i
autoreconf
./configure
make dist


