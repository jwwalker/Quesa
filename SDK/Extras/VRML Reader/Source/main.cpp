/*  NAME:
        main.cp

    DESCRIPTION:
        Plaform-dependent entry points.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

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
#include "quesa-methods.h"

#if __MACH__
	#include <Quesa/QuesaExtension.h>
#else
	#include <QuesaExtension.h>
#endif

#include <cstring>

#if QUESA_OS_MACINTOSH
	#if TARGET_RT_MAC_MACHO
		extern "C"
		{
		#if __MWERKS__
			__declspec(dllexport) void Macho_VRML_Reader_Entry();
			__declspec(dllexport) void Macho_VRML_Reader_Exit();
		#elif __GNUC__ >= 4
			void __attribute__((visibility("default"))) Macho_VRML_Reader_Entry()
				__attribute__ ((constructor));
			void __attribute__((visibility("default"))) Macho_VRML_Reader_Exit()
				__attribute__ ((destructor));
		#else
			void __attribute__((visibility("default"))) Macho_VRML_Reader_Entry();
			void __attribute__((visibility("default"))) Macho_VRML_Reader_Exit();
		#endif
		
		#if __MWERKS__
			extern void __destroy_global_chain(void);
		#endif
		}
	#else
		pascal OSErr __initialize(const CFragInitBlock *theInitBlock);
		pascal void __terminate(void);
		
		pascal OSErr	CFM_VRML_Reader_Entry( const CFragInitBlock *theInitBlock );
		pascal void		CFM_VRML_Reader_Exit(void);
	#endif
#elif QUESA_OS_WIN32
	extern "C"
	{
		BOOL __stdcall DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 );
	}
#endif

#if QUESA_OS_MACINTOSH

#if TARGET_RT_MAC_MACHO

#if __MWERKS__
#pragma CALL_ON_LOAD Macho_VRML_Reader_Entry
void Macho_VRML_Reader_Entry()
#elif __GNUC__ >= 4
void Macho_VRML_Reader_Entry()
#else
void Macho_VRML_Reader_Entry() __attribute__ ((constructor))
#endif
{
	TQ3XSharedLibraryInfo	sharedLibraryInfo;
	
	
	sharedLibraryInfo.registerFunction 	= RegisterVRMLReaderClass;
	sharedLibraryInfo.sharedLibrary 	= 0;
												
	Q3XSharedLibrary_Register(&sharedLibraryInfo);
}

#if __MWERKS__
#pragma CALL_ON_UNLOAD Macho_VRML_Reader_Exit
void Macho_VRML_Reader_Exit()
#elif __GNUC__ >= 4
void Macho_VRML_Reader_Exit()
#else
void Macho_VRML_Reader_Exit() __attribute__ ((destructor))
#endif
{
	UnregisterVRMLReaderClass();
	
	// It seems to be necessary to do this cleanup when using a CodeWarrior-built plugin
	// in an Xcode-built app.
	#if __MWERKS__
		__destroy_global_chain();
	#endif
}

#else	// CFM

//=============================================================================
//      CFM_VRML_Reader_Entry : CFM entry point for Mac.
//-----------------------------------------------------------------------------
pascal OSErr	CFM_VRML_Reader_Entry( const CFragInitBlock *theInitBlock )
{
	OSErr	err = __initialize( theInitBlock );
	
	TQ3XSharedLibraryInfo	sharedLibraryInfo;
	sharedLibraryInfo.registerFunction 	= RegisterVRMLReaderClass;
	sharedLibraryInfo.sharedLibrary 	= NULL;

	Q3XSharedLibrary_Register(&sharedLibraryInfo);
	
	return err;
}

//=============================================================================
//      CFM_VRML_Reader_Exit : CFM exit point for Mac.
//-----------------------------------------------------------------------------
pascal void
CFM_VRML_Reader_Exit(void)
{
	UnregisterVRMLReaderClass();
	__terminate();
}

#endif

#elif QUESA_OS_WIN32

BOOL __stdcall DllMain( HINSTANCE inst, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		TQ3XSharedLibraryInfo	sharedLibraryInfo;
		
		sharedLibraryInfo.registerFunction 	= RegisterVRMLReaderClass;
		sharedLibraryInfo.sharedLibrary 	= (unsigned long)inst;
													
		Q3XSharedLibrary_Register(&sharedLibraryInfo);
	}
	
	return (TRUE);
}

#endif


