#!/usr/bin/perl -w
#========================================================================
#   Description:
#       Generate the HTML for the status section of the docs.
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
use File::Basename;





#------------------------------------------------------------------------
# Globals
#------------------------------------------------------------------------
my @gHeaderList = ( "35 QD3D.h",
					"43 QD3DCamera.h",
					"47 QD3DController.h",
					"9 QD3DCustomElements.h",
					"49 QD3DDrawContext.h",
					"8 QD3DErrors.h",
					"16 QD3DExtension.h",
					"315 QD3DGeometry.h",
					"38 QD3DGroup.h",
					"103 QD3DIO.h",
					"45 QD3DLight.h",
					"133 QD3DMath.h",
					"34 QD3DPick.h",
					"49 QD3DRenderer.h",
					"21 QD3DSet.h",
					"38 QD3DShader.h",
					"26 QD3DStorage.h",
					"6 QD3DString.h",
					"46 QD3DStyle.h",
					"49 QD3DTransform.h",
					"48 QD3DView.h",
					"57 QD3DViewer.h",
					"47 QD3DWinViewer.h" );

my $kTemplateHeader = <<'TEMPLATE_HEADER';
<!--=========================================================================-->
<!-- Page Header                                                             -->
<!--=========================================================================-->
<html>
<head>
	<title>Quesa - Status</title>
</head>
<body bgcolor=white text=black background="../images/logo_gray.gif">

<p align=center>
<a href="../index.html"><img src="../images/logo_text.jpg" alt="Quesa" width=311 height=105 border=0></a>
</p>



<!--=========================================================================-->
<!-- Page Content                                                            -->
<!--=========================================================================-->
<table border=0>
<tr>
<td valign=top width=90 bgcolor="#EEEEEE">
	<table border=0 bgcolor="#404090" width="100%">
	<tr><td><font color=white size="+1">Info</font></td></tr></table><br>
	<a href="../info/status.html">Status</a><br>
	<a href="../info/screenshots.html">Screenshots</a><br>
	<a href="../info/download.html">Download</a><br>
	<a href="../info/list.html">Mailing List</a><br>
	<a href="../info/list_archive.html">List Archive</a><br>
	<a href="http://www.quesa.org/quesa_forum.html">Quesa Forum</a><br>

	<br>
	<table border=0 bgcolor="#404090" width="100%">
	<tr><td><font color=white size="+1">Developer</font></td></tr></table><br>
	<a href="../developer/todo.html">Todo</a><br>
	<a href="../developer/bugs.html">Bug List</a><br>
	<a href="../developer/cvs.html">CVS Server</a><br>
	<a href="../developer/apiref/MasterTOC.html">API Reference</a><br>

	<br>
	<table border=0 bgcolor="#404090" width="100%">
	<tr><td><font color=white size="+1">Reference</font></td></tr></table><br>
	<a href="../reference/docs.html">Documentation</a><br>
	<a href="../reference/build.html">Building Quesa</a><br>
	<a href="../reference/lgpl.html">LGPL License</a><br>
	<a href="../reference/carbon.html">Carbon Dating</a><br>

	<br>
	<table border=0 bgcolor="#404090" width="100%">
	<tr><td><font color=white size="+1">Other</font></td></tr></table><br>
	<a href="../other/contributors.html">Contributors</a><br>
	<a href="../other/logo.html">Quesa Logo</a><br>
	<a href="../other/links.html">Links</a><br>
</td>
<!--=========================================================================-->
<td width=10>&nbsp;</td>
<td valign=top>


<h3>Status</h3>
<p>
The QD3D API contains 1262 API calls, exposed through 23 header files.
</p>

<p>
Quesa currently implements the majority of these API calls, as detailed below:
</p>

TEMPLATE_HEADER

my $kTemplateFooter = <<'TEMPLATE_FOOTER';

<p>
This table is generated automatically on each release from the list of currently implemented
functions, and does not cover extensions to the main QD3D API.
</p>

</td>
</tr>
</table>





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
Quesa Web Site hosted by <a href="http://www.DesignCommunity.com/home.html">DesignCommunity.com</a>
</font>
</td>
</tr>
</table>

</body>
</html>
TEMPLATE_FOOTER





