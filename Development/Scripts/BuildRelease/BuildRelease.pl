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
# Globals
#------------------------------------------------------------------------
# Constants
my $kCodeWarriorIDE                             = "CodeWarrior IDE 4.2.6";
my $kPathToPCBuild                              = "/Volumes/10.0.0.6/Dair/quesa/";

my $kTemplateMacReadMe = <<'MAC_README_TEMPLATE';
<!--=========================================================================-->
<!-- Page Header                                                             -->
<!--=========================================================================-->
<html>
<head>
	<title>Quesa</title>
</head>
<body bgcolor=white text=black background="images/logo_gray.gif">



<!--=========================================================================-->
<!-- Page Content                                                            -->
<!--=========================================================================-->
<div align=center>
<a href="http://www.quesa.org/"><img src="images/logo_text.jpg" alt="Quesa" width=311 height=105></a>
</div>

<h3>Quesa RELEASE_VERSION</h3>
<p>
Welcome to the RELEASE_VERSION release of
<a href="http://www.quesa.org/">Quesa</a>, a high-level 3D graphics library released as
<a href="http://www.quesa.org/reference/lgpl.html">Open Source</a>.
</p>

<p>
This archive contains pre-built binaries and example applications for end users,
and the latest stub libraries and header files for developers.
</p>

<p>
The archive is divided into four main sections:
</p>

<ul>
	<li>Carbon. This folder contains libraries built for Carbon and a demo application.</li>
	<li>Classic. This folder contains libraries built for Classic Mac OS and a demo application.</li>
	<li>Models. This folder contains example models in the 3DMF format.</li>
	<li>Developers. This folder contains stub libraries and header files for developers.</li>
</ul>

<p>
Carbon builds of Quesa support Mac OS 8.6 and higher, and will run native on Mac OS X.
Classic builds of Quesa support up to Mac OS 9.1.
</p>

<p>
Please note that Quesa is still at the development stage, and should not yet be considered
a complete replacement for QD3D. For more information on the current status of Quesa,
please see our <a href="http://www.quesa.org/">web site</a>.
</p>



<h3>Installation on Mac OS X</h3>
<p>
Since Mac OS X does not support QuickDraw 3D, only the Carbon Quesa
libraries need to be installed.
</p>

<p>
This can be performed by coying the following file from the Carbon
folder into your /Library/CFMSupport folder:
</p>

<blockquote>
	Quesa
</blockquote>

<p>
You may find that you need to create the /Library/CFMSupport folder
before installing this library.
</p>



<h3>Installation on Mac OS 9.x</h3>
<p>
If you're running MacOS 9.x or earlier, you probably already have
QuickDraw&#8482; 3D  installed. QuickDraw 3D is an Apple library that
provides essentially the same services as Quesa, but only for Classic
applications - it's not available in Carbon.
</p>

<h4>Classic</h4>
<p>
Since Quesa is still at the development stage, you might want to
continue using QuickDraw 3D instead of Quesa for your classic applications.
On the other hand, if you have applications which need some feature provided
only by Quesa, you can install Quesa over QuickDraw 3D by disabling
these Extensions:
</p>

<blockquote>
	QuickDraw&#8482; 3D<br>
	QuickDraw&#8482; 3D IR<br>
	QuickDraw&#8482; 3D RAVE<br>
	QuickDraw&#8482; 3D Viewer
</blockquote>

<p>
and copying these files from the Classic folder into your Extensions folder:
</p>

<blockquote>
	Quesa Classic<br>
	Quesa Custom Elements<br>
	Quesa Gestalt<br>
</blockquote>

<p>
If you are using a Classic application, you may prefer to continue to use the
original QuickDraw 3D library - however Quesa should also be compatible.
</p>

<h4>Carbon</h4>
<p>
Since Carbon applications do not use QuickDraw 3D, you should also install
the following file from the Carbon folder into your Extensions folder:
</p>

<blockquote>
	Quesa<br>
</blockquote>

<p>
This provides 3D services to Carbon applications.  You do not need to remove or
disable anything before installing this file.
</p>



<h3>CVS Server</h3>
<p>
If you are a developer interested in Quesa, we recommend you also check out the
Quesa <a href="http://www.quesa.org/developer/cvs.html">CVS server</a>. This holds
the most up to date copy of the source code to Quesa, and will normally contain
bug fixes and improvements made after this release.
</p>





