/*  NAME:
        E3Utils.c

    DESCRIPTION:
        Quesa utility functions.

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
#include <ctype.h>

#include "E3Prefix.h"
#include "E3Set.h"
#include "E3GeometryTriMesh.h"
#include "E3Utils.h"
#include "E3View.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Line clipping op-codes
#define kClipAccept												0x00
#define kClipLeft												0x01
#define kClipRight												0x02
#define kClipBottom												0x04
#define kClipTop												0x08





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3clip_calc_opcode : Calculate a Cohen-Sutherland clipping code.
//-----------------------------------------------------------------------------
static TQ3Uns8
e3clip_calc_opcode(const TQ3Area *theRect, float x, float y)
{	TQ3Uns8		clipCode;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRect), kClipAccept);



	// Calculate the clipping op-code
	clipCode = kClipAccept;

	if (y < theRect->min.y)
		clipCode = kClipTop;
   else if (y > theRect->max.y)
		clipCode = kClipBottom;

	if (x < theRect->min.x)
		clipCode += kClipLeft;
	else if (x > theRect->max.x)
		clipCode += kClipRight;
      
   return clipCode;
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Shared_Acquire : Acquire a new reference to a shared object.
//-----------------------------------------------------------------------------
//		Note :	We initialise *newRef to NULL (if theObject is NULL), or a
//				new reference to theObject.
//-----------------------------------------------------------------------------
void
E3Shared_Acquire(TQ3SharedObject *newRef, TQ3SharedObject theObject)
	{


	// Validate our parameters
	Q3_REQUIRE ( Q3_VALID_PTR ( newRef ) ) ;
	*newRef = NULL ;
	if ( theObject != NULL )
		{
		Q3_REQUIRE ( theObject->IsObjectValid () ) ;
		Q3_REQUIRE ( Q3_OBJECT_IS_CLASS ( theObject, E3Shared ) ) ;



	// Acquire a new reference
		*newRef = ( (E3Shared*) theObject )->GetReference () ;
		Q3_ASSERT_VALID_PTR ( *newRef ) ;
		}
	}





//=============================================================================
//      E3Shared_Replace : Get a new reference to a shared object.
//-----------------------------------------------------------------------------
//		Note :	We obtain a new reference to newObject, and replace any
//				existing reference in origObject with the new reference.
//
//				Note that acquiring a new reference can never fail, so we do
//				not need to provide a return value.
//-----------------------------------------------------------------------------
void
E3Shared_Replace(TQ3SharedObject *origObject, TQ3SharedObject newObject)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(origObject));
	if (newObject != NULL)
		Q3_REQUIRE( Q3_OBJECT_IS_CLASS ( newObject, E3Shared ) ) ;



	// Do nothing if we're getting a reference to the current object
	if (*origObject == newObject)
		return;



	// Dispose of any existing reference
	if (*origObject != NULL)
		Q3Object_Dispose(*origObject);



	// Acquire a new reference
	E3Shared_Acquire(origObject, newObject);
}





//=============================================================================
//      E3SBitmap_Replace :	Copy the bitmap pointed to by original into the
//							bitmap pointed to by copy.
//-----------------------------------------------------------------------------
//		Note :	Will only call Q3Bitmap_Empty on copy if we own copy
//-----------------------------------------------------------------------------
TQ3Status E3Bitmap_Replace(const TQ3Bitmap *original, TQ3Bitmap *copy, TQ3Boolean isCopyInternal)
{
	void *			originalData;
	TQ3Uns32		originalSize = original->height * original->rowBytes;
	
	
	
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(original), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(copy),     kQ3Failure);



	// Dispose of any existing bitmap copy may point to
	if ( isCopyInternal == kQ3True )
	{
		Q3Bitmap_Empty( copy );
	}
	
	// Copy the bitmap fields from original to copy
	copy->image    = NULL;
	copy->width    = original->width;
	copy->height   = original->height;
	copy->rowBytes = original->rowBytes;
	copy->bitOrder = original->bitOrder;


	// Copy the bitmap image from original to copy
	if ( originalSize != 0 )
	{
		originalData = Q3Memory_Allocate( originalSize ) ;

		if (originalData == NULL)
		{
			return(kQ3Failure);						
		}
		
		Q3Memory_Copy(original->image, originalData, originalSize );
		copy->image = (unsigned char *) originalData;
	}
			
	return(kQ3Success);
}





//=============================================================================
//      E3AttributeSet_Combine : Combine two attribute sets.
//-----------------------------------------------------------------------------
//		Note :	Our internal wrapper around Q3AttributeSet_Inherit, to allow
//				us to handle NULL parent or child attribute sets.
//-----------------------------------------------------------------------------
void E3AttributeSet_Combine(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet *result)
{


	// Initialise a return value
	*result = NULL;



	// Combine the attribute sets	
	if (parent != NULL)
		{
		// We have both a parent and child -- combine them
		if (child != NULL)
			{
			*result = Q3AttributeSet_New();
			if (*result != NULL)
				Q3AttributeSet_Inherit(parent, child, *result);
			}
		
		// Parent only, so just get another reference
		else
			E3Shared_Acquire(result, parent);
		}


	// Child only, so just get another reference
	else
		E3Shared_Acquire(result, child);
}





//=============================================================================
//      E3Geometry_AddNormalIndicators : Add normal indicators to a geometry.
//-----------------------------------------------------------------------------
//		Note :	Useful when debugging implicit surfaces, lets you check
//				visually that everything is pointing in the right direction.
//-----------------------------------------------------------------------------
void E3Geometry_AddNormalIndicators(TQ3GroupObject group, TQ3Uns32 numPoints,
									const TQ3Point3D *points, const TQ3Vector3D *normals)
{	TQ3ColorRGB			color = {1.0f, 1.0f, 0.0f};
	TQ3LineData			lineData;
	TQ3GeometryObject	theLine;
	TQ3Uns32			i;
	TQ3Vector3D			v;



	// Set up the attributes
	lineData.vertices[0].attributeSet = NULL;
	lineData.vertices[1].attributeSet = NULL;
	lineData.lineAttributeSet         = Q3AttributeSet_New();

	if (lineData.lineAttributeSet != NULL)
		Q3AttributeSet_Add(lineData.lineAttributeSet, kQ3AttributeTypeDiffuseColor, &color);



	// Add the normal indicators
	for (i=0; i<numPoints; i++) {
		lineData.vertices[0].point = points[i];
		Q3Vector3D_Scale( &normals[i], 0.5f, &v );
		Q3Point3D_Vector3D_Add( &points[i], &v, &lineData.vertices[1].point );
		theLine = Q3Line_New(&lineData);
		Q3Group_AddObject( group, theLine );
	} 



	// Clean up
	if (lineData.lineAttributeSet != NULL)
		Q3Object_Dispose(lineData.lineAttributeSet);
}





//=============================================================================
//      E3Matrix4x4_IsIdentity : Is a TQ3Matrix4x4 an identity matrix?
//-----------------------------------------------------------------------------
TQ3Boolean	E3Matrix4x4_IsIdentity(const TQ3Matrix4x4 *theMatrix)
{	static const TQ3Matrix4x4	identityMatrix = { { { 1.0f, 0.0f, 0.0f, 0.0f},
												     { 0.0f, 1.0f, 0.0f, 0.0f},
												     { 0.0f, 0.0f, 1.0f, 0.0f},
												     { 0.0f, 0.0f, 0.0f, 1.0f} } };
	TQ3Boolean					isIdentity;



	// Check the matrix
	isIdentity = (TQ3Boolean) (memcmp(&identityMatrix, theMatrix, sizeof(TQ3Matrix4x4)) == 0);
	
	return(isIdentity);

}





//=============================================================================
//      E3String_IsEqual : Is a str_a case insensitive equal to str_b?
//-----------------------------------------------------------------------------
TQ3Boolean	E3CString_IsEqual(const char *str_a, const char *str_b)
{


	// Compare the strings
	while ((*str_a != '\0') && (*str_b != '\0'))
		{
		if (toupper(*str_a++) != toupper(*str_b++))
			return(kQ3False);
		}

	return((TQ3Boolean) (*str_a == *str_b));
}





//=============================================================================
//      E3Rect_ClipLine : Clip a line to a rectangle.
//-----------------------------------------------------------------------------
//		Note :	Returns the clipped line, and indicates if the line intersects
//				the rectangle or not.
//-----------------------------------------------------------------------------
TQ3Boolean E3Rect_ClipLine(const TQ3Area *theRect, TQ3Point2D *lineStart, TQ3Point2D *lineEnd)
{	TQ3Uns8			code0, code1, codeOutside;
	TQ3Boolean		areDone, doesIntersect;
	float			x, y;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRect),   kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineStart), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineEnd),   kQ3False);



	// Calculate the initial clipping codes
	code0 = e3clip_calc_opcode(theRect, lineStart->x, lineStart->y);
	code1 = e3clip_calc_opcode(theRect, lineEnd->x,   lineEnd->y);



	// Clip the line
	areDone       = kQ3False;
	doesIntersect = kQ3False;

	while (!areDone)
		{
		// Check for trivial acceptance - both endpoints are inside
		if (code0 == kClipAccept && code1 == kClipAccept)
			{
			areDone       = kQ3True;
			doesIntersect = kQ3True;
			}


		// Check for trivial reject - both endpoints are now outside
		else if ((code0 & code1) != 0)
			{
			areDone       = kQ3True;
			doesIntersect = kQ3False;
			}


		// Otherwise, clip the line segment
		else
			{
			// Find the outside point
			if (code0 != kClipAccept)
				codeOutside = code0;
			else
				codeOutside = code1;


			// Find the intersection point with the clipping rectangle
			if (E3Bit_IsSet(codeOutside, kClipTop))
				{
				x = lineStart->x + (lineEnd->x - lineStart->x) * (theRect->min.y - lineStart->y) / (lineEnd->y - lineStart->y);
				y = theRect->min.y;
				}
			else if (E3Bit_IsSet(codeOutside, kClipBottom))
				{
				x = lineStart->x + (lineEnd->x - lineStart->x) * (theRect->max.y - lineStart->y) / (lineEnd->y - lineStart->y);
				y = theRect->max.y;
				}
			else if (E3Bit_IsSet(codeOutside, kClipRight))
				{
				y = lineStart->y + (lineEnd->y - lineStart->y) * (theRect->max.x - lineStart->x) / (lineEnd->x - lineStart->x);
				x = theRect->max.x;
				}
			else if (E3Bit_IsSet(codeOutside, kClipLeft))
				{
				y = lineStart->y + (lineEnd->y - lineStart->y) * (theRect->min.x - lineStart->x) / (lineEnd->x - lineStart->x);
				x = theRect->min.x;
				}


			// Clip the outside point in to the intersection poin
			if (codeOutside == code0)
				{
				lineStart->x = x;
				lineStart->y = y;
				code0       = e3clip_calc_opcode(theRect, x, y);
				}
			else
				{
				lineEnd->x = x;
				lineEnd->y = y;
				code1     = e3clip_calc_opcode(theRect, x, y);
				}
			}
		}



	// Return as we have an intersection
	return(doesIntersect);
}





//=============================================================================
//      E3Rect_ContainsLine : Does a rectangle contain a line?
//-----------------------------------------------------------------------------
TQ3Boolean E3Rect_ContainsLine(const TQ3Area *theRect, const TQ3Point2D *lineStart, const TQ3Point2D *lineEnd)
{	TQ3Point2D			clipStart, clipEnd;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRect),   kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineStart), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineEnd),   kQ3False);



	// Take a copy of the line for clipping
	clipStart = *lineStart;
	clipEnd   = *lineEnd;



	// Clip the line to test for intersection
	return(E3Rect_ClipLine(theRect, &clipStart, &clipEnd));
}





//=============================================================================
//      E3Rect_ContainsRect : Test to see if rect1 is contained within rect2.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Rect_ContainsRect(const TQ3Area *rect1, const TQ3Area *rect2)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect1), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect2), kQ3False);



	// Check we're inside in x
	if ((rect1->min.x < rect2->min.x || rect1->min.x > rect2->max.x) ||
		(rect1->max.x < rect2->min.x || rect1->max.x > rect2->max.x))
		return(kQ3False);



	// Check we're inside in y
	if ((rect1->min.y < rect2->min.y || rect1->min.y > rect2->max.y) ||
		(rect1->max.y < rect2->min.y || rect1->max.y > rect2->max.y))
		return(kQ3False);
	
	return(kQ3True);
}





//=============================================================================
//      E3Rect_IntersectRect : Test to see if two rects overlap.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Rect_IntersectRect(const TQ3Area *rect1, const TQ3Area *rect2)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect1), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect2), kQ3False);



	// Check for overlaps in x
	if ((rect1->min.x >= rect2->min.x || rect1->min.x <= rect2->max.x) ||
		(rect1->max.x >= rect2->min.x || rect1->max.x <= rect2->max.x))
		return(kQ3True);



	// Check for overlaps in y
	if ((rect1->min.y >= rect2->min.y || rect1->min.x <= rect2->max.y) ||
		(rect1->max.y >= rect2->min.y || rect1->max.x <= rect2->max.y))
		return(kQ3True);
	
	return(kQ3False);
}





//=============================================================================
//      E3Triangle_InterpolateHit : Interpolate a hit within a triangle.
//-----------------------------------------------------------------------------
//		Note :	Given the barycentric coordinates (the u and v components of
//				theHit) of a point within a triangle we interpolate to obtain
//				the XYZ, normal, and UV (if we can) that correspond to that
//				coordinate.
//-----------------------------------------------------------------------------
void
E3Triangle_InterpolateHit (	TQ3ViewObject			theView,
							const TQ3TriangleData	*theTriangle,
							const TQ3Param3D		*theHit,
							TQ3Point3D				*hitXYZ,
							TQ3Vector3D				*hitNormal,
							TQ3Param2D				*hitUV,
							TQ3Boolean				*haveUV)
{	TQ3Vector3D			*theNormal, triNormal;
	TQ3Vector3D			theNormals[3];
	TQ3Point3D			thePoints[3];
	float				oneMinusUV;
	TQ3Param2D			theUVs[3];
	TQ3AttributeSet		theSet;
	TQ3Uns32			n;
	TQ3Matrix4x4		localToWorldForNormals ;



	// Validate our parameters
 	Q3_REQUIRE(Q3_VALID_PTR(theTriangle));
	Q3_REQUIRE(Q3_VALID_PTR(theHit));
	Q3_REQUIRE(Q3_VALID_PTR(hitXYZ));
	Q3_REQUIRE(Q3_VALID_PTR(hitNormal));
	Q3_REQUIRE(Q3_VALID_PTR(hitUV));
	Q3_REQUIRE(Q3_VALID_PTR(haveUV));



	// Calculate the triangle normal, then override it with any attribute
	Q3Point3D_CrossProductTri(&theTriangle->vertices[0].point,
							  &theTriangle->vertices[1].point,
							  &theTriangle->vertices[2].point,
							  &triNormal);
	Q3Vector3D_Normalize(&triNormal, &triNormal);

	localToWorldForNormals = *E3View_State_GetMatrixLocalToWorld(theView) ;
	
	// See 'Computer Graphics principals and practice' second edition page 1108
	localToWorldForNormals.value [ 3 ] [ 0 ] = 0.0f ;
	localToWorldForNormals.value [ 3 ] [ 1 ] = 0.0f ;
	localToWorldForNormals.value [ 3 ] [ 2 ] = 0.0f ;
	localToWorldForNormals.value [ 3 ] [ 3 ] = 1.0f ;
	Q3Matrix4x4_Transpose ( &localToWorldForNormals , &localToWorldForNormals ) ;
	Q3Matrix4x4_Invert ( &localToWorldForNormals , &localToWorldForNormals ) ;

	if (theTriangle->triangleAttributeSet != NULL)
		{
		theNormal = (TQ3Vector3D *) Q3XAttributeSet_GetPointer(theTriangle->triangleAttributeSet, kQ3AttributeTypeNormal);

		if (theNormal != NULL)
			Q3Vector3D_Transform(theNormal, &localToWorldForNormals, &triNormal) ;
		}



	// Collect the state we need from the triangle
	*haveUV = kQ3True;
	for (n = 0; n < 3; n++)
		{
		// Get the point and normal
		thePoints[n]  = theTriangle->vertices[n].point;
		theNormals[n] = triNormal;


		// If we have an attribute set, try and override the normal/UV
		theSet = theTriangle->vertices[n].attributeSet;
		if (theSet != NULL)
			{
			// Override normal
			if ( Q3AttributeSet_Get(theSet, kQ3AttributeTypeNormal, &theNormals[n]) != kQ3Failure )
				Q3Vector3D_Transform(&theNormals[n], &localToWorldForNormals, &theNormals[n]) ;
			
			// Override UV
			if (Q3AttributeSet_Get(theSet, kQ3AttributeTypeSurfaceUV, &theUVs[n]) != kQ3Success)
				{
				if (Q3AttributeSet_Get(theSet, kQ3AttributeTypeShadingUV, &theUVs[n]) != kQ3Success)
					*haveUV = kQ3False;
				}
			}
		else
			*haveUV = kQ3False;
		}



	// Interpolate between the vertices to get our results
	oneMinusUV = (1.0f - theHit->u - theHit->v);
	
	hitXYZ->x = (thePoints[0].x * oneMinusUV) + (thePoints[1].x * theHit->u) + (thePoints[2].x * theHit->v);
	hitXYZ->y = (thePoints[0].y * oneMinusUV) + (thePoints[1].y * theHit->u) + (thePoints[2].y * theHit->v);
	hitXYZ->z = (thePoints[0].z * oneMinusUV) + (thePoints[1].z * theHit->u) + (thePoints[2].z * theHit->v);

	hitNormal->x = (theNormals[0].x * oneMinusUV) + (theNormals[1].x * theHit->u) + (theNormals[2].x * theHit->v);
	hitNormal->y = (theNormals[0].y * oneMinusUV) + (theNormals[1].y * theHit->u) + (theNormals[2].y * theHit->v);
	hitNormal->z = (theNormals[0].z * oneMinusUV) + (theNormals[1].z * theHit->u) + (theNormals[2].z * theHit->v);

	if (*haveUV)
		{
		hitUV->u = (theUVs[0].u * oneMinusUV) + (theUVs[1].u * theHit->u) + (theUVs[2].u * theHit->v);
		hitUV->v = (theUVs[0].v * oneMinusUV) + (theUVs[1].v * theHit->u) + (theUVs[2].v * theHit->v);
		}
}





//=============================================================================
//      E3TriMeshAttribute_GatherArray : Gather TriMesh attribute data.
//-----------------------------------------------------------------------------
//		Note :	Given an array of attribute sets and an attribute type, we
//				attempt to collect the attribute data from each set and return
//				as any of the sets contained the attribute.
//
//				If only some sets contain the attribute, we allocate a usage
//				array to indicate which ones do.
//
//
//				Because the attribute sets are held in several different forms
//				by callers of this routine, they are accessed by a callback.
//				This takes an index from 0..(numSets-1), and returns the
//				appropriate attribute set.
//
//				The callback should not increment the reference count of the
//				attribute set, as we do not decrement it.
//-----------------------------------------------------------------------------
TQ3Boolean
E3TriMeshAttribute_GatherArray(TQ3Uns32						numSets,
								E3GetSetForGatherProc		userCallback,
								const void					*userData,
								TQ3TriMeshAttributeData		*theAttribute,
								TQ3AttributeType			attributeType)
{	TQ3Uns32			n;
	TQ3AttributeSet		theSet;



	// Find out how large the data for each attribute is
	E3ClassInfoPtr theClass = E3ClassTree::GetClass  (E3Attribute_AttributeToClassType ( attributeType ) ) ;
	if ( theClass == NULL )
		return kQ3False ;

	TQ3Uns32 attributeSize = theClass->GetInstanceSize () ;



	// Scan the attribute sets to determine if this attribute is present
	TQ3Uns32 numPresent = 0 ;
	for (n = 0; n < numSets; n++)
		{
		theSet = userCallback(userData, n);
		if (theSet != NULL && Q3AttributeSet_Contains(theSet, attributeType))
			numPresent++;
		}

	if (numPresent == 0)
		return(kQ3False);



	// Allocate the attribute arrays
	theAttribute->attributeType     = attributeType;
	theAttribute->data              = Q3Memory_AllocateClear(numSets * attributeSize);
	theAttribute->attributeUseArray = NULL;
	
	if (theAttribute->data == NULL)
		return(kQ3False);

	if (numPresent != numSets)
		{
		theAttribute->attributeUseArray = (char *) Q3Memory_AllocateClear(numSets * sizeof(char));
		if (theAttribute->attributeUseArray == NULL)
			{
			Q3Memory_Free(&theAttribute->data);
			return(kQ3False);
			}
		}



	// Initialise the attribute arrays
	for (n = 0; n < numSets; n++)
		{
		// Grab the attribute data if it's present
		theSet    = userCallback(userData, n);
		TQ3Boolean isPresent = (TQ3Boolean) (theSet != NULL && Q3AttributeSet_Contains(theSet, attributeType));
		if (isPresent)
			{
			void* dataPtr = ( (TQ3Uns8 *) theAttribute->data ) + ( n * attributeSize ) ;
			Q3AttributeSet_Get(theSet, attributeType, dataPtr);
			}


		// Set up the use array if required
		if (theAttribute->attributeUseArray != NULL)
			theAttribute->attributeUseArray[n] = (char) isPresent;
		}
	
	return(kQ3True);
}





//=============================================================================
//      E3TriMesh_BuildOrientationGroup : Build a TriMesh in a group.
//-----------------------------------------------------------------------------
//		Note :	Given a TriMesh object, we add triangle normals in a particular
//				orientation style and then wrap the TriMesh in a group which
//				contains an equivalent orientation style object.
//
//				This is used for cached geometries whose representation is of
//				a TriMesh with a fixed orientation - they must be wrapped in a
//				group to ensure that they are rendered with the same
//				orientation style that their normals were created under.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3TriMesh_BuildOrientationGroup(TQ3GeometryObject theTriMesh, TQ3OrientationStyle theOrientation)
{	TQ3GroupObject	theGroup;
	TQ3StyleObject	theStyle;



	// If the TriMesh couldn't be created, neither can the group
	if (theTriMesh == NULL)
		return(NULL);



	// Create the group
	theGroup = Q3DisplayGroup_New();
	if (theGroup != NULL)
		{
		// Add the orientation style
		theStyle = Q3OrientationStyle_New(theOrientation);
		Q3Group_AddObjectAndDispose(theGroup, &theStyle);


		// Build the normals and add the TriMesh
		E3TriMesh_AddTriangleNormals(theTriMesh, theOrientation);
		Q3Group_AddObjectAndDispose(theGroup, &theTriMesh);
		}		

	return(theGroup);
}
