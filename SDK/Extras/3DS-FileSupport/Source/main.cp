/*  NAME:
        main.cp

    DESCRIPTION:
        Plaform-dependent entry points.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
	  This program is  free  software;  you can redistribute it and/or modify it
	  under the terms of the  GNU Lesser General Public License  as published by 
	  the  Free Software Foundation;  either version 2.1 of the License,  or (at 
	  your option) any later version.
	 
	  This  program  is  distributed in  the  hope that it will  be useful,  but
	  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	  or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
	  License for more details.
	 
	  You should  have received  a copy of the GNU Lesser General Public License
	  along with  this program;  if not, write to the  Free Software Foundation,
	  Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
			__declspec(dllexport) void Macho_3DS_FileSupport_Entry();
			__declspec(dllexport) void Macho_3DS_FileSupport_Exit();
		#elif __GNUC__ >= 4
			void __attribute__((visibility("default"))) Macho_3DS_FileSupport_Entry()
				__attribute__ ((constructor));
			void __attribute__((visibility("default"))) Macho_3DS_FileSupport_Exit()
				__attribute__ ((destructor));
		#else
			void __attribute__((visibility("default"))) Macho_3DS_FileSupport_Entry();
			void __attribute__((visibility("default"))) Macho_3DS_FileSupport_Exit();
		#endif
		
		#if __MWERKS__
			extern void __destroy_global_chain(void);
		#endif
		}
	#else
		pascal OSErr __initialize(const CFragInitBlock *theInitBlock);
		pascal void __terminate(void);
		
		pascal OSErr	CFM_3DS_FileSupport_Entry( const CFragInitBlock *theInitBlock );
		pascal void		CFM_3DS_FileSupport_Exit(void);
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
#pragma CALL_ON_LOAD Macho_3DS_FileSupport_Entry
void Macho_3DS_FileSupport_Entry()
#elif __GNUC__ >= 4
void Macho_3DS_FileSupport_Entry()
#else
void Macho_3DS_FileSupport_Entry() __attribute__ ((constructor))
#endif
{
	TQ3XSharedLibraryInfo	sharedLibraryInfo;
	
	
	sharedLibraryInfo.registerFunction 	= Register_3DS_Class;
	sharedLibraryInfo.sharedLibrary 	= 0;
												
	Q3XSharedLibrary_Register(&sharedLibraryInfo);
}

#if __MWERKS__
#pragma CALL_ON_UNLOAD Macho_3DS_FileSupport_Exit
void Macho_3DS_FileSupport_Exit()
#elif __GNUC__ >= 4
void Macho_3DS_FileSupport_Exit()
#else
void Macho_3DS_FileSupport_Exit() __attribute__ ((destructor))
#endif
{
	Unregister_3DS_Class();
	
	// It seems to be necessary to do this cleanup when using a CodeWarrior-built plugin
	// in an Xcode-built app.
	#if __MWERKS__
		__destroy_global_chain();
	#endif
}

#else	// CFM

//=============================================================================
//      CFM_3DS_FileSupport_Entry : CFM entry point for Mac.
//-----------------------------------------------------------------------------
pascal OSErr	CFM_3DS_FileSupport_Entry( const CFragInitBlock *theInitBlock )
{
	OSErr	err = __initialize( theInitBlock );
	
	TQ3XSharedLibraryInfo	sharedLibraryInfo;
	sharedLibraryInfo.registerFunction 	= Register_3DS_Class;
	sharedLibraryInfo.sharedLibrary 	= NULL;

	Q3XSharedLibrary_Register(&sharedLibraryInfo);
	
	return err;
}

//=============================================================================
//      CFM_3DS_FileSupport_Exit : CFM exit point for Mac.
//-----------------------------------------------------------------------------
pascal void
CFM_3DS_FileSupport_Exit(void)
{
	Unregister_3DS_Class();
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
		
		sharedLibraryInfo.registerFunction 	= Register_3DS_Class;
		sharedLibraryInfo.sharedLibrary 	= (unsigned long)inst;
													
		Q3XSharedLibrary_Register(&sharedLibraryInfo);
	}
	
	return (TRUE);
}

#endif


