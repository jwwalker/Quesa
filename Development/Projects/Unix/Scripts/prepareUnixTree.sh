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
ln -sf ../../../Source/Platform/Unix
ln -sf ../../../Source/Renderers

popd

mkdir Examples
pushd Examples

ln -sf ../acinclude.m4
ln -sf ../ltmain.sh
ln -sf ../AUTHORS
ln -sf ../ChangeLog
ln -sf ../COPYING
ln -sf ../NEWS
ln -sf ../README
ln -sf ../INSTALL.examples INSTALL
ln -sf ../configure.in.examples configure.in
ln -sf ../Makefile.am.examples Makefile.am


ln -sf ../../../../SDK/Examples/Qut
ln -sf "../../../../SDK/Examples/Geom Test/Geom Test.c" GeomTest.c
ln -sf "../../../../SDK/Examples/Import Test/Import Test.c" ImportTest.c
ln -sf "../../../../SDK/Examples/Camera Test/Camera Test.c" CameraTest.c
ln -sf "../../../../SDK/Examples/Dump Group/Dump Group.c" DumpGroup.c
ln -sf "../../../../SDK/Examples/Light Test/Light Test.c" LightTest.c

mkdir Models
pushd Models

ln -sf "../../../../../SDK/Examples/Support Files/Models/3DMF/Quesa Logo.3dmf" QuesaLogo.3dmf
ln -sf "../../../../../SDK/Examples/Support Files/Models/3DMF/Pod Racer.3dmf" PodRacer.3dmf
ln -sf "../../../../../SDK/Examples/Support Files/Models/3DMF/F18.3dmf" F18.3dmf
ln -sf "../../../../../SDK/Examples/Support Files/Models/3DMF/F15.3dmf" F15.3dmf


popd

popd

popd
