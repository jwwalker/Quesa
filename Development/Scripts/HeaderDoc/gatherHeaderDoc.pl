#! /usr/bin/perl -w
#
# Script name: gatherHeaderDoc
# Synopsis: 	Finds all HeaderDoc generated docs in an input
#		folder and creates a top-level HTML page to them
#
# Author: Matt Morse (matt@apple.com)
# Last Updated: $Date $
# 
# Copyright (c) 1999 Apple Computer, Inc.  All Rights Reserved.
# The contents of this file constitute Original Code as defined in and are
# subject to the Apple Public Source License Version 1.1 (the "License").
# You may not use this file except in compliance with the License.  Please
# obtain a copy of the License at http://www.apple.com/publicsource and
# read it before using this file.
#
# This Original Code and all software distributed under the License are
# distributed on an TAS ISU basis, WITHOUT WARRANTY OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
# INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the License for
# the specific language governing rights and limitations under the
# License.
#
# $Revision: 1.1 $
######################################################################





######################################################################
# dair, Quesa template                                               #
######################################################################
my $quesaPageTemplate = qq(
<!--=========================================================================-->
<!-- Page Header                                                             -->
<!--=========================================================================-->
<html>
<head>
	<title>Quesa - API Reference</title>
</head>
<body bgcolor=white text=black background="../../images/logo_gray.gif">

<p align=center>
<a href="../../index.html"><img src="../../images/logo_text.jpg" alt="Quesa" width=311 height=105 border=0></a>
</p>



<!--=========================================================================-->
<!-- Page Content                                                            -->
<!--=========================================================================-->
<table border=0>
<tr>
<td valign=top width=90 bgcolor="#EEEEEE">
	<table border=0 bgcolor="#404090" width="100%">
	<tr><td><font color=white size="+1">Info</font></td></tr></table><br>
	<a href="../../info/status.html">Status</a><br>
	<a href="../../info/screenshots.html">Screenshots</a><br>
	<a href="../../info/download.html">Download</a><br>
	<a href="../../info/list.html">Mailing List</a><br>
	<a href="http://quesa.DesignCommunity.com/info/list_archive.html">List Archive</a><br>
	<a href="http://quesa.DesignCommunity.com/quesa_forum.html">Quesa Forum</a><br>

	<br>
	<table border=0 bgcolor="#404090" width="100%">
	<tr><td><font color=white size="+1">Developer</font></td></tr></table><br>
	<a href="../../developer/todo.html">Todo</a><br>
	<a href="../../developer/bugs.html">Bug List</a><br>
	<a href="../../developer/cvs.html">CVS Server</a><br>
	<a href="../../developer/apiref/MasterTOC.html">API Reference</a><br>

	<br>
	<table border=0 bgcolor="#404090" width="100%">
	<tr><td><font color=white size="+1">Reference</font></td></tr></table><br>
	<a href="../../reference/docs.html">Documentation</a><br>
	<a href="../../reference/build.html">Building Quesa</a><br>
	<a href="../../reference/lgpl.html">LGPL License</a><br>
	<a href="../../reference/carbon.html">Carbon Dating</a><br>

	<br>
	<table border=0 bgcolor="#404090" width="100%">
	<tr><td><font color=white size="+1">Other</font></td></tr></table><br>
	<a href="../../other/contributors.html">Contributors</a><br>
	<a href="../../other/logo.html">Quesa Logo</a><br>
	<a href="../../other/links.html">Links</a><br>
</td>
<!--=========================================================================-->
<td width=10>&nbsp;</td>
<td valign=top>


<h3>API Reference</h3>
<p>
A detailed API reference for Quesa is available, organised by header file:
</p>

<p>
QUESA_HEADER_LIST
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
);





use Cwd;
use File::Find;
use File::Copy;

my $pathSeparator;
my $isMacOS;
my $scriptDir;
my $framesetFileName;
my $masterTOCFileName;
BEGIN {
    if ($^O =~ /MacOS/i) {
            $pathSeparator = ":";
            $isMacOS = 1;
    } else {
            $pathSeparator = "/";
            $isMacOS = 0;
    }
    ################ Setup from Configuration File #######################
    # read configuration file
    $scriptDir = cwd();
    do "$scriptDir"."$pathSeparator"."headerDoc2HTML.config";

    if (defined $config{"defaultFrameName"}) {
        $framesetFileName = $config{"defaultFrameName"};
    } else {
        print "No default frame name defined in configuration file.  Using 'index.html'.\n";
        $framesetFileName = 'index.html';
    }

    if (defined $config{"masterTOCName"}) {
        $masterTOCFileName = $config{"masterTOCName"};
    } else {
        print "No default name for master table of contents.  Using 'masterTOC.html'.\n";
        $masterTOCFileName = 'masterTOC.html';
    }
}

