/*  NAME:
        E3Renderer.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3View.h"
#include "E3Set.h"
#include "E3DrawContext.h"
#include "E3Renderer.h"
#include "E3Main.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TQ3RendererData {
	// Interactive renderer state (for all interactive renderers)
	TQ3CSGEquation			interactiveCSGEquation;
	TQ3RaveVendorID			interactiveVendorID;
	TQ3RaveEngineID			interactiveEngineID;
	TQ3Boolean				interactiveDoubleBufferBypass;
	TQ3Uns32				raveContextHints;
	TQ3TextureFilter		raveTextureFilter;
	TQ3Boolean				drawContextReset;
} TQ3RendererData;





class E3Renderer : public E3Shared // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3SharedTypeRenderer, E3Renderer, E3Shared )
public :

	TQ3RendererData			instanceData ;
	} ;
	


class E3CSGAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3AttributeTypeConstructiveSolidGeometryID, E3CSGAttribute, E3Attribute )
public :

	TQ3CSGObjectID			instanceData ;
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3renderer_add_methods : Add the renderer methods to the class tree.
//-----------------------------------------------------------------------------
//		Note :	Renderer objects have several secondary metahandlers, which are
//				queried to find particular methods.
//
//				These methods won't be cached by the class tree (since they're
//				not accessed through our main metahandler), so we need to add
//				them to the class tree's cache manually.
//-----------------------------------------------------------------------------
static void
e3renderer_add_methods(TQ3RendererObject theRenderer)
	{
	TQ3Uns32										n, numMethods ;
	TQ3XFunctionPointer								theMethod ;

	TQ3XMethodType									matrixMethods[] = {
														kQ3XMethodTypeRendererUpdateMatrixLocalToWorld,
														kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverse,
														kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverseTranspose,
														kQ3XMethodTypeRendererUpdateMatrixLocalToCamera,
														kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum,
														kQ3XMethodTypeRendererUpdateMatrixWorldToCamera,
														kQ3XMethodTypeRendererUpdateMatrixWorldToFrustum,
														kQ3XMethodTypeRendererUpdateMatrixCameraToFrustum };

	TQ3XMethodType									geomMethods[] = {
														kQ3GeometryTypeBox,
														kQ3GeometryTypeCone,
														kQ3GeometryTypeCylinder,
														kQ3GeometryTypeDisk,
														kQ3GeometryTypeEllipse,
														kQ3GeometryTypeEllipsoid,
														kQ3GeometryTypeGeneralPolygon,
														kQ3GeometryTypeLine,
														kQ3GeometryTypeMarker,
														kQ3GeometryTypeMesh,
														kQ3GeometryTypeNURBCurve,
														kQ3GeometryTypeNURBPatch,
														kQ3GeometryTypePixmapMarker,
														kQ3GeometryTypePoint,
														kQ3GeometryTypePolyLine,
														kQ3GeometryTypePolygon,
														kQ3GeometryTypePolyhedron,
														kQ3GeometryTypeTorus,
														kQ3GeometryTypeTriangle,
														kQ3GeometryTypeTriGrid,
														kQ3GeometryTypeTriMesh };

	TQ3XMethodType									attributeMethods[] = {
														kQ3AttributeTypeSurfaceUV,
														kQ3AttributeTypeShadingUV,
														kQ3AttributeTypeNormal,
														kQ3AttributeTypeAmbientCoefficient,
														kQ3AttributeTypeDiffuseColor,
														kQ3AttributeTypeSpecularColor,
														kQ3AttributeTypeSpecularControl,
														kQ3AttributeTypeTransparencyColor,
														kQ3AttributeTypeSurfaceTangent,
														kQ3AttributeTypeHighlightState,
														kQ3AttributeTypeSurfaceShader };

	TQ3XMethodType									shaderMethods[] = {
														kQ3ShaderTypeSurface,
														kQ3ShaderTypeIllumination };

	TQ3XMethodType									styleMethods[] = {
														kQ3StyleTypeBackfacing,
														kQ3StyleTypeInterpolation,
														kQ3StyleTypeFill,
														kQ3StyleTypePickID,
														kQ3StyleTypeCastShadows,
														kQ3StyleTypeReceiveShadows,
														kQ3StyleTypeHighlight,
														kQ3StyleTypeSubdivision,
														kQ3StyleTypeOrientation,
														kQ3StyleTypePickParts,
														kQ3StyleTypeAntiAlias,
														kQ3StyleTypeFog };



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theRenderer);



	// Find the object class
	E3ClassInfoPtr theClass = theRenderer->GetClass () ;



	// Add the dummy cached method, so that we don't execute this again. Because
	// we should never be calling this 'method', we use a dummy value as the pointer.
	theClass->AddMethod ( kQ3XMethodTypeRendererMethodsCached, (TQ3XFunctionPointer) kQ3ObjectTypeQuesa ) ;



	// Add the matrix methods
	TQ3XRendererUpdateMatrixMetaHandlerMethod matrixMeta = (TQ3XRendererUpdateMatrixMetaHandlerMethod)
					theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixMetaHandler ) ;
	if ( matrixMeta != NULL )
		{
		numMethods = sizeof ( matrixMethods ) / sizeof ( TQ3ObjectType ) ;
		for ( n = 0 ; n < numMethods ; ++n )
			{
			theMethod = matrixMeta ( matrixMethods [ n ] ) ;
			if ( theMethod != NULL )
				theClass->AddMethod ( matrixMethods [ n ], theMethod ) ;
			}
		}



	// Add the geometry methods
	TQ3XRendererSubmitGeometryMetaHandlerMethod geomMeta = (TQ3XRendererSubmitGeometryMetaHandlerMethod)
					theClass->GetMethod ( kQ3XMethodTypeRendererSubmitGeometryMetaHandler ) ;
	if ( geomMeta != NULL )
		{
		numMethods = sizeof ( geomMethods ) / sizeof ( TQ3ObjectType ) ;
		for ( n = 0 ; n < numMethods ; ++n )
			{
			theMethod = geomMeta ( geomMethods [ n ] );
			if ( theMethod != NULL )
				theClass->AddMethod ( geomMethods [ n ], theMethod ) ;
			}
		}



	// Add the attribute methods
	TQ3XRendererUpdateAttributeMetaHandlerMethod attributeMeta = (TQ3XRendererUpdateAttributeMetaHandlerMethod)
					theClass->GetMethod ( kQ3XMethodTypeRendererUpdateAttributeMetaHandler ) ;
	if ( attributeMeta != NULL )
		{
		numMethods = sizeof ( attributeMethods ) / sizeof ( TQ3ObjectType ) ;
		for ( n = 0 ; n < numMethods ; ++n )
			{
			theMethod = attributeMeta ( attributeMethods [ n ] ) ;
			if ( theMethod != NULL )
				theClass->AddMethod ( attributeMethods [ n ], theMethod ) ;
			}
		}



	// Add the shader methods
	TQ3XRendererUpdateShaderMetaHandlerMethod shaderMeta = (TQ3XRendererUpdateShaderMetaHandlerMethod)
					theClass->GetMethod ( kQ3XMethodTypeRendererUpdateShaderMetaHandler ) ;
	if ( shaderMeta != NULL )
		{
		numMethods = sizeof ( shaderMethods ) / sizeof ( TQ3ObjectType ) ;
		for ( n = 0 ; n < numMethods ; ++n )
			{
			theMethod = shaderMeta ( shaderMethods [ n ] ) ;
			if ( theMethod != NULL )
				theClass->AddMethod ( shaderMethods [ n ], theMethod ) ;
			}
		}



	// Add the style methods
	TQ3XRendererUpdateStyleMetaHandlerMethod styleMeta = (TQ3XRendererUpdateStyleMetaHandlerMethod)
					theClass->GetMethod ( kQ3XMethodTypeRendererUpdateStyleMetaHandler ) ;
	if ( styleMeta != NULL )
		{
		numMethods = sizeof ( styleMethods ) / sizeof ( TQ3ObjectType ) ;
		for ( n = 0 ; n < numMethods ; ++n )
			{
			theMethod = styleMeta ( styleMethods [ n ] ) ;
			if ( theMethod != NULL )
				theClass->AddMethod ( styleMethods [ n ], theMethod ) ;
			}
		}
	}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Renderer_RegisterClass : Register the renderer classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Renderer_RegisterClass(void)
	{
	// Register the renderer classes
	TQ3Status qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameRenderer,
												NULL,
												E3Renderer ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeCSGID,
											NULL,
											E3CSGAttribute ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3Renderer_UnregisterClass : Unregister the renderer classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_UnregisterClass(void)
	{
	// Unregister the classes
	E3ClassTree::UnregisterClass(kQ3SharedTypeRenderer,                       kQ3True);
	return E3ClassTree::UnregisterClass(kQ3AttributeTypeConstructiveSolidGeometryID, kQ3True);
	}





//=============================================================================
//      E3Renderer_Method_StartFrame : Call the start frame method.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Renderer_Method_StartFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext)
	{
	E3Renderer* theRenderer = (E3Renderer*) E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;



	// Find the method, if implemented
	TQ3XRendererStartFrameMethod startFrame = (TQ3XRendererStartFrameMethod) theRenderer->GetMethod ( kQ3XMethodTypeRendererStartFrame ) ;
	if ( startFrame == NULL )
		return kQ3Success ;



	// Find the renderer instance data
	// If the draw context has to be reset, reset it now.
	//
	// This is slightly ugly: the reason for this is that changing the RAVE
	// texture hint has to flush the texture cache maintained by the IR.
	//
	// Since Q3InteractiveRenderer_SetRAVETextureFilter doesn't have access
	// to the draw context, we need to keep a flag indicating if it's
	// changed since the last frame.
	//
	// If it has changed, we reset the draw context validation flags and
	// thus cause a rebuild of the texture cache. We just reset all the
	// flags for now, rather than add a new API call to set specific
	// flags - assumption is that flushing the texture cache will be
	// expensive enough that resetting the reset of the validation flags
	// will be of no consequence.
	//
	// If this turns out to be too expensive, we should add an API so that
	// we can just reset kQ3XDrawContextValidationRAVETextureFilter.
	if ( theRenderer->instanceData.drawContextReset )
		{
		E3DrawContext_ResetState ( theDrawContext ) ;
		theRenderer->instanceData.drawContextReset = kQ3False ;
		}



	// Call the method
	return startFrame ( theView, theRenderer->FindLeafInstanceData (), theDrawContext ) ;
	}





//=============================================================================
//      E3Renderer_Method_StartPass : Call the start pass method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_StartPass(TQ3ViewObject theView, TQ3CameraObject theCamera, TQ3GroupObject theLights)
	{
	E3Renderer* theRenderer = (E3Renderer*) E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;



	// Find the method, if implemented
	TQ3XRendererStartPassMethod startPass = (TQ3XRendererStartPassMethod) theRenderer->GetMethod ( kQ3XMethodTypeRendererStartPass ) ;
	if ( startPass == NULL )
		return kQ3Success ;



	// Call the method
	return startPass ( theView, theRenderer->FindLeafInstanceData (), theCamera, theLights ) ;
	}





//=============================================================================
//      E3Renderer_Method_EndPass : Call the end pass method.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3Renderer_Method_EndPass(TQ3ViewObject theView)
	{
	E3Renderer* theRenderer = (E3Renderer*) E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3ViewStatusDone ;



	// Find the method, if implemented
	TQ3XRendererEndPassMethod endPass = (TQ3XRendererEndPassMethod) theRenderer->GetMethod ( kQ3XMethodTypeRendererEndPass ) ;
	if ( endPass == NULL )
		return kQ3ViewStatusDone ;



	// Call the method
	return endPass ( theView, theRenderer->FindLeafInstanceData () ) ;
	}





//=============================================================================
//      E3Renderer_Method_FlushFrame : Call the flush frame method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_FlushFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext)
	{
	TQ3RendererObject theRenderer = E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;



	// Find the method, if implemented
	TQ3XRendererFlushFrameMethod flushFrame = (TQ3XRendererFlushFrameMethod) theRenderer->GetMethod ( kQ3XMethodTypeRendererFlushFrame ) ;
	if ( flushFrame == NULL )
		return kQ3Failure ;



	// Call the method
	return flushFrame ( theView, theRenderer->FindLeafInstanceData (), theDrawContext ) ;
	}




//=============================================================================
//      E3Renderer_Method_EndFrame : Call the end frame method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_EndFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext)
	{
	TQ3RendererObject theRenderer = E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;



	// Find the method, if implemented
	TQ3XRendererEndFrameMethod endFrame = (TQ3XRendererEndFrameMethod) theRenderer->GetMethod ( kQ3XMethodTypeRendererEndFrame ) ;
	if ( endFrame == NULL )
		return kQ3Success ;



	// Call the method
	return endFrame ( theView, theRenderer->FindLeafInstanceData (), theDrawContext ) ;
	}





//=============================================================================
//      E3Renderer_Method_IsBBoxVisible : Call the bounds visible method.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Renderer_Method_IsBBoxVisible(TQ3ViewObject theView, const TQ3BoundingBox *theBBox)
	{
	TQ3RendererObject theRenderer = E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3True);



	// Find the method, if implemented
	TQ3XRendererIsBoundingBoxVisibleMethod isBoundingBoxVisible = (TQ3XRendererIsBoundingBoxVisibleMethod)
						theRenderer->GetMethod ( kQ3XMethodTypeRendererIsBoundingBoxVisible ) ;
	if ( isBoundingBoxVisible == NULL )
		return kQ3True ;



	// Call the method
	return isBoundingBoxVisible ( theView, theRenderer->FindLeafInstanceData (), theBBox ) ;
	}





//=============================================================================
//      E3Renderer_Method_UpdateMatrix : Matrix update method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateMatrix(TQ3ViewObject			theView,
								TQ3MatrixState			theState,
								const TQ3Matrix4x4		*localToWorld,
								const TQ3Matrix4x4		*worldToCamera,
								const TQ3Matrix4x4		*cameraToFrustum)
	{
	TQ3Matrix4x4		worldToLocal, tmpMatrix ;



	TQ3RendererObject theRenderer = E3View_AccessRenderer ( theView ) ;
	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;


	// Find the methods
	E3ClassInfoPtr theClass = theRenderer->GetClass () ;
	TQ3XRendererUpdateMatrixMethod updateLocalToWorld     = (TQ3XRendererUpdateMatrixMethod) theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixLocalToWorld ) ;
	TQ3XRendererUpdateMatrixMethod updateLocalToWorldInv  = (TQ3XRendererUpdateMatrixMethod) theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverse ) ;
	TQ3XRendererUpdateMatrixMethod updateLocalToWorldInvT = (TQ3XRendererUpdateMatrixMethod) theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverseTranspose ) ;
	TQ3XRendererUpdateMatrixMethod updateLocalToCamera    = (TQ3XRendererUpdateMatrixMethod) theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixLocalToCamera ) ;
	TQ3XRendererUpdateMatrixMethod updateLocalToFrustum   = (TQ3XRendererUpdateMatrixMethod) theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum ) ;
	TQ3XRendererUpdateMatrixMethod updateWorldToCamera    = (TQ3XRendererUpdateMatrixMethod) theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixWorldToCamera ) ;
	TQ3XRendererUpdateMatrixMethod updateWorldToFrustum   = (TQ3XRendererUpdateMatrixMethod) theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixWorldToFrustum ) ;
	TQ3XRendererUpdateMatrixMethod updateCameraToFrustum  = (TQ3XRendererUpdateMatrixMethod) theClass->GetMethod ( kQ3XMethodTypeRendererUpdateMatrixCameraToFrustum ) ;



	// Find the renderer instance data
	void* instanceData = theRenderer->FindLeafInstanceData () ;



	TQ3Status qd3dStatus = kQ3Success ;

	// Handle local-to-world changes
	if ( theState & kQ3MatrixStateLocalToWorld )
		{
		if (updateLocalToWorldInv != NULL || updateLocalToWorldInvT != NULL )
			Q3Matrix4x4_Invert(localToWorld, &worldToLocal);

		if ( qd3dStatus == kQ3Success && updateLocalToWorld != NULL )
			qd3dStatus = updateLocalToWorld(theView, instanceData, localToWorld);

		if ( qd3dStatus == kQ3Success && updateLocalToWorldInv != NULL )
			qd3dStatus = updateLocalToWorldInv(theView, instanceData, &worldToLocal);

		if ( qd3dStatus == kQ3Success && updateLocalToWorldInvT != NULL )
			{
			Q3Matrix4x4_Transpose(&worldToLocal, &tmpMatrix);
			qd3dStatus = updateLocalToWorldInvT(theView, instanceData, &tmpMatrix);
			}

		if ( qd3dStatus == kQ3Success && updateLocalToCamera != NULL )
			{
			Q3Matrix4x4_Multiply(localToWorld, worldToCamera, &tmpMatrix);
			qd3dStatus = updateLocalToCamera(theView, instanceData, &tmpMatrix);
			}

		if ( qd3dStatus == kQ3Success && updateLocalToFrustum != NULL )
			{
			Q3Matrix4x4_Multiply(localToWorld, worldToCamera,   &tmpMatrix);
			Q3Matrix4x4_Multiply(&tmpMatrix,   cameraToFrustum, &tmpMatrix);
			qd3dStatus = updateLocalToFrustum(theView, instanceData, &tmpMatrix);
			}
		}
	
	
	
	// Handle world-to-camera changes
	if (theState & kQ3MatrixStateWorldToCamera)
		{
		if ( qd3dStatus == kQ3Success && updateWorldToCamera != NULL )
			qd3dStatus = updateWorldToCamera(theView, instanceData, worldToCamera);

		if ( qd3dStatus == kQ3Success && updateWorldToFrustum != NULL )
			{
			Q3Matrix4x4_Multiply(worldToCamera, cameraToFrustum, &tmpMatrix);
			qd3dStatus = updateWorldToFrustum(theView, instanceData, &tmpMatrix);
			}
		}
	
	
	
	// Handle camera-to-frustum changes
	if (theState & kQ3MatrixStateCameraToFrustum)
		{
		if ( qd3dStatus == kQ3Success && updateCameraToFrustum != NULL )
			qd3dStatus = updateCameraToFrustum(theView, instanceData, cameraToFrustum);
		}

	return qd3dStatus ;
	}





//=============================================================================
//      E3Renderer_Method_UpdateShader : Shader update method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateShader(TQ3ViewObject theView, TQ3ObjectType shaderType, TQ3Object *theShader)
	{	
	TQ3RendererObject theRenderer = E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;



	// Find the method, if implemented
	TQ3XRendererUpdateShaderMethod updateMethod = (TQ3XRendererUpdateShaderMethod) theRenderer->GetMethod ( shaderType ) ;
	if ( updateMethod == NULL )
		return kQ3Success ;



	// Call the method
	return updateMethod ( theView, theRenderer->FindLeafInstanceData (), theShader ) ;
	}





//=============================================================================
//      E3Renderer_Method_UpdateStyle : Style update method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateStyle(TQ3ViewObject theView, TQ3ObjectType styleType, const void *paramData)
	{
	TQ3RendererObject theRenderer = E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;



	// Find the method, if implemented
	TQ3XRendererUpdateStyleMethod updateMethod = (TQ3XRendererUpdateStyleMethod) theRenderer->GetMethod ( styleType ) ;
	if ( updateMethod == NULL )
		return kQ3Success ;



	// Call the method
	return updateMethod ( theView, theRenderer->FindLeafInstanceData (), paramData ) ;
	}





//=============================================================================
//      E3Renderer_Method_UpdateAttribute : Attribute update method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateAttribute(TQ3ViewObject theView, TQ3AttributeType attributeType, const void *paramData)
	{
	TQ3RendererObject theRenderer = E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;



	// Find the method, if implemented
	TQ3XRendererUpdateAttributeMethod updateMethod = (TQ3XRendererUpdateAttributeMethod) theRenderer->GetMethod ( attributeType ) ;
	if ( updateMethod == NULL )
		return kQ3Success ;



	// Call the method
	return updateMethod ( theView, theRenderer->FindLeafInstanceData (), paramData ) ;
	}





//=============================================================================
//      E3Renderer_Method_SubmitGeometry : Submit a geometry to a renderer.
//-----------------------------------------------------------------------------
//		Note :	We update geomSupported to indicate if the renderer could accept
//				the geometry or not.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_SubmitGeometry(TQ3ViewObject		theView,
								 TQ3ObjectType		geomType,
								 TQ3Boolean			*geomSupported,
								 TQ3GeometryObject	theGeom,
								 const void			*geomData)
	{
	TQ3Status								qd3dStatus  = kQ3Failure ;
	TQ3AttributeSet							attSet = NULL ;
	TQ3Boolean								hasSurfaceShader = kQ3False ;


	TQ3RendererObject theRenderer = E3View_AccessRenderer ( theView ) ;

	// No-op if no renderer set
	if ( theRenderer == NULL )
		return kQ3Success ;



	// Find the method
	TQ3XRendererSubmitGeometryMethod submitGeom = (TQ3XRendererSubmitGeometryMethod) theRenderer->GetMethod ( geomType ) ;
	*geomSupported = (TQ3Boolean) ( submitGeom != NULL ) ;



	// Test whether the geometry's attribute set contains a surface shader.
	// (How do we do this in immediate mode?)
	if ( ( theGeom != NULL ) && ( kQ3Success == Q3Geometry_GetAttributeSet ( theGeom, &attSet ) )
	&& ( attSet != NULL ) )
		{
		hasSurfaceShader = Q3AttributeSet_Contains ( attSet, kQ3AttributeTypeSurfaceShader ) ;
		Q3Object_Dispose ( attSet ) ;
		}
	
	
	// If there is a shader, we must push the view state
	if ( hasSurfaceShader )
		Q3Push_Submit ( theView ) ;


	// Call the method
	if ( submitGeom != NULL )
		qd3dStatus = submitGeom ( theView, theRenderer->FindLeafInstanceData (), theGeom, geomData ) ;


	if (hasSurfaceShader)
		Q3Pop_Submit( theView );

	return qd3dStatus ;
	}





//=============================================================================
//      E3Renderer_NewFromType : Create a new renderer object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3RendererObject
E3Renderer_NewFromType(TQ3ObjectType rendererObjectType)
	{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( rendererObjectType, kQ3False, NULL ) ;



	// If we've not yet cached this renderer's methods, do so now
	if ( theObject != NULL )
		{
		E3ClassInfoPtr theClass = theObject->GetClass () ;
		if ( theClass != NULL )
			{
			if ( theClass->GetMethod ( kQ3XMethodTypeRendererMethodsCached ) == NULL )
				e3renderer_add_methods ( theObject ) ;
			}
		}
	
	return theObject ;
	}





//=============================================================================
//      E3Renderer_GetType : Return the type of a renderer.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Renderer_GetType(TQ3RendererObject theRenderer)
	{
	// Return the type
	return theRenderer->GetObjectType ( kQ3SharedTypeRenderer ) ;
	}





//=============================================================================
//      E3Renderer_IsInteractive : Is a renderer interactive?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Renderer_IsInteractive(TQ3RendererObject theRenderer)
	{
	// Return as the method is defined or not
	return (TQ3Boolean) ( theRenderer->GetMethod ( kQ3XMethodTypeRendererIsInteractive ) != NULL ) ;
	}





//=============================================================================
//      E3Renderer_HasModalConfigure : Does a renderer have a configure dialog?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Renderer_HasModalConfigure(TQ3RendererObject theRenderer)
	{
	// Return as the method is defined or not
	return (TQ3Boolean) ( theRenderer->GetMethod ( kQ3XMethodTypeRendererModalConfigure ) != NULL ) ;
	}





//=============================================================================
//      E3Renderer_ModalConfigure : Invoke a renderer's modal configure dialog.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_ModalConfigure(TQ3RendererObject theRenderer, TQ3DialogAnchor dialogAnchor, TQ3Boolean *cancelled)
	{
	// Find the method
	TQ3XRendererModalConfigureMethod modalConfigure = (TQ3XRendererModalConfigureMethod)
						theRenderer->GetMethod ( kQ3XMethodTypeRendererModalConfigure ) ;
	if ( modalConfigure == NULL )
		return kQ3Failure ;



	// Call the method
	return modalConfigure ( theRenderer, dialogAnchor, cancelled, theRenderer->FindLeafInstanceData () ) ;
	}





//=============================================================================
//      E3RendererClass_GetNickNameString : Get the renderer nick name.
//-----------------------------------------------------------------------------
TQ3Status
E3RendererClass_GetNickNameString(TQ3ObjectType rendererClassType, TQ3ObjectClassNameString rendererClassString)
	{
	// Initialise a return value
	rendererClassString [ 0 ] = 0x00 ;



	// Find the renderer class, and get the method
	E3ClassInfoPtr rendererClass = E3ClassTree::GetClass ( rendererClassType ) ;
	if ( rendererClass == NULL )
		return kQ3Failure ;

	TQ3XRendererGetNickNameStringMethod nickNameMethod = (TQ3XRendererGetNickNameStringMethod)
								rendererClass->GetMethod ( kQ3XMethodTypeRendererGetNickNameString ) ;
	if ( nickNameMethod == NULL )
		return kQ3Failure ;



	TQ3Uns32 numWritten ;
	// Call the method
	return nickNameMethod ( (unsigned char *) rendererClassString,
									sizeof ( TQ3ObjectClassNameString ),
									&numWritten ) ;
	}





//=============================================================================
//      E3Renderer_GetConfigurationData : Get a renderer's config data.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_GetConfigurationData(TQ3RendererObject theRenderer, unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
	{
	// Initialise a return value
	*actualDataSize = 0 ;



	// Find the method
	TQ3XRendererGetConfigurationDataMethod getConfigData = (TQ3XRendererGetConfigurationDataMethod)
						theRenderer->GetMethod ( kQ3XMethodTypeRendererGetConfigurationData ) ;
	if ( getConfigData == NULL )
		return kQ3Failure ;



	// Call the method
	return getConfigData ( theRenderer,
								dataBuffer,
								bufferSize,
								actualDataSize,
								theRenderer->FindLeafInstanceData () ) ;
	}





//=============================================================================
//      E3Renderer_SetConfigurationData : Set a renderer's config data.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_SetConfigurationData(TQ3RendererObject theRenderer, unsigned char *dataBuffer, TQ3Uns32 bufferSize)
	{
	// Find the method
	TQ3XRendererSetConfigurationDataMethod setConfigData = (TQ3XRendererSetConfigurationDataMethod)
						theRenderer->GetMethod ( kQ3XMethodTypeRendererSetConfigurationData ) ;
	if ( setConfigData == NULL )
		return kQ3Failure ;



	// Call the method
	TQ3Status qd3dStatus = setConfigData ( theRenderer,
								dataBuffer,
								bufferSize,
								theRenderer->FindLeafInstanceData () ) ;

	Q3Shared_Edited ( theRenderer ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3InteractiveRenderer_SetCSGEquation : Set the CSG equation.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3InteractiveRenderer_SetCSGEquation ( TQ3RendererObject theRenderer, TQ3CSGEquation equation )
	{
	// Set the field
	( (E3Renderer*) theRenderer )->instanceData.interactiveCSGEquation = equation ;
	Q3Shared_Edited ( theRenderer ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_GetCSGEquation : Get the CSG equation.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetCSGEquation(TQ3RendererObject theRenderer, TQ3CSGEquation *equation)
	{
	// Get the field
	*equation = ( (E3Renderer*) theRenderer )->instanceData.interactiveCSGEquation ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_SetPreferences : Set the vendor/engine ID.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_SetPreferences(TQ3RendererObject theRenderer, TQ3RaveVendorID vendorID, TQ3RaveEngineID engineID)
	{
	// Set the fields
	( (E3Renderer*) theRenderer )->instanceData.interactiveVendorID = vendorID ;
	( (E3Renderer*) theRenderer )->instanceData.interactiveEngineID = engineID ;
	Q3Shared_Edited ( theRenderer ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_GetPreferences : Get the vendor/engine ID.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetPreferences(TQ3RendererObject theRenderer, TQ3RaveVendorID *vendorID, TQ3RaveEngineID *engineID)
	{
	// Get the fields
	*vendorID = ( (E3Renderer*) theRenderer )->instanceData.interactiveVendorID ;
	*engineID = ( (E3Renderer*) theRenderer )->instanceData.interactiveEngineID ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_SetDoubleBufferBypass : Set the buffer bypass.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_SetDoubleBufferBypass(TQ3RendererObject theRenderer, TQ3Boolean bypass)
	{
	// Set the field
	( (E3Renderer*) theRenderer )->instanceData.interactiveDoubleBufferBypass = bypass ;
	Q3Shared_Edited ( theRenderer ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_GetDoubleBufferBypass : Get the buffer bypass.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetDoubleBufferBypass(TQ3RendererObject theRenderer, TQ3Boolean *bypass)
	{
	// Get the field
	*bypass = ( (E3Renderer*) theRenderer )->instanceData.interactiveDoubleBufferBypass ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_SetRAVEContextHints : Set the RAVE hints.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_SetRAVEContextHints(TQ3RendererObject theRenderer, TQ3Uns32 RAVEContextHints)
	{
	// Set the field
	( (E3Renderer*) theRenderer )->instanceData.raveContextHints = RAVEContextHints ;
	Q3Shared_Edited ( theRenderer ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_GetRAVEContextHints : Get the RAVE hints.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetRAVEContextHints(TQ3RendererObject theRenderer, TQ3Uns32 *RAVEContextHints)
	{
	// Get the field
	*RAVEContextHints = ( (E3Renderer*) theRenderer )->instanceData.raveContextHints ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_SetRAVETextureFilter : Set the RAVE filter.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_SetRAVETextureFilter(TQ3RendererObject theRenderer, TQ3TextureFilter raveTextureFilterValue)
	{
	// Set the field, and flag that we need to reset the draw context state.
	( (E3Renderer*) theRenderer )->instanceData.raveTextureFilter = raveTextureFilterValue ;
	( (E3Renderer*) theRenderer )->instanceData.drawContextReset  = kQ3True ;

	Q3Shared_Edited ( theRenderer ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_GetRAVETextureFilter : Get the RAVE filter.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetRAVETextureFilter(TQ3RendererObject theRenderer, TQ3TextureFilter *raveTextureFilterValue)
	{
	// Get the field
	*raveTextureFilterValue = ( (E3Renderer*) theRenderer )->instanceData.raveTextureFilter ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_CountRAVEDrawContexts : Count RAVE contexts.
//-----------------------------------------------------------------------------
//		Note : Quesa does not support RAVE, so we return 0.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_CountRAVEDrawContexts(TQ3RendererObject theRenderer, TQ3Uns32 *numRAVEContexts)
	{
#pragma unused(theRenderer)



	// RAVE is not required for Quesa
	*numRAVEContexts = 0 ;

	return kQ3Success ;
	}





//=============================================================================
//      E3InteractiveRenderer_GetRAVEDrawContexts : Get RAVE contexts.
//-----------------------------------------------------------------------------
//		Note : Quesa does not support RAVE, so we return NULL.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetRAVEDrawContexts(TQ3RendererObject theRenderer, TQADrawContext **raveDrawContextList, TQAEngine **raveDrawingEnginesList, TQ3Uns32 *numRAVEContexts, TQ3RaveDestroyCallback raveDestroyCallback)
	{
#pragma unused(theRenderer)
#pragma unused(raveDestroyCallback)



	// RAVE is not required for Quesa
	*raveDrawContextList    = NULL ;
	*raveDrawingEnginesList = NULL ;
	*numRAVEContexts        = 0 ;

	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawContext_GetDrawRegion :	Return the first draw region.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3XDrawContext_GetDrawRegion(TQ3DrawContextObject drawContext, TQ3XDrawRegion *drawRegion)
	{
	TQ3DrawContextUnionData* instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the first draw region
	if ( instanceData->numDrawRegions != 0 )
		*drawRegion = &instanceData->drawRegions [ 0 ] ;
	else
		*drawRegion = NULL ;

	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawContext_ClearValidationFlags : Clear the validation flags.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawContext_ClearValidationFlags(TQ3DrawContextObject drawContext)
	{
	TQ3DrawContextUnionData* instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Clear the flags
	instanceData->theState = kQ3XDrawContextValidationClearFlags ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawContext_GetValidationFlags : Return the validation flags.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawContext_GetValidationFlags(TQ3DrawContextObject drawContext, TQ3XDrawContextValidation *validationFlags)
	{
	TQ3DrawContextUnionData* instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the flags
	*validationFlags = instanceData->theState ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetDeviceScaleX : Get the device scale X.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceScaleX(TQ3XDrawRegion drawRegion, float *deviceScaleX)
	{
	// Return the value
	*deviceScaleX = drawRegion->deviceScaleX ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetDeviceScaleY : Get the device scale Y.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceScaleY(TQ3XDrawRegion drawRegion, float *deviceScaleY)
	{
	// Return the value
	*deviceScaleY = drawRegion->deviceScaleY ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetDeviceOffsetX : Get the device offset X.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceOffsetX(TQ3XDrawRegion drawRegion, float *deviceOffsetX)
	{
	// Return the value
	*deviceOffsetX = drawRegion->deviceOffsetX ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetDeviceOffsetY : Get the device offset Y.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceOffsetY(TQ3XDrawRegion drawRegion, float *deviceOffsetY)
	{
	// Return the value
	*deviceOffsetY = drawRegion->deviceOffsetY ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetWindowScaleX : Get the window scale X.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetWindowScaleX(TQ3XDrawRegion drawRegion, float *windowScaleX)
	{
	// Return the value
	*windowScaleX = drawRegion->windowScaleX ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetWindowScaleY : Get the window scale Y.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetWindowScaleY(TQ3XDrawRegion drawRegion, float *windowScaleY)
	{
	// Return the value
	*windowScaleY = drawRegion->windowScaleY ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetWindowOffsetX : Get the window offset X.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetWindowOffsetX(TQ3XDrawRegion drawRegion, float *windowOffsetX)
	{
	// Return the value
	*windowOffsetX = drawRegion->windowOffsetX ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetWindowOffsetY : Get the window offset Y.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetWindowOffsetY(TQ3XDrawRegion drawRegion, float *windowOffsetY)
	{
	// Return the value
	*windowOffsetY = drawRegion->windowOffsetY ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_IsActive : Test if a draw region is active.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_IsActive(TQ3XDrawRegion drawRegion, TQ3Boolean *isActive)
	{
	// Return the value
	*isActive = drawRegion->isActive ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetNextRegion : Get the next draw region.
//-----------------------------------------------------------------------------
//		Note :	Each draw region maintains a pointer back to its owning draw
//				context, along with its index in the draw context's array of
//				draw regions.
//
//				With this information, we can step through the draw regions for
//				a draw context given any arbitrary draw region.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetNextRegion(TQ3XDrawRegion drawRegion, TQ3XDrawRegion *nextDrawRegion)
	{
	// Get the next draw region for the draw context
	if ( drawRegion->ownerIndex == ( drawRegion->theOwner->numDrawRegions - 1 ) )
		*nextDrawRegion = NULL ;
	else
		*nextDrawRegion = &drawRegion->theOwner->drawRegions [ drawRegion->ownerIndex + 1 ] ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_Start : Start access to a draw region.
//-----------------------------------------------------------------------------
//		Note :	The draw region services request flag is currently ignored,
//				since this flag isn't used by the Interactive Renderer.
//
//				This method is called by the Interactive Renderer when drawing
//				to window-based draw contexts.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_Start(TQ3XDrawRegion drawRegion, TQ3XDrawRegionServices services, TQ3XDrawRegionDescriptor **descriptor)
	{
#pragma unused(services)



	// Return the value
	*descriptor = &drawRegion->theDescriptor ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_StartAccessToImageBuffer : Start access to a draw region.
//-----------------------------------------------------------------------------
//		Note :	The draw region services request flag is currently ignored,
//				since this flag isn't used by the Interactive Renderer.
//
//				This method is called by the Interactive Renderer when drawing
//				to Pixmap draw contexts.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_StartAccessToImageBuffer(TQ3XDrawRegion drawRegion, TQ3XDrawRegionServices services, TQ3XDrawRegionDescriptor **descriptor, void **image)
	{
#pragma unused(services)



	// Return the values
	*descriptor = &drawRegion->theDescriptor ;
	*image      =  drawRegion->imageBuffer ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_End : End access to a draw region.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_End(TQ3XDrawRegion drawRegion)
	{
	// Nothing to do
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetDeviceTransform : Get the device transform.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceTransform(TQ3XDrawRegion drawRegion, TQ3Matrix4x4 **deviceTransform)
	{
	// Return the value
	*deviceTransform = &drawRegion->deviceTransform ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetClipFlags : Get the clip flags.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetClipFlags(TQ3XDrawRegion drawRegion, TQ3XClipMaskState *clipMaskState)
	{
	// Return the value
	*clipMaskState = drawRegion->clipMaskState ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetClipMask : Get the clip mask.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetClipMask(TQ3XDrawRegion drawRegion, TQ3Bitmap **clipMask)
	{
	// Return the value
	*clipMask = drawRegion->theDescriptor.clipMask ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetRendererPrivate : Get the renderer private data.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetRendererPrivate(TQ3XDrawRegion drawRegion, void **rendererPrivate)
	{
	// Return the value
	*rendererPrivate = drawRegion->rendererPrivate ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_SetRendererPrivate : Set the renderer private data.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_SetRendererPrivate(TQ3XDrawRegion drawRegion, const void *rendererPrivate, TQ3XDrawRegionRendererPrivateDeleteMethod deleteMethod)
	{
	// Set the values (casting away the const, since the prototype for
	// this function is incorrect: GetRendererPrivate and the deleteMethod
	// both work with 'void *', so we assume that's what was meant)
	drawRegion->rendererPrivate       = (void *) rendererPrivate ;
	drawRegion->rendererPrivateDelete = deleteMethod ;

	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_SetUseDefaultRendererFlag : Set the default flag.
//-----------------------------------------------------------------------------
//		Note : Unable to find any documentation on the purpose of this routine.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_SetUseDefaultRendererFlag(TQ3XDrawRegion drawRegion, TQ3Boolean flag)
	{
	// Set the value
	drawRegion->useDefaultRenderer = flag ;

	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetUseDefaultRendererFlag : Get the default flag.
//-----------------------------------------------------------------------------
//		Note : Unable to find any documentation on the purpose of this routine.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetUseDefaultRendererFlag(TQ3XDrawRegion drawRegion, TQ3Boolean *useDefaultRenderingFlag)
	{
	// Return the value
	*useDefaultRenderingFlag = drawRegion->useDefaultRenderer ;
	return kQ3Success ;
	}





//=============================================================================
//      E3XDrawRegion_GetClipRegion : Get the clip region as a RgnHandle.
//-----------------------------------------------------------------------------
//		Note :	Although it's not indicated in the QD3D docs, it seems that we
//				need to return a new region each time since the Interactive
//				Renderer disposes of the region we return to it.
//
//				Mac specific.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
TQ3Status
E3XDrawRegion_GetClipRegion(TQ3XDrawRegion drawRegion, RgnHandle *rgnHandle)
	{
	// Check for no clipping region
	if ( drawRegion->platformClip == NULL )
		*rgnHandle = NULL ;


	// Return a copy of the clipping region
	else
		{
		*rgnHandle = NewRgn () ;
		if ( *rgnHandle != NULL )
			CopyRgn ( (RgnHandle) drawRegion->platformClip, *rgnHandle ) ;
		}

	return kQ3Success ;
	}
#endif





//=============================================================================
//      E3XDrawRegion_GetGDHandle : Get the GDevice for a draw region.
//-----------------------------------------------------------------------------
//		Note : Mac specific.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
TQ3Status
E3XDrawRegion_GetGDHandle(TQ3XDrawRegion drawRegion, GDHandle *gdHandle)
	{
	// Return the value
	*gdHandle = (GDHandle) drawRegion->platformHandle ;
	return kQ3Success ;
	}
#endif





//=============================================================================
//      E3XView_IdleProgress : Call the idle method for a view.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3XView_IdleProgress(TQ3ViewObject theView, TQ3Uns32 current, TQ3Uns32 completed)
	{
	// Call the view's idle method
	return E3View_CallIdleMethod ( theView, current, completed ) ;
	}





//=============================================================================
//      E3XView_EndFrame : An async renderer has finished.
//-----------------------------------------------------------------------------
TQ3Status
E3XView_EndFrame(TQ3ViewObject theView)
	{
	// Tell the view we've finished
	E3View_EndFrame ( theView ) ;
	
	return kQ3Success ;
	}

