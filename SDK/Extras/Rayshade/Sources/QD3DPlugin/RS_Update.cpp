/*  NAME:
        RS_Update.c

    DESCRIPTION:
        RayShade Renderer update handler

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