use strict;
use FindBin qw ($Bin);
use lib "$Bin"."$pathSeparator"."Modules";

# Modules specific to gatherHeaderDoc
use HeaderDoc::DocReference;
use HeaderDoc::Utilities qw(findRelativePath safeName getAPINameAndDisc convertCharsForFileMaker printArray printHash);

my $debugging = 1;
######################################## Design Overview ###################################
# - We scan input directory for frameset files (index.html, by default).
# - For each frameset file, we look for a special HTML comment (left by HeaderDoc)
#   that tell us the name of the header/class and the type (header or cppclass). 
# - We create a DocReference object to store this info, and also the path to the
#   frameset file.
# - We run through array of DocRef objs and create a master TOC based on the info
# - Finally, we visit each TOC file in each frameset and add a "[Top]" link
#   back to the master TOC.  [This is fragile in the current implementation, since
#   we find TOCs based on searching for a file called "toc.html" in the frameset dir.
# 
############################################# Input Folder and Files #######################
my @inputFiles;
my $inputDir;

if (($#ARGV == 0) && (-d $ARGV[0])) {
    $inputDir = $ARGV[0];
    $inputDir =~ s|(.*)/$|$1|; # get rid of trailing slash, if any

	# dair, don't do this on Mac OS
    if ($^O !~ /MacOS/i) {
	    if ($inputDir !~ /^\//) { # not absolute path -- !!! should check for ~
	        my $cwd = cwd();
	        $inputDir = $cwd.$pathSeparator.$inputDir;
	    }
    }
# dair, changed for MacPerl
#    &find({wanted => \&getFiles, follow => 1}, $inputDir);
    &find(\&getFiles, $inputDir);
} else {
    die "You must specify a single input directory for processing.\n";
}
unless (@inputFiles) { print "No valid input files specified. \n\n";};

sub getFiles {
    my $filePath = $File::Find::name;
    my $fileName = $_;

    if ($fileName =~ /$framesetFileName/) {
        push(@inputFiles, $filePath);
    }
}
################### Find HeaderDoc Comments #######################
my @headerFramesetRefs;
my @classFramesetRefs;

my $oldRecSep = $/;
undef $/; # read in files as strings

foreach my $file (@inputFiles) {
    open (INFILE, "<$file") || die "Can't open $file: $!\n";
    my $fileString = <INFILE>;
    close INFILE;
    if ($fileString =~ /<--\s+(headerDoc\s*=.*?)-->/) {
        my $fullComment = $1;
        my @pairs = split(/;/, $fullComment);
        my $docRef = HeaderDoc::DocReference->new;
        $docRef->path($file);
        foreach my $pair (@pairs) {
            my ($key, $value) = split(/=/, $pair);
            $key =~ s/^\s+|\s+$//;
            $value =~ s/^\s+|\s+$//;
            SWITCH: {
                ($key =~ /headerDoc/) && 
                    do {
                        $docRef->type($value);
                        last SWITCH;
                    };
                ($key =~ /name/) && 
                    do {
                        $docRef->name($value);
                        last SWITCH;
                    };
            }
        }
        my $tmpType = $docRef->type();
        if ($tmpType eq "Header") {
            push (@headerFramesetRefs, $docRef);
        } elsif ($tmpType eq "CPPClass") {
            push (@classFramesetRefs, $docRef);
        } else {
            my $tmpName = $docRef->name();
            my $tmpPath = $docRef->path();
            print "Unknown type '$tmpType' for document with name '$tmpName' and path '$tmpPath'\n";
        }
    }
}
$/ = $oldRecSep;

# create master TOC if we have any framesets
if (scalar(@headerFramesetRefs) + scalar(@classFramesetRefs)) {
    &printMasterTOC();
    &addTopLinkToFramesetTOCs();
} else {
    print "gatherHeaderDoc.pl: No HeaderDoc framesets found--returning\n" if ($debugging); 
    return;
}
exit 0;

################### Print Navigation Page #######################
sub printMasterTOC {
    my $outputDir = $inputDir;
    my $masterTOC = $outputDir.$pathSeparator. $masterTOCFileName;
    my $headersLinkString= '';
    my $classesLinkString = '';
    my $fileString;
    my $localDebug = 0;

    # get the HTML links to each header 
    foreach my $ref (sort objName @headerFramesetRefs) {
        my $name = $ref->name();
        my $path = $ref->path();        
        my $tmpString = &getLinkToFramesetFrom($masterTOC, $path, $name);
        $headersLinkString .= $tmpString;
    }
    print "\$headersLinkString is '$headersLinkString'\n" if ($localDebug);
    
    # get the HTML links to each class 
    foreach my $ref (sort objName @classFramesetRefs) {
        my $name = $ref->name();
        my $path = $ref->path();        
        my $tmpString = &getLinkToFramesetFrom($masterTOC, $path, $name);
        $classesLinkString .= $tmpString;
    }
    if (($localDebug) && length($classesLinkString)) {print "\$classesLinkString is '$classesLinkString'\n";};
    
    # put together header/footer with linkString--could use template
    my $htmlHeader = "<html><head><title>Header Documentation</title></head><body bgcolor=\"#cccccc\"><h1>Header Documentation</h1><hr><br>\n";
    my $headerSection = "<h2>Headers</h2>\n<blockquote>\n".$headersLinkString."\n</blockquote>\n";
    my $classesSection = '';
    if (length($classesLinkString)) {
    	$classesSection = "<h2>Classes</h2>\n<blockquote>\n".$classesLinkString."\n</blockquote>\n";
    }
    my $htmlFooter = "</body>\n</html>\n";
    $fileString = $htmlHeader.$headerSection.$classesSection.$htmlFooter;


	# dair, adjust layout for Quesa
	$fileString = $quesaPageTemplate;
	$fileString =~ s/QUESA_HEADER_LIST/$headersLinkString/;


    # write out page
    print "gatherHeaderDoc.pl: writing master TOC to $masterTOC\n" if ($localDebug);
    open(OUTFILE, ">$masterTOC") || die "Can't write $masterTOC.\n";

	# dair, set file type on Mac OS
    if ($^O =~ /MacOS/i) {MacPerl::SetFileInfo('MSIE', 'TEXT', "$masterTOC");};

    print OUTFILE $fileString;
    close OUTFILE;
}

sub addTopLinkToFramesetTOCs {
    my $masterTOC = $inputDir.$pathSeparator. $masterTOCFileName;
    my $tocFileName = "toc.html";
    my @allDocRefs;
    push(@allDocRefs, @headerFramesetRefs);
    push(@allDocRefs, @classFramesetRefs);
    my $localDebug = 0;
    
    foreach my $ref (@allDocRefs) {
        my $name = $ref->name();
        my $type = $ref->type();
        my $path = $ref->path();
        my $tocFile = $path;   				# path to index.html
        my $fsName = quotemeta($framesetFileName);
        $tocFile =~ s/$fsName$/toc.html/; 		# path to toc.html
        
        if (-e "$tocFile" ) {
            my $oldRecSep = $/;
            undef $/; # read in file as string
            open(INFILE, "<$tocFile") || die "Can't read file $tocFile.\n";
            my $fileString = <INFILE>;
            close INFILE;
            $/ = $oldRecSep;
            
            my $uniqueMarker = "headerDoc=\"topLink\"";
            if ($fileString !~ /$uniqueMarker/) { # we haven't been here before
                my $relPathToMasterTOC = &findRelativePath($tocFile, $masterTOC);
# dair, updated for Quesa
#                my $topLink = "\n<font size=\"-2\"><a href=\"$relPathToMasterTOC\" target=\"_top\" $uniqueMarker>[Top]</a></font><br>\n";
                my $topLink = "\n<a href=\"$relPathToMasterTOC\" target=\"_top\" $uniqueMarker>[Up To API Reference]</a><br><br>\n";
                
                $fileString =~ s/(<body[^>]*>)/$1$topLink/i;
            
                open (OUTFILE, ">$tocFile") || die "Can't write file $tocFile.\n";
                print OUTFILE $fileString;
                close (OUTFILE);
            }
        } elsif ($debugging) {
            print "--> '$tocFile' doesn't exist!\n";
            print "Cannot add [top] link for frameset doc reference:\n";
            print "   name: $name\n";
            print "   type: $type\n";
            print "   path: $path\n";
        }
    }
}

sub getLinkToFramesetFrom {
    my $masterTOCFile = shift;
    my $dest = shift;    
    my $name = shift;    
    my $linkString;
    
    my $relPath = &findRelativePath($masterTOCFile, $dest);
    $linkString = "<a href=\"$relPath\" target =\"_top\">$name</a><br>\n"; 
    return $linkString;
}


sub objName { # for sorting objects by their names
    uc($a->name()) cmp uc($b->name());
}
