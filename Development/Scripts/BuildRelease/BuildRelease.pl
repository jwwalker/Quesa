#!/usr/bin/perl -w
#========================================================================
#   Description:
#       Generates the release archives for Quesa.
#
#		Incorporates code from Chris Nandor's tarzipme.plx, from the
#		cpan-mac release.
#
#
#		Note that this assumes we are running on a Mac, using MacPerl
#		configured as <http://www.mozilla.org/build/mac.html>.
#
#		The link to cpan-mac on that page points to an old release,
#		but if you strip off the file name and look in the directory
#		you will find the latest release. The latest release includes
#		the Compress::Zlib module, and so the only addition required
#		is the Archive::Zip module.
#
#		We also assume we can invoke DropStuff to generate the Mac .sit
#		archive.
#
#   Author:
#		Dair Grant
#       <mailto:dair@webthing.net>
#
#   Version History
#		1.0  - First release
#------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------
use strict;
use Archive::Tar;
use Archive::Zip;
use File::Basename;
use File::Copy;
use File::Find;
use File::Path;
use File::Spec;
use Mac::Files;
use Mac::MoreFiles;





#------------------------------------------------------------------------
# Constants
#------------------------------------------------------------------------
use constant LINE_UNIX => 0;
use constant LINE_DOS  => 1;




#------------------------------------------------------------------------
# Create a .zip archive from a directory
#------------------------------------------------------------------------
sub createZipArchive {
    my($dir, $zip, $file, @f, $mdir, $ndir, $zdir, $edir);
    local $|;

    $dir  = $_[0] or die "Need directory name";
    $file = getFilename($dir, ".zip");
    $ndir = basename($dir);
    $edir = "$ENV{TMPDIR}macperlzip:";
    $zdir = "$edir$ndir";
    $mdir = dirname($zdir);
    $zip  = new Archive::Zip;

    die "Cannot continue: archive $file exists\n" if -e $file;
    print "    copying source\n";
    createDir($dir, $edir, $zdir);
    createFile($file, "ZIP ", "SITx");
    print "        done\n";

    chdir($edir) or die "Can't chdir $edir: $!";

    print "    converting files\n";
    find(sub {
        my $f = $File::Find::name;
        return if ! -f $f || -z $f || $f =~ /:Icon\n$/;
        (my $n = $f) =~ s/^$mdir//;
        $n = convert($f, $n, LINE_DOS);
        push @f, $n;
    }, $zdir);
    print "        done\n";

    foreach my $theFile (@f)
	   	{
    	my $member = $zip->addFile($theFile);
    	if (defined($member))
    		{
			$theFile =~ s/:/\\/g;
			$member->fileName($theFile);
			$member->desiredCompressionLevel(9);
			}
		}

    print "    writing archive\n";
    $zip->writeToFileNamed($file);
    print "    cleaning up\n";

	deleteItems($zdir);
}





#------------------------------------------------------------------------
# Create a .tar.gz archive from a directory
#------------------------------------------------------------------------
sub createTarGzipArchive {
    my($dir, $tar, $file, @f, $mdir, $ndir, $tdir, $edir);
    local $|;

    $dir  = $_[0] or die "Need directory name";
    $file = getFilename($dir, ".tar.gz");
    $ndir = basename($dir);
    $edir = "$ENV{TMPDIR}macperltar:";
    $tdir = "$edir$ndir";
    $mdir = dirname($tdir);
    $tar  = new Archive::Tar;

    die "Cannot continue: archive $file exists\n" if -e $file;
    print "    copying source\n";
    createDir($dir, $edir, $tdir);
    createFile($file, "Gzip", "Gzip");
    print "        done\n";

    chdir($edir) or die "Can't chdir $edir: $!";

    print "    converting files\n";
    find(sub {
        my $f = $File::Find::name;
        return if ! -f $f || -z $f || $f =~ /:Icon\n$/;
        (my $n = $f) =~ s/^$mdir//;
        $n = convert($f, $n, LINE_UNIX);
        push @f, $n;
    }, $tdir);
    print "        done\n";

    print "    adding files to archive\n";
    $tar->add_files(@f);
    print "    writing archive\n";
    $tar->write($file, 1);
    print "    cleaning up\n";
    deleteItems($tdir);
}





