/*  NAME:
        E3GeometryEllipse.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

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
#include "E3Geometry.h"
#include "E3GeometryEllipse.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Ellipse : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeEllipse, E3Ellipse, E3Geometry )
public :

	TQ3EllipseData			instanceData ;

	} ;
	


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
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Ellipse_EmptyData(instanceData);
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
			
			default:
				Q3_ASSERT(!"Unknown subdivision method");
				break;
			}
		}
	numSides = E3Num_Clamp(numSides, 4, 256);
	numPoints = numSides + 1;



	// Allocate the memory we need for the PolyLine
	theVertices = (TQ3Vertex3D *) Q3Memory_AllocateClear(static_cast<TQ3Uns32>(numPoints * sizeof(TQ3Vertex3D)));
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
e3geom_ellipse_get_attribute ( E3Ellipse* ellipse )
	{	
	// Return the address of the geometry attribute set
	return & ellipse->instanceData.ellipseAttributeSet ;
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
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryEllipse,
								e3geom_ellipse_metahandler,
								E3Ellipse ) ;
	}





//=============================================================================
//      E3GeometryEllipse_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryEllipse_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeEllipse, kQ3True);
	
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
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeEllipse, kQ3False, &defaultEllipse ) ;
	}
	else
	{
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeEllipse, kQ3False, ellipseData ) ;
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
	{
	E3Ellipse* ellipse = (E3Ellipse*) theEllipse ;

	// Set the data
	ellipse->instanceData.origin      = ellipseData->origin ;
	ellipse->instanceData.majorRadius = ellipseData->majorRadius ;
	ellipse->instanceData.minorRadius = ellipseData->minorRadius ;
	ellipse->instanceData.uMin   	  = ellipseData->uMin ;
	ellipse->instanceData.uMax   	  = ellipseData->uMax ;

	E3Shared_Replace ( & ellipse->instanceData.ellipseAttributeSet, ellipseData->ellipseAttributeSet ) ;

	Q3Shared_Edited ( ellipse ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipse_GetData : Get the data for an ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_GetData(TQ3GeometryObject theEllipse, TQ3EllipseData *ellipseData)
	{
	E3Ellipse* ellipse = (E3Ellipse*) theEllipse ;

	// Get the data
	ellipseData->origin      = ellipse->instanceData.origin ;
	ellipseData->majorRadius = ellipse->instanceData.majorRadius ;
	ellipseData->minorRadius = ellipse->instanceData.minorRadius ;
	ellipseData->uMin   	  = ellipse->instanceData.uMin ;
	ellipseData->uMax   	  = ellipse->instanceData.uMax ;

	E3Shared_Acquire ( & ellipseData->ellipseAttributeSet, ellipse->instanceData.ellipseAttributeSet ) ;

	return kQ3Success ;
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
	{
	E3Ellipse* ellipse = (E3Ellipse*) theEllipse ;

	// Set the origin
	ellipse->instanceData.origin = *origin ;
	
	Q3Shared_Edited ( ellipse ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipse_SetMajorRadius : Set the major radius for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_SetMajorRadius(TQ3GeometryObject theEllipse, const TQ3Vector3D *majorRadius)
	{
	E3Ellipse* ellipse = (E3Ellipse*) theEllipse ;

	// Set the major radius
	ellipse->instanceData.majorRadius = *majorRadius ;
	
	Q3Shared_Edited ( ellipse ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipse_SetMinorRadius : Set the minor radius for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_SetMinorRadius(TQ3GeometryObject theEllipse, const TQ3Vector3D *minorRadius)
	{
	E3Ellipse* ellipse = (E3Ellipse*) theEllipse ;

	// Set the minor radius
	ellipse->instanceData.minorRadius = *minorRadius ;
	
	Q3Shared_Edited ( ellipse ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipse_GetOrigin : Get the origin for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_GetOrigin(TQ3GeometryObject theEllipse, TQ3Point3D *origin)
	{
	E3Ellipse* ellipse = (E3Ellipse*) theEllipse ;

	// Get the origin
	*origin = ellipse->instanceData.origin ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipse_GetMajorRadius : Get the major radius for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_GetMajorRadius(TQ3GeometryObject theEllipse, TQ3Vector3D *majorRadius)
	{
	E3Ellipse* ellipse = (E3Ellipse*) theEllipse ;

	// Get the major radius
	*majorRadius = ellipse->instanceData.majorRadius ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipse_GetMinorRadius : Get the minor radius for a ellipse object.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipse_GetMinorRadius(TQ3GeometryObject theEllipse, TQ3Vector3D *minorRadius)
	{
	E3Ellipse* ellipse = (E3Ellipse*) theEllipse ;

	// Get the minor radius
	*minorRadius = ellipse->instanceData.minorRadius ;

	return kQ3Success ;
	}
