/*  NAME:
        RS_Update.c

    DESCRIPTION:
        RayShade Renderer update handler

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
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#include "RSPlugin.h"
#include "RS_Attributes.h"
#include "RS_Texture.h"

#if USE_QUESA_INCLUDES
	#include "Quesa.h"
	#include "QuesaView.h"
	#include "QuesaRenderer.h"
#else
	#include <QD3D.h>
	#include <QD3DView.h>
	#include <QD3DRenderer.h>
#endif

/*===========================================================================*\
 *
 *	Routine:	SR_Update_LocalToWorldMatrix()
 *
 *	Comments:	Track changes to local-to-world transform
 *
\*===========================================================================*/
TQ3Status RS_Update_LocalToWorldMatrix(
	TQ3ViewObject 			view,
	TRSRendererPrivate		*rsPrivate,
	TQ3Matrix4x4			*localToWorld)
{
	Q3_ASSERT(view != NULL);
	Q3_ASSERT(rsPrivate != NULL);
	Q3_ASSERT(localToWorld != NULL);
	
	Q3_UNUSED(view);
	
	rsPrivate->localToWorld = *localToWorld;
	
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	RS_Update_Shader()
 *
 *	Comments:	
 *
\*===========================================================================*/
TQ3Status 
RS_Update_SurfaceShader(
				TQ3ViewObject 			/*view*/,
				TRSRendererPrivate		*rsPrivate,
				TQ3ShaderObject 		*shaderObject)
{
	RS_SetShaderSurface(rsPrivate,shaderObject);
	return kQ3Success;
}
#if RS_DEBUG
/*===========================================================================*\
 *
 *	Routine:	SR_Update_LocalToWorldMatrix()
 *
 *	Comments:	Track changes to local-to-world transform
 *
\*===========================================================================*/
TQ3Status RS_Update_LocalToFrustumMatrix(
	TQ3ViewObject 			view,
	TRSRendererPrivate		*srPrivate,
	TQ3Matrix4x4			*localToFrustum)
{
	Q3_ASSERT(view != NULL);
	Q3_ASSERT(rsPrivate != NULL);
	Q3_ASSERT(localToWorld != NULL);
	
	Q3_UNUSED(view);
	
	rsPrivate->localToFrustum = *localToFrustum;
	
	return kQ3Success;
}
#endif /* RS_DEBUG */
