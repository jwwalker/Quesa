#!/usr/bin/perl -w
#========================================================================
#   Description:
#       Generates a skeleton QD3D implementation from the QD3D headers.
#
#		Note that this skeleton no longer corresponds directly to the
#		Quesa file layout, but this script is included anyway in case
#		you ever feel like seeing what the starting point was.
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





#------------------------------------------------------------------------
# Globals
#------------------------------------------------------------------------
# Hashes
my %gParamType = (  "TQ3ElementObject"				=> "kQ3ObjectTypeElement",
					"TQ3SharedObject"				=> "kQ3ObjectTypeShared",
					"TQ3ViewObject"					=> "kQ3ObjectTypeView",
					"TQ3PickObject"					=> "kQ3ObjectTypePick",
					"TQ3RendererObject"				=> "kQ3SharedTypeRenderer",
					"TQ3DrawContextObject"			=> "kQ3SharedTypeDrawContext",
					"TQ3SetObject"					=> "kQ3SharedTypeSet",
					"TQ3FileObject"					=> "kQ3SharedTypeFile",
					"TQ3ShapeObject"				=> "kQ3SharedTypeShape",
					"TQ3ShapePartObject"			=> "kQ3SharedTypeShapePart",
					"TQ3ControllerStateObject"		=> "kQ3SharedTypeControllerState",
					"TQ3TrackerObject"				=> "kQ3SharedTypeTracker",
					"TQ3StringObject"				=> "kQ3SharedTypeString",
					"TQ3StorageObject"				=> "kQ3SharedTypeStorage",
					"TQ3TextureObject"				=> "kQ3SharedTypeTexture",
					"TQ3ViewHintsObject"			=> "kQ3SharedTypeViewHints",
					"TQ3AttributeSet"				=> "kQ3SetTypeAttribute", 
					"TQ3GroupObject"				=> "kQ3ShapeTypeGroup",
					"TQ3GeometryObject"				=> "kQ3ShapeTypeGeometry",
					"TQ3ShaderObject"				=> "kQ3ShapeTypeShader",
					"TQ3StyleObject"				=> "kQ3ShapeTypeStyle",
					"TQ3TransformObject"			=> "kQ3ShapeTypeTransform",
					"TQ3LightObject"				=> "kQ3ShapeTypeLight",
					"TQ3CameraObject"				=> "kQ3ShapeTypeCamera",
					"TQ3UnknownObject"				=> "kQ3ShapeTypeUnknown",
					"TQ3ReferenceObject"			=> "kQ3ShapeTypeReference",
					"TQ3StateOperatorObject"		=> "kQ3ShapeTypeStateOperator",
					"TQ3MeshPartObject"				=> "kQ3ShapePartTypeMeshPart",
					"TQ3MeshFacePartObject"			=> "kQ3MeshPartTypeMeshFacePart",
					"TQ3MeshEdgePartObject"			=> "kQ3MeshPartTypeMeshEdgePart",
					"TQ3MeshVertexPartObject"		=> "kQ3MeshPartTypeMeshVertexPart",
					"TQ3DisplayGroupObject"			=> "kQ3GroupTypeDisplay",
					"TQ3SurfaceShaderObject"		=> "kQ3ShaderTypeSurface",
					"TQ3IlluminationShaderObject"	=> "kQ3ShaderTypeIllumination");


