/*  NAME:
       QOStatics.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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
#include "QORenderer.h"
#include "QOStatics.h"



//=============================================================================
//      Local constants
//-----------------------------------------------------------------------------

namespace
{
	const char*	kQORendererNickName = "Quesa OpenGL";
}



//=============================================================================
//      Local Functions
//-----------------------------------------------------------------------------


static TQ3Status
QORenderer_nickname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	// Return the amount of space we need
    *actualDataSize = strlen( kQORendererNickName ) + 1;



	// If we have a buffer, return the nick name
	if (dataBuffer != NULL)
	{
		// Clamp the buffer size
		if (bufferSize < *actualDataSize)
			*actualDataSize = bufferSize;


		// Return the string
		Q3Memory_Copy( kQORendererNickName, dataBuffer, *actualDataSize );
	}

    return(kQ3Success);
}



//=============================================================================
//      Method Implementations
//-----------------------------------------------------------------------------

TQ3Status	QORenderer::Statics::ObjectNewMethod(
									TQ3Object theObject,
									void *privateData,
									void *paramData )
{
#pragma unused( paramData )
	TQ3Status	status = kQ3Failure;
	try
	{
		QORenderer::Renderer*	theRenderer = new QORenderer::Renderer( theObject );
		*(QORenderer::Renderer**)privateData = theRenderer;
		status = kQ3Success;
	}
	catch (...)
	{
	}
	return status;
}

void		QORenderer::Statics::ObjectDeleteMethod(
									TQ3Object object,
                           			void* privateData )
{
#pragma unused(object)
	QORenderer::Renderer*	theRenderer = *(QORenderer::Renderer**)privateData;
	
	delete theRenderer;
}

TQ3Status	QORenderer::Statics::StartFrameMethod(
								TQ3ViewObject inView,
								void* privateData,
								TQ3DrawContextObject inDrawContext )
{
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	try
	{
		result = me->StartFrame( inView, inDrawContext );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3Status	QORenderer::Statics::EndFrameMethod(
								TQ3ViewObject inView,
								void* privateData,
								TQ3DrawContextObject inDrawContext )
{
#pragma unused( inDrawContext )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	try
	{
		result = me->EndFrame( inView );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3Status	QORenderer::Statics::StartPassMethod(
								TQ3ViewObject inView,
								void* privateData,
								TQ3CameraObject inCamera,
								TQ3GroupObject inLights )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	try
	{
		me->StartPass( inCamera, inLights );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3ViewStatus	QORenderer::Statics::EndPassMethod(
								TQ3ViewObject inView,
								void* privateData )
{
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	try
	{
		me->EndPass( inView );
	}
	catch (...)
	{
	}
	return kQ3ViewStatusDone;
}

TQ3Status	QORenderer::Statics::SubmitTriMeshMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3GeometryObject inGeomObject,
									const void* inGeomData )
{
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	
	try
	{
		bool	didHandle = me->SubmitTriMesh( inView, inGeomObject,
			reinterpret_cast<const TQ3TriMeshData*>(inGeomData) );
		result = didHandle? kQ3Success : kQ3Failure;
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3Status	QORenderer::Statics::SubmitTriangleMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3GeometryObject inGeomObject,
									const void* inGeomData )
{
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	
	try
	{
		me->SubmitTriangle( inView, inGeomObject,
			reinterpret_cast<const TQ3TriangleData*>(inGeomData) );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3Status	QORenderer::Statics::SubmitPointMethod(
							TQ3ViewObject inView,
							void* privateData,
							TQ3GeometryObject inGeomObject,
							const void* inGeomData )
{
#pragma unused( inView, inGeomObject )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	
	try
	{
		me->SubmitPoint(
			reinterpret_cast<const TQ3PointData*>(inGeomData) );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3Status	QORenderer::Statics::SubmitLineMethod(
							TQ3ViewObject inView,
							void* privateData,
							TQ3GeometryObject inGeomObject,
							const void* inGeomData )
{
#pragma unused( inView, inGeomObject )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	
	try
	{
		me->SubmitLine(
			reinterpret_cast<const TQ3LineData*>(inGeomData) );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3Status	QORenderer::Statics::SubmitPolyLineMethod(
							TQ3ViewObject inView,
							void* privateData,
							TQ3GeometryObject inGeomObject,
							const void* inGeomData )
{
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	
	try
	{
		me->SubmitPolyLine( inView, inGeomObject,
			reinterpret_cast<const TQ3PolyLineData*>(inGeomData) );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3XRendererSubmitGeometryMethod QORenderer::Statics::SubmitGeometrySubMetaHandler(
    								TQ3ObjectType inGeomType )
{
	TQ3XRendererSubmitGeometryMethod	theMethod = NULL;
	
	switch (inGeomType)
	{
		case kQ3GeometryTypeTriMesh:
			theMethod = &QORenderer::Statics::SubmitTriMeshMethod;
			break;
		
		case kQ3GeometryTypeTriangle:
			theMethod = &QORenderer::Statics::SubmitTriangleMethod;
			break;
		
		case kQ3GeometryTypePoint:
			theMethod = &QORenderer::Statics::SubmitPointMethod;
			break;
		
		case kQ3GeometryTypeLine:
			theMethod = &QORenderer::Statics::SubmitLineMethod;
			break;
		
		case kQ3GeometryTypePolyLine:
			theMethod = &QORenderer::Statics::SubmitPolyLineMethod;
			break;
	}
	
	return theMethod;
}

TQ3Status	QORenderer::Statics::UpdateLocalToCameraMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3Matrix4x4* inMatrix )
{
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	try
	{
		result = me->UpdateLocalToCamera( inView, *inMatrix );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3Status	QORenderer::Statics::UpdateCameraToFrustumMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3Matrix4x4* inMatrix )
{
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	TQ3Status	result = kQ3Success;
	try
	{
		result = me->UpdateCameraToFrustum( inView, *inMatrix );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3XRendererUpdateMatrixMethod
							QORenderer::Statics::UpdateMatrixSubMetaHandler(
									TQ3ObjectType inMatrixType )
{
	TQ3XRendererUpdateMatrixMethod	theMethod = NULL;
	
	switch (inMatrixType)
	{
		case kQ3XMethodTypeRendererUpdateMatrixLocalToCamera:
			theMethod = &QORenderer::Statics::UpdateLocalToCameraMethod;
			break;
		
		case kQ3XMethodTypeRendererUpdateMatrixCameraToFrustum:
			theMethod = &QORenderer::Statics::UpdateCameraToFrustumMethod;
			break;
	}
	
	return theMethod;
}

TQ3Status	QORenderer::Statics::UpdateDiffuseColorMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3ColorRGB* inAttColor )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateDiffuseColor( inAttColor );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateSpecularColorMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3ColorRGB* inAttColor )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateSpecularColor( inAttColor );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateSpecularControlMethod(
									TQ3ViewObject inView,
									void* privateData,
									const float* inAttValue )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateSpecularControl( inAttValue );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateTransparencyColorMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3ColorRGB* inAttColor )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateTransparencyColor( inAttColor );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateHiliteStateMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3Switch* inAttState )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateHiliteState( inAttState );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateSurfaceShaderMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3ShaderObject* inAttShader )
{
#pragma unused( inView )
	TQ3Status	result = kQ3Success;
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	try
	{
		TQ3ShaderObject	theShader = (inAttShader == NULL)? NULL : *inAttShader;
		me->UpdateSurfaceShader( theShader );
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3Status	QORenderer::Statics::UpdateIlluminationShaderMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3ShaderObject* inShader )
{
#pragma unused( inView )
	TQ3Status	result = kQ3Success;
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	try
	{
		if (inShader != NULL)
		{
			me->UpdateIlluminationShader( *inShader );
		}
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}

TQ3XRendererUpdateAttributeMethod
							QORenderer::Statics::UpdateAttributeSubMetaHandler(
									TQ3AttributeType inAttrType )
{
	TQ3XRendererUpdateAttributeMethod	theMethod = NULL;
	
	switch (inAttrType)
	{
		case kQ3AttributeTypeDiffuseColor:
			theMethod = (TQ3XRendererUpdateAttributeMethod)
				&QORenderer::Statics::UpdateDiffuseColorMethod;
			break;
		 
		case kQ3AttributeTypeSpecularColor:
			theMethod = (TQ3XRendererUpdateAttributeMethod)
				&QORenderer::Statics::UpdateSpecularColorMethod;
			break;
		
		case kQ3AttributeTypeSpecularControl:
			theMethod = (TQ3XRendererUpdateAttributeMethod)
				&QORenderer::Statics::UpdateSpecularControlMethod;
			break;
		
		case kQ3AttributeTypeTransparencyColor:
			theMethod = (TQ3XRendererUpdateAttributeMethod)
				&QORenderer::Statics::UpdateTransparencyColorMethod;
			break;
		
		case kQ3AttributeTypeHighlightState:
			theMethod = (TQ3XRendererUpdateAttributeMethod)
				&QORenderer::Statics::UpdateHiliteStateMethod;
			break;
		
		case kQ3AttributeTypeSurfaceShader:
			theMethod = (TQ3XRendererUpdateAttributeMethod)
				&QORenderer::Statics::UpdateSurfaceShaderMethod;
			break;
	}
	
	return theMethod;
}

TQ3XRendererUpdateShaderMethod
							QORenderer::Statics::UpdateShaderSubMetaHandler(
									TQ3ObjectType inShaderType )
{
	TQ3XRendererUpdateShaderMethod	theMethod = NULL;
	
	switch (inShaderType)
	{
		case kQ3ShaderTypeIllumination:
			theMethod = &QORenderer::Statics::UpdateIlluminationShaderMethod;
			break;
		
		case kQ3ShaderTypeSurface:
			theMethod = &QORenderer::Statics::UpdateSurfaceShaderMethod;
			break;
	}
	
	return theMethod;
}

TQ3Status	QORenderer::Statics::UpdateInterpolationStyleMethod(
								TQ3ViewObject inView,
								void* privateData,
								const void* publicData )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateInterpolationStyle( (TQ3InterpolationStyle*) publicData );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateBackfacingStyleMethod(
								TQ3ViewObject inView,
								void* privateData,
								const void* publicData )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateBackfacingStyle( (TQ3BackfacingStyle*) publicData );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateFillStyleMethod(
								TQ3ViewObject inView,
								void* privateData,
								const void* publicData )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateFillStyle( (TQ3FillStyle*) publicData );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateOrientationStyleMethod(
								TQ3ViewObject inView,
								void* privateData,
								const void* publicData )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateOrientationStyle( (TQ3OrientationStyle*) publicData );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateHighlightStyleMethod(
								TQ3ViewObject inView,
								void* privateData,
								const void* publicData )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateHighlightStyle( (TQ3AttributeSet*) publicData );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateAntiAliasStyleMethod(
								TQ3ViewObject inView,
								void* privateData,
								const void* publicData )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateAntiAliasStyle( (TQ3AntiAliasStyleData*) publicData );
	return kQ3Success;
}

TQ3Status	QORenderer::Statics::UpdateFogStyleMethod(
								TQ3ViewObject inView,
								void* privateData,
								const void* publicData )
{
#pragma unused( inView )
	QORenderer::Renderer*	me = *(QORenderer::Renderer**)privateData;
	me->UpdateFogStyle( (TQ3FogStyleData*) publicData );
	return kQ3Success;
}

TQ3XRendererUpdateStyleMethod
							QORenderer::Statics::UpdateStyleSubMetaHandler(
									TQ3ObjectType inStyleType )
{
	TQ3XRendererUpdateStyleMethod	theMethod = NULL;
	
	switch (inStyleType)
	{
		case kQ3StyleTypeInterpolation:
			theMethod = (TQ3XRendererUpdateStyleMethod)
				&QORenderer::Statics::UpdateInterpolationStyleMethod;
			break;

		case kQ3StyleTypeBackfacing:
			theMethod = (TQ3XRendererUpdateStyleMethod)
				&QORenderer::Statics::UpdateBackfacingStyleMethod;
			break;

		case kQ3StyleTypeFill:
			theMethod = (TQ3XRendererUpdateStyleMethod)
				&QORenderer::Statics::UpdateFillStyleMethod;
			break;

		case kQ3StyleTypeOrientation:
			theMethod = (TQ3XRendererUpdateStyleMethod)
				&QORenderer::Statics::UpdateOrientationStyleMethod;
			break;

		case kQ3StyleTypeHighlight:
			theMethod = (TQ3XRendererUpdateStyleMethod)
				&QORenderer::Statics::UpdateHighlightStyleMethod;
			break;

		case kQ3StyleTypeAntiAlias:
			theMethod = (TQ3XRendererUpdateStyleMethod)
				&QORenderer::Statics::UpdateAntiAliasStyleMethod;
			break;

		case kQ3StyleTypeFog:
			theMethod = (TQ3XRendererUpdateStyleMethod)
				&QORenderer::Statics::UpdateFogStyleMethod;
			break;
	}
	
	return theMethod;
}

TQ3XFunctionPointer		QORenderer::Statics::MetaHandler( TQ3XMethodType inMethodType )
{
	TQ3XFunctionPointer		theMethod = NULL;
	
	switch (inMethodType)
	{
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) &QORenderer::Statics::ObjectNewMethod;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) &QORenderer::Statics::ObjectDeleteMethod;
			break;
		
		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) &QORenderer_nickname;
			break;

		case kQ3XMethodTypeRendererIsInteractive:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
			
		case kQ3XMethodTypeRendererStartFrame:
			theMethod = (TQ3XFunctionPointer) &QORenderer::Statics::StartFrameMethod;
			break;
			
		case kQ3XMethodTypeRendererEndFrame:
			theMethod = (TQ3XFunctionPointer) &QORenderer::Statics::EndFrameMethod;
			break;
			
		case kQ3XMethodTypeRendererStartPass:
			theMethod = (TQ3XFunctionPointer) &QORenderer::Statics::StartPassMethod;
			break;
			
		case kQ3XMethodTypeRendererEndPass:
			theMethod = (TQ3XFunctionPointer) &QORenderer::Statics::EndPassMethod;
			break;
			
		case kQ3XMethodTypeRendererSubmitGeometryMetaHandler:
			theMethod = (TQ3XFunctionPointer)
				&QORenderer::Statics::SubmitGeometrySubMetaHandler;
			break;
		
		case kQ3XMethodTypeRendererUpdateMatrixMetaHandler:
			theMethod = (TQ3XFunctionPointer)
				&QORenderer::Statics::UpdateMatrixSubMetaHandler;
			break;
		
		case kQ3XMethodTypeRendererUpdateAttributeMetaHandler:
			theMethod = (TQ3XFunctionPointer)
				&QORenderer::Statics::UpdateAttributeSubMetaHandler;
			break;
		
		case kQ3XMethodTypeRendererUpdateShaderMetaHandler:
			theMethod = (TQ3XFunctionPointer)
				&QORenderer::Statics::UpdateShaderSubMetaHandler;
			break;
		
		case kQ3XMethodTypeRendererUpdateStyleMetaHandler:
			theMethod = (TQ3XFunctionPointer)
				&QORenderer::Statics::UpdateStyleSubMetaHandler;
			break;
	}
	
	return theMethod;
}
