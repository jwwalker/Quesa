/*  NAME:
        E3Renderer.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3View.h"
#include "E3Set.h"
#include "E3DrawContext.h"
#include "E3Renderer.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct {
	// Interactive renderer state (for all interactive renderers)
	TQ3CSGEquation			interactiveCSGEquation;
	TQ3Uns32				interactiveVendorID;
	TQ3Uns32				interactiveEngineID;
	TQ3Boolean				interactiveDoubleBufferBypass;
	TQ3Uns32				raveContextHints;
	TQ3Uns32				raveTextureFilter;
} TQ3RendererData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3renderer_findinstance : Get the instance data for a renderer object.
//-----------------------------------------------------------------------------
//		Note : Given a renderer object, we locate the renderer instance data.
//-----------------------------------------------------------------------------
static TQ3RendererData *
e3renderer_findinstance(TQ3RendererObject theRenderer)
{	TQ3RendererData		*instanceData;
	TQ3RendererObject	theObject;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRenderer), NULL);



	// Find the renderer object for this object. This may in fact be
	// theRenderer, but we need to walk upwards in case it isn't.
	theObject = E3ClassTree_FindParentInstance(theRenderer, kQ3SharedTypeRenderer);
	if (theObject == NULL)
		return(NULL);



	// Get the instance data for the renderer object		
	instanceData = (TQ3RendererData *) theObject->instanceData;
	Q3_ASSERT_VALID_PTR(instanceData);
	Q3_ASSERT_VALID_PTR(E3ClassTree_GetInstanceSize(theObject->theClass) == sizeof(TQ3RendererData));

	return(instanceData);
}





//=============================================================================
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
{	TQ3Uns32										n, numMethods;
	TQ3XRendererUpdateAttributeMetaHandlerMethod	attributeMeta;
	TQ3XRendererUpdateMatrixMetaHandlerMethod		matrixMeta;
	TQ3XRendererSubmitGeometryMetaHandlerMethod		geomMeta;
	TQ3XRendererUpdateShaderMetaHandlerMethod		shaderMeta;
	TQ3XRendererUpdateStyleMetaHandlerMethod		styleMeta;
	TQ3XFunctionPointer								theMethod;

	TQ3XMethodType									matrixMethods[] = {
														kQ3XMethodTypeRendererUpdateMatrixLocalToWorld,
														kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverse,
														kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverseTranspose,
														kQ3XMethodTypeRendererUpdateMatrixLocalToCamera,
														kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum,
														kQ3XMethodTypeRendererUpdateMatrixWorldToFrustum };

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
														kQ3StyleTypeReceiveShadows,
														kQ3StyleTypeHighlight,
														kQ3StyleTypeSubdivision,
														kQ3StyleTypeOrientation,
														kQ3StyleTypePickParts,
														kQ3StyleTypeAntiAlias,
														kQ3StyleTypeFog };



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theRenderer);



	// Add the dummy cached method, so that we don't execute this again. Because
	// we should never be calling this 'method', we use a dummy value as the pointer.
	E3ClassTree_AddMethod(theRenderer->theClass,
						  kQ3XMethodTypeRendererMethodsCached,
						  (TQ3XFunctionPointer) kQ3ObjectTypeQuesa);



	// Add the matrix methods
	matrixMeta = (TQ3XRendererUpdateMatrixMetaHandlerMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
											kQ3XMethodTypeRendererUpdateMatrixMetaHandler);
	if (matrixMeta != NULL)
		{
		numMethods = sizeof(matrixMethods) / sizeof(TQ3ObjectType);
		for (n = 0; n < numMethods; n++)
			{
			theMethod = matrixMeta(matrixMethods[n]);
			if (theMethod != NULL)
				E3ClassTree_AddMethod(theRenderer->theClass, matrixMethods[n], theMethod);
			}
		}



	// Add the geometry methods
	geomMeta = (TQ3XRendererSubmitGeometryMetaHandlerMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
											kQ3XMethodTypeRendererSubmitGeometryMetaHandler);
	if (geomMeta != NULL)
		{
		numMethods = sizeof(geomMethods) / sizeof(TQ3ObjectType);
		for (n = 0; n < numMethods; n++)
			{
			theMethod = geomMeta(geomMethods[n]);
			if (theMethod != NULL)
				E3ClassTree_AddMethod(theRenderer->theClass, geomMethods[n], theMethod);
			}
		}



	// Add the attribute methods
	attributeMeta = (TQ3XRendererUpdateAttributeMetaHandlerMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
											kQ3XMethodTypeRendererUpdateAttributeMetaHandler);
	if (attributeMeta != NULL)
		{
		numMethods = sizeof(attributeMethods) / sizeof(TQ3ObjectType);
		for (n = 0; n < numMethods; n++)
			{
			theMethod = attributeMeta(attributeMethods[n]);
			if (theMethod != NULL)
				E3ClassTree_AddMethod(theRenderer->theClass, attributeMethods[n], theMethod);
			}
		}



	// Add the shader methods
	shaderMeta = (TQ3XRendererUpdateShaderMetaHandlerMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
											kQ3XMethodTypeRendererUpdateShaderMetaHandler);
	if (shaderMeta != NULL)
		{
		numMethods = sizeof(shaderMethods) / sizeof(TQ3ObjectType);
		for (n = 0; n < numMethods; n++)
			{
			theMethod = shaderMeta(shaderMethods[n]);
			if (theMethod != NULL)
				E3ClassTree_AddMethod(theRenderer->theClass, shaderMethods[n], theMethod);
			}
		}



	// Add the style methods
	styleMeta = (TQ3XRendererUpdateStyleMetaHandlerMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
											kQ3XMethodTypeRendererUpdateStyleMetaHandler);
	if (styleMeta != NULL)
		{
		numMethods = sizeof(styleMethods) / sizeof(TQ3ObjectType);
		for (n = 0; n < numMethods; n++)
			{
			theMethod = styleMeta(styleMethods[n]);
			if (theMethod != NULL)
				E3ClassTree_AddMethod(theRenderer->theClass, styleMethods[n], theMethod);
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
{	TQ3Status		qd3dStatus;



	// Register the renderer classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3SharedTypeRenderer,
											kQ3ClassNameRenderer,
											NULL,
											sizeof(TQ3RendererData));

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_UnregisterClass : Unregister the renderer classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the renderer classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeRenderer, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_Method_StartFrame : Call the start frame method.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Renderer_Method_StartFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext)
{	TQ3RendererObject				theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererStartFrameMethod	startFrame;
	TQ3Status						qd3dStatus;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	startFrame = (TQ3XRendererStartFrameMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
										  kQ3XMethodTypeRendererStartFrame);
	// No-op if renderer doesn't implements kQ3XMethodTypeRendererStartFrame
	if (startFrame == NULL)
		return(kQ3Success);



	// Call the method
	qd3dStatus = startFrame(theView, theRenderer->instanceData, theDrawContext);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_Method_StartPass : Call the start pass method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_StartPass(TQ3ViewObject theView, TQ3CameraObject theCamera, TQ3GroupObject theLights)
{	TQ3RendererObject				theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererStartPassMethod		startPass;
	TQ3Status						qd3dStatus;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	startPass = (TQ3XRendererStartPassMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
										  kQ3XMethodTypeRendererStartPass);
	// No-op if renderer doesn't implements kQ3XMethodTypeRendererStartPass
	if (startPass == NULL)
		return(kQ3Success);



	// Call the method
	qd3dStatus = startPass(theView, theRenderer->instanceData, theCamera, theLights);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_Method_EndPass : Call the end pass method.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3Renderer_Method_EndPass(TQ3ViewObject theView)
{	TQ3RendererObject				theRenderer = E3View_AccessRenderer(theView);
	TQ3ViewStatus					viewStatus;
	TQ3XRendererEndPassMethod		endPass;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3ViewStatusDone);



	// Find the method
	endPass = (TQ3XRendererEndPassMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
										  kQ3XMethodTypeRendererEndPass);
	// No-op if renderer doesn't implements kQ3XMethodTypeRendererEndPass
	if (endPass == NULL)
		return(kQ3ViewStatusDone);



	// Call the method
	viewStatus = endPass(theView, theRenderer->instanceData);

	return(viewStatus);
}





//=============================================================================
//      E3Renderer_Method_FlushFrame : Call the flush frame method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_FlushFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext)
{	TQ3RendererObject				theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererFlushFrameMethod	flushFrame;
	TQ3Status						qd3dStatus;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	flushFrame = (TQ3XRendererFlushFrameMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
										  kQ3XMethodTypeRendererFlushFrame);
	if (flushFrame == NULL)
		return(kQ3Failure);



	// Call the method
	qd3dStatus = flushFrame(theView, theRenderer->instanceData, theDrawContext);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_Method_EndFrame : Call the end frame method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_EndFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext)
{	TQ3RendererObject				theRenderer = E3View_AccessRenderer(theView);
	TQ3Status						qd3dStatus;
	TQ3XRendererEndFrameMethod		endFrame;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	endFrame = (TQ3XRendererEndFrameMethod)
					E3ClassTree_GetMethod(theRenderer->theClass,
										  kQ3XMethodTypeRendererEndFrame);
										  
	// No-op if renderer doesn't implements kQ3XMethodTypeRendererEndFrame
	if (endFrame == NULL)
		return(kQ3Success);



	// Call the method
	qd3dStatus = endFrame(theView, theRenderer->instanceData, theDrawContext);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_Method_IsBBoxVisible : Call the bounds visible method.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Renderer_Method_IsBBoxVisible(TQ3ViewObject theView, const TQ3BoundingBox *theBBox)
{	TQ3RendererObject							theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererIsBoundingBoxVisibleMethod		isBoundingBoxVisible;
	TQ3Boolean									isVisible;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3True);



	// Find the method
	isBoundingBoxVisible = (TQ3XRendererIsBoundingBoxVisibleMethod)
							E3ClassTree_GetMethod(theRenderer->theClass,
												  kQ3XMethodTypeRendererIsBoundingBoxVisible);
	if (isBoundingBoxVisible == NULL)
		return(kQ3True);



	// Call the method
	isVisible = isBoundingBoxVisible(theView, theRenderer->instanceData, theBBox);

	return(isVisible);
}





//=============================================================================
//      E3Renderer_Method_UpdateMatrixLocalToWorld : Matrix update method.
//-----------------------------------------------------------------------------
//		Note :	As well as updating the local to world matrix, we also update
//				the corresponding local-to-xxxx methods if the renderer needs
//				them.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateMatrixLocalToWorld(TQ3ViewObject theView, const TQ3Matrix4x4 *theMatrix)
{	TQ3RendererObject					theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererUpdateMatrixMethod		updateMatrixLocalToWorldInverseTranspose;
	TQ3XRendererUpdateMatrixMethod		updateMatrixLocalToWorldInverse;
	TQ3XRendererUpdateMatrixMethod		updateMatrixLocalToWorld;
	TQ3XRendererUpdateMatrixMethod		updateMatrixLocalToFrustum;
	TQ3XRendererUpdateMatrixMethod		updateMatrixLocalToCamera;
	TQ3Status							qd3dStatus = kQ3Success;
	TQ3Boolean							haveInverse;
	TQ3CameraObject						theCamera;
	TQ3Matrix4x4						tmpMatrix;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the methods
	updateMatrixLocalToWorld = (TQ3XRendererUpdateMatrixMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
					  				kQ3XMethodTypeRendererUpdateMatrixLocalToWorld);

	updateMatrixLocalToWorldInverse = (TQ3XRendererUpdateMatrixMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
					  				kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverse);

	updateMatrixLocalToWorldInverseTranspose = (TQ3XRendererUpdateMatrixMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
					  				kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverseTranspose);

	updateMatrixLocalToFrustum = (TQ3XRendererUpdateMatrixMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
					  				kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum);

	updateMatrixLocalToCamera = (TQ3XRendererUpdateMatrixMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
					  				kQ3XMethodTypeRendererUpdateMatrixLocalToCamera);



	// Call the local to world method
	if (updateMatrixLocalToWorld != NULL)
		{
		qd3dStatus = updateMatrixLocalToWorld(theView, theRenderer->instanceData, theMatrix);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		}



	// Call the inverse method
	haveInverse = kQ3False;
	if (updateMatrixLocalToWorldInverse != NULL)
		{
		// Calculate the matrix
		haveInverse = kQ3True;
		Q3Matrix4x4_Invert(theMatrix, &tmpMatrix);
		
		
		// Call the method
		qd3dStatus = updateMatrixLocalToWorldInverse(theView, theRenderer->instanceData, &tmpMatrix);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		}



	// Call the inverse transpose method
	if (updateMatrixLocalToWorldInverseTranspose != NULL)
		{
		// Calculate the matrix
		if (!haveInverse)
			Q3Matrix4x4_Invert(theMatrix, &tmpMatrix);

		Q3Matrix4x4_Transpose(&tmpMatrix, &tmpMatrix);
		
		
		// Call the method
		qd3dStatus = updateMatrixLocalToWorldInverseTranspose(theView, theRenderer->instanceData, &tmpMatrix);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		}



	// Call the local to frustum method
	if (updateMatrixLocalToFrustum != NULL)
		{
		// Calculate the matrix
		qd3dStatus = E3View_GetWorldToFrustumMatrixState(theView, &tmpMatrix);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);

		Q3Matrix4x4_Multiply(theMatrix, &tmpMatrix, &tmpMatrix);


		// Call the method
		qd3dStatus = updateMatrixLocalToFrustum(theView, theRenderer->instanceData, &tmpMatrix);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		}



	// Call the local to camera method
	if (updateMatrixLocalToCamera != NULL)
		{
		// Get the camera
		theCamera = E3View_AccessCamera(theView);
		Q3_ASSERT_VALID_PTR(theCamera);


		// Calculate the matrix
		qd3dStatus = Q3Camera_GetWorldToView(theCamera, &tmpMatrix);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);

		Q3Matrix4x4_Multiply(theMatrix, &tmpMatrix, &tmpMatrix);



		// Call the method
		qd3dStatus = updateMatrixLocalToCamera(theView, theRenderer->instanceData, &tmpMatrix);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		}

	return(qd3dStatus);
}



//=============================================================================
//      E3Renderer_Method_UpdateMatrixWorldToFrustum : Matrix update method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateMatrixWorldToFrustum(TQ3ViewObject theView, const TQ3Matrix4x4 *theMatrix)
{	TQ3RendererObject					theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererUpdateMatrixMethod		updateMatrixWorldToFrustum;

	TQ3Status							qd3dStatus = kQ3Success;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	updateMatrixWorldToFrustum = (TQ3XRendererUpdateMatrixMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
					  				kQ3XMethodTypeRendererUpdateMatrixWorldToFrustum);



	// Call the method
	if (updateMatrixWorldToFrustum != NULL)
		{

		qd3dStatus = updateMatrixWorldToFrustum(theView, theRenderer->instanceData, theMatrix);
		}
	return(qd3dStatus);

}





//=============================================================================
//      E3Renderer_Method_UpdateShader : Shader update method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateShader(TQ3ViewObject theView, TQ3ObjectType shaderType, TQ3Object *theShader)
{	TQ3RendererObject					theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererUpdateShaderMethod		updateMethod;
	TQ3Status							qd3dStatus;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	updateMethod = (TQ3XRendererUpdateShaderMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
													  shaderType);
	if (updateMethod == NULL)
		return(kQ3Success);



	// Call the method
	qd3dStatus = updateMethod(theView, theRenderer->instanceData, theShader);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_Method_UpdateStyle : Style update method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateStyle(TQ3ViewObject theView, TQ3ObjectType styleType, const void *paramData)
{	TQ3RendererObject					theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererUpdateStyleMethod		updateMethod;
	TQ3Status							qd3dStatus;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	updateMethod = (TQ3XRendererUpdateStyleMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
													  styleType);
	if (updateMethod == NULL)
		return(kQ3Success);



	// Call the method
	qd3dStatus = updateMethod(theView, theRenderer->instanceData, paramData);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_Method_UpdateAttribute : Attribute update method.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_Method_UpdateAttribute(TQ3ViewObject theView, TQ3AttributeType attributeType, const void *paramData)
{	TQ3RendererObject					theRenderer = E3View_AccessRenderer(theView);
	TQ3XRendererUpdateAttributeMethod	updateMethod;
	TQ3Status							qd3dStatus;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	updateMethod = (TQ3XRendererUpdateAttributeMethod)
								E3ClassTree_GetMethod(theRenderer->theClass,
													  attributeType);
	if (updateMethod == NULL)
		return(kQ3Success);



	// Call the method
	qd3dStatus = updateMethod(theView, theRenderer->instanceData, paramData);

	return(qd3dStatus);
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
{	TQ3RendererObject						theRenderer = E3View_AccessRenderer(theView);
	TQ3Status								qd3dStatus  = kQ3Failure;
	TQ3XRendererSubmitGeometryMethod		submitGeom;



	// No-op if no renderer set
	if (theRenderer == NULL)
		return(kQ3Success);



	// Find the method
	submitGeom = (TQ3XRendererSubmitGeometryMethod)
					E3ClassTree_GetMethod(theRenderer->theClass, geomType);



	// Indicate if the geometry is supported or not
	*geomSupported = (TQ3Boolean) (submitGeom != NULL);



	// Call the method
	if (submitGeom != NULL)
		qd3dStatus = submitGeom(theView, theRenderer->instanceData, theGeom, geomData);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_NewFromType : Create a new renderer object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3RendererObject
E3Renderer_NewFromType(TQ3ObjectType rendererObjectType)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(rendererObjectType, kQ3False, NULL);



	// If we've not yet cached this renderer's methods, do so now
	if (theObject != NULL && E3ClassTree_GetMethod(theObject->theClass, kQ3XMethodTypeRendererMethodsCached) == NULL)
		e3renderer_add_methods(theObject);
	
	return(theObject);
}





//=============================================================================
//      E3Renderer_GetType : Return the type of a renderer.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Renderer_GetType(TQ3RendererObject theRenderer)
{


	// Return the type
	return(E3ClassTree_GetObjectType(theRenderer, kQ3SharedTypeRenderer));
}





//=============================================================================
//      E3Renderer_IsInteractive : Is a renderer interactive?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Renderer_IsInteractive(TQ3RendererObject theRenderer)
{


	// Return as the method is defined or not
	return((TQ3Boolean) E3ClassTree_GetMethod(theRenderer->theClass,
										 		kQ3XMethodTypeRendererIsInteractive));
}





//=============================================================================
//      E3Renderer_HasModalConfigure : Does a renderer have a configure dialog?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Renderer_HasModalConfigure(TQ3RendererObject theRenderer)
{


	// Return as the method is defined or not
	return((TQ3Boolean) (E3ClassTree_GetMethod(theRenderer->theClass,
										 		kQ3XMethodTypeRendererModalConfigure) != NULL));
}





//=============================================================================
//      E3Renderer_ModalConfigure : Invoke a renderer's modal configure dialog.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_ModalConfigure(TQ3RendererObject theRenderer, TQ3DialogAnchor dialogAnchor, TQ3Boolean *cancelled)
{	TQ3XRendererModalConfigureMethod	modalConfigure;
	TQ3Status							qd3dStatus;



	// Find the method
	modalConfigure = (TQ3XRendererModalConfigureMethod)
						E3ClassTree_GetMethod(theRenderer->theClass,
											  kQ3XMethodTypeRendererModalConfigure);
	if (modalConfigure == NULL)
		return(kQ3Failure);



	// Call the method
	qd3dStatus = modalConfigure(theRenderer, dialogAnchor, cancelled, theRenderer->instanceData);

	return(qd3dStatus);
}





//=============================================================================
//      E3RendererClass_GetNickNameString : Get the renderer nick name.
//-----------------------------------------------------------------------------
TQ3Status
E3RendererClass_GetNickNameString(TQ3ObjectType rendererClassType, TQ3ObjectClassNameString rendererClassString)
{	TQ3XRendererGetNickNameStringMethod		nickNameMethod;
	E3ClassInfoPtr							rendererClass;
	TQ3Status								qd3dStatus;
	TQ3Uns32								numWritten;
	


	// Initialise a return value
	rendererClassString[0] = 0x00;



	// Find the renderer class, and get the method
	rendererClass = E3ClassTree_GetClassByType(rendererClassType);
	if (rendererClass == NULL)
		return(kQ3Failure);

	nickNameMethod = (TQ3XRendererGetNickNameStringMethod)
								E3ClassTree_GetMethod(rendererClass,
													  kQ3XMethodTypeRendererGetNickNameString);
	if (nickNameMethod == NULL)
		return(kQ3Failure);



	// Call the method
	qd3dStatus = nickNameMethod((unsigned char *) rendererClassString,
									sizeof(TQ3ObjectClassNameString),
									&numWritten);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_GetConfigurationData : Get a renderer's config data.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_GetConfigurationData(TQ3RendererObject theRenderer, unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{	TQ3XRendererGetConfigurationDataMethod	getConfigData;
	TQ3Status								qd3dStatus;



	// Initialise a return value
	*actualDataSize = 0;



	// Find the method
	getConfigData = (TQ3XRendererGetConfigurationDataMethod)
						E3ClassTree_GetMethod(theRenderer->theClass,
											  kQ3XMethodTypeRendererGetConfigurationData);
	if (getConfigData == NULL)
		return(kQ3Failure);



	// Call the method
	qd3dStatus = getConfigData(theRenderer,
								dataBuffer,
								bufferSize,
								actualDataSize,
								theRenderer->instanceData);

	return(qd3dStatus);
}





//=============================================================================
//      E3Renderer_SetConfigurationData : Set a renderer's config data.
//-----------------------------------------------------------------------------
TQ3Status
E3Renderer_SetConfigurationData(TQ3RendererObject theRenderer, unsigned char *dataBuffer, TQ3Uns32 bufferSize)
{	TQ3XRendererSetConfigurationDataMethod	setConfigData;
	TQ3Status								qd3dStatus;



	// Find the method
	setConfigData = (TQ3XRendererSetConfigurationDataMethod)
						E3ClassTree_GetMethod(theRenderer->theClass,
											  kQ3XMethodTypeRendererSetConfigurationData);
	if (setConfigData == NULL)
		return(kQ3Failure);



	// Call the method
	qd3dStatus = setConfigData(theRenderer,
								dataBuffer,
								bufferSize,
								theRenderer->instanceData);

	Q3Shared_Edited(theRenderer);

	return(qd3dStatus);
}





//=============================================================================
//      E3InteractiveRenderer_SetCSGEquation : Set the CSG equation.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3InteractiveRenderer_SetCSGEquation(TQ3RendererObject theRenderer, TQ3CSGEquation equation)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Set the field
	instanceData->interactiveCSGEquation = equation;
	Q3Shared_Edited(theRenderer);
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_GetCSGEquation : Get the CSG equation.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetCSGEquation(TQ3RendererObject theRenderer, TQ3CSGEquation *equation)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Get the field
	*equation = instanceData->interactiveCSGEquation;
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_SetPreferences : Set the vendor/engine ID.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_SetPreferences(TQ3RendererObject theRenderer, TQ3Int32 vendorID, TQ3Int32 engineID)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Set the fields
	instanceData->interactiveVendorID = vendorID;
	instanceData->interactiveEngineID = engineID;
	Q3Shared_Edited(theRenderer);
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_GetPreferences : Get the vendor/engine ID.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetPreferences(TQ3RendererObject theRenderer, TQ3Int32 *vendorID, TQ3Int32 *engineID)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Get the fields
	*vendorID = instanceData->interactiveVendorID;
	*engineID = instanceData->interactiveEngineID;
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_SetDoubleBufferBypass : Set the buffer bypass.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_SetDoubleBufferBypass(TQ3RendererObject theRenderer, TQ3Boolean bypass)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Set the field
	instanceData->interactiveDoubleBufferBypass = bypass;
	Q3Shared_Edited(theRenderer);
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_GetDoubleBufferBypass : Get the buffer bypass.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetDoubleBufferBypass(TQ3RendererObject theRenderer, TQ3Boolean *bypass)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Get the field
	*bypass = instanceData->interactiveDoubleBufferBypass;
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_SetRAVEContextHints : Set the RAVE hints.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_SetRAVEContextHints(TQ3RendererObject theRenderer, TQ3Uns32 RAVEContextHints)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Set the field
	instanceData->raveContextHints = RAVEContextHints;
	Q3Shared_Edited(theRenderer);
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_GetRAVEContextHints : Get the RAVE hints.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetRAVEContextHints(TQ3RendererObject theRenderer, TQ3Uns32 *RAVEContextHints)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Get the field
	*RAVEContextHints = instanceData->raveContextHints;
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_SetRAVETextureFilter : Set the RAVE filter.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_SetRAVETextureFilter(TQ3RendererObject theRenderer, TQ3Uns32 RAVEtextureFilterValue)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Set the field
	instanceData->raveTextureFilter = RAVEtextureFilterValue;
	Q3Shared_Edited(theRenderer);
	
	return(kQ3Success);
}





//=============================================================================
//      E3InteractiveRenderer_GetRAVETextureFilter : Get the RAVE filter.
//-----------------------------------------------------------------------------
TQ3Status
E3InteractiveRenderer_GetRAVETextureFilter(TQ3RendererObject theRenderer, TQ3Uns32 *RAVEtextureFilterValue)
{	TQ3RendererData		*instanceData = e3renderer_findinstance(theRenderer);



	// Get the field
	*RAVEtextureFilterValue = instanceData->raveTextureFilter;
	
	return(kQ3Success);
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
	*numRAVEContexts = 0;

	return(kQ3Success);
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
	*raveDrawContextList    = NULL;
	*raveDrawingEnginesList = NULL;
	*numRAVEContexts        = 0;

	return(kQ3Success);
}





//=============================================================================
//      E3XDrawContext_GetDrawRegion :	Return the first draw region.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3XDrawContext_GetDrawRegion(TQ3DrawContextObject drawContext, TQ3XDrawRegion *drawRegion)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the first draw region
	if (instanceData->numDrawRegions != 0)
		*drawRegion = &instanceData->drawRegions[0];
	else
		*drawRegion = NULL;

	return(kQ3Success);
}





//=============================================================================
//      E3XDrawContext_ClearValidationFlags : Clear the validation flags.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawContext_ClearValidationFlags(TQ3DrawContextObject drawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Clear the flags
	instanceData->theState = kQ3XDrawContextValidationClearFlags;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawContext_GetValidationFlags : Return the validation flags.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawContext_GetValidationFlags(TQ3DrawContextObject drawContext, TQ3XDrawContextValidation *validationFlags)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the flags
	*validationFlags = instanceData->theState;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetDeviceScaleX : Get the device scale X.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceScaleX(TQ3XDrawRegion drawRegion, float *deviceScaleX)
{


	// Return the value
	*deviceScaleX = drawRegion->deviceScaleX;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetDeviceScaleY : Get the device scale Y.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceScaleY(TQ3XDrawRegion drawRegion, float *deviceScaleY)
{


	// Return the value
	*deviceScaleY = drawRegion->deviceScaleY;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetDeviceOffsetX : Get the device offset X.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceOffsetX(TQ3XDrawRegion drawRegion, float *deviceOffsetX)
{


	// Return the value
	*deviceOffsetX = drawRegion->deviceOffsetX;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetDeviceOffsetY : Get the device offset Y.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceOffsetY(TQ3XDrawRegion drawRegion, float *deviceOffsetY)
{


	// Return the value
	*deviceOffsetY = drawRegion->deviceOffsetY;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetWindowScaleX : Get the window scale X.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetWindowScaleX(TQ3XDrawRegion drawRegion, float *windowScaleX)
{


	// Return the value
	*windowScaleX = drawRegion->windowScaleX;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetWindowScaleY : Get the window scale Y.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetWindowScaleY(TQ3XDrawRegion drawRegion, float *windowScaleY)
{


	// Return the value
	*windowScaleY = drawRegion->windowScaleY;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetWindowOffsetX : Get the window offset X.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetWindowOffsetX(TQ3XDrawRegion drawRegion, float *windowOffsetX)
{


	// Return the value
	*windowOffsetX = drawRegion->windowOffsetX;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetWindowOffsetY : Get the window offset Y.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetWindowOffsetY(TQ3XDrawRegion drawRegion, float *windowOffsetY)
{


	// Return the value
	*windowOffsetY = drawRegion->windowOffsetY;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_IsActive : Test if a draw region is active.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_IsActive(TQ3XDrawRegion drawRegion, TQ3Boolean *isActive)
{


	// Return the value
	*isActive = drawRegion->isActive;
	return(kQ3Success);
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
	if (drawRegion->ownerIndex == (drawRegion->theOwner->numDrawRegions-1))
		*nextDrawRegion = NULL;
	else
		*nextDrawRegion = &drawRegion->theOwner->drawRegions[drawRegion->ownerIndex+1];
	
	return(kQ3Success);
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
	*descriptor = &drawRegion->theDescriptor;
	return(kQ3Success);
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
	*descriptor = &drawRegion->theDescriptor;
	*image      =  drawRegion->imageBuffer;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_End : End access to a draw region.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_End(TQ3XDrawRegion drawRegion)
{


	// Nothing to do
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetDeviceTransform : Get the device transform.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetDeviceTransform(TQ3XDrawRegion drawRegion, TQ3Matrix4x4 **deviceTransform)
{


	// Return the value
	*deviceTransform = &drawRegion->deviceTransform;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetClipFlags : Get the clip flags.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetClipFlags(TQ3XDrawRegion drawRegion, TQ3XClipMaskState *clipMaskState)
{


	// Return the value
	*clipMaskState = drawRegion->clipMaskState;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetClipMask : Get the clip mask.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetClipMask(TQ3XDrawRegion drawRegion, TQ3Bitmap **clipMask)
{


	// Return the value
	*clipMask = drawRegion->theDescriptor.clipMask;
	return(kQ3Success);
}





//=============================================================================
//      E3XDrawRegion_GetRendererPrivate : Get the renderer private data.
//-----------------------------------------------------------------------------
TQ3Status
E3XDrawRegion_GetRendererPrivate(TQ3XDrawRegion drawRegion, void **rendererPrivate)
{


	// Return the value
	*rendererPrivate = drawRegion->rendererPrivate;
	return(kQ3Success);
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
	drawRegion->rendererPrivate       = (void *) rendererPrivate;
	drawRegion->rendererPrivateDelete = deleteMethod;

	return(kQ3Success);
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
	drawRegion->useDefaultRenderer = flag;

	return(kQ3Success);
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
	*useDefaultRenderingFlag = drawRegion->useDefaultRenderer;
	return(kQ3Success);
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
	if (drawRegion->platformClip == NULL)
		*rgnHandle = NULL;


	// Return a copy of the clipping region
	else
		{
		*rgnHandle = NewRgn();
		if (*rgnHandle != NULL)
			CopyRgn((RgnHandle) drawRegion->platformClip, *rgnHandle);
		}

	return(kQ3Success);
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
	*gdHandle = (GDHandle) drawRegion->platformHandle;
	return(kQ3Success);
}
#endif





//=============================================================================
//      E3XView_IdleProgress : Call the idle method for a view.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3XView_IdleProgress(TQ3ViewObject theView, TQ3Uns32 current, TQ3Uns32 completed)
{	TQ3Status		qd3dStatus;



	// Call the view's idle method
	qd3dStatus = E3View_CallIdleMethod(theView, current, completed);
	return(qd3dStatus);
}





//=============================================================================
//      E3XView_EndFrame : An async renderer has finished.
//-----------------------------------------------------------------------------
TQ3Status
E3XView_EndFrame(TQ3ViewObject theView)
{


	// Tell the view we've finished
	E3View_EndFrame(theView);
	
	return(kQ3Success);
}





//=============================================================================
//      E3XAttributeSet_GetPointer : Get a pointer to an attribute's data.
//-----------------------------------------------------------------------------
void *
E3XAttributeSet_GetPointer(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType)
{	TQ3Status		qd3dStatus;
	TQ3Uns32		dataSize;
	void			*theData;



	// Get the size and pointer for the data for the attribute
	attributeType = E3Attribute_AttributeToClassType(attributeType);
	qd3dStatus    = E3Set_AccessElementData(attributeSet, attributeType, &dataSize, &theData);
	if (qd3dStatus != kQ3Success)
		return(NULL);



	// Return the data for the attribute
	return(theData);
}





//=============================================================================
//      E3XAttributeSet_GetMask : Return the mask of attributes in a set.
//-----------------------------------------------------------------------------
TQ3XAttributeMask
E3XAttributeSet_GetMask(TQ3AttributeSet attributeSet)
{	TQ3XAttributeMask	theMask;



	// Return the mask for the attribute set
	if (attributeSet != NULL)
		theMask = E3AttributeSet_AccessMask(attributeSet);
	else
		theMask = kQ3AttributeTypeNone;

	return(theMask);
}