# Hashes
my %gFuncResult = ( "BOOL"						=> "FALSE",
					"Boolean"					=> "false",
					"CGrafPtr"					=> "NULL",
					"HBITMAP"					=> "NULL",
					"HWND"						=> "NULL",
					"OSErr"						=> "paramErr",
					"PicHandle"					=> "NULL",
					"TQ3AttributeSet"			=> "NULL",
					"TQ3Boolean"				=> "kQ3False",
					"TQ3CameraObject"			=> "NULL",
					"TQ3ControllerRef"			=> "NULL",
					"TQ3ControllerStateObject"	=> "NULL",
					"TQ3DrawContextObject"		=> "NULL",
					"TQ3Error"					=> "kQ3ErrorNone",
					"TQ3FileObject"				=> "NULL",
					"TQ3GeometryObject"			=> "NULL",
					"TQ3GroupObject"			=> "NULL",
					"TQ3GroupPosition"			=> "NULL",
					"TQ3LightObject"			=> "NULL",
					"TQ3MeshComponent"			=> "NULL",
					"TQ3MeshContour"			=> "NULL",
					"TQ3MeshEdge"				=> "NULL",
					"TQ3MeshFace"				=> "NULL",
					"TQ3MeshVertex"				=> "NULL",
					"TQ3Notice"					=> "kQ3NoticeNone",
					"TQ3Object"					=> "NULL",
					"TQ3ObjectType"				=> "kQ3ObjectTypeInvalid",
					"TQ3PickObject"				=> "NULL",
					"TQ3RendererObject"			=> "NULL",
					"TQ3SetObject"				=> "NULL",
					"TQ3ShaderObject"			=> "NULL",
					"TQ3SharedObject"			=> "NULL",
					"TQ3Size"					=> "NULL",
					"TQ3StateOperatorObject"	=> "NULL",
					"TQ3Status"					=> "kQ3Failure",
					"TQ3StorageObject"			=> "NULL",
					"TQ3StringObject"			=> "NULL",
					"TQ3StyleObject"			=> "NULL",
					"TQ3TextureObject"			=> "NULL",
					"TQ3TrackerObject"			=> "NULL",
					"TQ3TransformObject"		=> "NULL",
					"TQ3ViewHintsObject"		=> "NULL",
					"TQ3ViewObject"				=> "NULL",
					"TQ3ViewStatus"				=> "kQ3ViewStatusError",
					"TQ3ViewerObject"			=> "NULL",
					"TQ3Warning"				=> "kQ3WarningNone",
					"TQ3XAttributeMask"			=> "NULL",
					"TQ3XBufferObject"			=> "NULL",
					"TQ3XFunctionPointer"		=> "NULL",
					"TQ3XObjectClass"			=> "NULL",
					"float"						=> "0.0f",
					"unsigned long"				=> "0");


