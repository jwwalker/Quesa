/*  NAME:
        E3GeometryEllipse.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryEllipse.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_ellipse_new : Ellipse new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipse_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3EllipseData			*instanceData = (TQ3EllipseData *)       privateData;
	const TQ3EllipseData	*ellipseData  = (const TQ3EllipseData *) paramData;
	TQ3Status					qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3EllipseData));
	
	qd3dStatus = Q3Ellipse_SetData(theObject, ellipseData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_ellipse_delete : Ellipse delete method.
//-----------------------------------------------------------------------------
static void
e3geom_ellipse_delete(TQ3Object theObject, void *privateData)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) privateData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Ellipse_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_ellipse_duplicate : Ellipse duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipse_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						 TQ3Object toObject,   void       *toPrivateData)
{	TQ3EllipseData		*toInstanceData = (TQ3EllipseData *) toPrivateData;
	TQ3Status			qd3dStatus;
	TQ3AttributeSet		dupSet;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Ellipse_GetData(fromObject, toInstanceData);
	
	if ( (qd3dStatus == kQ3Success) &&
		(toInstanceData->ellipseAttributeSet != NULL) )
	{
		dupSet = Q3Object_Duplicate( toInstanceData->ellipseAttributeSet );
		Q3Object_Dispose( toInstanceData->ellipseAttributeSet );
		toInstanceData->ellipseAttributeSet = dupSet;
		if (dupSet == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
	}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_ellipse_cache_new : Ellipse cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_ellipse_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3EllipseData *geomData)
{	float						theAngle, deltaAngle;
	TQ3SubdivisionStyleData		subdivisionData;
	TQ3Vertex3D					*theVertices;
	TQ3Uns32					numPoints, numSides, n;
	TQ3PolyLineData				polyLineData;
	TQ3GeometryObject			thePolyLine;
	TQ3Status					qd3dStatus;
	TQ3Vector3D					theVector;



	// Get the subdivision style, to figure out how many sides we should have.
	numSides  = 10;
	qd3dStatus = Q3View_GetSubdivisionStyleState(theView, &subdivisionData);
	if (qd3dStatus == kQ3Success)
		{
		switch (subdivisionData.method) {
			case kQ3SubdivisionMethodConstant:
				// For an ellipse, parameter c1 is the number of sides and c2 is unused
				numSides = (TQ3Uns32) subdivisionData.c1;
				break;
			
			case kQ3SubdivisionMethodWorldSpace:
				// Keep the length of any side less than or equal to c1
				{
					TQ3Matrix4x4	localToWorld;
					TQ3Vector3D		bigRadius;
					
					// Find the longer of the two radius vectors.
					bigRadius = geomData->majorRadius;
					if (Q3Vector3D_LengthSquared( &geomData->majorRadius ) <
						Q3Vector3D_LengthSquared( &geomData->minorRadius ) )
					{
						bigRadius = geomData->minorRadius;
					}

					// divide the circumference by c1
					Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
					Q3Vector3D_Transform( &bigRadius, &localToWorld, &theVector );
					numSides = (TQ3Uns32) ((kQ32Pi * Q3Vector3D_Length(&theVector))
							/ subdivisionData.c1);
				}
				break;

			case kQ3SubdivisionMethodScreenSpace:
				// Not implemented
				break;
			}
		}
	numSides = E3Num_Max(E3Num_Min(numSides, 256), 4);
	numPoints = numSides + 1;



	// Allocate the memory we need for the PolyLine
	theVertices = (TQ3Vertex3D *) Q3Memory_AllocateClear(numPoints * sizeof(TQ3Vertex3D));
	if (theVertices == NULL)
		return(NULL);



	// Define the ellipse, as a cosine/sine combination of major and minor radius vectors
	theAngle   = (geomData->uMin * kQ32Pi);
	deltaAngle = ((geomData->uMax - geomData->uMin) * kQ32Pi) / (float) numSides;

	for (n = 0; n < numPoints; n++)
		{
		Q3Vector3D_Scale(&geomData->majorRadius,  (float) cos(theAngle), &theVector);
		Q3Point3D_Vector3D_Add(&geomData->origin, &theVector, &theVertices[n].point);

		Q3Vector3D_Scale(&geomData->minorRadius,      (float) sin(theAngle), &theVector);
		Q3Point3D_Vector3D_Add(&theVertices[n].point, &theVector, &theVertices[n].point);

		theAngle += deltaAngle;
		}



	// Initialise the PolyLine data
	polyLineData.numVertices          = numPoints;
	polyLineData.vertices             = theVertices;
	polyLineData.segmentAttributeSet  = NULL;
	polyLineData.polyLineAttributeSet = geomData->ellipseAttributeSet;



	// Create the PolyLine and clean up
	thePolyLine = Q3PolyLine_New(&polyLineData);

	Q3Memory_Free(&theVertices);

	return(thePolyLine);
}





//=============================================================================
//      e3geom_ellipse_get_attribute : Ellipse get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_ellipse_get_attribute(TQ3GeometryObject theObject)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->ellipseAttributeSet);
}





//=============================================================================
//      e3geom_ellipse_metahandler : Ellipse metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_ellipse_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipse_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipse_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipse_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipse_cache_new;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipse_get_attribute;
			break;
		
		case kQ3XMethodTypeGeomUsesSubdivision:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryEllipse_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryEllipse_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeEllipse,
											kQ3ClassNameGeometryEllipse,
											e3geom_ellipse_metahandler,
											sizeof(TQ3EllipseData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryEllipse_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryEllipse_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeEllipse, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipse_New : Create an ellipse object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Ellipse_New(const TQ3EllipseData *ellipseData)
{	TQ3Object		theObject;


	if (ellipseData == NULL)
	{
		// This is not what the 3DMF specification claims to be the default, but it seems
		// to be what QD3D actually does.
		TQ3EllipseData	defaultEllipse = {
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			0.0f, 1.0f, NULL
		};
		theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeEllipse, kQ3False, &defaultEllipse);
	}
	else
	{
		theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeEllipse, kQ3False, ellipseData);
	}


	return(theObject);
}





//=============================================================================
//      E3Ellipse_Submit : Submit an ellipse.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_Submit(const TQ3EllipseData *ellipseData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeEllipse, ellipseData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipse_SetData : Set the data for an ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_SetData(TQ3GeometryObject theEllipse, const TQ3EllipseData *ellipseData)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theEllipse->instanceData;



	// Set the data
	instanceData->origin      = ellipseData->origin;
	instanceData->majorRadius = ellipseData->majorRadius;
	instanceData->minorRadius = ellipseData->minorRadius;
	instanceData->uMin   	  = ellipseData->uMin;
	instanceData->uMax   	  = ellipseData->uMax;

	E3Shared_Replace(&instanceData->ellipseAttributeSet, ellipseData->ellipseAttributeSet);

	Q3Shared_Edited(theEllipse);

	return(kQ3Success);
}





//=============================================================================
//      E3Ellipse_GetData : Get the data for an ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_GetData(TQ3GeometryObject theEllipse, TQ3EllipseData *ellipseData)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theEllipse->instanceData;



	// Get the data
	ellipseData->origin      = instanceData->origin;
	ellipseData->majorRadius = instanceData->majorRadius;
	ellipseData->minorRadius = instanceData->minorRadius;
	ellipseData->uMin   	  = instanceData->uMin;
	ellipseData->uMax   	  = instanceData->uMax;

	E3Shared_Acquire(&ellipseData->ellipseAttributeSet, instanceData->ellipseAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Ellipse_EmptyData : Empty the data for an ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_EmptyData(TQ3EllipseData *ellipseData)
{


	// Release the data
	Q3Object_CleanDispose(&ellipseData->ellipseAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Ellipse_SetOrigin : Set the origin for an ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_SetOrigin(TQ3GeometryObject theEllipse, const TQ3Point3D *origin)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theEllipse->instanceData;



	// Set the origin
	instanceData->origin = *origin;
	
	Q3Shared_Edited(theEllipse);

	return(kQ3Success);
}





//=============================================================================
//      E3Ellipse_SetMajorRadius : Set the major radius for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_SetMajorRadius(TQ3GeometryObject theEllipse, const TQ3Vector3D *majorRadius)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theEllipse->instanceData;



	// Set the major radius
	instanceData->majorRadius = *majorRadius;
	
	Q3Shared_Edited(theEllipse);

	return(kQ3Success);
}





//=============================================================================
//      E3Ellipse_SetMinorRadius : Set the minor radius for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_SetMinorRadius(TQ3GeometryObject theEllipse, const TQ3Vector3D *minorRadius)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theEllipse->instanceData;



	// Set the minor radius
	instanceData->minorRadius = *minorRadius;
	
	Q3Shared_Edited(theEllipse);

	return(kQ3Success);
}





//=============================================================================
//      E3Ellipse_GetOrigin : Get the origin for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_GetOrigin(TQ3GeometryObject theEllipse, TQ3Point3D *origin)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theEllipse->instanceData;



	// Get the origin
	*origin = instanceData->origin;

	return(kQ3Success);
}





//=============================================================================
//      E3Ellipse_GetMajorRadius : Get the major radius for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_GetMajorRadius(TQ3GeometryObject theEllipse, TQ3Vector3D *majorRadius)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theEllipse->instanceData;



	// Get the major radius
	*majorRadius = instanceData->majorRadius;

	return(kQ3Success);
}





//=============================================================================
//      E3Ellipse_GetMinorRadius : Get the minor radius for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_GetMinorRadius(TQ3GeometryObject theEllipse, TQ3Vector3D *minorRadius)
{	TQ3EllipseData		*instanceData = (TQ3EllipseData *) theEllipse->instanceData;



	// Get the minor radius
	*minorRadius = instanceData->minorRadius;

	return(kQ3Success);
}
