/*  NAME:
        E3WindowsSystem.c

    DESCRIPTION:
        Windows specific routines.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3System.h"

#include <string.h>




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct E3WindowsSystem_DLLSlot *E3WindowsSystem_DLLSlotPtr;

typedef struct E3WindowsSystem_DLLSlot {
	HMODULE						Dll;
	E3WindowsSystem_DLLSlotPtr	nextSlot;
} E3WindowsSystem_DLLSlot;





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
static E3WindowsSystem_DLLSlot* e3windowssystem_dllSlotHead = NULL;





//=============================================================================
//      Private functions
//-----------------------------------------------------------------------------
//      e3windowsystem_load1plugin : Opens and memorizes the DLL pointed by path.
//-----------------------------------------------------------------------------
static E3WindowsSystem_DLLSlot*
e3windowsystem_load1plugin(LPTSTR path)
{
	E3WindowsSystem_DLLSlot* newSlot = NULL;



	newSlot = (E3WindowsSystem_DLLSlot*)Q3Memory_Allocate(sizeof(E3WindowsSystem_DLLSlot));
	if(newSlot != NULL){
		newSlot->Dll = LoadLibrary(path);
		if(newSlot->Dll != NULL){
			newSlot->nextSlot = e3windowssystem_dllSlotHead;
			e3windowssystem_dllSlotHead = newSlot;
		}
		else{//LoadLibrary failed
			Q3Memory_Free(&newSlot);
		}
	}

	return newSlot;

}





//=============================================================================
//      e3windowsystem_loadplugins : Scans the directory dir 
//										and loads files with extension ext.
//-----------------------------------------------------------------------------
static void
e3windowsystem_loadplugins(LPTSTR dir, LPTSTR ext)
{

	WIN32_FIND_DATA FindFileData; 
	HANDLE hFindFile; 
	TCHAR szFiles[MAX_PATH]; 
	TCHAR szFileName[MAX_PATH]; 
	E3WindowsSystem_DLLSlot* lastLoadedModule;
 
	lstrcpy(szFiles, dir); 
	lstrcat(szFiles, TEXT("\\*.")); 
	lstrcat(szFiles, ext); 
 

	hFindFile = FindFirstFile(szFiles, &FindFileData); 
 
	if (hFindFile == INVALID_HANDLE_VALUE) { 
		return; 
	} 
 

	
	do {
		if (FindFileData.cFileName[0] != '.')
		{
			lstrcpy(szFileName, dir); 
			lstrcat(szFileName, TEXT("\\")); 
			lstrcat(szFileName, FindFileData.cFileName); 
			
			lastLoadedModule = e3windowsystem_load1plugin(szFileName);
		}
	} while (FindNextFile(hFindFile, &FindFileData) 
						|| (GetLastError() != ERROR_NO_MORE_FILES)); 
 
	FindClose(hFindFile); 
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3WindowsSystem_Initialise : Initialise the system.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3WindowsSystem_Initialise(void)
{
#if QUESA_WIN32_MASK_FP_EXCEPTIONS
	// Mask out the floating point exceptions: zerodivide, overflow, underflow,
	// invalid, inexact.
	// The Visual C++ docs say that you should prefer _controlfp_s to _controlfp,
	// but CodeWarrior does not know about _controlfp_s.
	_controlfp( _MCW_EM, _MCW_EM );
#endif

	// Initialise the system
	return(kQ3Success);
}





//=============================================================================
//      E3WindowsSystem_Terminate : Terminate the system.
//-----------------------------------------------------------------------------
#pragma mark -
void
E3WindowsSystem_Terminate(void)
{
	// Terminate the system
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3WindowsSystem_LoadPlugins : Scan for and load plug-ins.
//-----------------------------------------------------------------------------
#pragma mark -
void
E3WindowsSystem_LoadPlugins(void)
{	TCHAR thePath[MAX_PATH];


	// Register Quesa plug-ins
	
	// Look in the application directory
	if (GetModuleFileName( 0, thePath, MAX_PATH ) > 0)
	{
		char*	backslashLoc = strrchr( thePath, '\\' );
		if (backslashLoc != NULL)
		{
			*backslashLoc = '\0';
			e3windowsystem_loadplugins(thePath, TEXT("xq3"));
		}
	}
	
	if(GetSystemDirectory(thePath, MAX_PATH) > 0)
		e3windowsystem_loadplugins(thePath, TEXT("xq3"));

	if(GetWindowsDirectory(thePath, MAX_PATH) > 0)
		e3windowsystem_loadplugins(thePath, TEXT("xq3"));
}




//=============================================================================
//      E3WindowsSystem_UnloadPlugins : Unload plug-ins.
//-----------------------------------------------------------------------------
#pragma mark -
void
E3WindowsSystem_UnloadPlugins(void)
{
		E3WindowsSystem_DLLSlot* nextSlot;
		E3WindowsSystem_DLLSlot* currentSlot;

		nextSlot = e3windowssystem_dllSlotHead;

		while( nextSlot != NULL){
			currentSlot = nextSlot;
			nextSlot = currentSlot->nextSlot;
			FreeLibrary(currentSlot->Dll);
			Q3Memory_Free(&currentSlot);
		}

}
