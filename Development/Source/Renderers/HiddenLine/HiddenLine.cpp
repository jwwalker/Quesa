/*  NAME:
        HiddenLine.cpp

    DESCRIPTION:
        Cartoon-style renderer.

    COPYRIGHT:
        Copyright (c) 1999-2008, Quesa Developers. All rights reserved.

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
/*
	___________________________________________________________________________
	REMARKS
	
	___________________________________________________________________________
*/
#include "HiddenLine.h"
#include "GLPrefix.h"

#include "QORenderer.h"

#include "GLDrawContext.h"
#include "GLUtils.h"
#include "E3Compatibility.h"
#include "CQ3ObjectRef_Gets.h"


#define kQ3ClassNameRendererHiddenLine				"Quesa HiddenLine"

#if Q3_DEBUG
	#define		CHECK_GL_ERROR	Q3_ASSERT( (sLastGLError = glGetError()) == GL_NO_ERROR )
#else
	#define		CHECK_GL_ERROR
#endif

namespace
{
	TQ3XObjectClass	s_ParentRendererClass = NULL;
}

//____________________________________________________________________________________

static TQ3XFunctionPointer GetParentRendererMethod(TQ3XMethodType methodType)
{
	if (NULL == s_ParentRendererClass)
	{
		s_ParentRendererClass = Q3XObjectHierarchy_FindClassByType(kQ3RendererTypeOpenGL);	
	}

	if (NULL == s_ParentRendererClass)
	{
		return 0;
	}

	return Q3XObjectClass_GetMethod( s_ParentRendererClass, methodType );
}




//____________________________________________________________________________________
//____________________________________________________________________________________
//____________________________________   Renderer    _________________________________
//____________________________________________________________________________________
//____________________________________________________________________________________



namespace
{
	

	#pragma mark class CHiddenLineRendererQuesa
	class CHiddenLineRendererQuesa : public QORenderer::Renderer
	{
	public:
		CHiddenLineRendererQuesa( TQ3RendererObject inRenderer );

		~CHiddenLineRendererQuesa();
		
		void	StartPassHiddenLine(
								TQ3ViewObject inView,
								TQ3CameraObject inCamera,
								TQ3GroupObject inLights );
																
		void UpdateIlluminationShaderHiddenLine(
								TQ3ShaderObject inShader );
		
		TQ3ColorRGB mLineColor;
		TQ3ColorRGB mFillColor;
	};

}

//____________________________________________________________________________________

CHiddenLineRendererQuesa::CHiddenLineRendererQuesa( TQ3RendererObject inRenderer )
	: QORenderer::Renderer( inRenderer )
{
	TQ3ColorRGB fillColor = {1.0,1.0,1.0};
	TQ3ColorRGB lineColor = {0.0,0.0,0.0};
	mFillColor = fillColor;
	mLineColor = lineColor;
	

	mNumPasses = 3;
}

//____________________________________________________________________________________

CHiddenLineRendererQuesa::~CHiddenLineRendererQuesa()
{

}



//____________________________________________________________________________________