<!--=========================================================================-->
<!-- Page Footer                                                             -->
<!--=========================================================================-->
<br>
<br>
<hr width="90%">
<table border=0 align=center>
<tr>
<td colspan=2 align=center>
<font size="-1">
<a href="http://www.quesa.org/">Quesa Home Page</a>
</font>
</td>
</tr>
</table>

</body>
</html>
MAC_README_TEMPLATE

my $kTemplateWinReadMe = <<'WIN_README_TEMPLATE';
<!--=========================================================================-->
<!-- Page Header                                                             -->
<!--=========================================================================-->
<html>
<head>
	<title>Quesa</title>
</head>
<body bgcolor=white text=black background="images/logo_gray.gif">



<!--=========================================================================-->
<!-- Page Content                                                            -->
<!--=========================================================================-->
<div align=center>
<a href="http://www.quesa.org/"><img src="images/logo_text.jpg" alt="Quesa" width=311 height=105></a>
</div>

<h3>Quesa RELEASE_VERSION</h3>
<p>
Welcome to the RELEASE_VERSION release of
<a href="http://www.quesa.org/">Quesa</a>, a high-level 3D graphics library released as
<a href="http://www.quesa.org/reference/lgpl.html">Open Source</a>.
</p>

<p>
This archive contains pre-built binaries and example applications for end users,
and the latest stub libraries and header files for developers.
</p>

<p>
The archive is divided into three main sections:
</p>

<ul>
	<li>Program Files. This folder contains pre-built libraries and a demo application.
	
	<li>Models. This folder contains example models in the 3DMF format, suitable for
		viewing with the Quesa Model Viewer (not available on Windows yet).</li>
	
	<li>Developers. This folder contains stub libraries and header files for developers.</li>
</ul>

<p>
Quesa should support Windows 95 or later, and requires OpenGL.
</p>

<p>
Please note that Quesa is still at the development stage, and should not yet be considered
a complete replacement for QD3D. For more information on the current status of Quesa,
please see our <a href="http://www.quesa.org/">web site</a>.
</p>


<h3>Installation</h3>
<p>
To install Quesa, simply copy this file to your Windows System directory:
</p>

<blockquote>
	Quesa.dll<br>
</blockquote>

<p>
This file provides 3D services to Quesa applications.
</p>



<h3>CVS Server</h3>
<p>
If you are a developer interested in Quesa, we recommend you also check out the
Quesa <a href="http://www.quesa.org/developer/cvs.html">CVS server</a>. This holds
the most up to date copy of the source code to Quesa, and will normally contain
bug fixes and improvements made after this release.
</p>





<!--=========================================================================-->
<!-- Page Footer                                                             -->
<!--=========================================================================-->
<br>
<br>
<hr width="90%">
<table border=0 align=center>
<tr>
<td colspan=2 align=center>
<font size="-1">
<a href="http://www.quesa.org/">Quesa Home Page</a>
</font>
</td>
</tr>
</table>

</body>
</html>
WIN_README_TEMPLATE





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
# Remove the binary files from the release.
#------------------------------------------------------------------------
sub removeBinaryFiles
{


	# Retrieve our parameters
	my ($releaseDir) = @_;



	# Remove any left-overs from builds
	print "    Removing binaries from temporary directory";
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
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Background Test/Background Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Camera Test/Camera Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group");
	push(@theFiles, $releaseDir . "/SDK/Examples/Dump Group/Dump Group.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Geom Test/Geom Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Import Test/Import Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Light Test/Light Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test (Classic)");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test (Classic).xSYM");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test Data/");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test");
	push(@theFiles, $releaseDir . "/SDK/Examples/Shader Test/Shader Test.xSYM");
	push(@theFiles, $releaseDir . "/SDK/Extras/Cocoa Example/Quesa3DView/Quesa3DView.pbproj/dair.pbxuser");
	push(@theFiles, $releaseDir . "/SDK/Extras/Cocoa Example/Quesa3DView/build/");
	push(@theFiles, $releaseDir . "/SDK/Libraries/");



	# Add temporary files scheduled to be removed from CVS
	push(@theFiles, $releaseDir . "/Development/Source/Platform/Windows/Quesa Viewer/");
	push(@theFiles, $releaseDir . "/Development/Source/StackCrawl/");



	# Add alias files, CVS directories, and .DS_Store files
	find(sub
		{
		my $isAlias = `GetFileInfo -aa "$_"`;
		chomp($isAlias);
		
		if (($isAlias eq "1"                   ) ||
			($_       eq "CVS"       &&   -d $_) ||
			($_       eq ".DS_Store" && ! -d $_))
			{
			push(@theFiles, $File::Find::dir . "/" . $_);
			}
		}, $releaseDir);



	# Delete everything on the list
	deleteItems(@theFiles);
	print "                                done\n";
}





