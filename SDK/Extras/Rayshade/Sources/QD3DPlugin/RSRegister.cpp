/*  NAME:
        RSRegister.c

    DESCRIPTION:
        RayShade renderer registration

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
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#include "RSPlugin.h"
#include "RS_Rasterize.h"

#include "RT_Light.h"
#include "RT_Tracer.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaView.h>
	#include <QuesaDrawContext.h>
	#include <QuesaGroup.h>
	#include <QuesaCamera.h>
	#include <QuesaRenderer.h>
	#include <QuesaExtension.h>
	#include <QuesaIO.h>
#else
	#include <QD3D.h>
	#include <QD3DView.h>
	#include <QD3DDrawContext.h>
	#include <QD3DGroup.h>
	#include <QD3DCamera.h>
	#include <QD3DRenderer.h>
	#include <QD3DExtension.h>
	#include <QD3DIO.h>
#endif

#include <string.h>

#if defined(macintosh)

#include <CodeFragments.h>
#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Shared library initialization entry point
 */
OSErr RS_Initialize( 
	const CFragInitBlock	*initBlock);
/*
 *  Shared library exit function
 */
TQ3Status RS_Exit( 
	void);
	
#ifdef __cplusplus
}
#endif

#endif /* WINDOW_SYSTEM_MACINTOSH */

#if defined(WIN32)
#include <Windows.h>
/*
 * Shared library entry function
 */
BOOL __stdcall DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 );
/*
 * Shared library exit function
 */
__declspec(dllexport) TQ3Status RS_Exit (
	void);
#endif


/******************************************************************************
 **																			 **
 **									Constants								 **
 **																			 **
 *****************************************************************************/
const char* kRSRenderer_NickName				= "RayShadeRenderer";

/******************************************************************************
 **																			 **
 **									Globals									 **
 **																			 **
 *****************************************************************************/
static TQ3XObjectClass	gRS_RendererClass;
static TQ3ObjectType	gRS_ClassType;
static unsigned long 	gRS_SharedLibrary = NULL;

/******************************************************************************
 **																			 **
 **							Rayshade Renderer routines						 **
 **																			 **
 *****************************************************************************/
 
/*===========================================================================*\
 *
 *	Routine:	RS_New()
 *
 *	Comments:	Returns the renderer's Nick Name
 *
\*===========================================================================*/
static 
TQ3Status RS_GetNameString(
	unsigned char				*dataBuffer, 
	unsigned long				bufferSize,
	unsigned long				*actualDataSize)
{
	// Return the amount of space we need
	*actualDataSize = strlen(kRSRenderer_NickName)+1;
	
	// If we have a buffer, return the nick name
	if (dataBuffer != NULL)
	{
		// Clamp the buffer size
		if (bufferSize < *actualDataSize)
			*actualDataSize = bufferSize;
		// Return the string
		memcpy(dataBuffer, kRSRenderer_NickName, (*actualDataSize)-1);
    	dataBuffer[(*actualDataSize)-1] = 0x00;
	}
	
    return kQ3Success;
}

/*===========================================================================*\
 *
 *	Routine:	RS_New()
 *
 *	Comments:	Initialize the private state.
 *
\*===========================================================================*/

static TQ3Status RS_New(
	TQ3RendererObject 		renderer,
	TRSRendererPrivate		*rsPrivate,
	void					*initData)
{
	Q3_UNUSED(renderer);
	Q3_UNUSED(initData);
	
	Q3_ASSERT(renderer != NULL);
	Q3_ASSERT(rsPrivate != NULL);

	/*
	 *  Zero everything out.
	 */
	memset(rsPrivate, 0, sizeof(*rsPrivate));
	
	rsPrivate->raytracer = RT_Create();
	if (!rsPrivate->raytracer)
		return kQ3Failure;
	
	RTSurfaceData_Init(&rsPrivate->surfaceCache);
	rsPrivate->isCurrentSurface = kQ3False;

	return kQ3Success;
}

/*===========================================================================*\
 *
 *	Routine:	RS_Delete()
 *
 *	Comments:	Get rid of anything we have a reference to.
 *
\*===========================================================================*/

