/*  NAME:
        QutMac.h

    DESCRIPTION:
        Header file for QutMac.c

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#ifndef QUT_MAC_HDR
#define QUT_MAC_HDR
//=============================================================================
//      Carbon compatibility macros
//-----------------------------------------------------------------------------
#if !TARGET_API_MAC_CARBON
	// Get the bounds for a port
	#define GetPortBounds(_port, _rect)				*(_rect) = _port->portRect;


	// Get the port for a window
	#ifndef GetWindowPort
		#define GetWindowPort(_window)				(GrafPtr) (_window);
	#endif


	// Get the data for an AE descriptor
	#define AEGetDescData(_ae, _ptr, _size)			memcpy(_ptr, *((_ae)->dataHandle), _size);
	
	
	// Get the port bits of a port
	#define GetPortBitMapForCopyBits(_port)			&((GrafPtr) _port)->portBits


	// Validate a port rect
	#define ValidWindowRect(_window, _rect)			\
		do											\
			{	GrafPtr		_savePort;				\
													\
			GetPort(&_savePort);					\
			SetPort((GrafPtr) _window);				\
			ValidRect(_rect);						\
			SetPort(_savePort);						\
			}										\
		while (0)


	// Older UH support
	#if (UNIVERSAL_INTERFACES_VERSION == 0x0320)
		#ifndef DisposeNavEventUPP
			#define DisposeNavEventUPP(userUPP)		DisposeRoutineDescriptor(userUPP)
		#endif
	#endif

#endif


Boolean		QutMac_SelectMetafileToOpen(FSSpec* fileSpecPtr);
Boolean		QutMac_SelectMetafileToSaveTo(FSSpec* fileSpecPtr);

#endif