#------------------------------------------------------------------------
# Convert line endings.
#------------------------------------------------------------------------
sub convert {
    my($f, $n, $type) = @_;
    if (-s $f && -T _)
    	{
    	if ($type == LINE_UNIX)
    		{
	        return cr2lf($f, $n);
	        }
	    else
	    	{
	        return cr2crlf($f, $n);
	        }
    	}
    
    return ($n);
}





#------------------------------------------------------------------------
# Convert to Unix line endings.
#------------------------------------------------------------------------
sub cr2lf {
    local(*F, $/);
    my($f, $n, $t) = @_;
    open(F, "< $f\0") or die "Can't open $f: $!";
    $t = <F>;
    close(F);
    $t =~ s/\015\012?/\012/g if $t;
    open(F, "> $f\0") or die "Can't open $f: $!";
    print F $t;
    close(F);
    return $n;
}





#------------------------------------------------------------------------
# Convert to DOS line endings.
#------------------------------------------------------------------------
sub cr2crlf {
    local(*F, $/);
    my($f, $n, $t) = @_;
    open(F, "< $f\0") or die "Can't open $f: $!";
    $t = <F>;
    close(F);
    $t =~ s/\015\012?/\012/g if $t;  # Dos/Mac to Unix
    $t =~ s/\012/\015\012/g  if $t;  # Unix to Dos
    open(F, "> $f\0") or die "Can't open $f: $!";
    print F $t;
    close(F);
    return $n;
}





#------------------------------------------------------------------------
# Create a file.
#------------------------------------------------------------------------
sub createFile {
	my ($fileName, $fileType, $fileCreator) = @_;
	FSpCreate($fileName, $fileCreator, $fileType) or die $^E;
}





#------------------------------------------------------------------------
# Create a directory.
#------------------------------------------------------------------------
sub createDir
{
    my($dir, $edir, $tdir) = @_;
    unless (-d $edir) {mkdir $edir, 0777 or die "Cannot create $edir: $!"}
    deleteItems($tdir) if -d $tdir;
    FSpDirectoryCopy($dir, $edir, 1)
        or die "Can't copy $dir to $edir: $^E";
}





#------------------------------------------------------------------------
# Construct a filename from a directory name with an extension.
#------------------------------------------------------------------------
sub getFilename
{
    my $name = shift;
    my $ext  = shift;
    my $smallExt = ($ext eq ".tar.gz" ? ".tgz" : ".zip");
    my($file, $path) = fileparse($name, '');
    my $tfile = length($file) < 24 ? "$file$ext" :
        length($file) < 28 ? "$file$smallExt" :
        substr($file, 0, 23) . "\xC9.$ext";
    return "$path$tfile";
}





#------------------------------------------------------------------------
# Locate the directory to use as the root of the Quesa tree.
#------------------------------------------------------------------------
sub findSrcDir
{


	# Find the root Quesa directory - we assume we're on a Mac
	my $theDir;
	chop($theDir = `pwd`);
	
	$theDir  =~ s/:[\w\s]+:[\w\s]+:[\w\s]+$//;
	$theDir .= ":";
	
	return($theDir);
}





#------------------------------------------------------------------------
# Find the directory to leave our output in.
#------------------------------------------------------------------------
sub findDstDir
{


	# Find the desktop folder on the Quesa disk - we assume we're on a Mac
	my $theDir;
	chop($theDir = `pwd`);

	$theDir =~ s/^([\w\s]+:).*/$1Desktop Folder:/;

	return($theDir);
}





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
			# Delete directories
			if (-d $theItem)
				{
				finddepth(sub
					{
					my $n = $File::Find::name;
					if (-d $n)
						{
						rmdir $n or die "Can't rmdir $n: $!";
						}
					else
						{
						unlink $n or die "Can't unlink $n: $!";
						}
					}, $theItem);
				}

			# Delete files
			else
				{
				unlink($theItem) or die "Can't unlink $theItem: $!";
				}
			}
		}
}





