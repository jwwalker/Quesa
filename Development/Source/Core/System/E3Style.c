/*  NAME:
        E3Style.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#include "E3Style.h"
#include "E3View.h"


//=============================================================================
//      Internal functions
//=============================================================================
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
#pragma mark -



//-----------------------------------------------------------------------------
//      e3style_subdivision_submit : Subdivision submit method.
//-----------------------------------------------------------------------------
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
//      e3style_hilight_metahandler : Hilight metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3style_hilight_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
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
E3SubdivisionStyle_SetData(TQ3StyleObject subdiv, const TQ3SubdivisionStyleData *data)
{	TQ3SubdivisionStyleData* instanceData = (TQ3SubdivisionStyleData*) subdiv->instanceData ;



	*instanceData = *data ;
	Q3Shared_Edited(subdiv);

	return(kQ3Success);
}





//=============================================================================
//      E3SubdivisionStyle_GetData : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3SubdivisionStyle_GetData(TQ3StyleObject subdiv, TQ3SubdivisionStyleData *data)
{	TQ3SubdivisionStyleData* instanceData = (TQ3SubdivisionStyleData*)subdiv->instanceData ;
	
	
	
	*data = *instanceData ;
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
E3PickIDStyle_Get(TQ3StyleObject pickIDObject, TQ3Uns32 *id)
{	TQ3Uns32* instanceData = (TQ3Uns32*)pickIDObject->instanceData ;
	
	
	
	*id = *instanceData ;
	return(kQ3Success);
}





//=============================================================================
//      E3PickIDStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3PickIDStyle_Set(TQ3StyleObject pickIDObject, TQ3Uns32 id)
{	TQ3Uns32* instanceData = (TQ3Uns32*)pickIDObject->instanceData ;
	
	
	
	*instanceData = id;
	Q3Shared_Edited(pickIDObject);

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
E3PickPartsStyle_Get(TQ3StyleObject pickPartsObject, TQ3PickParts *parts)
{	TQ3PickParts* instanceData = (TQ3PickParts*)pickPartsObject->instanceData ;
	
	
	
	*parts = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3PickPartsStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3PickPartsStyle_Set(TQ3StyleObject pickPartsObject, TQ3PickParts parts)
{	TQ3PickParts* instanceData = (TQ3PickParts*)pickPartsObject->instanceData ;



	*instanceData = parts;
	Q3Shared_Edited(pickPartsObject);

	return(kQ3Success);
}





//=============================================================================
//      E3ReceiveShadowsStyle_New : Create a new receive shadows style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3ReceiveShadowsStyle_New(TQ3Boolean receives)
{	TQ3StyleObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StyleTypeReceiveShadows, kQ3False, &receives);
	return(theObject);
}





//=============================================================================
//      E3ReceiveShadowsStyle_Submit : Submit the style.
//-----------------------------------------------------------------------------
TQ3Status
E3ReceiveShadowsStyle_Submit(TQ3Boolean receives, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StyleTypeReceiveShadows, &receives);

	return(qd3dStatus);
}





//=============================================================================
//      E3ReceiveShadowsStyle_Get : Get the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3ReceiveShadowsStyle_Get(TQ3StyleObject styleObject, TQ3Boolean *receives)
{	TQ3Boolean* instanceData = (TQ3Boolean*)styleObject->instanceData ;
	
	
	
	*receives = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3ReceiveShadowsStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3ReceiveShadowsStyle_Set(TQ3StyleObject styleObject, TQ3Boolean receives)
{	TQ3Boolean* instanceData = (TQ3Boolean*)styleObject->instanceData ;
	
	
	
	*instanceData = receives ;
	Q3Shared_Edited(styleObject);

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
E3FillStyle_Get(TQ3StyleObject styleObject, TQ3FillStyle *fillStyle)
{	TQ3FillStyle* instanceData = (TQ3FillStyle*)styleObject->instanceData ;
	
	
	
	*fillStyle = *instanceData ;
	return(kQ3Success);
}





//=============================================================================
//      E3FillStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3FillStyle_Set(TQ3StyleObject styleObject, TQ3FillStyle fillStyle)
{	TQ3FillStyle* instanceData = (TQ3FillStyle*)styleObject->instanceData ;
	
	
	
	*instanceData = fillStyle ;
	Q3Shared_Edited(styleObject);

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
E3BackfacingStyle_Get(TQ3StyleObject backfacingObject, TQ3BackfacingStyle *backfacingStyle)
{	TQ3BackfacingStyle* instanceData = (TQ3BackfacingStyle*)backfacingObject->instanceData ;
	
	
	
	*backfacingStyle = *instanceData ;
	return(kQ3Success);
}





//=============================================================================
//      E3BackfacingStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3BackfacingStyle_Set(TQ3StyleObject backfacingObject, TQ3BackfacingStyle backfacingStyle)
{	TQ3BackfacingStyle* instanceData = (TQ3BackfacingStyle*)backfacingObject->instanceData ;
	
	
	
	*instanceData = backfacingStyle;
	Q3Shared_Edited(backfacingObject);

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
E3InterpolationStyle_Get(TQ3StyleObject interpolationObject, TQ3InterpolationStyle *interpolationStyle)
{	TQ3InterpolationStyle* instanceData = (TQ3InterpolationStyle*)interpolationObject->instanceData ;
	
	
	
	*interpolationStyle = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3InterpolationStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3InterpolationStyle_Set(TQ3StyleObject interpolationObject, TQ3InterpolationStyle interpolationStyle)
{	TQ3InterpolationStyle* instanceData = (TQ3InterpolationStyle*)interpolationObject->instanceData ;
	
	
	
	*instanceData = interpolationStyle;
	Q3Shared_Edited(interpolationObject);

	return(kQ3Success);
}





//=============================================================================
//      E3HighlightStyle_New : Create a new hilight style.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StyleObject
E3HighlightStyle_New(TQ3AttributeSet highlightAttribute)
{	TQ3StyleObject	theObject;
	TQ3AttributeSet ref ;

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
E3HighlightStyle_Get(TQ3StyleObject highlight, TQ3AttributeSet *highlightAttribute)
{	TQ3AttributeSet* instanceData = (TQ3AttributeSet*)highlight->instanceData ;
	
	
	
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
E3HighlightStyle_Set(TQ3StyleObject highlight, TQ3AttributeSet highlightAttribute)
{	TQ3AttributeSet* instanceData = (TQ3AttributeSet*)highlight->instanceData ;



	if ( *instanceData )
		Q3Object_Dispose ( *instanceData ) ;
		
	if ( highlightAttribute )
		*instanceData = Q3Shared_GetReference ( highlightAttribute ) ;
	else
		*instanceData = NULL ;

	Q3Shared_Edited(highlight);
	
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
E3OrientationStyle_Get(TQ3StyleObject frontFacingDirectionObject, TQ3OrientationStyle *frontFacingDirection)
{	TQ3OrientationStyle* instanceData = (TQ3OrientationStyle*)frontFacingDirectionObject->instanceData ;
	
	
	
	*frontFacingDirection = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3OrientationStyle_Set : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3OrientationStyle_Set(TQ3StyleObject frontFacingDirectionObject, TQ3OrientationStyle frontFacingDirection)
{	TQ3OrientationStyle* instanceData = (TQ3OrientationStyle*)frontFacingDirectionObject->instanceData ;
	
	
	
	*instanceData = frontFacingDirection;
	Q3Shared_Edited(frontFacingDirectionObject);

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
E3AntiAliasStyle_GetData(TQ3StyleObject styleObject, TQ3AntiAliasStyleData *data)
{	TQ3AntiAliasStyleData* instanceData = (TQ3AntiAliasStyleData*)styleObject->instanceData ;
	
	
	
	*data = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3AntiAliasStyle_SetData : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3AntiAliasStyle_SetData(TQ3StyleObject styleObject, const TQ3AntiAliasStyleData *data)
{	TQ3AntiAliasStyleData* instanceData = (TQ3AntiAliasStyleData*)styleObject->instanceData ;
	
	
	
	*instanceData = *data;
	Q3Shared_Edited(styleObject);

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
E3FogStyle_GetData(TQ3StyleObject styleObject, TQ3FogStyleData *data)
{	TQ3FogStyleData* instanceData = (TQ3FogStyleData*)styleObject->instanceData ;
	
	
	
	*data = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3FogStyle_SetData : Set the data for the style.
//-----------------------------------------------------------------------------
TQ3Status
E3FogStyle_SetData(TQ3StyleObject styleObject, const TQ3FogStyleData *data)
{	TQ3FogStyleData* instanceData = (TQ3FogStyleData*)styleObject->instanceData ;
	
	
	
	*instanceData = *data;
	Q3Shared_Edited(styleObject);

	return(kQ3Success);
}





