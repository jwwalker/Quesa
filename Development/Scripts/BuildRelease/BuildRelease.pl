#!/usr/bin/perl -w
#========================================================================
#   Description:
#       Generates the release archives for Quesa.
#
#		Note that this assumes we are running on a Mac OS X 10.2 or later
#		machine. We also assume we can invoke DropStuff to generate the
#		Mac .sit archive.
#
#   Author:
#		Dair Grant
#       <mailto:dair@webthing.net>
#------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------
use strict;
use File::Find;
use File::Path;





#------------------------------------------------------------------------
# Delete a list of files/directories.
#------------------------------------------------------------------------
sub deleteItems
{


	# Retrieve our parameters
	my (@theItems) = @_;



	# Delete the items which still exist
	foreach my $theItem (@theItems)
		{
		if (-e $theItem)
			{
			system("rm -fR '$theItem'");
			}
		}
}





#------------------------------------------------------------------------
# Move a file to the desktop.
#------------------------------------------------------------------------
sub moveToDesktop
{


	# Retrieve our parameters
	my ($theItem) = @_;



	# Move the file
	system("mv \"$theItem\" ~/Desktop");
}





#------------------------------------------------------------------------
# Copy the release source to a temporary area.
#------------------------------------------------------------------------
sub copyReleaseToTempArea
{


	# Retrieve our parameters
	my ($srcDir, $dstDir, $releaseName) = @_;



	# Print some progress
	print "Copying release to $dstDir\n";



	# Set up our temporary directory
	my $tmpDir     = $dstDir . "/Quesa Temporary Items/";
	my $releaseDir = $tmpDir . $releaseName;
	deleteItems($tmpDir);

	print "    creating temporary directory\n";
	die ("copyReleaseToTemp failed - remove Quesa Temporary Items!\n") if (-e $tmpDir);
	mkpath($releaseDir);
	print "        done\n";



	# Copy the files
	print "    copying source\n";
	system("ditto -rsrcFork \"$srcDir\" \"$releaseDir\"");
	print "        done\n";



	# Return the temporary directory
	return($tmpDir);
}





#------------------------------------------------------------------------
# Remove the binary files from the release.
#------------------------------------------------------------------------
sub removeBinaryFiles
{


	# Retrieve our parameters
	my ($releaseDir) = @_;



	# Remove any left-overs from builds
	print "    removing binaries\n";
	my @theFiles = ();

	push(@theFiles, $releaseDir . "/Development/Projects/Mac/Project Builder/build/");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/Quesa Custom Elements Data/");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/Quesa Data/");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/Quesa Gestalt Data/");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/Quesa Stubs Data/");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/Quesa Viewer Data/");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaCarbonDebug_PCH");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaCarbonProfile_PCH");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaCarbonRelease_PCH");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaClassicDebug_PCH");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaClassicProfile_PCH");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaClassicRelease_PCH");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaStaticDebug_PCH");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaStaticProfile_PCH");
	push(@theFiles, $releaseDir . "/Development/Projects/Mac/QuesaStaticRelease_PCH");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Libraries/");



	# Add temporary files scheduled to be removed from CVS
	push(@theFiles, $releaseDir . "/Development/Source/Platform/Windows/Quesa Viewer/");
	push(@theFiles, $releaseDir . "/Development/Source/StackCrawl/");



	# Add CVS/.DS_Store junk
	find(sub
		{
		if (($_ eq "CVS"       &&   -d $_) ||
			($_ eq ".DS_Store" && ! -d $_))
			{
			push(@theFiles, $File::Find::dir . "/" . $_);
			}
		}, $releaseDir);



	# Delete everything on the list
	deleteItems(@theFiles);
	print "        done\n";
}





#------------------------------------------------------------------------
# Locate the directory to use as the root of the Quesa tree.
#------------------------------------------------------------------------
sub findSrcDir
{


	# Find the root Quesa directory - we assume we're on a Mac
	my $theDir;
	chop($theDir = `pwd`);
	
	$theDir  =~ s/\/[\w\s]+\/[\w\s]+\/[\w\s]+$//;
	$theDir .= "/";
	
	return($theDir);
}