# Constants
my $kDirGlueFiles   = "";
my $kDirSystemFiles = "";
my $kDirHFiles      = "";
my $kTemplateQ3C = <<'Q3_CFILE_TEMPLATE';
/*  NAME:
        QD3DFILE_NAME.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3FILE_NAME.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





Q3_CFILE_TEMPLATE


my $kTemplateE3C = <<'E3_CFILE_TEMPLATE';
/*  NAME:
        E3FILE_NAME.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3FILE_NAME.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





E3_CFILE_TEMPLATE


my $kTemplateE3H = <<'E3_HFILE_TEMPLATE';
/*  NAME:
        E3FILE_NAME.h

    DESCRIPTION:
        Header file for E3FILE_NAME.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
#ifndef E3UPPER_NAME_HDR
#define E3UPPER_NAME_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
FUNCTION_PROTOTYPES




//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

E3_HFILE_TEMPLATE





#------------------------------------------------------------------------
# Generate a Q3Foo.c source file.
#------------------------------------------------------------------------
sub generateQ3Source
{


    # Retrieve our parameters
    my ($functionListRef, $hFile) = @_;
	my @functionList = @$functionListRef;



	# Break down the file name
	my $fileName = $hFile;
	$fileName    =~ s/QD3D(.*)\.h/$1/;
	$fileName    = "Main" if ($fileName eq "");
	


	# Generate the source file
	my $theFile = "QD3D$fileName.c";
    open(OUT_FILE, ">$kDirGlueFiles$theFile") or die("Can't open $theFile for writing: $!\n");



	# Generate the template
	my $theTemplate = $kTemplateQ3C;
	$theTemplate    =~ s/FILE_NAME/$fileName/g;

    print OUT_FILE "$theTemplate";



	# Generate the functions
	my $firstPublic = 1;
	
	foreach my $theFunction (@functionList)
		{
		# Break up the function into its components
		$theFunction =~ s/^(.*)\s+(Q3|CE)(\w+)\((.*)\)/$1\n$2$3\($4\)/;
		
		my $funcResult = $1;
		my $funcName   = "$2$3";
		my $funcParams = $4;

		die("result not found: [$theFunction] $!\n") if !defined($funcResult);
		die("name   not found: [$theFunction] $!\n") if !defined($funcName);
		die("params not found: [$theFunction] $!\n") if !defined($funcParams);



		# Initialise ourselves for this function
		my $implementFunc = "E3$3";
		my $resultValue   = $gFuncResult{$funcResult} ? $gFuncResult{$funcResult} : "NULL";
		my @paramList     = split(",", $funcParams);



		# Generate the method
		if ($firstPublic)
			{
			print OUT_FILE "//=============================================================================\n";
			print OUT_FILE "//      Public functions\n";
			print OUT_FILE "//-----------------------------------------------------------------------------\n";
			}

		print OUT_FILE "//=============================================================================\n" if !$firstPublic;
		print OUT_FILE "//      $funcName : Quesa API entry point.\n";
		print OUT_FILE "//-----------------------------------------------------------------------------\n";
		print OUT_FILE "#pragma mark -\n" if $firstPublic;
		print OUT_FILE "$theFunction\n";
		print OUT_FILE "{\n";



		# Add dummy parameter checking code
		if ($funcParams ne "void")
			{
			print OUT_FILE "\n";
			print OUT_FILE "\n";
			print OUT_FILE "	// Release build checks\n";

			foreach my $theParam (@paramList)
				{
				my $isPtr    = ($theParam =~ /\*/ ? 1 : 0);
				my $isObject = ($theParam =~ /(TQ3\w+Object|TQ3Object\W)/) ? 1 : 0;
				$isObject    = 0 if ($theParam =~ /TQ3XObject/);
				$isObject    = 0 if ($theParam =~ /TQ3ViewerObject/);

				my $paramType = ($theParam =~ /(TQ3\w+Object|TQ3Object\W)/) ? $1 : "";
				$paramType    =~ s/\s+$//;

				my $objectType = $gParamType{$paramType};
				$theParam      =~ /(\w+)$/;


				if ($isPtr)
					{
					print OUT_FILE "	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR($1), $resultValue);\n" if ($funcResult ne "void");
					print OUT_FILE "	Q3_REQUIRE(Q3_VALID_PTR($1));\n"                         if ($funcResult eq "void");
					}

				if ($isObject && !$isPtr)
					{
					print OUT_FILE "	Q3_REQUIRE_OR_RESULT($1->quesaTag == kQ3ObjectTypeQuesa, $resultValue);\n" if ($funcResult ne "void");
					print OUT_FILE "	Q3_REQUIRE($1->quesaTag == kQ3ObjectTypeQuesa);\n"         				   if ($funcResult eq "void");

					if (defined($objectType))
						{
						print OUT_FILE "	Q3_REQUIRE_OR_RESULT(Q3Object_IsType($1, $objectType), $resultValue);\n" if ($funcResult ne "void");
						print OUT_FILE "	Q3_REQUIRE(Q3Object_IsType($1, $objectType));\n"      				    if ($funcResult eq "void");
						}
					}
				}

			print OUT_FILE "\n";
			print OUT_FILE "\n";
			print OUT_FILE "\n";
			print OUT_FILE "	// Debug build checks\n";
			print OUT_FILE "#if Q3_DEBUG\n";

			my $firstParam = 1;
			foreach my $theParam (@paramList)
				{
				$theParam =~ /(\w+)$/;
				print OUT_FILE "\n" if !$firstParam;
				print OUT_FILE "	if (0) // Further checks on $1\n";
				print OUT_FILE "		return($resultValue);\n" if ($funcResult ne "void");
				print OUT_FILE "		return;\n"               if ($funcResult eq "void");
				$firstParam = 0;
				}

			print OUT_FILE "#endif\n";
			print OUT_FILE "\n";
			}



		# Call the bottleneck
		if (!($funcName =~ /Q3(Error|Warning|Notice)_Get/))
			{
			print OUT_FILE "\n";
			print OUT_FILE "\n";
			print OUT_FILE "	// Call the bottleneck\n";
			print OUT_FILE "	E3System_Bottleneck();\n";
			print OUT_FILE "\n";
			}



		# Build the calling parameters
		my $callParams  = "";
		if ($funcParams ne "void")
			{
			my $firstParam = 1;
			foreach my $theParam (@paramList)
				{
				$theParam   =~ /(\w+)$/;
				$callParams .= ", " if !$firstParam;
				$callParams .= $1;
				$firstParam = 0;
				}
			}



		# Call the implementation
		print OUT_FILE "\n";
		print OUT_FILE "\n";
		print OUT_FILE "	// Call our implementation\n";
		print OUT_FILE "	$implementFunc($callParams);\n"         if ($funcResult eq "void");
		print OUT_FILE "	return($implementFunc($callParams));\n" if ($funcResult ne "void");

		print OUT_FILE "}\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";

		$firstPublic = 0;
		}



	# Clean up
	close(OUT_FILE);
}





