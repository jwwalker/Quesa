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
#   due the file distribution of quesa, not quite standard, make dist-check always fails
#
#


# make sure we're on the correct directory
pushd ../../Unix || exit

# create the build system for libquesa
######################################
ACLOCALADDPATHS=
if test -d /usr/local/share/aclocal; then
 ACLOCALADDPATHS="${ACLOCALADDPATHS} -I /usr/local/share/aclocal"
fi

aclocal --force ${ACLOCALADDPATHS}
automake --add-missing
automake
autoconf
./configure

# build libquesa distribution
######################################
make dist



# create the build system for the examples
######################################

pushd Examples || exit

# make sure we can find gtk-config
PATH=${PATH}:/usr/local/bin

export PATH

aclocal --force ${ACLOCALADDPATHS}
automake --add-missing
automake
autoconf
./configure

# build examples distribution
######################################
make dist

popd
popd

# at this point you'll find the tar.gz packages in ../Unix and ../Unix/Examples
######################################
