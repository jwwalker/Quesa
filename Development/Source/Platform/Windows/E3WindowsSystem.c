/*  NAME:
        E3WindowsSystem.c

    DESCRIPTION:
        Windows specific routines.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3System.h"


//=============================================================================
//      Private types
//-----------------------------------------------------------------------------
typedef struct E3WindowsSystem_DLLSlot *E3WindowsSystem_DLLSlotPtr;

typedef struct E3WindowsSystem_DLLSlot{

	HMODULE						Dll;
	E3WindowsSystem_DLLSlotPtr	nextSlot;

}E3WindowsSystem_DLLSlot;



//=============================================================================
//      Private global data
//-----------------------------------------------------------------------------


E3WindowsSystem_DLLSlot* e3windowssystem_dllSlotHead = NULL;


//=============================================================================
//      Private functions
//-----------------------------------------------------------------------------
//      e3windowsystem_unloadplugins : Closes and unloads the plug-ins.
//-----------------------------------------------------------------------------
static void
e3windowsystem_unloadplugins()
{

		E3WindowsSystem_DLLSlot* nextSlot;
		E3WindowsSystem_DLLSlot* currentSlot;

		nextSlot = e3windowssystem_dllSlotHead;

		while( nextSlot != NULL){
			currentSlot = nextSlot;
			nextSlot = currentSlot->nextSlot;
			FreeLibrary(currentSlot->Dll);
			E3Memory_Free(&currentSlot);
		}

}


//=============================================================================
//      e3windowsystem_load1plugin : Opens and memorizes the DLL pointed by path.
//-----------------------------------------------------------------------------
static E3WindowsSystem_DLLSlot*
e3windowsystem_load1plugin(LPTSTR path)
{
	E3WindowsSystem_DLLSlot* newSlot = NULL;



	newSlot = (E3WindowsSystem_DLLSlot*)E3Memory_Allocate(sizeof(E3WindowsSystem_DLLSlot));
	if(newSlot != NULL){
		newSlot->Dll = LoadLibrary(path);
		if(newSlot->Dll != NULL){
			newSlot->nextSlot = e3windowssystem_dllSlotHead;
			e3windowssystem_dllSlotHead = newSlot;
		}
		else{//LoadLibrary failed
			E3Memory_Free(&newSlot);
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
		lstrcpy(szFileName, dir); 
		lstrcat(szFileName, TEXT("\\")); 
		lstrcat(szFileName, FindFileData.cFileName); 
		
		lastLoadedModule = e3windowsystem_load1plugin(szFileName);

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

	e3windowsystem_unloadplugins();
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
{
	TCHAR systemDir[MAX_PATH];

	if(GetSystemDirectory(systemDir, MAX_PATH * sizeof(*systemDir)) > 0)
		e3windowsystem_loadplugins(systemDir,TEXT("xq3"));

	if(GetWindowsDirectory(systemDir, MAX_PATH * sizeof(*systemDir)) > 0)
		e3windowsystem_loadplugins(systemDir,TEXT("xq3"));
	
}