#------------------------------------------------------------------------
# Copy the release source to the temporary area.
#------------------------------------------------------------------------
sub copyReleaseToTemp
{


	# Retrieve our parameters
	my ($srcDir, $releaseDir) = @_;



	# Copy the source
	print "    Copying source to temporary directory";
	system("ditto -rsrcFork \"$srcDir\" \"$releaseDir\"");
	print "                                     done\n";



	# Remove binaries
	removeBinaryFiles($releaseDir);
}





#------------------------------------------------------------------------
# Convert line endings to a particular format.
#------------------------------------------------------------------------
sub convertLineEndings
{


	# Retrieve our parameters
	my ($lineEnding, $theDir) = @_;



	# Identify the files we want to process
	print "    Converting line endings";
	my @theFiles = ();
	find(sub
		{
		if ($_ =~ /\.c$/		||
			$_ =~ /\.h$/		||
			$_ =~ /\.m$/		||
			$_ =~ /\.pl$/		||
			$_ =~ /\.rc$/		||
			$_ =~ /\.dsp$/		||
			$_ =~ /\.dsw$/		||
			$_ =~ /\.exp$/		||
			$_ =~ /makefile/)
			{
			push(@theFiles, $File::Find::dir . "/" . $_);
			}
		}, $theDir);



	# Rewrite each file to convert the line endings	
	foreach my $theFile (@theFiles)
		{
		# Read in the file
		my $fileData = "";
		{
		local($/) = undef;
		open(IN_FILE, $theFile) or die "Can't open $theFile for reading: $!\n";
		$fileData = <IN_FILE>;
		close(IN_FILE);
		}


		# Convert the line endings
		#
		# We normalise to Unix first, then convert to DOS or Mac as required.
		#
		# The normalisation is needed so that we don't process line endings twice.
		# E.g., Mac to DOS produces \r\n, which can't then be processed for Unix
		# to DOS or the \n in \r\n will be expanded to \r\n - giving us a final
		# line ending of \r\r\n.
		$fileData =~ s/\r\n/\n/g;						# DOS to Unix
		$fileData =~ s/\r/\n/g;							# Mac to Unix

		if ($lineEnding eq "\r")
			{
			$fileData =~ s/\n/\r/g;						# Unix to Mac
			}
		elsif ($lineEnding eq "\r\n")
			{
			$fileData =~ s/\n/\r\n/g;					# Unix to DOS
			}


		# Write out the file
		open(OUT_FILE, ">$theFile") or die "Can't open $theFile for writing: $!\n";
		print(OUT_FILE $fileData);
		close(OUT_FILE);
		}

	print "                                                   done\n";
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
# Find the version for the release.
#------------------------------------------------------------------------
sub findVersion
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
	return($1);
}





#------------------------------------------------------------------------
# Generate the Mac release.
#------------------------------------------------------------------------
sub generateReleaseMac
{


	# Retrieve our parameters
	my ($releaseDir) = @_;



	# Prepare to create the archive
	my $theArchive = $releaseDir . ".sit";
	deleteItems($theArchive);



	# Convert line endings to Mac format
	convertLineEndings("\r", $releaseDir);



	# Tell DropStuff to stuff the release directory
	my $theScript = "tell application \"DropStuff\"\n";
	$theScript   .= "    with timeout of (60 * 60) seconds\n";
	$theScript   .= "        activate\n";
	$theScript   .= "        open alias POSIX file \"$releaseDir\"\n";
	$theScript   .= "        quit\n";
	$theScript   .= "    end timeout\n";
	$theScript   .= "end tell\n";

	print "    Creating archive";
	system("osascript -e '$theScript' 2> /dev/null");
	print "                                                          done\n";



	# Finish off
	moveToDesktop($theArchive);
}





