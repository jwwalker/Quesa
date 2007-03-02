/*  NAME:
        3dsReader.h

    DESCRIPTION:
        Header file for 3dsReader.c.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
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
#ifndef _3DS_READER_HDR
#define _3DS_READER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QuesaIO.h"



//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif


#define kE3FFormatReaderType3DS ((TQ3ObjectType) Q3_FOUR_CHARACTER_CONSTANT('3', 'D', 'S', 'F'))
#define kQ3ClassNameFileFormatReader_3DS "Quesa:FileFormat:Reader:3ds"




//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

// Static Registration
#if ((defined QUESA_REGISTER_BUILTIN_PLUGINS) && (QUESA_REGISTER_BUILTIN_PLUGINS == 0)) 

	TQ3Status				E3FFormat_3ds_Reader_RegisterClass(void);
	TQ3Status				E3FFormat_3ds_Reader_UnregisterClass(void);
#endif


// CFM entry points (Mac)
#if QUESA_OS_MACINTOSH
	OSErr    E3FFormat_3ds_Reader_Entry(void);
	TQ3Int32 E3FFormat_3ds_Reader_Exit(void);
#endif

//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

