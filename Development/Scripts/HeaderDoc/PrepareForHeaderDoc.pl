#!/usr/bin/perl -w
#========================================================================
#   Description:
#       Prepare the pre-HeaderDoc headers for HeaderDoc.
#
#		Note that this script is no longer required - it was used once
#		to insert the template HeaderDoc comments, and is only included
#		just for reference.
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
my $kDirHFiles = "";





#------------------------------------------------------------------------
# Process a single header file.
#------------------------------------------------------------------------
sub processHeader
{


    # Retrieve our parameters
    my ($hFile) = @_;



	# Read in the header file, and process it to add HeaderDoc declarations
    my $headerData = "";
    my $previous1  = "";
    my $previous2  = "";
    my $previous3  = "";
    my $previous4  = "";
    my $previous5  = "";
    
    open(IN_FILE, "$kDirHFiles$hFile") or die("Can't open $hFile for reading: $!\n");
 	print "Processing $hFile\n";

    while (defined(my $theLine = <IN_FILE>))
        {
        # Eat the newline
        chomp($theLine);



        # Shuffle our comment stack down
        $previous1 = $previous2;
        $previous2 = $previous3;
        $previous3 = $previous4;
        $previous4 = $previous5;
        $previous5 = $theLine;



		# If we've found a function, start processing it
        if ($theLine =~ /^Q3_EXTERN_API_C\s*\(\s*(.*)\s*\)/)
            {
            # Grab the return type
            my $theResult = $1;


            # Grab the function name from the previous comment
            $previous2 =~ /(Q3\w+)/;
            my $functionName = $1;


            # Keep reading lines until we find the end of the prototype,
            # and build the list of parameter names.
            my $theProto   = "$theLine\n";
            my @paramNames = ();
            while (not $theLine =~ /;/)
            	{
            	# Read a line and build up the prototype
            	$theLine   = <IN_FILE>;
            	$theProto .= $theLine;
            	
            	
            	# Grab non-void parameters out of the line
            	if ($theLine =~ /(\w+)(.)$/)
            		{
            		my $paramName = $1 . ($2 ne "," ? $2 : "");
	            	push(@paramNames, $paramName) if ($paramName ne "void");
	            	}
            	}


			# Strip off the previous four lines of the old header
			for (my $n = 0; $n < 5; $n++)
				{
				my $m       = rindex($headerData, "\n");
				$headerData = substr($headerData, 0, $m);
				}


			# Write out the new header, followed by the existing prototype
			$headerData .= "\n";
			$headerData .= "/*!\n";
			$headerData .= " *  \@function\n";
			$headerData .= " *      $functionName\n";
			$headerData .= " *  \@discussion\n";
			$headerData .= " *      One-line description of this function.\n";
			$headerData .= " *\n";
			$headerData .= " *      A more extensive description can be supplied here, covering\n";
			$headerData .= " *      the typical usage of this function and any special requirements.\n";
			$headerData .= " *\n";
			
			foreach my $paramName (@paramNames)
				{
				$paramName = sprintf("%-16s", $paramName);
				$headerData .= " *  \@param $paramName Description of the parameter.\n";
				}

			if ($theResult ne "")
				{
				$headerData .= " *  \@result                 Description of the function result.\n";
				}

			$headerData .= " */\n";
			$headerData .= "$theProto";
            }


		# Otherwise, append the line unchanged
		else
			{
			$headerData .= "$theLine\n";
			}          
        }

    close IN_FILE;



	# Write the processed header file back out again
# dair
#    open( OUT_FILE, ">$kDirHFiles$hFile") or die("Can't open $hFile for writing: $!\n");
#    print OUT_FILE "$headerData\n";
#	close(OUT_FILE);
print "$headerData";
}





#------------------------------------------------------------------------
# Program entry point
#------------------------------------------------------------------------
# Set up our constants
if ($^O eq "MacOS")
	{
	$kDirHFiles = "Bender:Desktop Folder:Quesa:";
	}
else
	{
	$kDirHFiles = "";
	}



# Grab the files to process
opendir(DIR, $kDirHFiles) || die("Can't open $kDirHFiles: $!");
my @fileList = readdir(DIR);
closedir(DIR);



# Process the headers
print "Warning - this script was used to prepare for HeaderDoc, so\n";
print "probably doesn't need to be run again!\n";
print "\n";

foreach my $theFile (@fileList)
	{
	processHeader($theFile) if ($theFile =~ /\.h$/);
	}

