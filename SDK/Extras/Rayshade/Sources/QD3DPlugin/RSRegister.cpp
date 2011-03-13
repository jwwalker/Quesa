/*  NAME:
        RSRegister.c

    DESCRIPTION:
        RayShade renderer registration

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
#include "RS_Rasterize.h"

#include "RT_Light.h"
#include "RT_Tracer.h"

#if __MACH__
	#include <Quesa/Quesa.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaExtension.h>
#else
	#include <Quesa.h>
	#include <QuesaGroup.h>
	#include <QuesaExtension.h>
#endif

#include <stdlib.h>
#include <string>
#include <string.h>

#if QUESA_OS_MACINTOSH

#if __MWERKS__
#include <CodeFragments.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if TARGET_RT_MAC_MACHO
/*
 *  Shared library initialization entry point
 */

#if __MWERKS__
__declspec(dllexport) void RS_Initialize();
#elif __GNUC__ >= 4
void RS_Initialize() __attribute__ ((constructor));
#endif

#else	// CFM
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

#endif

#ifdef __cplusplus
}
#endif

#endif /* QUESA_OS_MACINTOSH */

#if QUESA_OS_WIN32
#include <Windows.h>

extern "C"
{

/*
 * Shared library entry function
 */
BOOL __stdcall DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 );
/*
 * Shared library exit function
 */
__declspec(dllexport) TQ3Status RS_Exit (
	void);

}
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
#if TARGET_RT_MAC_MACHO
	#pragma unused(gRS_SharedLibrary)
#endif


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
	
	Q3Matrix3x3_SetIdentity( &rsPrivate->uvTransform );

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
                        TQ3Float32 radius;
                        
                        Q3PointLight_GetData(theLight,&thePointLightData);
                        Q3PointLight_GetRadius(theLight,&radius);
                        RT_AddPointLight(
                        		rsPrivate->raytracer,
                                &thePointLightData.location,
                                thePointLightData.castsShadows,
                                &theLightData.color,
                                theLightData.brightness,
                                radius);
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
	TQ3Float32*		buf = NULL ;
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
    	int width,height, left, top;
    	
    	theRasterizer = RSRasterizer_Create(theDrawContext);
    	if (theRasterizer == NULL)
    	{
    		result = kQ3ViewStatusError;
    		goto cleanup;
    	}
    	
    	theStatus = RSRasterizer_GetSize(theRasterizer,&width,&height, &left, &top);
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
	       
	       buf = (TQ3Float32*) malloc ( xres * 4 * sizeof ( TQ3Float32 ) ) ;
	       
	        if (! buf){
	              result = kQ3ViewStatusError;
	              goto cleanup;
	        }
	      
	      	RSRasterizer_Start( theRasterizer );
	                
	        for (int i = 0; i < yres; i++)
	        {
	        	/*
	        	 * RayTrace the next scan line
	        	 */
	        	if (RTRayTracer_ScanNextLine(theTracer,i,(TQ3Float32(*)[4])buf, xres * 4 * sizeof ( TQ3Float32 ) ) != kQ3Success)
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
	            RSRasterizer_Rasterize_RGB_Span(theRasterizer,left,i+top,width,(TQ3Float32(*)[4])buf);
           		RSRasterizer_Unlock(theRasterizer);

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
            
	      	RSRasterizer_Finish( theRasterizer );

            RSRasterizer_Delete(theRasterizer);
            theRasterizer = NULL;
            RTRayTracer_Delete(theTracer);
            theTracer = NULL;
            RT_Reset(rsPrivate->raytracer);
		 	if (buf)
				free(buf);
			buf = NULL;
            
		}
    	Q3Object_Dispose(theDrawContext);
    }
	return kQ3ViewStatusDone;
	
cleanup:
	if (theRasterizer){
		RSRasterizer_Finish( theRasterizer );
		RSRasterizer_Delete( theRasterizer );
		}
		
	if (theDrawContext)
		Q3Object_Dispose(theDrawContext);
	if (theTracer)
		RTRayTracer_Delete(theTracer);
	if (buf)
		free(buf);
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

#if TARGET_RT_MAC_CFM
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



#elif TARGET_RT_MAC_MACHO

void RS_Initialize()
{
	TQ3XSharedLibraryInfo	sharedLibraryInfo;
	
	sharedLibraryInfo.registerFunction 	= RS_Register;
	sharedLibraryInfo.sharedLibrary 	= NULL;
												
	Q3XSharedLibrary_Register(&sharedLibraryInfo);
}

#endif	/* WINDOW_SYSTEM_MACINTOSH */

#if QUESA_OS_WIN32


BOOL __stdcall DllMain( HINSTANCE inst, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	TQ3XSharedLibraryInfo	sharedLibraryInfo;
	
	sharedLibraryInfo.registerFunction 	= RS_Register;
	sharedLibraryInfo.sharedLibrary 	= (unsigned long)inst;
												
	Q3XSharedLibrary_Register(&sharedLibraryInfo);
	
	gRS_SharedLibrary = (unsigned long)inst;
	
	return (TRUE);
}



#endif
