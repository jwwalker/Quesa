/*!
	@header		QOStatics.h
	
	Static method functions.
*/

/*  NAME:
       QOStatics.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
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

#include "E3Prefix.h"


//=============================================================================
//      Class Declaration
//-----------------------------------------------------------------------------

namespace QORenderer
{

/*!
	@class		Statics
	
	@abstract	This is a collection of metahandlers and other static methods
				used by QORenderer.  They have been separated out just to
				reduce the size of QORenderer.h.  They are in a class so that
				they can be made friends of QORenderer easily.
*/
class Statics
{
public:
	//
	// Main metahandler
	//
	static
	TQ3XFunctionPointer		MetaHandler( TQ3XMethodType inMethodType );
	
private:
	//
	// Top-level static methods returned by main metahandler
	//
	static TQ3Status		ObjectNewMethod(
									TQ3Object theObject,
									void *privateData,
									void *paramData );

	static void				ObjectDeleteMethod(
									TQ3Object object,
									void* privateData );

	static TQ3Status		StartFrameMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3DrawContextObject inDrawContext );

	static TQ3Status		EndFrameMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3DrawContextObject inDrawContext );

	static TQ3Status		StartPassMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3CameraObject inCamera,
									TQ3GroupObject inLights );

	static TQ3ViewStatus	EndPassMethod(
									TQ3ViewObject inView,
									void* privateData );

	static TQ3Boolean		IsBoundingBoxVisibleMethod(
									TQ3ViewObject           theView,
		                            void                    *rendererPrivate,
		                            const TQ3BoundingBox    *theBounds );

	static TQ3XRendererSubmitGeometryMethod
							SubmitGeometrySubMetaHandler(
									TQ3ObjectType inGeomType );
	
	static TQ3XRendererUpdateMatrixMethod
							UpdateMatrixSubMetaHandler(
									TQ3ObjectType inMatrixType );
	
	static TQ3XRendererUpdateAttributeMethod
							UpdateAttributeSubMetaHandler(
									TQ3AttributeType inAttrType );

	static TQ3XRendererUpdateShaderMethod
							UpdateShaderSubMetaHandler(
									TQ3ObjectType inShaderType );
	
	static TQ3XRendererUpdateStyleMethod
							UpdateStyleSubMetaHandler(
									TQ3ObjectType inStyleType );

	//
	// static methods returned by submit-geometry metahandler
	//
	static TQ3Status		SubmitTriangleMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3GeometryObject inGeomObject,
									const void* inGeomData );

	static TQ3Status		SubmitTriMeshMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3GeometryObject inGeomObject,
									const void* inGeomData );

	static TQ3Status		SubmitPointMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3GeometryObject inGeomObject,
									const void* inGeomData );

	static TQ3Status		SubmitLineMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3GeometryObject inGeomObject,
									const void* inGeomData );

	static TQ3Status		SubmitPolyLineMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3GeometryObject inGeomObject,
									const void* inGeomData );


	//
	// static methods returned by update-matrix metahandler
	//
	static TQ3Status		UpdateLocalToCameraMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3Matrix4x4* inMatrix );

	static TQ3Status		UpdateCameraToFrustumMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3Matrix4x4* inMatrix );

	//
	// static methods returned by update-attribute metahandler
	//
	static TQ3Status		UpdateDiffuseColorMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3ColorRGB* inAttColor );

	static TQ3Status		UpdateSpecularColorMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3ColorRGB* inAttColor );

	static TQ3Status		UpdateSpecularControlMethod(
									TQ3ViewObject inView,
									void* privateData,
									const float* inAttValue );

	static TQ3Status		UpdateTransparencyColorMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3ColorRGB* inAttColor );

	static TQ3Status		UpdateEmissiveColorMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3ColorRGB* inAttColor );

	static TQ3Status		UpdateHiliteStateMethod(
									TQ3ViewObject inView,
									void* privateData,
									const TQ3Switch* inAttState );

	//
	// static methods returned by update-shader metahandler
	static TQ3Status		UpdateSurfaceShaderMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3ShaderObject* inShader );

	static TQ3Status		UpdateIlluminationShaderMethod(
									TQ3ViewObject inView,
									void* privateData,
									TQ3ShaderObject* inShader );

	//
	// static methods returned by update-style metahandler
	//
	static TQ3Status			UpdateInterpolationStyleMethod(
									TQ3ViewObject inView,
									void* privateData,
									const void* publicData );

	static TQ3Status			UpdateBackfacingStyleMethod(
									TQ3ViewObject inView,
									void* privateData,
									const void* publicData );

	static TQ3Status			UpdateFillStyleMethod(
									TQ3ViewObject inView,
									void* privateData,
									const void* publicData );

	static TQ3Status			UpdateOrientationStyleMethod(
									TQ3ViewObject inView,
									void* privateData,
									const void* publicData );

	static TQ3Status			UpdateHighlightStyleMethod(
									TQ3ViewObject inView,
									void* privateData,
									const void* publicData );

	static TQ3Status			UpdateAntiAliasStyleMethod(
									TQ3ViewObject inView,
									void* privateData,
									const void* publicData );

	static TQ3Status			UpdateFogStyleMethod(
									TQ3ViewObject inView,
									void* privateData,
									const void* publicData );

	static TQ3Status			UpdateCastShadowsStyleMethod(
									TQ3ViewObject inView,
									void* privateData,
									const void* publicData );

};
	
}