#------------------------------------------------------------------------
# Generate the Unix release.
#------------------------------------------------------------------------
sub generateReleaseUnix
{



	# Retrieve our parameters
	my ($releaseDir, $releaseName) = @_;



	# Prepare to create the archive
	my $theArchive = $releaseDir . ".tar.gz";
	deleteItems($theArchive);



	# Convert line endings to Unix format
	convertLineEndings("\n", $releaseDir);



	# Create the .tar.gz file
	print "    Creating archive";
	system("cd \"$releaseDir\"/..; tar cf \"$releaseName\".tar \"$releaseName\"; gzip \"$releaseName\".tar");
	print "                                                          done\n";


	# Finish off
	moveToDesktop($theArchive);
}





#------------------------------------------------------------------------
# Generate the Windows release.
#------------------------------------------------------------------------
sub generateReleaseWin
{



	# Retrieve our parameters
	my ($releaseDir, $releaseName) = @_;



	# Prepare to create the archive
	my $theArchive = $releaseDir . ".zip";
	deleteItems($theArchive);



	# Convert line endings to DOS format
	convertLineEndings("\r\n", $releaseDir);



	# Create the .zip file
	print "    Creating archive";
	system("cd \"$releaseDir\"/..; zip -r -q \"$releaseName\".zip \"$releaseName\"");
	print "                                                          done\n";



	# Finish off
	moveToDesktop($theArchive);
}





