/*  NAME:
        E3Style.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Style.h"
#include "E3View.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3style_metahandler : Style metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectIsDrawable:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_subdivision_submit : Subdivision submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_subdivision_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3SubdivisionStyleData		*instanceData = (const TQ3SubdivisionStyleData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleSubdivision(theView, instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_subdivision_metahandler : Subdivision metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_subdivision_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_subdivision_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_pickid_submit : Pick ID submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_pickid_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3Uns32			*instanceData = (const TQ3Uns32 *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStylePickID(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_pickid_metahandler : Pick ID metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_pickid_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_pickid_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_pickparts_submit : Pick Parts submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_pickparts_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3PickParts			*instanceData = (const TQ3PickParts *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStylePickParts(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_pickparts_metahandler : Pick Parts metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_pickparts_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_pickparts_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_castshadows_submit : Cast Shadows submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_castshadows_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3Boolean			*instanceData = (const TQ3Boolean *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleCastShadows(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_castshadows_metahandler : Cast Shadows metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_castshadows_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_castshadows_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_receiveshadows_submit : Receive Shadows submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_receiveshadows_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3Boolean			*instanceData = (const TQ3Boolean *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleReceiveShadows(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_receiveshadows_metahandler : Receive Shadows metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_receiveshadows_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_receiveshadows_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_fill_submit : Fill submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_fill_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3FillStyle			*instanceData = (const TQ3FillStyle *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleFill(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_fill_metahandler : Fill metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_fill_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_fill_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_backfacing_submit : Backfacing submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_backfacing_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3BackfacingStyle		*instanceData = (const TQ3BackfacingStyle *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleBackfacing(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_backfacing_metahandler : Backfacing metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_backfacing_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_backfacing_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_interpolation_submit : Interpolation submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_interpolation_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3InterpolationStyle		*instanceData = (const TQ3InterpolationStyle *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleInterpolation(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_interpolation_metahandler : Interpolation metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_interpolation_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_interpolation_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_hilight_submit : Hilight submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_hilight_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3AttributeSet		*instanceData = (const TQ3AttributeSet *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	if (*instanceData != NULL)
		E3View_State_SetStyleHighlight(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_hilight_delete : Highlight style delete method.
//-----------------------------------------------------------------------------
static void
e3style_hilight_delete(TQ3Object theObject, void *privateData)
{	TQ3AttributeSet		theAttSet = *(TQ3AttributeSet*)privateData;



	// Release our data
	if (theAttSet != NULL)
	{
		Q3_ASSERT( Q3Object_IsType( theAttSet, kQ3SetTypeAttribute ) );
		Q3Object_Dispose( theAttSet );
	}
}





//=============================================================================
//      e3style_hilight_metahandler : Hilight metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_hilight_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3style_hilight_delete;
			break;
		
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_hilight_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_orientation_submit : Orientation submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_orientation_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3OrientationStyle			*instanceData = (const TQ3OrientationStyle *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleOrientation(theView, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_orientation_metahandler : Orientation metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_orientation_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_orientation_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_antialias_submit : Anti-alias submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_antialias_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3AntiAliasStyleData			*instanceData = (const TQ3AntiAliasStyleData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleAntiAlias(theView, instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_antialias_metahandler : Anti-alias metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_antialias_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_antialias_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3style_fog_submit : Fog submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3style_fog_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3FogStyleData			*instanceData = (const TQ3FogStyleData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the style
	E3View_State_SetStyleFog(theView, instanceData);

	return(kQ3Success);
}





//=============================================================================
//      e3style_fog_metahandler : Fog metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_fog_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3style_fog_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Style_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Style_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the Style classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
											kQ3ShapeTypeStyle,
											kQ3ClassNameStyle,
											e3style_metahandler,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeSubdivision,
												kQ3ClassNameStyleSubdivision,
												e3style_subdivision_metahandler,
												sizeof(TQ3SubdivisionStyleData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypePickID,
												kQ3ClassNameStylePickID,
												e3style_pickid_metahandler,
												sizeof(TQ3Uns32));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypePickParts,
												kQ3ClassNameStylePickParts,
												e3style_pickparts_metahandler,
												sizeof(TQ3PickParts));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeCastShadows,
												kQ3ClassNameStyleCastShadows,
												e3style_castshadows_metahandler,
												sizeof(TQ3Boolean));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeReceiveShadows,
												kQ3ClassNameStyleReceiveShadows,
												e3style_receiveshadows_metahandler,
												sizeof(TQ3Boolean));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeFill,
												kQ3ClassNameStyleFill,
												e3style_fill_metahandler,
												sizeof(TQ3FillStyle));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeBackfacing,
												kQ3ClassNameStyleBackfacing,
												e3style_backfacing_metahandler,
												sizeof(TQ3BackfacingStyle));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeInterpolation,
												kQ3ClassNameStyleInterpolation,
												e3style_interpolation_metahandler,
												sizeof(TQ3InterpolationStyle));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeHighlight,
												kQ3ClassNameStyleHighlight,
												e3style_hilight_metahandler,
												sizeof(TQ3AttributeSet));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeOrientation,
												kQ3ClassNameStyleOrientation,
												e3style_orientation_metahandler,
												sizeof(TQ3OrientationStyle));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeAntiAlias,
												kQ3ClassNameStyleAntiAlias,
												e3style_antialias_metahandler,
												sizeof(TQ3AntiAliasStyleData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStyle,
												kQ3StyleTypeFog,
												kQ3ClassNameStyleFog,
												e3style_fog_metahandler,
												sizeof(TQ3FogStyleData));

	return(qd3dStatus);
}





//=============================================================================
//      E3Style_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Style_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeFog,         			 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeAntiAlias,         	 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeOrientation,         	 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeHighlight,            	 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeInterpolation,			 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeBackfacing,      		 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeFill,    				 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeCastShadows,	  		 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeReceiveShadows,  		 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypePickParts, 			 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypePickID,    			 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StyleTypeSubdivision,			 kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeStyle,     			 kQ3True);

	return(qd3dStatus);
}



//=============================================================================
//      E3Style_GetType : Get the type of a style object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Style_GetType(TQ3StyleObject theStyle)
{


	// Get the type
	return(E3ClassTree_GetObjectType(theStyle, kQ3ShapeTypeStyle));
}





//=============================================================================
//      E3Style_Submit : Submit a style object.
//-----------------------------------------------------------------------------
TQ3Status
E3Style_Submit(TQ3StyleObject theStyle, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitRetained(theView, theStyle);

	return(qd3dStatus);
}





//=============================================================================
//      E3SubdivisionStyle_New : Create a new subdivision style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3SubdivisionStyle_New(const TQ3SubdivisionStyleData *data)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeSubdivision, kQ3False, data);

	return(theObject);
}





//=============================================================================
//      E3SubdivisionStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3SubdivisionStyle_Submit(const TQ3SubdivisionStyleData *data, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeSubdivision, data);

	return(qd3dStatus);
}





//=============================================================================
//      E3SubdivisionStyle_SetData : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3SubdivisionStyle_SetData(TQ3StyleObject theStyle, const TQ3SubdivisionStyleData *data)
{	TQ3SubdivisionStyleData* instanceData = (TQ3SubdivisionStyleData*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeSubdivision);



	// Set the data
	*instanceData = *data;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3SubdivisionStyle_GetData : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3SubdivisionStyle_GetData(TQ3StyleObject theStyle, TQ3SubdivisionStyleData *data)
{	TQ3SubdivisionStyleData* instanceData = (TQ3SubdivisionStyleData*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeSubdivision);



	// Get the data
	*data = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3PickIDStyle_New : Create a new pick ID style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3PickIDStyle_New(TQ3Uns32 id)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypePickID, kQ3False, &id);

	return(theObject);
}





//=============================================================================
//      E3PickIDStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3PickIDStyle_Submit(TQ3Uns32 id, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypePickID, &id);

	return(qd3dStatus);
}





//=============================================================================
//      E3PickIDStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3PickIDStyle_Get(TQ3StyleObject theStyle, TQ3Uns32 *id)
{	TQ3Uns32* instanceData = (TQ3Uns32*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypePickID);



	// Get the data
	*id = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3PickIDStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3PickIDStyle_Set(TQ3StyleObject theStyle, TQ3Uns32 id)
{	TQ3Uns32* instanceData = (TQ3Uns32*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypePickID);



	// Set the data	
	*instanceData = id;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3PickPartsStyle_New : Create a new pick parts style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3PickPartsStyle_New(TQ3PickParts parts)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypePickParts, kQ3False, &parts);

	return(theObject);
}





//=============================================================================
//      E3PickPartsStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3PickPartsStyle_Submit(TQ3PickParts parts, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypePickParts, &parts);

	return(qd3dStatus);
}





//=============================================================================
//      E3PickPartsStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3PickPartsStyle_Get(TQ3StyleObject theStyle, TQ3PickParts *parts)
{	TQ3PickParts* instanceData = (TQ3PickParts*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypePickParts);



	// Get the data
	*parts = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3PickPartsStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3PickPartsStyle_Set(TQ3StyleObject theStyle, TQ3PickParts parts)
{	TQ3PickParts* instanceData = (TQ3PickParts*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypePickParts);



	// Set the data
	*instanceData = parts;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3CastShadowsStyle_New : Create a new cast shadows style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3CastShadowsStyle_New(TQ3Boolean castShadows)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeCastShadows, kQ3False, &castShadows);

	return(theObject);
}





//=============================================================================
//      E3CastShadowsStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3CastShadowsStyle_Submit(TQ3Boolean castShadows, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeCastShadows, &castShadows);

	return(qd3dStatus);
}





//=============================================================================
//      E3CastShadowsStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3CastShadowsStyle_Get(TQ3StyleObject theStyle, TQ3Boolean *castShadows)
{	TQ3Boolean* instanceData = (TQ3Boolean*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeCastShadows);



	// Get the data
	*castShadows = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3CastShadowsStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3CastShadowsStyle_Set(TQ3StyleObject theStyle, TQ3Boolean castShadows)
{	TQ3Boolean* instanceData = (TQ3Boolean*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeCastShadows);



	// Set the data
	*instanceData = castShadows;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3ReceiveShadowsStyle_New : Create a new receive shadows style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3ReceiveShadowsStyle_New(TQ3Boolean receiveShadows)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeReceiveShadows, kQ3False, &receiveShadows);

	return(theObject);
}





//=============================================================================
//      E3ReceiveShadowsStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3ReceiveShadowsStyle_Submit(TQ3Boolean receiveShadows, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeReceiveShadows, &receiveShadows);

	return(qd3dStatus);
}





//=============================================================================
//      E3ReceiveShadowsStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3ReceiveShadowsStyle_Get(TQ3StyleObject theStyle, TQ3Boolean *receiveShadows)
{	TQ3Boolean* instanceData = (TQ3Boolean*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeReceiveShadows);



	// Get the data
	*receiveShadows = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3ReceiveShadowsStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3ReceiveShadowsStyle_Set(TQ3StyleObject theStyle, TQ3Boolean receiveShadows)
{	TQ3Boolean* instanceData = (TQ3Boolean*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeReceiveShadows);



	// Set the data
	*instanceData = receiveShadows;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3FillStyle_New : Create a new fill style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3FillStyle_New(TQ3FillStyle fillStyle)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeFill, kQ3False, &fillStyle);

	return(theObject);
}





//=============================================================================
//      E3FillStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3FillStyle_Submit(TQ3FillStyle fillStyle, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeFill, &fillStyle);

	return(qd3dStatus);
}





//=============================================================================
//      E3FillStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3FillStyle_Get(TQ3StyleObject theStyle, TQ3FillStyle *fillStyle)
{	TQ3FillStyle* instanceData = (TQ3FillStyle*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeFill);



	// Get the data
	*fillStyle = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3FillStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3FillStyle_Set(TQ3StyleObject theStyle, TQ3FillStyle fillStyle)
{	TQ3FillStyle* instanceData = (TQ3FillStyle*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeFill);



	// Set the data
	*instanceData = fillStyle;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3BackfacingStyle_New : Create a new backfacing style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3BackfacingStyle_New(TQ3BackfacingStyle backfacingStyle)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeBackfacing, kQ3False, &backfacingStyle);

	return(theObject);
}





//=============================================================================
//      E3BackfacingStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3BackfacingStyle_Submit(TQ3BackfacingStyle backfacingStyle, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeBackfacing, &backfacingStyle);

	return(qd3dStatus);
}





//=============================================================================
//      E3BackfacingStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3BackfacingStyle_Get(TQ3StyleObject theStyle, TQ3BackfacingStyle *backfacingStyle)
{	TQ3BackfacingStyle* instanceData = (TQ3BackfacingStyle*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeBackfacing);



	// Get the data
	*backfacingStyle = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3BackfacingStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3BackfacingStyle_Set(TQ3StyleObject theStyle, TQ3BackfacingStyle backfacingStyle)
{	TQ3BackfacingStyle* instanceData = (TQ3BackfacingStyle*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeBackfacing);



	// Set the data
	*instanceData = backfacingStyle;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3InterpolationStyle_New : Create a new interpolation style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3InterpolationStyle_New(TQ3InterpolationStyle interpolationStyle)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeInterpolation, kQ3False, &interpolationStyle);

	return(theObject);
}





//=============================================================================
//      E3InterpolationStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3InterpolationStyle_Submit(TQ3InterpolationStyle interpolationStyle, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeInterpolation, &interpolationStyle);

	return(qd3dStatus);
}





//=============================================================================
//      E3InterpolationStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3InterpolationStyle_Get(TQ3StyleObject theStyle, TQ3InterpolationStyle *interpolationStyle)
{	TQ3InterpolationStyle* instanceData = (TQ3InterpolationStyle*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeInterpolation);



	// Get the data
	*interpolationStyle = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3InterpolationStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3InterpolationStyle_Set(TQ3StyleObject theStyle, TQ3InterpolationStyle interpolationStyle)
{	TQ3InterpolationStyle* instanceData = (TQ3InterpolationStyle*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeInterpolation);



	// Set the data
	*instanceData = interpolationStyle;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3HighlightStyle_New : Create a new hilight style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3HighlightStyle_New(TQ3AttributeSet highlightAttribute)
{	TQ3StyleObject		theObject;
	TQ3AttributeSet		ref;



	// Create the object
	if ( highlightAttribute )
		ref = Q3Shared_GetReference ( highlightAttribute ) ;
	else
		ref = NULL ;

	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeHighlight, kQ3False, &ref);

	return(theObject);
}





//=============================================================================
//      E3HighlightStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3HighlightStyle_Submit(TQ3AttributeSet highlightAttribute, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeHighlight, &highlightAttribute);

	return(qd3dStatus);
}





//=============================================================================
//      E3HighlightStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3HighlightStyle_Get(TQ3StyleObject theStyle, TQ3AttributeSet *highlightAttribute)
{	TQ3AttributeSet* instanceData = (TQ3AttributeSet*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeHighlight);



	// Get the data
	if ( instanceData )
		{
		*highlightAttribute = Q3Shared_GetReference ( *instanceData ) ;
		return(kQ3Success);
		}
	else if (highlightAttribute)
		*highlightAttribute = NULL;

	return(kQ3Failure);
}





//=============================================================================
//      E3HighlightStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3HighlightStyle_Set(TQ3StyleObject theStyle, TQ3AttributeSet highlightAttribute)
{	TQ3AttributeSet* instanceData = (TQ3AttributeSet*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeHighlight);



	// Set the data
	if ( *instanceData )
		Q3Object_Dispose ( *instanceData ) ;
		
	if ( highlightAttribute )
		*instanceData = Q3Shared_GetReference ( highlightAttribute ) ;
	else
		*instanceData = NULL ;

	Q3Shared_Edited(theStyle);
	
	return(kQ3Success);
}





//=============================================================================
//      E3OrientationStyle_New : Create a new orientation style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3OrientationStyle_New(TQ3OrientationStyle frontFacingDirection)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeOrientation, kQ3False, &frontFacingDirection);

	return(theObject);
}





//=============================================================================
//      E3OrientationStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3OrientationStyle_Submit(TQ3OrientationStyle frontFacingDirection, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeOrientation, &frontFacingDirection);

	return(qd3dStatus);
}





//=============================================================================
//      E3OrientationStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3OrientationStyle_Get(TQ3StyleObject theStyle, TQ3OrientationStyle *frontFacingDirection)
{	TQ3OrientationStyle* instanceData = (TQ3OrientationStyle*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeOrientation);



	// Get the data
	*frontFacingDirection = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3OrientationStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3OrientationStyle_Set(TQ3StyleObject theStyle, TQ3OrientationStyle frontFacingDirection)
{	TQ3OrientationStyle* instanceData = (TQ3OrientationStyle*)  E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeOrientation);



	// Set the data
	*instanceData = frontFacingDirection;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3AntiAliasStyle_New : Create a new anti-alias style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3AntiAliasStyle_New(const TQ3AntiAliasStyleData *data)
{	TQ3StyleObject	theObject;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeAntiAlias, kQ3False, data);

	return(theObject);
}





//=============================================================================
//      E3AntiAliasStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3AntiAliasStyle_Submit(const TQ3AntiAliasStyleData *data, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeAntiAlias, data);

	return(qd3dStatus);
}





//=============================================================================
//      E3AntiAliasStyle_GetData : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3AntiAliasStyle_GetData(TQ3StyleObject theStyle, TQ3AntiAliasStyleData *data)
{	TQ3AntiAliasStyleData* instanceData = (TQ3AntiAliasStyleData*)  E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeAntiAlias);



	// Get the data
	*data = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3AntiAliasStyle_SetData : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3AntiAliasStyle_SetData(TQ3StyleObject theStyle, const TQ3AntiAliasStyleData *data)
{	TQ3AntiAliasStyleData* instanceData = (TQ3AntiAliasStyleData*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeAntiAlias);



	// Set the data
	*instanceData = *data;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





//=============================================================================
//      E3FogStyle_New : Create a new fog style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3FogStyle_New(const TQ3FogStyleData *data)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeFog, kQ3False, data);

	return(theObject);
}





//=============================================================================
//      E3FogStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3FogStyle_Submit(const TQ3FogStyleData *data, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeFog, data);

	return(qd3dStatus);
}





//=============================================================================
//      E3FogStyle_GetData : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3FogStyle_GetData(TQ3StyleObject theStyle, TQ3FogStyleData *data)
{	TQ3FogStyleData* instanceData = (TQ3FogStyleData*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeFog);



	// Get the data
	*data = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3FogStyle_SetData : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3FogStyle_SetData(TQ3StyleObject theStyle, const TQ3FogStyleData *data)
{	TQ3FogStyleData* instanceData = (TQ3FogStyleData*) E3ClassTree_FindInstanceData(theStyle, kQ3StyleTypeFog);



	// Set the data
	*instanceData = *data;
	Q3Shared_Edited(theStyle);

	return(kQ3Success);
}