#------------------------------------------------------------------------
# Ask the Finder to move an item to the desktop.
#------------------------------------------------------------------------
sub finderMoveToDesktop
{


	# Retrieve our parameters
	my ($theItem) = @_;



	# Make sure it exists
	if (! -e $theItem)
		{
		print "Error: finderMoveToDesktop can't find $theItem\n";
		return;
		}



	# Find the desktop folder
	my $theDst = $theItem;
	$theDst    =~ s/^([\w\s]+:).*/$1Desktop Folder:/;



	# Ask the Finder to move the item
	my $theScript = "tell application \"Finder\"\n";
	$theScript   .= "    move alias \"$theItem\" to alias \"$theDst\"\n";
	$theScript   .= "end tell\n";

	MacPerl::DoAppleScript($theScript);
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
	my $tmpDir     = $dstDir . "Quesa Temporary Items:";
	my $releaseDir = $tmpDir . $releaseName;
	deleteItems($tmpDir);

	print "    creating temporary directory\n";
	die ("copyReleaseToTemp failed - remove Quesa Temporary Items!\n") if (-e $tmpDir);
	mkpath($releaseDir);
	print "        done\n";



	# Copy the files
	print "    copying source\n";
	print "        copying Quesa/Development\n";
	FSpDirectoryCopy($srcDir . "Development",   $releaseDir, 1) or die($^E); 

	print "        copying Quesa/Documentation\n";
	FSpDirectoryCopy($srcDir . "Documentation", $releaseDir, 1) or die($^E); 

	print "        copying Quesa/SDK\n";
	FSpDirectoryCopy($srcDir . "SDK",           $releaseDir, 1) or die($^E); 
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
	#
	# dair - perhaps do this with a scan for file types, to pick up
	# shared libs/profile output in the example apps area?
	print "    removing binaries\n";
	my @theFiles = ();

	push(@theFiles, $releaseDir . ":Development:Projects:Mac:Quesa Custom Elements Data:");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:Quesa Data:");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:Quesa Gestalt Data:");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:Quesa Stubs Data:");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:Quesa Viewer Data:");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaCarbonDebug_PCH");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaCarbonProfile_PCH");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaCarbonRelease_PCH");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaClassicDebug_PCH");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaClassicProfile_PCH");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaClassicRelease_PCH");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaStaticDebug_PCH");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaStaticProfile_PCH");
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:QuesaStaticRelease_PCH");
	push(@theFiles, $releaseDir . ":SDK:Examples:Background Test:Background Test");
	push(@theFiles, $releaseDir . ":SDK:Examples:Background Test:Background Test (Classic)");
	push(@theFiles, $releaseDir . ":SDK:Examples:Background Test:Background Test (Classic).xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Background Test:Background Test Data:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Background Test:Background Test.xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Camera Test:Camera Test");
	push(@theFiles, $releaseDir . ":SDK:Examples:Camera Test:Camera Test (Classic)");
	push(@theFiles, $releaseDir . ":SDK:Examples:Camera Test:Camera Test (Classic).xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Camera Test:Camera Test Data:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Camera Test:Camera Test.xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Dump Group:Dump Group");
	push(@theFiles, $releaseDir . ":SDK:Examples:Dump Group:Dump Group (Classic)");
	push(@theFiles, $releaseDir . ":SDK:Examples:Dump Group:Dump Group (Classic).xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Dump Group:Dump Group Data:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Dump Group:Dump Group.xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Geom Test:Geom Test");
	push(@theFiles, $releaseDir . ":SDK:Examples:Geom Test:Geom Test (Classic)");
	push(@theFiles, $releaseDir . ":SDK:Examples:Geom Test:Geom Test (Classic).xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Geom Test:Geom Test Data:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Geom Test:Geom Test.xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Import Test:Import Test");
	push(@theFiles, $releaseDir . ":SDK:Examples:Import Test:Import Test (Classic)");
	push(@theFiles, $releaseDir . ":SDK:Examples:Import Test:Import Test (Classic).xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Import Test:Import Test Data:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Import Test:Import Test.xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Light Test:Light Test");
	push(@theFiles, $releaseDir . ":SDK:Examples:Light Test:Light Test (Classic)");
	push(@theFiles, $releaseDir . ":SDK:Examples:Light Test:Light Test (Classic).xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Light Test:Light Test Data:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Light Test:Light Test.xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Shader Test:Shader Test");
	push(@theFiles, $releaseDir . ":SDK:Examples:Shader Test:Shader Test (Classic)");
	push(@theFiles, $releaseDir . ":SDK:Examples:Shader Test:Shader Test (Classic).xSYM");
	push(@theFiles, $releaseDir . ":SDK:Examples:Shader Test:Shader Test Data:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Shader Test:Shader Test.xSYM");
	push(@theFiles, $releaseDir . ":SDK:Libraries:");



	# Add temporary files scheduled to be removed from CVS
	push(@theFiles, $releaseDir . ":Development:Projects:Mac:Experimental:");
	push(@theFiles, $releaseDir . ":Development:VersionControl:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Background Test:Experimental:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Camera Test:Experimental:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Dump Group:Experimental:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Geom Test:Experimental:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Import Test:Experimental:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Light Test:Experimental:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Shader Test:Experimental:");
	push(@theFiles, $releaseDir . ":SDK:Examples:Style Test:");



	# Add CVS folders
	find(sub
		{
		push(@theFiles, $File::Find::dir . $_) if (-d $_ && $_ eq "CVS");
		}, $releaseDir);



	# Delete everything on the list
	deleteItems(@theFiles);
	print "        done\n";
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



	# Check for Dair's Mac, launch DropStuff automatically
	my $dropStuff = "Tuvok:Applications (Mac OS 9):StuffIt Deluxe" . chr(170) . " 5.1 Folder:DropStuff" . chr(170);
	if (-e $dropStuff)
		{
		my $theScript = "tell application \"Finder\"\n";
		$theScript   .= "    open alias \"$dropStuff\"\n";
		$theScript   .= "end tell\n";

		MacPerl::DoAppleScript($theScript);
		}



	# Tell DropStuff to stuff the release directory
	my $theScript = "tell application \"DropStuff" . chr(170) . "\"\n";
	$theScript   .= "    activate\n";
	$theScript   .= "    open alias \"$releaseDir\"\n";
	$theScript   .= "    quit\n";
	$theScript   .= "end tell\n";

	MacPerl::DoAppleScript($theScript);



	# Finish off
	finderMoveToDesktop($theArchive);
	print "    done\n";
}