#------------------------------------------------------------------------
# Generate an E3Foo.c source file.
#------------------------------------------------------------------------
sub generateE3Source
{


    # Retrieve our parameters
    my ($functionListRef, $hFile) = @_;
	my @functionList = @$functionListRef;



	# Break down the file name
	my $fileName = $hFile;
	$fileName    =~ s/QD3D(.*)\.h/$1/;
	$fileName    = "Main" if ($fileName eq "");



	# Generate the source file
	my $theFile = "E3$fileName.c";
    open(OUT_FILE, ">$kDirSystemFiles$theFile") or die("Can't open $theFile for writing: $!\n");



	# Generate the template
	my $theTemplate = $kTemplateE3C;
	$theTemplate    =~ s/FILE_NAME/$fileName/g;

    print OUT_FILE "$theTemplate";



	# Generate the functions
	my $firstPublic = 1;
	
	foreach my $theFunction (@functionList)
		{
		# Break up the function into its components
		$theFunction =~ s/^(.*)\s+(Q3|CE)(\w+)\((.*)\)/$1\nE3$3\($4\)/;
		
		my $funcResult  = $1;
		my $funcName    = "E3$3";

		die("result not found: [$theFunction] $!\n") if !defined($funcResult);
		die("name   not found: [$theFunction] $!\n") if !defined($funcName);



		# Initialise ourselves for this function
		my $resultValue = $gFuncResult{$funcResult} ? $gFuncResult{$funcResult} : "NULL";



		# Generate the method
		if ($firstPublic)
			{
			print OUT_FILE "//=============================================================================\n";
			print OUT_FILE "//      Public functions\n";
			print OUT_FILE "//-----------------------------------------------------------------------------\n";
			}

		print OUT_FILE "//=============================================================================\n" if !$firstPublic;
		print OUT_FILE "//      $funcName : One-line description of the method.\n";
		print OUT_FILE "//-----------------------------------------------------------------------------\n";
		print OUT_FILE "//		Note : More detailed comments can be placed here if required.\n";
		print OUT_FILE "//-----------------------------------------------------------------------------\n";
		print OUT_FILE "#pragma mark -\n" if $firstPublic;
		print OUT_FILE "$theFunction\n";
		print OUT_FILE "{\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";
		print OUT_FILE "	// To be implemented...\n";
		print OUT_FILE "	return($resultValue);\n" if ($funcResult ne "void");
		print OUT_FILE "}\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";
		print OUT_FILE "\n";

		$firstPublic = 0;
		}



	# Clean up
	close(OUT_FILE);
}





#------------------------------------------------------------------------
# Generate an E3Foo.h header file.
#------------------------------------------------------------------------
sub generateE3Header
{


    # Retrieve our parameters
    my ($functionListRef, $hFile) = @_;
	my @functionList = @$functionListRef;



	# Break down the file name
	my $fileName = $hFile;
	$fileName    =~ s/QD3D(.*)\.h/$1/;
	$fileName    = "Main" if ($fileName eq "");



	# Generate the source file
	my $theFile = "E3$fileName.h";
    open(OUT_FILE, ">$kDirSystemFiles$theFile") or die("Can't open $theFile for writing: $!\n");



	# Generate the prototype list
	my $thePrototypes = "";

	foreach my $theFunction (@functionList)
		{
		$theFunction    =~ s/^(.*)\s+(Q3|CE)(\w+)\((.*)\)/$1\t\t\tE3$3\($4\);\n/;
		$thePrototypes .= $theFunction;
		}



	# Generate the template
	my $theTemplate = $kTemplateE3H;
	$theTemplate    =~ s/FILE_NAME/$fileName/g;
	$theTemplate    =~ s/UPPER_NAME/\U$fileName/g;
	$theTemplate    =~ s/FUNCTION_PROTOTYPES/$thePrototypes/g;

    print OUT_FILE "$theTemplate";



	# Clean up
	close(OUT_FILE);
}





