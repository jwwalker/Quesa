#! /bin/sh

##################################################
# rearrage things to meet (almost) GNU guidelines
#
#  several pitfalls
#
# Be sure to not apply this to a CVS tree or if you do remove links before update
#
# Be sure the files have unix line endings
#
# Be sure to be on the /Development/Projects/Unix/Scripts directory before run this
#

pushd ../../Unix || exit


ln -sf ../../../SDK/Includes/Quesa APIincludes

mkdir Source
pushd Source

ln -sf ../../../Source/Core
ln -sf ../../../Source/FileFormats
ln -sf ../../../Source/PlatForm/Unix
ln -sf ../../../Source/Renderers

popd

mkdir Examples
pushd Examples

ln -sf ../acinclude.m4
ln -sf ../ltmain.sh
ln -sf ../configure.in.examples configure.in
ln -sf ../Makefile.am.examples Makefile.am


ln -sf ../../../../SDK/Examples/Qut
ln -sf "../../../../SDK/Examples/Geom Test/Geom Test.c" GeomTest.c
ln -sf "../../../../SDK/Examples/Import Test/Import Test.c" ImportTest.c

popd

popd