#------------------------------------------------------------------------
# Generate the Mac SDK release.
#------------------------------------------------------------------------
sub generateReleaseMacSDK
{


	# Retrieve our parameters
	my ($srcDir, $tmpDir, $releaseVersion) = @_;



	# Set up CW and the projects we need
	my $projectQuesa    = $srcDir. "Development/Projects/Mac/Quesa.mcp";
	my $projectGeomTest = $srcDir. "SDK/Examples/Geom Test/Geom Test.mcp";
	my $theScript       = "";



	# Build Quesa
	$theScript  = "tell application \"$kCodeWarriorIDE\"\n";
	$theScript .= "    with timeout of (60 * 60) seconds\n";
	$theScript .= "        activate\n";
	$theScript .= "        open alias POSIX file \"$projectQuesa\"\n";
	$theScript .= "        Set Current Target \"Build All\"\n";
	$theScript .= "        Make Project\n";
	$theScript .= "        Close Project\n";
	$theScript .= "    end timeout\n";
	$theScript .= "end tell\n";

	print "    Building Quesa";
	system("osascript -e '$theScript'");
	print "                                                            done\n";



	# Build Geom Test
	$theScript  = "tell application \"$kCodeWarriorIDE\"\n";
	$theScript .= "    with timeout of (60 * 60) seconds\n";
	$theScript .= "        activate\n";
	$theScript .= "        open alias POSIX file \"$projectGeomTest\"\n";
	$theScript .= "        Set Current Target \"Carbon Release\"\n";
	$theScript .= "        Make Project\n";
	$theScript .= "        Set Current Target \"Classic Release\"\n";
	$theScript .= "        Make Project\n";
	$theScript .= "        Close Project\n";
	$theScript .= "    end timeout\n";
	$theScript .= "end tell\n";

	print "    Building Geom Test";
	system("osascript -e '$theScript'");
	print "                                                        done\n";



	# Create the output directories
	print "    Creating output directories";

	my $outputDir = $tmpDir . "Quesa $releaseVersion/";
	deleteItems($outputDir);

	mkpath($outputDir);
	mkpath($outputDir . "Carbon");
	mkpath($outputDir . "Classic");
	mkpath($outputDir . "Images");
	mkpath($outputDir . "Models");
	mkpath($outputDir . "Developers/Includes/Quesa");
	mkpath($outputDir . "Developers/Stub Libraries");
	print "                                               done\n";



	# Copy the files
	my $cpFrom = "ditto -rsrcFork \"$srcDir";
	my $cpTo   = "\" \"$outputDir";
	my $cpEnd  = "\"";

	print "    Copying files";
	system($cpFrom . "SDK/Libraries/Mac/Release/Quesa"                        . $cpTo . "Carbon"  . $cpEnd);
	system($cpFrom . "SDK/Examples/Geom Test/Geom Test"                       . $cpTo . "Carbon"  . $cpEnd);
	system($cpFrom . "SDK/Examples/Support Files/Images/Quesa.jpg"            . $cpTo . "Carbon"  . $cpEnd);
	system($cpFrom . "SDK/Libraries/Mac/Release/Quesa Classic"                . $cpTo . "Classic" . $cpEnd);
	system($cpFrom . "SDK/Libraries/Mac/Stub/Quesa Custom Elements"           . $cpTo . "Classic" . $cpEnd);
	system($cpFrom . "SDK/Libraries/Mac/Stub/Quesa Gestalt"                   . $cpTo . "Classic" . $cpEnd);
	system($cpFrom . "SDK/Examples/Geom Test/Geom Test (Classic)"             . $cpTo . "Classic" . $cpEnd);
	system($cpFrom . "SDK/Examples/Support Files/Images/Quesa.jpg"            . $cpTo . "Classic" . $cpEnd);
	system($cpFrom . "Documentation/images/logo_gray.gif"                     . $cpTo . "Images"  . $cpEnd);
	system($cpFrom . "Documentation/images/logo_text.jpg"                     . $cpTo . "Images"  . $cpEnd);
	system($cpFrom . "SDK/Examples/Support Files/Models/3DMF/F18.3dmf"        . $cpTo . "Models"  . $cpEnd);
	system($cpFrom . "SDK/Examples/Support Files/Models/3DMF/Quesa Logo.3dmf" . $cpTo . "Models"  . $cpEnd);
	system($cpFrom . "SDK/Examples/Support Files/Models/3DMF/Pod Racer.3dmf"  . $cpTo . "Models"  . $cpEnd);
	system($cpFrom . "SDK/Includes/Quesa/\"*.h\""                             . $cpTo . "Developers/Includes/Quesa" . $cpEnd);
	system($cpFrom . "SDK/Libraries/Mac/Stub/QuesaLib"                        . $cpTo . "Developers/Stub Libraries" . $cpEnd);
	print "                                                             done\n";



	# Convert line endings to Mac format
	convertLineEndings("\r", $outputDir);



	# Generate the read me
	my $theTemplate = $kTemplateMacReadMe;
	$theTemplate    =~ s/RELEASE_VERSION/$releaseVersion/g;

	my $theFile = $outputDir . "Quesa Read Me.html";
    open( OUT_FILE, ">$theFile") or die("Can't open $theFile for writing: $!\n");
    print(OUT_FILE "$theTemplate");
	close(OUT_FILE);

	system("SetFile -t TEXT -c MSIE \"$theFile\"");



	# Prepare to create the archive
	my $theArchive = $tmpDir . "Quesa $releaseVersion.sit";
	deleteItems($theArchive);



	# Tell DropStuff to stuff the release directory
	$theScript  = "tell application \"DropStuff\"\n";
	$theScript .= "    with timeout of (60 * 60) seconds\n";
	$theScript .= "        activate\n";
	$theScript .= "        open alias POSIX file  \"$outputDir\"\n";
	$theScript .= "        quit\n";
	$theScript .= "    end timeout\n";
	$theScript .= "end tell\n";

	print "    Creating archive";
	system("osascript -e '$theScript' 2> /dev/null");
	print "                                                          done\n";



	# Finish off
	my $finalArchive = $theArchive;
	$finalArchive    =~ s/ ($releaseVersion.sit)$/_SDK_$1/;

	system("mv \"$theArchive\" \"$finalArchive\"");
	moveToDesktop($finalArchive);
}





