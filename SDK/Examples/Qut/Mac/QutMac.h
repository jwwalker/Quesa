/*  NAME:
        QutMac.h

    DESCRIPTION:
        Header file for QutMac.c

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
