/*  NAME:
        E3UnixPrefix.h

    DESCRIPTION:
        Unix specific prefix file.

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
#ifndef __E3UNIXPREFIX__
#define __E3UNIXPREFIX__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//      Universal Headers compatibility
//-----------------------------------------------------------------------------
// Symbols, prevents QD3D headers #including Mac specific headers
#define __CONDITIONALMACROS__
#define __MOVIES__
#define __RAVE__


// Macros, Universal Header macros required by Mac specific headers
#define CALLBACK_API_C(_type, _name)           _type (*_name)
#define EXTERN_API_C(_type)                    extern _type
#define FOUR_CHAR_CODE(_x)                     _x


// Mac OS types, all QuickTime types so not available under Unix
#define QUESA_SUPPORT_QUICKTIME                0
#define QTAtomContainer                        long
#define TQADrawContext                         long
#define TQAEngine                              long
#define PixMapHandle                           long
#define CodecType                              long
#define CodecComponent                         long
#define CodecQ                                 long


// X11 types, only for MOSX builds
#define TARGET_OS_MOSX 0
#if TARGET_OS_MOSX
	#define Display                            long
	#define Drawable                           long
	#define Visual                             long
	#define Colormap                           long
	#define Window                             long
	#define XVisualInfo                        long
	#define Screen                             long
#endif





//=============================================================================
//      Build constants
//-----------------------------------------------------------------------------
// Platform identifier
#define TARGET_OS_UNIX                          1


// We don't support Apple's QD3D renderers under Unix
#define QUESA_REPLACE_QD3D_RENDERERS			1


// QuickTime is not supported on Unix
#define QUESA_SUPPORT_QUICKTIME					0





#endif
