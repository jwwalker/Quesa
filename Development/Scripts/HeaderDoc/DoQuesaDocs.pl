#!/usr/local/bin/perl -w
#========================================================================
#   Description:
#      Invoke headerDoc2HTML.pl to generate the Quesa docs.
#
#   Author:
#       dair@webthing.net
#
#   Author:
#		Dair Grant
#       <mailto:dair@webthing.net>
#
#   Version History
#		1.0  - First release
#------------------------------------------------------------------------
# Script header
#------------------------------------------------------------------------
use strict;





#------------------------------------------------------------------------
# Generate the documentation from the header files.
#------------------------------------------------------------------------
sub generateDocs
{


	# Find out where everything is
	my $inputDir   = "";
	my $outputDir  = "";
	my $currentDir = "";
	
	if ($^O eq "MacOS")
		{
		chop($currentDir = `pwd`);
		$inputDir  = $currentDir . "::::SDK:Includes";
		$outputDir = $currentDir . "::::Documentation:developer:apiref";
		}

	elsif ($^O eq "MSWin32")
        {
        $currentDir = Win32::GetCwd();
		$inputDir   = $currentDir . "..\\..\\..\\SDK\\Includes";
		$outputDir  = $currentDir . "..\\..\\..\\Documentation\\developer\\apiref";
        }

    else
        {
		chop($currentDir = `pwd`);
		$inputDir  = $currentDir . "../../../SDK/Includes";
		$outputDir = $currentDir . "../../../Documentation/developer/apiref";
        }



    # Make sure our directories exist
	if (!-e $inputDir)
        {
        print "Couldn't find $inputDir\n";
        exit(-1);
        }

	if (!-e $outputDir)
        {
        print "Creating $outputDir\n";
        mkdir($outputDir, 0);
        }



	# Invoke HeaderDoc to generate the documentation
	no strict "refs";
	local $^W   = 0;
	my @TmpArgs = @ARGV;

	@ARGV = ("-o", $outputDir, $inputDir);
	do("headerDoc2HTML.pl");

	@ARGV = ($outputDir);
	do("gatherHeaderDoc.pl");

	@ARGV = @TmpArgs;
	use strict;
}





#------------------------------------------------------------------------
# Program entry point
#------------------------------------------------------------------------
generateDocs();

print "...Finished\n";