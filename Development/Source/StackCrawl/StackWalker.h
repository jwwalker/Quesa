/* Copyright © Biit Inc 1996-2000 All Rights Reserved */
/*
	Minor changes were made by James W. Walker for the Quesa project:
	
	* To make this compile in plain C mode, reference parameters were
	  changed to pointer parameters.
	
	* When asked for a name further up the stack than was available,
	  GetCallerName was returning the last name that was available.
	  Now it returns a length of zero.
	  
	* For some reason, all the names returned by GetStackCrawl were
	  prefixed with a period, which I have removed.
*/
#pragma once

void GetCallerName( const char** outName, short* outLength, short steps /* =2 */ );
void GetStackCrawl( short depth, char* buffer, long* ioBufferSize );