#------------------------------------------------------------------------
# Process a QD3D header file.
#------------------------------------------------------------------------
sub processHeader
{


    # Retrieve our parameters
    my ($hFile) = @_;



    # Process the header file
    my @functionList = ();
    
    open(IN_FILE, "$kDirHFiles$hFile") or die("Can't open $hFile for reading: $!\n");
 	print "Processing $hFile\n";

    while (defined(my $theLine = <IN_FILE>))
        {
        # Process the line
        if ($theLine =~ /^Q3_EXTERN_API_C\(\s*(.*)\s*\)/)
            {
            # Grab the return type
            my $theResult = $1;


            # Keep reading lines until we find the end of the prototype
            my $theProto = "";
            while (not $theLine =~ /;/)
            	{
            	$theLine = <IN_FILE>;
            	chomp($theLine);
            	
            	$theProto .= $theLine;
            	}


            # Strip out any whitespace, tabs, or newlines
            $theResult =~ s/^\s+//g;
            $theResult =~ s/\s+$//g;

            $theProto =~ s/\s+/ /g;
            $theProto =~ s/\t+/ /g;
            $theProto =~ s/\n//g;


			# Remove the space between the first bracket, drop the semicolon,
			# and push the star for pointers over to the variable name.
			$theProto =~ s/ \(/\(/;
			$theProto =~ s/;//;
			$theProto =~ s/\*\s+/\*/g;


            # Save the function details
            push(@functionList, "$theResult $theProto");
            }
        }

    close IN_FILE;



	# Generate the source files
	generateQ3Source(\@functionList, $hFile);
	generateE3Source(\@functionList, $hFile);
	generateE3Header(\@functionList, $hFile);



    # Print some stats
    my $numFunctions  = @functionList;
	my $numTrivial    = 0;
	my $numNonTrivial = 0;
	foreach my $theFunction (@functionList)
		{
		if ($theFunction =~ /Q3.*_(G|S)et/ || $theFunction =~ /Q3.*_New/)
			{
			$numTrivial++;
			}
		else
			{
			$numNonTrivial++;
			}
		}

    print "  generated " . @functionList . " API functions\n";
    print "  $numTrivial trivial\n";
    print "  $numNonTrivial non-trivial\n";

	return($numFunctions, $numTrivial, $numNonTrivial);
}





#------------------------------------------------------------------------
# Program entry point
#------------------------------------------------------------------------
# Set up our constants
if ($^O eq "MacOS")
	{
	$kDirGlueFiles   = ":Source:Core:Glue:";
	$kDirSystemFiles = ":Source:Core:System:";
	$kDirHFiles      = ":Includes:QuickDraw 3D 1.6:";
	}
else
	{
	$kDirGlueFiles   = "";
	$kDirSystemFiles = "";
	$kDirHFiles      = "";
	}



# Grab the files to process
opendir(DIR, $kDirHFiles) || die("Can't open $kDirHFiles: $!");
my @fileList = readdir(DIR);
closedir(DIR);



# Process the headers
my $numFunctions   = 0;
my $numTrivial     = 0;
my $numNonTrivial  = 0;
my $percentTrivial = 0;

foreach my $theFile (@fileList)
	{
	my ($countFunctions, $countTrivial, $countNonTrivial) = processHeader($theFile) if ($theFile =~ /\.h$/);

	$numFunctions  += $countFunctions  if defined($countFunctions);
	$numTrivial    += $countTrivial    if defined($countTrivial);
	$numNonTrivial += $countNonTrivial if defined($countNonTrivial);
	}

$percentTrivial = int(($numTrivial / $numFunctions) * 100.0) if ($numFunctions != 0);



# Print some stats
print "\n\n";
print "  Total Functions: $numFunctions API functions\n";
print "    Total Trivial: $numTrivial\n";
print "Total Non-Trivial: $numNonTrivial\n";
print "  Percent Trivial: $percentTrivial%\n";
