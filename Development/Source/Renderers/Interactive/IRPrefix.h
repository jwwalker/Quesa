/*  NAME:
        IRPrefix.h

    DESCRIPTION:
        Global prefix file for Quesa's interactive renderer.

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
#ifndef __IRPREFIX__
#define __IRPREFIX__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Quesa
#include "E3Prefix.h"
#include "GLPrefix.h"





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Interactive renderer data
typedef struct TQ3InteractiveData {
	// GL state
	GLbitfield				glClearFlags;
	void					*glContext;


	// Light state
	TQ3Uns32				lightCount;
	GLfloat					glAmbientLight[4];


	// Texture state
	TQ3Uns32				textureCount;
	TQ3TextureObject		*textureCache;


	// QD3D state
	TQ3FillStyle			stateFill;
	TQ3AttributeSet			stateHilight;
	TQ3ColorRGB				stateDefaultDiffuseColour;
	TQ3ColorRGB				stateDefaultSpecularColour;
	TQ3ColorRGB				stateDefaultTransparencyColour;
	GLfloat					stateCurrentSpecularColour[4];
	float					stateCurrentSpecularControl;
	TQ3Boolean				stateTextureActive;
	TQ3Boolean				stateTextureForceWhite;

	TQ3ColorRGB				*stateGeomDiffuseColour;
	TQ3ColorRGB				*stateGeomSpecularColour;
	TQ3ColorRGB				*stateGeomTransparencyColour;
	float					stateGeomSpecularControl;
	TQ3Switch				stateGeomHilightState;

	TQ3ColorRGB				*stateViewDiffuseColour;
	TQ3ColorRGB				*stateViewSpecularColour;
	TQ3ColorRGB				*stateViewTransparencyColour;
	float					stateViewSpecularControl;
	TQ3Switch				stateViewHilightState;
	TQ3ObjectType			stateViewIllumination;
} TQ3InteractiveData;





#endif