void CHiddenLineRendererQuesa::StartPassHiddenLine(
								TQ3ViewObject inView,
								TQ3CameraObject inCamera,
								TQ3GroupObject inLights )
{
	StartPass( inCamera, inLights ); // call parent Method

	TQ3CameraViewPort viewPort;	
	
	TQ3Boolean useColor = kQ3True;
	
	TQ3Float32 lineW = 1.0f;
	TQ3Float32 factor = 1.0f;
	Q3Object_GetProperty( mRendererObject, kQ3RendererPropertyLineWidth,
			sizeof(lineW), NULL, &lineW );
			
	Q3Object_GetProperty( mRendererObject, kQ3RendererPropertyUseColor,
			sizeof(useColor), NULL, &useColor );
			
	CQ3ObjectRef	theDrawContext( CQ3View_GetDrawContext( inView ) );
			
	if ( theDrawContext.isvalid() && (Q3Camera_GetViewPort(inCamera, &viewPort)))
    	{
		TQ3Area		thePane;
		Q3DrawContext_GetPane( theDrawContext.get(), &thePane );
		
		float diagonal = Q3FastPoint2D_Distance(&thePane.max,&thePane.min);

		
		factor = diagonal/720.0f*(4.0f/(viewPort.width + viewPort.height)); // scale (rather arbirarly) the line to the canvas size
		
		}
	
	TQ3FillStyle	fillStyle;

	switch(mPassIndex)
	{
	case 0:
		{
			glEnable( GL_POLYGON_OFFSET_FILL );
			glPolygonOffset( +(.1f), +(.1f) );
			fillStyle = kQ3FillStyleFilled;
			UpdateFillStyle( &fillStyle );
			//For the shader null
			glLineWidth( 1.0f );
			if(useColor == kQ3False){
				mViewState.diffuseColor = &mFillColor;
				mAttributesMask &= ~kQ3XAttributeMaskDiffuseColor; // don't update the diffuse color
				}
			UpdateIlluminationShader(NULL);
			mUpdateShader = false;
 		}
		break;
		
	case 1:
		{
		glDisable( GL_POLYGON_OFFSET_FILL );
		glEnable( GL_POLYGON_OFFSET_LINE );
		fillStyle = kQ3FillStyleEdges;
		UpdateFillStyle( &fillStyle );
			glPolygonOffset( +(factor * 5.1f), +(factor * 5.1f) );
		mLineWidth = lineW;
		glLineWidth( mLineWidth );
		mViewState.diffuseColor = &mLineColor;
		mAttributesMask &= ~kQ3XAttributeMaskDiffuseColor; // don't update the diffuse color
		mStyleState.mFill = kQ3FillStyleEdges;
		UpdateIlluminationShader(NULL);
//		mStyleState.mExplicitEdges = true;
		mUpdateShader = false;
		}
		break;
	case 2:
		{
//		glDisable( GL_POLYGON_OFFSET_LINE );
		fillStyle = kQ3FillStyleEdges;
		UpdateFillStyle( &fillStyle );
		glDisable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 0.0f,0.0f );
		mLineWidth = lineW * factor;
		glLineWidth( mLineWidth );
		mViewState.diffuseColor = &mLineColor;
		mAttributesMask &= ~kQ3XAttributeMaskDiffuseColor; // don't update the diffuse color
		UpdateIlluminationShader(NULL);
		mStyleState.mFill = kQ3FillStyleEdges;
		mStyleState.mExplicitEdges = true;
		}
		break;


	}
}


//____________________________________________________________________________________

void	CHiddenLineRendererQuesa::UpdateIlluminationShaderHiddenLine(
								TQ3ShaderObject inShader )
{
	if(mPassIndex == 0)
		UpdateIlluminationShader(inShader);
}



//____________________________________________________________________________________
//____________________________________________________________________________________
//____________________________________   Statics    __________________________________
//____________________________________________________________________________________
//____________________________________________________________________________________


static TQ3Status	hiddenline_startpass(
								TQ3ViewObject inView,
								void* privateData,
								TQ3CameraObject inCamera,
								TQ3GroupObject inLights )
{
#pragma unused( inView )
	TQ3Status status = kQ3Success;

	CHiddenLineRendererQuesa*	me = *(CHiddenLineRendererQuesa**) privateData;
	
	try
	{
		me->StartPassHiddenLine( inView, inCamera, inLights); // call our Method
	}
	catch (...)
	{
	}
	
	return status;
}


//____________________________________________________________________________________

static TQ3Status	hiddenline_update_illumination_shader(
									TQ3ViewObject inView,
									void* privateData,
									TQ3ShaderObject* inShader )
{
#pragma unused( inView )
	TQ3Status	result = kQ3Success;
	CHiddenLineRendererQuesa*	me = *(CHiddenLineRendererQuesa**)privateData;
	try
	{
		if (inShader != NULL)
		{
			me->UpdateIlluminationShaderHiddenLine( *inShader );
		}
	}
	catch (...)
	{
		result = kQ3Failure;
	}
	return result;
}





//____________________________________________________________________________________

static TQ3Status	hiddenline_update_fill_style(
								TQ3ViewObject ,
								void* ,
								const void*  )
{
	return kQ3Success;
}



//____________________________________________________________________________________

static TQ3Status
hiddenline_nickname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	// Return the amount of space we need
    *actualDataSize = (TQ3Uns32)strlen(kQ3ClassNameRendererHiddenLine) + 1;

	// If we have a buffer, return the nick name
	if (dataBuffer != NULL)
		{
		// Clamp the buffer size
		if (bufferSize < *actualDataSize)
			*actualDataSize = bufferSize;
		
		
		// Return the string
		Q3Memory_Copy(kQ3ClassNameRendererHiddenLine, dataBuffer, (*actualDataSize)-1);
        dataBuffer[(*actualDataSize)-1] = 0x00;
	}

    return(kQ3Success);
}


//____________________________________________________________________________________