static void RS_Delete(
	TQ3RendererObject	renderer,
	TRSRendererPrivate	*rsPrivate)
{
	Q3_UNUSED(renderer);
	Q3_UNUSED(rsPrivate);
	
	Q3_ASSERT(renderer != NULL);
	Q3_ASSERT(rsPrivate != NULL);
	
	RT_Delete(rsPrivate->raytracer);
}

/*===========================================================================*\
 *
 *	Routine:	SR_StartFrame()
 *
 *	Comments:	Called by the View at the start of the frame, set up
 *				the initial data.
 *
\*===========================================================================*/
static TQ3Status SR_StartFrame(
		TQ3ViewObject			pView,
		TRSRendererPrivate 		*rsPrivate,
		TQ3DrawContextObject	drawContext)
{
	TQ3Status	theStatus;
	pView; drawContext;
	
#if 0	
	if ( Q3View_GetDrawContext ( pView , &drawContext ) != kQ3Failure )
	{
		TQ3DrawContextData theData ;
		if ( Q3DrawContext_GetData ( drawContext , &theData ) != kQ3Failure )
		{		
			{
				CWindowPtr theWindow ;
				TQ3Area		theArea;
				Rect		theRect;

				theArea = theData.pane;

				theRect.left = theArea.min.x;
				theRect.right = theArea.max.x;
				theRect.top = theArea.min.y;
				theRect.bottom = theArea.max.y;
				if ( Q3MacDrawContext_GetWindow ( drawContext , &theWindow ) != kQ3Failure )
				{
					SetPort ( GetWindowGrafPort ( theWindow ) ) ;
					EraseRect ( &R ) ; 
				}
			}
		}
    	Q3Object_Dispose(theDrawContext);
	}
#endif
	
	/*
	 * Initialize the Rayshade renderer
	 */
	theStatus= RT_BeginScene(rsPrivate->raytracer);
	return theStatus;
}
/*===========================================================================*\
 *
 *	Routine:	SR_SetCamera()
 *
 *	Comments:	Converts the QuickDraw3D's camera to RayShade.
 *
\*===========================================================================*/
static 
TQ3Status RS_SetCamera(
	TRSRendererPrivate		*rsPrivate,
	TQ3CameraObject			inCamera)
{
	TQ3ViewAngleAspectCameraData	angleAspectData;
	TQ3ObjectType					cameraType;
	
	cameraType = Q3Camera_GetType(inCamera);
	switch(cameraType) {
		case kQ3CameraTypeViewAngleAspect:
			Q3ViewAngleAspectCamera_GetData(inCamera, &angleAspectData);
			RT_SetCamera(
				rsPrivate->raytracer,
				&angleAspectData.cameraData.placement.cameraLocation,
				&angleAspectData.cameraData.placement.pointOfInterest,
				&angleAspectData.cameraData.placement.upVector,
				angleAspectData.fov,
				angleAspectData.aspectRatioXToY);
			break;
	}
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	SR_SetLights()
 *
 *	Comments:	Converts the QuickDraw3D's light group to RayShade lights.
 *
\*===========================================================================*/
static 
TQ3Status RS_SetLights(
	TRSRendererPrivate		*rsPrivate,
	TQ3GroupObject			inLightGroup)
{
	TQ3GroupPosition        lightPos;
    TQ3Status               theStatus;
    
    lightPos = NULL;
    theStatus = Q3Group_GetFirstPosition(inLightGroup,&lightPos);
    while ( theStatus == kQ3Success && 
            lightPos != NULL)	
    {
         TQ3Boolean 		isOn;
         TQ3LightObject 	theLight;
         
         theStatus = Q3Group_GetPositionObject(inLightGroup, lightPos, &theLight);
         theStatus = Q3Light_GetState(theLight, &isOn);   
         
         if (isOn)
         {
         	TQ3LightData   theLightData;
         	Q3Light_GetData(theLight,&theLightData);
         	
         	switch(Q3Light_GetType(theLight))
            {
            	case kQ3LightTypeAmbient:
            		RT_AddAmbientLight(
            					rsPrivate->raytracer,
            					&theLightData.color,
            					theLightData.brightness);
            		break;
            	case kQ3LightTypeDirectional:
            		{
                        TQ3DirectionalLightData theDirectionalLightData;
                        
                        Q3DirectionalLight_GetData(theLight,&theDirectionalLightData);
                        RT_AddDirectionalLight(
                        		rsPrivate->raytracer,
                                &theDirectionalLightData.direction,
                                theDirectionalLightData.castsShadows,
                                &theLightData.color,
                                theLightData.brightness);
                    }
                    break;
                 case kQ3LightTypePoint:
                    {
                        TQ3PointLightData thePointLightData;
                        
                        Q3PointLight_GetData(theLight,&thePointLightData);
                        RT_AddPointLight(
                        		rsPrivate->raytracer,
                                &thePointLightData.location,
                                thePointLightData.castsShadows,
                                &theLightData.color,
                                theLightData.brightness);
                    }
                    break;
                  case kQ3LightTypeSpot:
                    {
                        TQ3SpotLightData theSpotLightData;
                        
                        Q3SpotLight_GetData(theLight,&theSpotLightData);
                        RT_AddSpotLight(
                        		rsPrivate->raytracer,
                                &theSpotLightData.location,
                                &theSpotLightData.direction,
                                theSpotLightData.hotAngle,
                                theSpotLightData.outerAngle,
                                theSpotLightData.fallOff,
                                theSpotLightData.castsShadows,
                                &theLightData.color,
                                theLightData.brightness);
                    }
                    break;
            }
         }
         // Get the next light
         Q3Object_Dispose(theLight);
         
         theStatus = Q3Group_GetNextPosition(inLightGroup, &lightPos);         	
    }
    return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	SR_StartPass()
 *
 *	Comments:	Called at the start of each pass.
 *
\*===========================================================================*/
static
TQ3Status RS_StartPass(
	TQ3ViewObject			/*pView*/,
	TRSRendererPrivate		*rsPrivate,
	TQ3CameraObject			pCamera,
	TQ3GroupObject			pLightGroup)
{
	TQ3Status theStatus;
    /*
     * Process camera information:
     */
    theStatus = RS_SetCamera(rsPrivate,pCamera);
    if (theStatus != kQ3Success)
    	return kQ3Failure;
    /*
     * Add lights:
     */
    theStatus = RS_SetLights(rsPrivate,pLightGroup);
    if (theStatus != kQ3Success)
    	return kQ3Failure;
    
    return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	SR_EndPass()
 *
 *	Comments:	Called by the view at the end of the pass. 
 *
\*===========================================================================*/
static
TQ3ViewStatus RS_EndPass(
	TQ3ViewObject			pView,
	TRSRendererPrivate		*rsPrivate)
{
	TRTRayTracer 	*theTracer = NULL;
	TQ3ViewStatus	result = kQ3ViewStatusDone;
	TQ3Status 		theStatus;
	TRSRasterizer	*theRasterizer = NULL;
	/*
	 * Do the raytracing:
	 */
    TQ3DrawContextObject theDrawContext = NULL;
    
    theStatus = RT_EndScene(rsPrivate->raytracer);
    if (theStatus != kQ3Success)
    {
	    result = kQ3ViewStatusError;
		goto cleanup;    	
    }
    
    if ( Q3View_GetDrawContext ( pView , &theDrawContext ) != kQ3Failure )
    {
    	int width,height;
    	
    	theRasterizer = RSRasterizer_Create(theDrawContext);
    	if (theRasterizer == NULL)
    	{
    		result = kQ3ViewStatusError;
    		goto cleanup;
    	}
    	
    	theStatus = RSRasterizer_GetSize(theRasterizer,&width,&height);
    	if (theStatus != kQ3Success)
    	{
    		result = kQ3ViewStatusError;
    		goto cleanup;
    	}
    	
    	theTracer = RTRayTracer_Create(	rsPrivate->raytracer,
									width,
									height);
		if (!theTracer)
		{
			result = kQ3ViewStatusError;
			goto cleanup;
		}
		
		if (theTracer)
		{
			int xres = width;
	        int yres = height;
	        TQ3Uns8 buf[2048][3];
	                
	        for (int i = 0; i < yres; i++)
	        {
	        	/*
	        	 * RayTrace the next scan line
	        	 */
	        	if (RTRayTracer_ScanNextLine(theTracer,i,buf,sizeof(buf)) != kQ3Success)
	            {
	                result = kQ3ViewStatusError;
	                goto cleanup;
	            }
	            /*
	             * Display the scanline
	             */
	            theStatus = RSRasterizer_Lock(theRasterizer);
            	if (theStatus != kQ3Success)
            	{
            		result = kQ3ViewStatusError;	
            		goto cleanup;
            	}
	            RSRasterizer_Rasterize_RGB_Span(theRasterizer,0,i,width,buf);
           		RSResterizer_Unlock(theRasterizer);

         		/*
         		 * Call user supplied idle method and cancel if the user 
         		 * wants to.
         		 */
                if (Q3XView_IdleProgress(pView,i,yres) != kQ3Success)
                {
            		result = kQ3ViewStatusCancelled;
            		goto cleanup;
            	}
            	
            }
            RSRasterizer_Delete(theRasterizer);
            theRasterizer = NULL;
            RTRayTracer_Delete(theTracer);
            theTracer = NULL;
            RT_Reset(rsPrivate->raytracer);
            
		}
    	Q3Object_Dispose(theDrawContext);
    }
	return kQ3ViewStatusDone;
	
cleanup:
	if (theRasterizer)
		RSRasterizer_Delete(theRasterizer);
	if (theDrawContext)
		Q3Object_Dispose(theDrawContext);
	if (theTracer)
		RTRayTracer_Delete(theTracer);
	RT_Reset(rsPrivate->raytracer);
	return result;
}
/*===========================================================================*\
 *
 *	Routine:	RS_Cancel()
 *
 *	Comments:	Called when the view is cancelled. 
 *
\*===========================================================================*/
static void RS_Cancel(
	TQ3ViewObject 			/*view*/,
	TRSRendererPrivate		*rsPrivate)
{
	RT_Reset(rsPrivate->raytracer);
}
/*===========================================================================*\
 *
 *	Routine:	RS_Geometry_MetaHandler()
 *
 *	Comments:	Provide entry points for geometric primitive rendering
 *				functions.
 *
\*===========================================================================*/
static TQ3XFunctionPointer RS_Geometry_MetaHandler(
	TQ3XMethodType			methodType)
{
	switch (methodType) {
		case kQ3GeometryTypeLine: {
			return (TQ3XFunctionPointer) RS_Geometry_Line;
			break;
		}
		case kQ3GeometryTypeMarker: {
			return (TQ3XFunctionPointer) RS_Geometry_Marker;
			break;
		}
		case kQ3GeometryTypePixmapMarker: {
			return (TQ3XFunctionPointer) RS_Geometry_PixmapMarker;
			break;
		}
		case kQ3GeometryTypePoint: {
			return (TQ3XFunctionPointer) RS_Geometry_Point;
			break;
		}
		case kQ3GeometryTypeTriangle: {
			return (TQ3XFunctionPointer) RS_Geometry_Triangle;
			break;
		}
		default: {
			return NULL;
			break;
		}
	}
}


/*===========================================================================*\
 *
 *	Routine:	RS_Attribute_MetaHandler()
 *
 *	Comments:	Provide callbacks for updating attributes the sample
 *				renderer cares about. Since it's only a simple wireframe
 *				renderer, we only deal with diffuse color and highlight state.
 *
\*===========================================================================*/
static TQ3XFunctionPointer RS_Attribute_MetaHandler(
	TQ3XMethodType			/*methodType*/)
{
	return NULL;
	/* ToDo: Update the current Surface, if required */
	/*
	switch (methodType) {
	} */
}
/*===========================================================================*\
 *
 *	Routine:	RS_Attribute_MetaHandler()
 *
 *	Comments:	Provide callbacks for updating attributes the sample
 *				renderer cares about. Since it's only a simple wireframe
 *				renderer, we only deal with diffuse color and highlight state.
 *
\*===========================================================================*/
static TQ3XFunctionPointer RS_Shader_MetaHandler(
	TQ3XMethodType			methodType)
{
	switch (methodType) {
		case kQ3ShaderTypeSurface:
			return ((TQ3XFunctionPointer)RS_Update_SurfaceShader);
			break;
		/* ToDo: Update the current Surface, if required */
		default: {
			return NULL;	
			break;
		}
	}
}
/*===========================================================================*\
 *
 *	Routine:	RS_Matrix_MetaHandler()
 *
 *	Comments:	Provide callbacks for updating the various transformation
 *				states.
 *
\*===========================================================================*/
static TQ3XFunctionPointer RS_Matrix_MetaHandler(
	TQ3XMethodType			methodType)
{
	switch (methodType) {
		case kQ3XMethodTypeRendererUpdateMatrixLocalToWorld: {
			return ((TQ3XFunctionPointer) RS_Update_LocalToWorldMatrix);
			break;
		}
#if RS_DEBUG
		case kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum: {
			return ((TQ3XFunctionPointer) RS_Update_LocalToFrustumMatrix);
			break;
		}
#endif
		default: {
			return (NULL);
			break;
		}
	}
	
}
/*===========================================================================*\
 *
 *	Routine:	RS_Read()
 *
 *	Comments:	
 *
\*===========================================================================*/

static TQ3RendererObject RS_Read(
	TQ3FileObject		file)
{
	Q3_UNUSED(file);
	
	return Q3Renderer_NewFromType(gRS_ClassType);
}
/*===========================================================================*\
 *
 *	Routine:	RS_Traverse()
 *
 *	Comments:	
 *
\*===========================================================================*/

static TQ3Status RS_Traverse(
	TQ3RendererObject	renderer,
	void				*unused,
	TQ3ViewObject		view)
{
	Q3_UNUSED(unused);
	Q3_UNUSED(renderer);
	
	return Q3XView_SubmitWriteData(view, 0, NULL, NULL);
}


/*===========================================================================*\
 *
 *	Routine:	RS_MetaHandler()
 *
 *	Comments:	Main metahandler. Provides entry point for various
 *				required and optional renderer functions, and for the
 *				metahandlers for geometric primitive rendering and
 *				style, matrix, and attribute updates.
 *
\*===========================================================================*/
static TQ3XFunctionPointer RS_MetaHandler(
	TQ3XMethodType		methodType)
{
	switch (methodType) {
		/* 
		 *  Object 
		 */
		case kQ3XMethodTypeObjectNew: {
			return (TQ3XFunctionPointer) RS_New;
			break;
		}
		case kQ3XMethodTypeObjectDelete: {
			return (TQ3XFunctionPointer) RS_Delete;
			break;
		}
		
		/*
		 * I/O
		 */
		case kQ3XMethodTypeObjectRead: {
			return (TQ3XFunctionPointer) RS_Read;
			break;
		}
		case kQ3XMethodTypeObjectAttach: {
			return (TQ3XFunctionPointer) NULL;
			break;
		}
		case kQ3XMethodTypeObjectTraverse: {
			return (TQ3XFunctionPointer) RS_Traverse;
			break;
		}
		case kQ3XMethodTypeObjectWrite: {
			return (TQ3XFunctionPointer) NULL;
			break;
		}
			
		/* 
		 *  Renderer 
		 */	
		case kQ3XMethodTypeRendererStartFrame: {
			return (TQ3XFunctionPointer) SR_StartFrame;
			break;
		}
		case kQ3XMethodTypeRendererStartPass: {
			return (TQ3XFunctionPointer) RS_StartPass;
			break;
		}
		case kQ3XMethodTypeRendererEndPass: {
			return (TQ3XFunctionPointer) RS_EndPass;
			break;
		}
		case kQ3XMethodTypeRendererCancel: {
			return (TQ3XFunctionPointer) RS_Cancel;
			break;
		}
		/*
		 * Renderer Draw:
		 */		
		case kQ3XMethodTypeRendererSubmitGeometryMetaHandler: {
			return (TQ3XFunctionPointer) RS_Geometry_MetaHandler;
			break;
		/* 
		 *  Renderer Update 
		 */
		case kQ3XMethodTypeRendererUpdateAttributeMetaHandler: {
			return (TQ3XFunctionPointer) RS_Attribute_MetaHandler;
			break;
		}
		case kQ3XMethodTypeRendererUpdateMatrixMetaHandler: {
			return (TQ3XFunctionPointer) RS_Matrix_MetaHandler;
			break;
		}
		case kQ3XMethodTypeRendererUpdateShaderMetaHandler: {
			return (TQ3XFunctionPointer) RS_Shader_MetaHandler;
			break;
		}
		
		/*
		 * Modal dialog
		 */
		case kQ3XMethodTypeRendererModalConfigure: {
			return (TQ3XFunctionPointer)NULL;
			break;
		}
		/*
		 *  "isInteractive" control
		 */
		case kQ3XMethodTypeRendererIsInteractive: {
			return (TQ3XFunctionPointer) kQ3False;
			break;
		}
		/*
		 *  renderer name string
		 */
		case kQ3XMethodTypeRendererGetNickNameString: {
			return (TQ3XFunctionPointer) RS_GetNameString;
			break;
		}		
		/*
		 *  Configuration data
		 */
		case kQ3XMethodTypeRendererGetConfigurationData: {
			return (TQ3XFunctionPointer) NULL;
			break;
		}

		case kQ3XMethodTypeRendererSetConfigurationData: {
			return (TQ3XFunctionPointer) NULL;
			break;
		}
		default:
			return (TQ3XFunctionPointer) NULL;
		}
	}
}

/*===========================================================================*\
 *
 *	Routine:	RS_Register()
 *
 *	Comments:	Create/register this  renderer, as a subclass of 
 *				kQ3SharedTypeRenderer
 *
\*===========================================================================*/

static TQ3Status RS_Register(void)
{
	/*
	 *  Create/register the class
	 */
	gRS_RendererClass = 
		Q3XObjectHierarchy_RegisterClass(
			kQ3SharedTypeRenderer,
			&gRS_ClassType,
			(char*)kRSRenderer_NickName,
			RS_MetaHandler,
			NULL,
			0,
			sizeof(TRSRendererPrivate));

	/*
	 *  Make sure it worked
	 */
	if (gRS_RendererClass == NULL) {
		return (kQ3Failure);
	}

	return (kQ3Success);
}

/*===========================================================================*\
 *
 *	Routine:	SR_Exit()
 *
 *	Comments:	Called on exiting from QD3D, this function unregisters this
 *				plug-in renderer.
 *
\*===========================================================================*/
TQ3Status RS_Exit( 
	void)
{
	if (gRS_SharedLibrary != NULL) {
		Q3XSharedLibrary_Unregister(gRS_SharedLibrary);
		gRS_SharedLibrary = NULL;
	}
	return (kQ3Success);
}

#if defined(macintosh)
/*===========================================================================*\
 *
 *	Routine:	RS_Initialize()
 *
 *	Comments:	This is the initialization routine called by the shared library
 *				manager.  This function passes RS_Register to QuickDraw 3D
 *
\*===========================================================================*/
OSErr RS_Initialize( 
	const CFragInitBlock	*initBlock)
{
	TQ3XSharedLibraryInfo	sharedLibraryInfo;
	OSErr					err = noErr;
	
	sharedLibraryInfo.registerFunction 	= RS_Register;
	sharedLibraryInfo.sharedLibrary 	= (unsigned long)
												initBlock->connectionID;
												
	Q3XSharedLibrary_Register(&sharedLibraryInfo);
	
	gRS_SharedLibrary = (unsigned long)initBlock->connectionID;
	
	return (err);
}

#endif	/* WINDOW_SYSTEM_MACINTOSH */
#if defined(WIN32)
BOOL __stdcall DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	TQ3XSharedLibraryInfo	sharedLibraryInfo;
	
	sharedLibraryInfo.registerFunction 	= RS_Register;
	sharedLibraryInfo.sharedLibrary 	= (unsigned long)hModule;
												
	Q3XSharedLibrary_Register(&sharedLibraryInfo);
	
	gRS_SharedLibrary = (unsigned long)hModule;
	
	return (TRUE);
}
 
#endif
