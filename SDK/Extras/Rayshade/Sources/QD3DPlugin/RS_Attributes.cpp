/*  NAME:
        RS_Attribute.c

    DESCRIPTION:
        RayShade geometry handlers

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
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#include "RSPlugin.h"
#include "RS_Attributes.h"
#include "RS_Texture.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaView.h>
	#include <QuesaRenderer.h>
#else
	#include <QD3D.h>
	#include <QD3DView.h>
	#include <QD3DRenderer.h>
#endif

/*===========================================================================*\
 *
 *	Routine:	rs_Attributes_Combine()
 *
 *	Comments:	Combines the attributes of theView with geomAttributes,
 *				callee should destroy the resulting attribute set.
 *
\*===========================================================================*/
static TQ3AttributeSet
rs_Attributes_Combine(
			TQ3ViewObject 				theView, 
			TQ3AttributeSet 			geomAttributes)	
{
    TQ3AttributeSet     newAttributes; 
    TQ3AttributeSet     viewAttributes;
    TQ3Status           theStatus;
    
    theStatus = Q3View_GetAttributeSetState(theView, &viewAttributes);
    if (theStatus != kQ3Success)
        return(geomAttributes);
    
    if (geomAttributes == NULL)
		return(viewAttributes);
    
 	newAttributes = Q3AttributeSet_New();   
	if (newAttributes == NULL)
		return(viewAttributes);
	
	Q3AttributeSet_Inherit(viewAttributes, geomAttributes, newAttributes);
	Q3Object_Dispose(viewAttributes);
	
	return newAttributes;
}
/*===========================================================================*\
 *
 *	Routine:	RS_UpdateAttributes()
 *
 *	Comments:	Updates the attributes if neccesary.
 *
\*===========================================================================*/
TQ3Status RS_UpdateAttributes(
				TQ3ViewObject 				inView, 
				TRSRendererPrivate			*rsPrivate,
				TQ3AttributeSet				inObjectAttributeSet)
{
	TQ3AttributeSet		theAttributeSet;
	TQ3Status			result;
    TRTSurfaceData      theNewSurface;
    TQ3XAttributeMask	theMask;
    
    theAttributeSet = rs_Attributes_Combine(inView,inObjectAttributeSet);
   
	if (theAttributeSet == NULL)
	{
		result = kQ3Failure;
		goto cleanup;
	}
	
	

	
	
	theMask = Q3XAttributeSet_GetMask(theAttributeSet);

	/*
	 * Upadate texture info:
	 */
	if ((theMask & kQ3XAttributeMaskSurfaceShader) != 0)
	{
		TQ3ShaderObject *theShader;
		/*
		 * Get the texture
		 */
		theShader = (TQ3ShaderObject *) Q3XAttributeSet_GetPointer(theAttributeSet, kQ3AttributeTypeSurfaceShader);
		if ((theShader != NULL) && (*theShader != NULL))
		{
			result = RS_SetShaderSurface(rsPrivate,theShader);
			if (result != kQ3Success) 
				goto cleanup;
		}
	}
	
	RTSurfaceData_Init(&theNewSurface);

	  
	/* Update our state to reflect the attribute set */
	if (theMask & kQ3XAttributeMaskAmbientCoefficient)
	    Q3AttributeSet_Get(
                theAttributeSet,
				kQ3AttributeTypeAmbientCoefficient,
				&theNewSurface.ambientCoefficient);
				
	if (theMask &  kQ3XAttributeMaskDiffuseColor)
	    Q3AttributeSet_Get(
                theAttributeSet,
				kQ3AttributeTypeDiffuseColor,
				&theNewSurface.diffuseColor);
				
    if (theMask & kQ3XAttributeMaskSpecularColor)
            Q3AttributeSet_Get(
                theAttributeSet,
				kQ3AttributeTypeSpecularColor,
				&theNewSurface.specularColor);
				
    if (theMask & kQ3XAttributeMaskTransparencyColor)
            Q3AttributeSet_Get(
                theAttributeSet,
				kQ3AttributeTypeTransparencyColor,
				&theNewSurface.transparencyColor);
				
    if (theMask & kQ3XAttributeMaskSpecularControl)
            Q3AttributeSet_Get(
                theAttributeSet,
				kQ3AttributeTypeSpecularControl,
				&theNewSurface.specularControl);
				
    if (theMask & kQ3XAttributeMaskHighlightState)
            Q3AttributeSet_Get(
                theAttributeSet,
				kQ3AttributeTypeHighlightState,
				&theNewSurface.hilightState);  
	
	if ((!rsPrivate->isCurrentSurface) ||
		(!RTSurfaceData_Compare(&theNewSurface,&rsPrivate->surfaceCache)))
	{
		rsPrivate->surfaceCache = theNewSurface;
		result = RT_SetCurrentSurface(rsPrivate->raytracer,&rsPrivate->surfaceCache);
		if (result != kQ3Success)
		{
			rsPrivate->isCurrentSurface = kQ3False;
			goto cleanup;
		}
		
		rsPrivate->isCurrentSurface = kQ3True;
	}
	
	Q3Object_Dispose(theAttributeSet);
	return kQ3Success;
	
cleanup:
	if (theAttributeSet)
		Q3Object_Dispose(theAttributeSet);	
	return result;
}



