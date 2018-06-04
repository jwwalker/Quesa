/*  NAME:
        E3GeometryGeneralPolygon.c

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
#include "E3Set.h"
#include "E3Tessellate.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"
#include "E3GeometryGeneralPolygon.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3GeneralPolygon : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeGeneralPolygon, E3GeneralPolygon, E3Geometry )
public :

	TQ3GeneralPolygonData	instanceData ;

	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_generalpolygon_new : GeneralPolygon new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3GeneralPolygonData			*instanceData		= (TQ3GeneralPolygonData *)       privateData;
	const TQ3GeneralPolygonData		*generalPolygonData = (const TQ3GeneralPolygonData *) paramData;
	TQ3Status						qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3GeneralPolygonData));
	
	qd3dStatus = Q3GeneralPolygon_SetData(theObject, generalPolygonData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_generalpolygon_delete : GeneralPolygon delete method.
//-----------------------------------------------------------------------------
static void
e3geom_generalpolygon_delete(TQ3Object theObject, void *privateData)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	Q3GeneralPolygon_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_generalpolygon_copydata : Copy TQ3GeneralPolygonData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_copydata( const TQ3GeneralPolygonData* src,
	TQ3GeneralPolygonData* dst, TQ3Boolean isDuplicate )
{
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Uns32					contourIndex, vertexIndex;


	// Allocate some space for the new contours
	dst->contours = (TQ3GeneralPolygonContourData *)
		Q3Memory_Allocate(static_cast<TQ3Uns32>(src->numContours * sizeof(TQ3GeneralPolygonContourData)));
	if (dst->contours == NULL)
		return (kQ3Failure);

	
	// Copy non-contour fields
	dst->numContours = src->numContours;
	dst->shapeHint = src->shapeHint;
	if (src->generalPolygonAttributeSet == NULL)
	{
		dst->generalPolygonAttributeSet = NULL;
	}
	else if (isDuplicate)
	{
		dst->generalPolygonAttributeSet = Q3Object_Duplicate( src->generalPolygonAttributeSet );
		if (dst->generalPolygonAttributeSet == NULL)
			qd3dStatus = kQ3Failure;
	}
	else
	{
		E3Shared_Acquire( &dst->generalPolygonAttributeSet, src->generalPolygonAttributeSet );
	}
	
	
	// Copy contour data
	for (contourIndex = 0; contourIndex < dst->numContours; ++contourIndex)
	{
		dst->contours[ contourIndex ].numVertices = src->contours[ contourIndex ].numVertices;
		dst->contours[ contourIndex ].vertices = (TQ3Vertex3D *) Q3Memory_Allocate(
			static_cast<TQ3Uns32>(src->contours[ contourIndex ].numVertices * sizeof(TQ3Vertex3D)) );
		if (dst->contours[ contourIndex ].vertices == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
		else
		{
			for (vertexIndex = 0;
				vertexIndex < dst->contours[ contourIndex ].numVertices;
				++vertexIndex)
			{
				dst->contours[ contourIndex ].vertices[vertexIndex].point =
					src->contours[ contourIndex ].vertices[vertexIndex].point;
				
				if (src->contours[ contourIndex ].vertices[vertexIndex].attributeSet == NULL)
				{
					dst->contours[ contourIndex ].vertices[vertexIndex].attributeSet = NULL;
				}
				else if (isDuplicate)
				{
					dst->contours[ contourIndex ].vertices[vertexIndex].attributeSet =
						Q3Object_Duplicate(
							src->contours[ contourIndex ].vertices[vertexIndex].attributeSet );
					
					if (dst->contours[ contourIndex ].vertices[vertexIndex].attributeSet == NULL)
						qd3dStatus = kQ3Failure;
				}
				else
				{
					E3Shared_Acquire( &dst->contours[ contourIndex ].vertices[vertexIndex].attributeSet,
						src->contours[ contourIndex ].vertices[vertexIndex].attributeSet );
				}
			}
		}
	}
	
	
	
	// Clean up after failure
	if (qd3dStatus == kQ3Failure)
	{
		E3GeneralPolygon_EmptyData( dst );
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_generalpolygon_duplicate : GeneralPolygon duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  		TQ3Object toObject,   void       *toPrivateData)
{	TQ3GeneralPolygonData		*toInstanceData = (TQ3GeneralPolygonData *) toPrivateData;
	TQ3GeneralPolygonData		*fromInstanceData = (TQ3GeneralPolygonData *) fromPrivateData;
	TQ3Status					qd3dStatus;
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = e3geom_generalpolygon_copydata( fromInstanceData, toInstanceData, kQ3True );


	return(qd3dStatus);
}





//=============================================================================
//      e3geom_generalpolygon_cache_new : GeneralPolygon cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_generalpolygon_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3GeneralPolygonData *geomData)
{	TQ3OrientationStyle		theOrientation;
	TQ3Contour				*theContours;
	TQ3GeometryObject		theTriMesh;
#pragma unused(theGeom)



	// Obtain the contour data
	//
	// For now we can simply cast between the two structures, since they are identical - we
	// assert this to make sure, since TQ3Contour is internal at present and so may change.
	Q3_REQUIRE_OR_RESULT(sizeof(TQ3Contour) == sizeof(TQ3GeneralPolygonContourData), NULL);
	theContours = (TQ3Contour *) geomData->contours;



	// Tessellate the polygon
	theTriMesh = E3Tessellate_Contours(geomData->numContours, theContours, geomData->generalPolygonAttributeSet);	
	if (theTriMesh != NULL)
		{
		theOrientation = E3View_State_GetStyleOrientation(theView);
		E3TriMesh_AddTriangleNormals(theTriMesh, theOrientation);
		}

	return(theTriMesh);
}





//=============================================================================
//      e3geom_generalpolygon_bounds : GeneralPolygon bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3GeneralPolygonData			*instanceData = (const TQ3GeneralPolygonData *) objectData;
	TQ3Uns32							n;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds from our contours - although general polys are normally
	// defined as an outer polygon with interior holes, the contours are allowed
	// to be disjoint and so we need to accumulate each contour in turn.
	for (n = 0; n < instanceData->numContours; n++)
		E3View_UpdateBounds(theView,
							instanceData->contours[n].numVertices,
							sizeof(TQ3Vertex3D),
							&instanceData->contours[n].vertices[0].point);

	return(kQ3Success);
}





//=============================================================================
//      e3geom_generalpolygon_get_attribute : GeneralPolygon get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_generalpolygon_get_attribute ( E3GeneralPolygon* generalPolygon )
	{
	// Return the address of the geometry attribute set
	return & generalPolygon->instanceData.generalPolygonAttributeSet ;
	}





//=============================================================================
//      e3geom_generalpolygon_metahandler : GeneralPolygon metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_generalpolygon_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_get_attribute;
			break;

		case kQ3XMethodTypeGeomUsesOrientation:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryGeneralPolygon_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryGeneralPolygon_RegisterClass(void)
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryGeneralPolygon,
								e3geom_generalpolygon_metahandler,
								E3GeneralPolygon ) ;
	}





//=============================================================================
//      E3GeometryGeneralPolygon_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryGeneralPolygon_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeGeneralPolygon, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3GeneralPolygon_New : Create a general polygon object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3GeneralPolygon_New(const TQ3GeneralPolygonData *generalPolygonData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeGeneralPolygon, kQ3False, generalPolygonData);
	return(theObject);
}





//=============================================================================
//      E3GeneralPolygon_Submit : Submit a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_Submit(const TQ3GeneralPolygonData *generalPolygonData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeGeneralPolygon, generalPolygonData);
	return(qd3dStatus);
}





//=============================================================================
//      E3GeneralPolygon_SetData : Set the data fora general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetData(TQ3GeometryObject theGeneralPolygon, const TQ3GeneralPolygonData *generalPolygonData)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;
	
	E3GeneralPolygon_EmptyData ( & generalPolygon->instanceData ) ;
	
	TQ3Status qd3dStatus = e3geom_generalpolygon_copydata ( generalPolygonData, & generalPolygon->instanceData, kQ3False ) ;


	Q3Shared_Edited ( generalPolygon ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3GeneralPolygon_GetData : Get the data for a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetData(TQ3GeometryObject theGeneralPolygon, TQ3GeneralPolygonData *generalPolygonData)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;

	return e3geom_generalpolygon_copydata ( & generalPolygon->instanceData, generalPolygonData, kQ3False ) ;
	}





//=============================================================================
//      E3GeneralPolygon_EmptyData : Release the data for a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_EmptyData(TQ3GeneralPolygonData *generalPolygonData)
{	TQ3Uns32		n, m;



	// Release the data
	for (n = 0; n < generalPolygonData->numContours; n++)
		{
		for (m = 0; m < generalPolygonData->contours[n].numVertices; m++)
			Q3Object_CleanDispose(&generalPolygonData->contours[n].vertices[m].attributeSet);

		Q3Memory_Free(&generalPolygonData->contours[n].vertices);
		}

	Q3Memory_Free(&generalPolygonData->contours);
	Q3Object_CleanDispose(&generalPolygonData->generalPolygonAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_GetNumContours : Get the number of contours.
//-----------------------------------------------------------------------------
TQ3Uns32
E3GeneralPolygon_GetNumContours(TQ3GeometryObject theGeneralPolygon)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;
	
	return generalPolygon->instanceData.numContours ;
	}





//=============================================================================
//      E3GeneralPolygon_GetNumVertices : Get the number of vertices in specified contour.
//-----------------------------------------------------------------------------
TQ3Uns32
E3GeneralPolygon_GetNumVertices(TQ3GeometryObject theGeneralPolygon, TQ3Uns32 contourIndex)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;
	
	if ( contourIndex >= generalPolygon->instanceData.numContours )
		return 0 ;
		
	return generalPolygon->instanceData.contours [ contourIndex ].numVertices ;
	}





//=============================================================================
//      E3GeneralPolygon_GetVertexPosition : Get the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetVertexPosition(TQ3GeometryObject theGeneralPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3Point3D *position)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;
	
	if ( contourIndex >= generalPolygon->instanceData.numContours
	|| pointIndex >= generalPolygon->instanceData.contours [ contourIndex ].numVertices )
		return kQ3Failure ;

	// Get the vertex position
	*position = generalPolygon->instanceData.contours[contourIndex].vertices [ pointIndex ].point ;

	return kQ3Success ;
	}





//=============================================================================
//      E3GeneralPolygon_SetVertexPosition : Set the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetVertexPosition(TQ3GeometryObject theGeneralPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, const TQ3Point3D *position)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;

	if ( contourIndex >= generalPolygon->instanceData.numContours
	|| pointIndex >= generalPolygon->instanceData.contours [ contourIndex ].numVertices )
		return kQ3Failure ;

	// Set the vertex position
	generalPolygon->instanceData.contours[contourIndex].vertices [ pointIndex ].point = *position ;

	Q3Shared_Edited ( generalPolygon ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3GeneralPolygon_GetVertexAttributeSet : Get vertex attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetVertexAttributeSet(TQ3GeometryObject theGeneralPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet *attributeSet)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;

	if ( contourIndex >= generalPolygon->instanceData.numContours
	|| pointIndex >= generalPolygon->instanceData.contours [ contourIndex ].numVertices )
		return kQ3Failure ;

	// Get the vertex attribute set
	E3Shared_Acquire ( attributeSet, generalPolygon->instanceData.contours [ contourIndex ].vertices [ pointIndex ].attributeSet ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3GeneralPolygon_SetVertexAttributeSet : Set vertex attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetVertexAttributeSet(TQ3GeometryObject theGeneralPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet attributeSet)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;

	if ( contourIndex >= generalPolygon->instanceData.numContours
	|| pointIndex >= generalPolygon->instanceData.contours [ contourIndex ].numVertices )
		return kQ3Failure ;

	// Set the vertex attribute set
	E3Shared_Replace ( & generalPolygon->instanceData.contours [ contourIndex ].vertices [ pointIndex ].attributeSet, attributeSet ) ;

	Q3Shared_Edited ( generalPolygon ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3GeneralPolygon_GetShapeHint : Get the polygon shape hint.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetShapeHint(TQ3GeometryObject theGeneralPolygon, TQ3GeneralPolygonShapeHint *shapeHint)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;

	// Get the shape hint
	*shapeHint = generalPolygon->instanceData.shapeHint ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3GeneralPolygon_SetShapeHint : Set the polygon shape hint.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetShapeHint(TQ3GeometryObject theGeneralPolygon, TQ3GeneralPolygonShapeHint shapeHint)
	{
	E3GeneralPolygon* generalPolygon = (E3GeneralPolygon*) theGeneralPolygon ;

	// Set the shape hint
	generalPolygon->instanceData.shapeHint = shapeHint ;
	
	Q3Shared_Edited ( generalPolygon ) ;

	return kQ3Success ;
	}