#------------------------------------------------------------------------
# Calculate an integer percentage.
#------------------------------------------------------------------------
sub calcPercent
{


	# Retrieve our parameters
	my ($theTotal, $amountDone) = @_;



	# Work out the percentage, watching out for divide by 0
	my $percentDone = ($theTotal == 0) ? 100 : (($amountDone / $theTotal) * 100.0);
	
	return(int($percentDone));
}





#------------------------------------------------------------------------
# Work out what colour a row should be.
#------------------------------------------------------------------------
sub rowColour
{


	# Retrieve our parameters
	my ($percentDone) = @_;



	# Return an appropriate colour
	return "ff0000" if ($percentDone >  100);
	return "ffdddd" if ($percentDone == 100);
	return "ffffee" if ($percentDone == 0);
	
	return "ddddff";
}





#------------------------------------------------------------------------
# Process the status file.
#------------------------------------------------------------------------
sub processStatus
{


	# Retrieve our parameters
	my ($statusFile) = @_;



	# Initialise ourselves
	my $htmlFile     = basename($statusFile, ".txt") . ".html";
	my $rowColour    = "";
	my @functionList = ();



    # Parse the status file
    open(IN_FILE,  "<$statusFile") or die("Can't open $statusFile for reading: $!\n");

    while (defined(my $theLine = <IN_FILE>))
        {
        # Skip comments
        chomp $theLine;
        next if ($theLine eq "");
        next if ($theLine =~ /^#/);
        
        
        # Grab the header and name of each implemented function
        $theLine        =~ /(\w+\.h)\s+(\w+)/;
		my $theHeader   = $1;
		my $theFunction = $2;

		push(@functionList, "$theHeader $theFunction");
        }

    close IN_FILE;



	# Generate the html file
    open(OUT_FILE, ">$htmlFile")   or die("Can't open $htmlFile for writing: $!\n");

	print OUT_FILE $kTemplateHeader;



	# Generate the header file table
	print OUT_FILE "<p>\n";
	print OUT_FILE "<!-- table auto-generated by status.pl -->\n";
	print OUT_FILE "<table width=\"90%\" border=1 bgcolor=#ffffee align=center>\n";
	print OUT_FILE "<tr align=center>\n";
	print OUT_FILE "<th colspan=4>Quesa Header Status</th>\n";
	print OUT_FILE "<tr align=center>\n";
	print OUT_FILE "<th align=left>Header File</th>\n";
	print OUT_FILE "<th align=right>Function Count</th>\n";
	print OUT_FILE "<th align=right>Functions Implemented</th>\n";
	print OUT_FILE "<th align=right>Percentage Implemented</th>\n";

	my $totalFuncCount  = 0;
	my $totalImpCount   = 0;
	my $totalImpPercent = 0.0;

	foreach my $theHeader (@gHeaderList)
		{
		# Split up the header
		$theHeader     =~ /(\d+)\s+(.*)/;
		my $funcCount  = $1;
		my $headerName = $2;



		# Find out how much has been implemented
		my $impCount   = 0;
		my $impPercent = 0.0;
		foreach my $impFunction (@functionList)
			{
			$impFunction =~ /(\w+\.h)\s+(\w+)/;
			$impCount++ if ($1 eq $headerName);
			}
		$impPercent = calcPercent($funcCount, $impCount);



		# Generate the row
		$rowColour = rowColour($impPercent);
	 	print OUT_FILE "<tr align=right bgcolor=#$rowColour><td align=left>$headerName</td> <td>$funcCount</td> <td>$impCount</td> <td>$impPercent%</td>\n";

		$totalFuncCount += $funcCount;
		$totalImpCount  += $impCount;
		}

	$totalImpPercent = calcPercent($totalFuncCount, $totalImpCount);
	$rowColour       = rowColour($totalImpPercent);

	print OUT_FILE "<tr align=right bgcolor=#$rowColour><td align=left><i>Total</td> <td><i>$totalFuncCount</td> <td><i>$totalImpCount</td> <td><i>$totalImpPercent%\n";
	print OUT_FILE "</table>\n";
	print OUT_FILE "</p>\n";



	# Generate the footer
	print OUT_FILE $kTemplateFooter;



	# Clean up
    close OUT_FILE;
    
    MacPerl::SetFileInfo("MSIE", "TEXT", $htmlFile) if ($^O eq "MacOS");
}





#------------------------------------------------------------------------
# Program entry point
#------------------------------------------------------------------------
# Process the status file
processStatus("status.txt");