#------------------------------------------------------------------------
# Generate the Unix release archive.
#------------------------------------------------------------------------
sub generateReleaseUnix
{



	# Retrieve our parameters
	my ($releaseDir) = @_;



	# Create the .tar.gz archive
	my $theArchive = $releaseDir . ".tar.gz";

	deleteItems($theArchive);
	createTarGzipArchive($releaseDir);
	finderMoveToDesktop($theArchive);

	print "    done\n";
}





#------------------------------------------------------------------------
# Generate the Win release archive.
#------------------------------------------------------------------------
sub generateReleaseWin
{



	# Retrieve our parameters
	my ($releaseDir) = @_;



	# Create the .zip archive
	my $theArchive = $releaseDir . ".zip";

	deleteItems($theArchive);
	createZipArchive($releaseDir);
	finderMoveToDesktop($theArchive);

	print "    done\n";
}





#------------------------------------------------------------------------
# Build the release.
#------------------------------------------------------------------------
sub buildRelease
{


	# Prompt them for the release name
	my $releaseName = MacPerl::Ask("Please enter a name for this release:", "quesa_1.6d13");
	exit if !defined($releaseName);



	# Locate our directories
	my $srcDir = findSrcDir();
	my $dstDir = findDstDir();



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
# Check we're on a Mac, then build the release
die ("Sorry, this script requires MacPerl") if ($^O ne "MacOS");
buildRelease();