#------------------------------------------------------------------------
# Find the directory to leave our output in.
#------------------------------------------------------------------------
sub findDstDir
{


	# Find the desktop folder - we assume we're on a Mac
	my $theDir;
	chop($theDir = `echo \$HOME`);

	$theDir .= "/Desktop";
	
	return($theDir);
}





#------------------------------------------------------------------------
# Find the name for the release.
#------------------------------------------------------------------------
sub findReleaseName
{


	# Read E3Version.h
	my $versionHeader = "";
	{
		local($/) = undef;
		open(FILE, "../../Source/Core/Support/E3Version.h") or die ("Couldn't open E3Version.h: $!\n");
		$versionHeader = <FILE>;
		close(FILE);
  	}



	# Extract the version line
	$versionHeader =~ /kQ3StringVersion\s+\"(.*)\"\s+\/\//;
	return(undef) if ($1 eq "");

	my $releaseName = "quesa_$1";
	
	return($releaseName);
}





#------------------------------------------------------------------------
# Generate the Mac release archive.
#------------------------------------------------------------------------
sub generateReleaseMac
{


	# Retrieve our parameters
	my ($releaseDir) = @_;



	# Prepare to create the archive
	my $theArchive = $releaseDir . ".sit";
	deleteItems($theArchive);



	# Tell DropStuff to stuff the release directory
	my $theScript = "tell application \"DropStuff\"\n";
	$theScript   .= "    activate\n";
	$theScript   .= "    open alias POSIX file  \"$releaseDir\"\n";
	$theScript   .= "    quit\n";
	$theScript   .= "end tell\n";

	system("osascript -e '$theScript'");



	# Finish off
	moveToDesktop($theArchive);
	print "    done\n";
}





#------------------------------------------------------------------------
# Generate the Unix release archive.
#------------------------------------------------------------------------
sub generateReleaseUnix
{



	# Retrieve our parameters
	my ($releaseDir, $releaseName) = @_;



	# Prepare to create the archive
	my $theArchive = $releaseDir . ".tar.gz";
	deleteItems($theArchive);



	# Create the .tar.gz file
	system("cd \"$releaseDir\"/..; tar cf \"$releaseName\".tar \"$releaseName\"; gzip \"$releaseName\".tar");



	# Finish off
	moveToDesktop($theArchive);
	print "    done\n";
}





#------------------------------------------------------------------------
# Generate the Win release archive.
#------------------------------------------------------------------------
sub generateReleaseWin
{



	# Retrieve our parameters
	my ($releaseDir, $releaseName) = @_;



	# Prepare to create the archive
	my $theArchive = $releaseDir . ".zip";
	deleteItems($theArchive);



	# Create the .zip file
	system("cd \"$releaseDir\"/..; zip -r -q \"$releaseName\".zip \"$releaseName\"");



	# Finish off
	moveToDesktop($theArchive);
	print "    done\n";
}





#------------------------------------------------------------------------
# Build the release.
#------------------------------------------------------------------------
sub buildRelease
{


	# Locate our directories
	my $srcDir = findSrcDir();
	my $dstDir = findDstDir();



	# Identify the release
	my $releaseName = findReleaseName();
	die("Could not determine release name - findReleaseName failed") if !defined($releaseName);
	printf("Generating $releaseName\n");



	# Take a copy of the source and remove any binaries	
	my $tmpDir     = copyReleaseToTempArea($srcDir, $dstDir, $releaseName);
	my $releaseDir = $tmpDir . $releaseName;
	removeBinaryFiles($releaseDir);



	# Generate the releases
	print "Generating Mac release\n";
	generateReleaseMac($releaseDir);

	print "Generating Unix release\n";
	generateReleaseUnix($releaseDir, $releaseName);

	print "Generating Windows release\n";
	generateReleaseWin($releaseDir, $releaseName);



	# Clean up
	deleteItems($tmpDir);
	print "Finished\n";
}





#------------------------------------------------------------------------
# Script entry point
#------------------------------------------------------------------------
# Build the release
buildRelease();




