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

mkdir Source

ln -sf ../../../SDK/Includes/Quesa APIincludes

pushd Source

ln -sf ../../../Source/Core
ln -sf ../../../Source/FileFormats
ln -sf ../../../Source/PlatForm/Unix
ln -sf ../../../Source/Renderers

popd

popd