#------------------------------------------------------------------------
# Generate the Windows SDK release.
#------------------------------------------------------------------------
sub generateReleaseWinSDK
{


	# Retrieve our parameters
	my ($srcDir, $tmpDir, $releaseVersion) = @_;



	# Create the output directories
	print "    Creating output directories";

    my $outpuName = "Quesa $releaseVersion";
	my $outputDir = $tmpDir . $outpuName . "/";
	deleteItems($outputDir);

	mkpath($outputDir);
	mkpath($outputDir . "Program Files");
	mkpath($outputDir . "Images");
	mkpath($outputDir . "Models");
	mkpath($outputDir . "Developers/Includes/Quesa");
	mkpath($outputDir . "Developers/Stub Libraries");
	print "                                               done\n";



	# Copy the files
	my $cpFrom   = "ditto -rsrcFork \"$srcDir";
	my $cpFromPC = "ditto           \"$kPathToPCBuild";
	my $cpTo     = "\" \"$outputDir";
	my $cpEnd    = "\"";

	print "    Copying files";
	system($cpFromPC . "SDK/Libraries/Windows/Release/Quesa.dll"                . $cpTo . "Program Files" . $cpEnd);
	system($cpFromPC . "SDK/Examples/Geom Test/Release/Geom Test.exe"           . $cpTo . "Program Files" . $cpEnd);
	system($cpFrom   . "SDK/Examples/Support Files/Images/Quesa.jpg"            . $cpTo . "Program Files" . $cpEnd);
	system($cpFrom   . "Documentation/images/logo_gray.gif"                     . $cpTo . "Images"        . $cpEnd);
	system($cpFrom   . "Documentation/images/logo_text.jpg"                     . $cpTo . "Images"        . $cpEnd);
	system($cpFrom   . "SDK/Examples/Support Files/Models/3DMF/F18.3dmf"        . $cpTo . "Models"        . $cpEnd);
	system($cpFrom   . "SDK/Examples/Support Files/Models/3DMF/Quesa Logo.3dmf" . $cpTo . "Models"        . $cpEnd);
	system($cpFrom   . "SDK/Examples/Support Files/Models/3DMF/Pod Racer.3dmf"  . $cpTo . "Models"        . $cpEnd);
	system($cpFrom   . "SDK/Includes/Quesa/\"*.h\""                             . $cpTo . "Developers/Includes/Quesa" . $cpEnd);
	system($cpFromPC . "SDK/Libraries/Windows/Stub/Quesa.lib"                   . $cpTo . "Developers/Stub Libraries" . $cpEnd);
	print "                                                             done\n";



	# Convert line endings to DOS format
	convertLineEndings("\r\n", $outputDir);



	# Generate the read me
	my $theTemplate = $kTemplateWinReadMe;
	$theTemplate    =~ s/RELEASE_VERSION/$releaseVersion/g;

	my $theFile = $outputDir . "Quesa Read Me.html";
    open( OUT_FILE, ">$theFile") or die("Can't open $theFile for writing: $!\n");
    print(OUT_FILE "$theTemplate");
	close(OUT_FILE);



	# Create the archive
	my $theArchive = $tmpDir . "Quesa_SDK_$releaseVersion.zip";
	deleteItems($theArchive);

	print "    Creating archive";
	system("cd \"$outputDir\"/..; zip -r -q \"$theArchive\" \"$outpuName\"");
	print "                                                          done\n";

	moveToDesktop($theArchive);
}





#------------------------------------------------------------------------
# Build the release.
#------------------------------------------------------------------------
sub buildRelease
{


	# Locate our directories
	my $srcDir     = findSrcDir();
	my $dstDir     = findDstDir();
	my $tmpDir     = $dstDir . "/Quesa Temporary Items/";



	# Identify the release
	my $releaseVersion = findVersion();
	my $releaseName    = "quesa_" . $releaseVersion;
	my $releaseDir     = $tmpDir  . $releaseName;

	die("Could not determine release name - findVersion failed") if ($releaseVersion eq "");
	print "Preparing Quesa $releaseVersion release\n";



	# Create the temporary directory
	print "    Creating temporary directory";
	deleteItems($tmpDir);
	mkpath($releaseDir);
	print "                                              done\n";



	# Copy the release to the temporary directory
	copyReleaseToTemp($srcDir, $releaseDir);



	# Generate the releases
	print "\nGenerating Mac release\n";
	generateReleaseMac($releaseDir);

	print "\nGenerating Unix release\n";
	generateReleaseUnix($releaseDir, $releaseName);

	print "\nGenerating Windows release\n";
	generateReleaseWin($releaseDir, $releaseName);

	print "\nGenerating Mac SDK release\n";
	generateReleaseMacSDK($srcDir, $tmpDir, $releaseVersion);

	print "\nGenerating Windows SDK release\n";
	generateReleaseWinSDK($srcDir, $tmpDir, $releaseVersion);



	# Clean up
	deleteItems($tmpDir);
	print "\nFinished\n";
}





#------------------------------------------------------------------------
# Script entry point
#------------------------------------------------------------------------
# Build the release
buildRelease();




