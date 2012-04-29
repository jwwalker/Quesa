Binify3DMF is a command line program that reads a text or binary 3DMF file and
writes binary 3DMF to standard output.  Thus, to read a text 3DMF file and write
the binary form to a new output file, you could use a command line like:

	Binify3DMF text.3dmf > binary.3dmf

Error messages are sent to standard error, and will still show on the terminal.

If the input file is binary, the output file may not be exactly the same.  For
one reason, the output file will always be in the native byte order for
the Binify3DMF code, e.g., little-endian if it is running Intel code.
