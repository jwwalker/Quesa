/*  NAME:
        QD3DStyle.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Style_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3Style_GetType(TQ3StyleObject style)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(style, kQ3ShapeTypeStyle), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on style
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Style_GetType(style));
}





//=============================================================================
//      Q3Style_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Style_Submit(TQ3StyleObject style, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(style, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on style
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Style_Submit(style, view));
}




#pragma mark -
//=============================================================================
//      Q3SubdivisionStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3SubdivisionStyle_New(const TQ3SubdivisionStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SubdivisionStyle_New(data));
}





//=============================================================================
//      Q3SubdivisionStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SubdivisionStyle_Submit(const TQ3SubdivisionStyleData *data, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SubdivisionStyle_Submit(data, view));
}





//=============================================================================
//      Q3SubdivisionStyle_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SubdivisionStyle_SetData(TQ3StyleObject subdiv, const TQ3SubdivisionStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(subdiv, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on subdiv
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SubdivisionStyle_SetData(subdiv, data));
}





//=============================================================================
//      Q3SubdivisionStyle_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SubdivisionStyle_GetData(TQ3StyleObject subdiv, TQ3SubdivisionStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(subdiv, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on subdiv
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SubdivisionStyle_GetData(subdiv, data));
}



#pragma mark -

//=============================================================================
//      Q3PickIDStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3PickIDStyle_New(TQ3Uns32 id)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on id
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PickIDStyle_New(id));
}





//=============================================================================
//      Q3PickIDStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PickIDStyle_Submit(TQ3Uns32 id, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on id
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PickIDStyle_Submit(id, view));
}





//=============================================================================
//      Q3PickIDStyle_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PickIDStyle_Get(TQ3StyleObject pickIDObject, TQ3Uns32 *id)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pickIDObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(id), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pickIDObject
		return(kQ3Failure);

	if (0) // Further checks on id
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PickIDStyle_Get(pickIDObject, id));
}





//=============================================================================
//      Q3PickIDStyle_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PickIDStyle_Set(TQ3StyleObject pickIDObject, TQ3Uns32 id)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pickIDObject, (kQ3ShapeTypeStyle)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pickIDObject
		return(kQ3Failure);

	if (0) // Further checks on id
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PickIDStyle_Set(pickIDObject, id));
}


#pragma mark -


//=============================================================================
//      Q3PickPartsStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3PickPartsStyle_New(TQ3PickParts parts)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on parts
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PickPartsStyle_New(parts));
}





//=============================================================================
//      Q3PickPartsStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PickPartsStyle_Submit(TQ3PickParts parts, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on parts
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PickPartsStyle_Submit(parts, view));
}





//=============================================================================
//      Q3PickPartsStyle_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PickPartsStyle_Get(TQ3StyleObject pickPartsObject, TQ3PickParts *parts)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pickPartsObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(parts), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pickPartsObject
		return(kQ3Failure);

	if (0) // Further checks on parts
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PickPartsStyle_Get(pickPartsObject, parts));
}





//=============================================================================
//      Q3PickPartsStyle_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PickPartsStyle_Set(TQ3StyleObject pickPartsObject, TQ3PickParts parts)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pickPartsObject, (kQ3ShapeTypeStyle)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pickPartsObject
		return(kQ3Failure);

	if (0) // Further checks on parts
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PickPartsStyle_Set(pickPartsObject, parts));
}



#pragma mark -

//=============================================================================
//      Q3ReceiveShadowsStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3ReceiveShadowsStyle_New(TQ3Boolean receives)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on receives
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ReceiveShadowsStyle_New(receives));
}





//=============================================================================
//      Q3ReceiveShadowsStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ReceiveShadowsStyle_Submit(TQ3Boolean receives, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on receives
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ReceiveShadowsStyle_Submit(receives, view));
}





//=============================================================================
//      Q3ReceiveShadowsStyle_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ReceiveShadowsStyle_Get(TQ3StyleObject styleObject, TQ3Boolean *receives)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(styleObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(receives), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on styleObject
		return(kQ3Failure);

	if (0) // Further checks on receives
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ReceiveShadowsStyle_Get(styleObject, receives));
}





//=============================================================================
//      Q3ReceiveShadowsStyle_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ReceiveShadowsStyle_Set(TQ3StyleObject styleObject, TQ3Boolean receives)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(styleObject, (kQ3ShapeTypeStyle)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on styleObject
		return(kQ3Failure);

	if (0) // Further checks on receives
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ReceiveShadowsStyle_Set(styleObject, receives));
}



#pragma mark -

//=============================================================================
//      Q3FillStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3FillStyle_New(TQ3FillStyle fillStyle)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on fillStyle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FillStyle_New(fillStyle));
}





//=============================================================================
//      Q3FillStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3FillStyle_Submit(TQ3FillStyle fillStyle, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on fillStyle
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FillStyle_Submit(fillStyle, view));
}





//=============================================================================
//      Q3FillStyle_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3FillStyle_Get(TQ3StyleObject styleObject, TQ3FillStyle *fillStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(styleObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fillStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on styleObject
		return(kQ3Failure);

	if (0) // Further checks on fillStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FillStyle_Get(styleObject, fillStyle));
}





//=============================================================================
//      Q3FillStyle_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3FillStyle_Set(TQ3StyleObject styleObject, TQ3FillStyle fillStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(styleObject, (kQ3ShapeTypeStyle)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on styleObject
		return(kQ3Failure);

	if (0) // Further checks on fillStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FillStyle_Set(styleObject, fillStyle));
}



#pragma mark -

//=============================================================================
//      Q3BackfacingStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3BackfacingStyle_New(TQ3BackfacingStyle backfacingStyle)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on backfacingStyle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BackfacingStyle_New(backfacingStyle));
}





//=============================================================================
//      Q3BackfacingStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3BackfacingStyle_Submit(TQ3BackfacingStyle backfacingStyle, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on backfacingStyle
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BackfacingStyle_Submit(backfacingStyle, view));
}





//=============================================================================
//      Q3BackfacingStyle_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3BackfacingStyle_Get(TQ3StyleObject backfacingObject, TQ3BackfacingStyle *backfacingStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(backfacingObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(backfacingStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on backfacingObject
		return(kQ3Failure);

	if (0) // Further checks on backfacingStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BackfacingStyle_Get(backfacingObject, backfacingStyle));
}





//=============================================================================
//      Q3BackfacingStyle_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3BackfacingStyle_Set(TQ3StyleObject backfacingObject, TQ3BackfacingStyle backfacingStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(backfacingObject, (kQ3ShapeTypeStyle)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on backfacingObject
		return(kQ3Failure);

	if (0) // Further checks on backfacingStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BackfacingStyle_Set(backfacingObject, backfacingStyle));
}



#pragma mark -

//=============================================================================
//      Q3InterpolationStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3InterpolationStyle_New(TQ3InterpolationStyle interpolationStyle)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on interpolationStyle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InterpolationStyle_New(interpolationStyle));
}





//=============================================================================
//      Q3InterpolationStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InterpolationStyle_Submit(TQ3InterpolationStyle interpolationStyle, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on interpolationStyle
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InterpolationStyle_Submit(interpolationStyle, view));
}





//=============================================================================
//      Q3InterpolationStyle_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InterpolationStyle_Get(TQ3StyleObject interpolationObject, TQ3InterpolationStyle *interpolationStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(interpolationObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(interpolationStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on interpolationObject
		return(kQ3Failure);

	if (0) // Further checks on interpolationStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InterpolationStyle_Get(interpolationObject, interpolationStyle));
}





//=============================================================================
//      Q3InterpolationStyle_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InterpolationStyle_Set(TQ3StyleObject interpolationObject, TQ3InterpolationStyle interpolationStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(interpolationObject, (kQ3ShapeTypeStyle)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on interpolationObject
		return(kQ3Failure);

	if (0) // Further checks on interpolationStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InterpolationStyle_Set(interpolationObject, interpolationStyle));
}



#pragma mark -

//=============================================================================
//      Q3HighlightStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3HighlightStyle_New(TQ3AttributeSet highlightAttribute)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on highlightAttribute
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3HighlightStyle_New(highlightAttribute));
}





//=============================================================================
//      Q3HighlightStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3HighlightStyle_Submit(TQ3AttributeSet highlightAttribute, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on highlightAttribute
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3HighlightStyle_Submit(highlightAttribute, view));
}





//=============================================================================
//      Q3HighlightStyle_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3HighlightStyle_Get(TQ3StyleObject highlight, TQ3AttributeSet *highlightAttribute)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(highlight, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(highlightAttribute), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on highlight
		return(kQ3Failure);

	if (0) // Further checks on highlightAttribute
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3HighlightStyle_Get(highlight, highlightAttribute));
}





//=============================================================================
//      Q3HighlightStyle_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3HighlightStyle_Set(TQ3StyleObject highlight, TQ3AttributeSet highlightAttribute)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(highlight, (kQ3ShapeTypeStyle)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on highlight
		return(kQ3Failure);

	if (0) // Further checks on highlightAttribute
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3HighlightStyle_Set(highlight, highlightAttribute));
}




#pragma mark -
//=============================================================================
//      Q3OrientationStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3OrientationStyle_New(TQ3OrientationStyle frontFacingDirection)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on frontFacingDirection
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3OrientationStyle_New(frontFacingDirection));
}





//=============================================================================
//      Q3OrientationStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrientationStyle_Submit(TQ3OrientationStyle frontFacingDirection, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on frontFacingDirection
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3OrientationStyle_Submit(frontFacingDirection, view));
}





//=============================================================================
//      Q3OrientationStyle_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrientationStyle_Get(TQ3StyleObject frontFacingDirectionObject, TQ3OrientationStyle *frontFacingDirection)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(frontFacingDirectionObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(frontFacingDirection), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on frontFacingDirectionObject
		return(kQ3Failure);

	if (0) // Further checks on frontFacingDirection
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3OrientationStyle_Get(frontFacingDirectionObject, frontFacingDirection));
}





//=============================================================================
//      Q3OrientationStyle_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrientationStyle_Set(TQ3StyleObject frontFacingDirectionObject, TQ3OrientationStyle frontFacingDirection)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(frontFacingDirectionObject, (kQ3ShapeTypeStyle)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on frontFacingDirectionObject
		return(kQ3Failure);

	if (0) // Further checks on frontFacingDirection
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3OrientationStyle_Set(frontFacingDirectionObject, frontFacingDirection));
}



#pragma mark -

//=============================================================================
//      Q3AntiAliasStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3AntiAliasStyle_New(const TQ3AntiAliasStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AntiAliasStyle_New(data));
}





//=============================================================================
//      Q3AntiAliasStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AntiAliasStyle_Submit(const TQ3AntiAliasStyleData *data, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AntiAliasStyle_Submit(data, view));
}





//=============================================================================
//      Q3AntiAliasStyle_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AntiAliasStyle_GetData(TQ3StyleObject styleObject, TQ3AntiAliasStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(styleObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on styleObject
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AntiAliasStyle_GetData(styleObject, data));
}





//=============================================================================
//      Q3AntiAliasStyle_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AntiAliasStyle_SetData(TQ3StyleObject styleObject, const TQ3AntiAliasStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(styleObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on styleObject
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AntiAliasStyle_SetData(styleObject, data));
}



#pragma mark -

//=============================================================================
//      Q3FogStyle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StyleObject
Q3FogStyle_New(const TQ3FogStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FogStyle_New(data));
}





//=============================================================================
//      Q3FogStyle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3FogStyle_Submit(const TQ3FogStyleData *data, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FogStyle_Submit(data, view));
}





//=============================================================================
//      Q3FogStyle_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3FogStyle_GetData(TQ3StyleObject styleObject, TQ3FogStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(styleObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on styleObject
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FogStyle_GetData(styleObject, data));
}





//=============================================================================
//      Q3FogStyle_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3FogStyle_SetData(TQ3StyleObject styleObject, const TQ3FogStyleData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(styleObject, (kQ3ShapeTypeStyle)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on styleObject
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FogStyle_SetData(styleObject, data));
}





