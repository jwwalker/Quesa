/*  NAME:
        E3Main.h

    DESCRIPTION:
        Header file for E3Main.c.

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
#ifndef E3MAIN_HDR
#define E3MAIN_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3Initialize(void);
TQ3Status			E3Exit(void);
TQ3Boolean			E3IsInitialized(void);
TQ3Status			E3GetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision);
TQ3Status			E3GetReleaseVersion(TQ3Uns32 *releaseRevision);

TQ3Status			E3ObjectHierarchy_GetTypeFromString(const TQ3ObjectClassNameString objectClassString, TQ3ObjectType *objectClassType);
TQ3Status			E3ObjectHierarchy_GetStringFromType(TQ3ObjectType objectClassType, TQ3ObjectClassNameString objectClassString);
TQ3Boolean			E3ObjectHierarchy_IsTypeRegistered(TQ3ObjectType objectClassType);
TQ3Boolean			E3ObjectHierarchy_IsNameRegistered(const char *objectClassName);
TQ3Status			E3ObjectHierarchy_GetSubClassData(TQ3ObjectType objectClassType, TQ3SubClassData *subClassData);
TQ3Status			E3ObjectHierarchy_EmptySubClassData(TQ3SubClassData *subClassData);

TQ3Status			E3Object_Dispose(TQ3Object theObject);
TQ3Status			E3Object_CleanDispose(TQ3Object *object);
TQ3Object			E3Object_Duplicate(TQ3Object theObject);
TQ3Status			E3Object_Submit(TQ3Object theObject, TQ3ViewObject theView);
TQ3Boolean			E3Object_IsDrawable(TQ3Object theObject);
TQ3Boolean			E3Object_IsWritable(TQ3Object theObject, TQ3FileObject theFile);
TQ3ObjectType		E3Object_GetType(TQ3Object theObject);
TQ3ObjectType		E3Object_GetLeafType(TQ3Object theObject);
TQ3Boolean			E3Object_IsType(TQ3Object theObject, TQ3ObjectType theType);

TQ3ObjectType		E3Shared_GetType(TQ3SharedObject sharedObject);
TQ3SharedObject		E3Shared_GetReference(TQ3SharedObject sharedObject);
TQ3Boolean			E3Shared_IsReferenced(TQ3SharedObject sharedObject);
TQ3Uns32			E3Shared_GetReferenceCount( TQ3SharedObject sharedObject );
TQ3Uns32			E3Shared_GetEditIndex(TQ3SharedObject sharedObject);
TQ3Status			E3Shared_Edited(TQ3SharedObject sharedObject);

TQ3ObjectType		E3Shape_GetType(TQ3ShapeObject theShape);
TQ3Status			E3Shape_GetSet(TQ3ShapeObject theShape, TQ3SetObject *theSet);
TQ3Status			E3Shape_SetSet(TQ3ShapeObject theShape, TQ3SetObject theSet);
TQ3Status			E3Shape_AddElement(TQ3ShapeObject theShape, TQ3ElementType theType, const void *theData);
TQ3Status			E3Shape_GetElement(TQ3ShapeObject theShape, TQ3ElementType theType, void *theData);
TQ3Boolean			E3Shape_ContainsElement(TQ3ShapeObject theShape, TQ3ElementType theType);
TQ3Status			E3Shape_GetNextElementType(TQ3ShapeObject theShape, TQ3ElementType *theType);
TQ3Status			E3Shape_EmptyElements(TQ3ShapeObject theShape);
TQ3Status			E3Shape_ClearElement(TQ3ShapeObject theShape, TQ3ElementType theType);
TQ3Status			E3Shape_SubmitElements( TQ3ShapeObject inShape, TQ3ViewObject inView );

TQ3Status			E3Bitmap_Empty(TQ3Bitmap *theBitmap);
TQ3Uns32			E3Bitmap_GetImageSize(TQ3Uns32 theWidth, TQ3Uns32 theHeight);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

