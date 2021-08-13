/*  NAME:
        E3WindowsDebug.cpp

    DESCRIPTION:
        Windows debug implementation.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "ShlObj.h"
#include "E3Prefix.h"
#include "E3Debug.h"

#include <stdio.h>
#include <string>
#include <stdlib.h>



extern int gDebugMode;


//=============================================================================
//      Static variables
//-----------------------------------------------------------------------------
FILE*	sLogFile = NULL;





//=============================================================================
//      E3Assert : Report failed assertions.
//-----------------------------------------------------------------------------
void
E3Assert(const char *srcFile, TQ3Uns32 lineNum, const char *theAssertion)
{	char		theString[kQ3StringMaximumLength];



    // Check our parameters
    if (srcFile == NULL)
	{
    	srcFile = "Unknown";
	}

    if (theAssertion == NULL)
	{
    	theAssertion = "Unknown";
	}



	// Report the assertion failure
	sprintf(theString,
				"Quesa Assertion: '%s' failed on line %d of %s\n",
				theAssertion, lineNum, srcFile);

	//MessageBox(NULL, theString, srcFile, MB_OK | MB_ICONERROR);
	OutputDebugString(theString);
	E3LogMessage( theString );
	
	// DebugBreak caused a problem when not running under a debugger, or when
	// running under Delphi's debugger.
#if QUESA_USE_DEBUGBREAK
	DebugBreak();
#endif
	abort();
}





//=============================================================================
//      E3IsValidPtr : Check a pointer for validity.
//-----------------------------------------------------------------------------
TQ3Boolean
E3IsValidPtr(const void *thePtr)
{


	// Check the pointer
	if (thePtr == nullptr)
		return(kQ3False);



	// If we're still here, it looks OK	
	return(kQ3True);
}





//=============================================================================
//      E3LogMessage : Write a message to a log file.
//-----------------------------------------------------------------------------
void		E3LogMessage( const char* inMessage )
{
	if ( (inMessage == NULL) || (inMessage[0] == 0) )
	{
		return;
	}
	
	if (gDebugMode < 0)
	{
		return;
	}

	OutputDebugStringA(inMessage);

	if (sLogFile == NULL)
	{
		TCHAR thePath[MAX_PATH];
		
		HRESULT	res = SHGetFolderPath( NULL, CSIDL_PERSONAL, NULL,
			0, thePath );
		
		if (res == S_OK)
		{
			strcat( thePath, "\\Quesa.log" );
			
			sLogFile = fopen( thePath, "ab" );
			
			if (sLogFile != NULL)
			{
				// Make the stream unbuffered, so that if the program crashes,
				// we should not lose anything.
				setvbuf( sLogFile, NULL, _IONBF, 0 );

				char dateStr[200], timeStr[200];
				wchar_t dateStrw[100], timeStrw[100];
				int dateCharCount = GetDateFormatEx(L"en-US", 0, NULL, L"d MMM yyyy", dateStrw, 100, NULL);
				WideCharToMultiByte(CP_UTF8, 0, dateStrw, dateCharCount, dateStr, 200, NULL, NULL);
				int timeCharCount = GetTimeFormatEx(L"en-US", TIME_FORCE24HOURFORMAT, NULL, NULL, timeStrw, 100);
				WideCharToMultiByte(CP_UTF8, 0, timeStrw, timeCharCount, timeStr, 200, NULL, NULL);
				fprintf( sLogFile, "\n\n**** LOG START %s %s ****\n\n", dateStr, timeStr );
			}
		}
	}
	
	if (sLogFile != NULL)
	{
		fprintf( sLogFile, "%s", inMessage );
		fflush( sLogFile );
	}
}





//=============================================================================
//      E3CloseLog : Close the log file.
//-----------------------------------------------------------------------------
void		E3CloseLog()
{
	if (sLogFile != NULL)
	{
		fclose( sLogFile );
		sLogFile = NULL;
	}
}
