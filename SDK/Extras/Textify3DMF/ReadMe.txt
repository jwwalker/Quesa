Textify3DMF is a command line program that reads a binary 3DMF file and writes
the text 3DMF equivalent to standard output.  Thus, to read a binary 3DMF file
and write the text form to a new output file, you could use a command line like:

	Textify3DMF binary.3dmf > text.3dmf

Error messages are sent to standard error, and will still show on the terminal.

Textify3DMF is written without using any Quesa code, and the classes it knows
about may not be exactly the same as those known to Quesa.  If it sees a data
block of an unknown class, it will write an UnknownBinary block.
