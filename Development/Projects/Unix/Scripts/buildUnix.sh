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
# things that could fail:
#	aclocal complains because the undefined macro AM_PATH_GTK
#		if gtk is installed search a file called gtk.m4 and pass its directory
#			to aclocal as parameter es.
#					 aclocal -I /usr/local/share/aclocal
#
#
#	configure complains because doesn't find gtk-config
#			search gtk-config and add its parent directory to your PATH environment variable
#
#

pushd ../../Unix || exit

aclocal
automake --add-missing
automake
autoconf
./configure
make

popd

