/*  NAME:
        E3Utils.c

    DESCRIPTION:
        Quesa utility functions.

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
#include "E3Utils.h"





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
	Q3_REQUIRE(Q3_VALID_PTR(newRef));
	if (theObject != NULL)
		Q3_REQUIRE(Q3Object_IsType(theObject, kQ3ObjectTypeShared));



	// Acquire a new reference
	if (theObject != NULL)
		{
		*newRef = Q3Shared_GetReference(theObject);
		Q3_ASSERT_VALID_PTR(*newRef);
		}
	else
		*newRef = NULL;
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
		Q3_REQUIRE(Q3Object_IsType(newObject, kQ3ObjectTypeShared));



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
	void *			originalData  ;
	TQ3Uns32		originalSize = Q3Bitmap_GetImageSize(	original->width,
													  		original->height ) ;
	
	
	
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
		originalData = E3Memory_Allocate( originalSize ) ;

		if (originalData == NULL)
		{
			return(kQ3Failure);						
		}
		
		memcpy( originalData, original->image, originalSize );
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
		Q3Vector3D_Scale( &normals[i], 0.5, &v );
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