static TQ3Status
hiddenline_new_object( TQ3Object theObject, void *privateData, void *paramData )
{
#pragma unused(paramData)
	
	// Since we are deriving from the Quesa OpenGL renderer, make sure that its
	// methods have been cached.  This happens the first time such a renderer
	// is instantiated.
	E3ClassInfoPtr	qoClass = E3ClassTree::GetClass( kQ3RendererTypeOpenGL );
	Q3_ASSERT( qoClass != NULL );
	if ( qoClass->GetMethod ( kQ3XMethodTypeRendererMethodsCached ) == NULL )
	{
		TQ3Object	dummyRenderer = Q3Renderer_NewFromType( kQ3RendererTypeOpenGL );
		Q3Object_CleanDispose( &dummyRenderer );
		Q3_ASSERT( qoClass->GetMethod ( kQ3XMethodTypeRendererMethodsCached ) != NULL );
	}


	TQ3Status	theStatus;
	CHiddenLineRendererQuesa*	newHidden = new(std::nothrow) CHiddenLineRendererQuesa( theObject );
	if (newHidden == NULL)
	{
		theStatus = kQ3Failure;
	}
	else
	{
		*(CHiddenLineRendererQuesa**)privateData = newHidden;
		theStatus = kQ3Success;
	}
	
	return theStatus;
}


//____________________________________________________________________________________

static void
hiddenline_delete_object( TQ3Object theObject, void *privateData )
{
#pragma unused( theObject )
	CHiddenLineRendererQuesa*	me = *(CHiddenLineRendererQuesa**) privateData;
	
	delete me;
}






//____________________________________________________________________________________
//____________________________________________________________________________________
//____________________________________   Metahandlers    _____________________________
//____________________________________________________________________________________
//____________________________________________________________________________________


static TQ3XRendererUpdateStyleMethod hiddenline_style_metahandler (
									TQ3ObjectType inStyleType )
{
	TQ3XRendererUpdateStyleMethod	theMethod = NULL;
	
	switch (inStyleType)
	{
		case kQ3StyleTypeFill:
			theMethod = (TQ3XRendererUpdateStyleMethod)
				&hiddenline_update_fill_style;
			break;


		default:
			theMethod = (TQ3XRendererUpdateStyleMethod)GetParentRendererMethod(inStyleType);
			break;
	}
	
	return theMethod;
}



//____________________________________________________________________________________

static TQ3XRendererUpdateShaderMethod hiddenline_shader_metahandler (
									TQ3ObjectType inShaderType )
{
	TQ3XRendererUpdateShaderMethod	theMethod = NULL;
	
	switch (inShaderType)
	{
		case kQ3ShaderTypeIllumination:
			theMethod = (TQ3XRendererUpdateShaderMethod)
				&hiddenline_update_illumination_shader;
			break;


		default:
			theMethod = (TQ3XRendererUpdateShaderMethod)GetParentRendererMethod(inShaderType);
			break;
	}
	
	return theMethod;
}



//____________________________________________________________________________________

static TQ3XFunctionPointer
hiddenline_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;	

	switch(methodType)
	{
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) hiddenline_new_object;
			break;
		
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) hiddenline_delete_object;
			break;
		
		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) hiddenline_nickname;
			break;
		
		case kQ3XMethodTypeRendererStartPass:
			theMethod = (TQ3XFunctionPointer) &hiddenline_startpass;
			break;
			
		case kQ3XMethodTypeRendererMethodsCached:
			// Deliberately return NULL here, so we do not inherit from parent
			break;
		
		case kQ3XMethodTypeRendererUpdateStyleMetaHandler:
			theMethod = (TQ3XFunctionPointer) &hiddenline_style_metahandler;
			break;

		case kQ3XMethodTypeRendererUpdateShaderMetaHandler:
			theMethod = (TQ3XFunctionPointer)&hiddenline_shader_metahandler;
			break;

		default:
			theMethod = GetParentRendererMethod(methodType);
			break;
	}
	
	return theMethod;
}




//____________________________________________________________________________________
//____________________________________________________________________________________
//____________________________________   Register    _________________________________
//____________________________________________________________________________________
//____________________________________________________________________________________




TQ3Status HiddenLine_Register()
{
	// Register the class
	//
	TQ3XObjectClass		theClass = EiObjectHierarchy_RegisterClassByType(
														kQ3SharedTypeRenderer,
														kQ3RendererTypeHiddenLine,
														kQ3ClassNameRendererHiddenLine,
														hiddenline_metahandler,
														NULL,
														0,
														sizeof(CHiddenLineRendererQuesa*));


	return(theClass == NULL ? kQ3Failure : kQ3Success);
}

//____________________________________________________________________________________

void HiddenLine_Unregister()
{
	TQ3Status			qd3dStatus;
	TQ3XObjectClass		theClass;

	// Find the renderer class
	theClass = Q3XObjectHierarchy_FindClassByType( kQ3RendererTypeHiddenLine );
	if (theClass == NULL)
		return;

	// Unregister the class
	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(theClass);
	
	s_ParentRendererClass = NULL;
}
