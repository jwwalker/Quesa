/*  NAME:
        E3IOData.h

    DESCRIPTION:
        Header file for E3IOData.c.

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
#ifndef __E3IODATA__
#define __E3IODATA__
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
TQ3Status			E3Shared_ClearEditTracking(TQ3SharedObject sharedObject);
TQ3Boolean			E3Shared_GetEditTrackingState(TQ3SharedObject sharedObject);
TQ3Status			E3NewLine_Write(TQ3FileObject theFile);

TQ3Status			E3Uns8_Read(TQ3Uns8 *data, TQ3FileObject theFile);
TQ3Status			E3Uns8_Write(TQ3Uns8 data, TQ3FileObject theFile);
TQ3Status			E3Uns16_Read(TQ3Uns16 *data, TQ3FileObject theFile);
TQ3Status			E3Uns16_Write(TQ3Uns16 data, TQ3FileObject theFile);
TQ3Status			E3Uns32_Read(TQ3Uns32 *data, TQ3FileObject theFile);
TQ3Status			E3Uns32_Write(TQ3Uns32 data, TQ3FileObject theFile);
TQ3Status			E3Int8_Read(TQ3Int8 *data, TQ3FileObject theFile);
TQ3Status			E3Int8_Write(TQ3Int8 data, TQ3FileObject theFile);
TQ3Status			E3Int16_Read(TQ3Int16 *data, TQ3FileObject theFile);
TQ3Status			E3Int16_Write(TQ3Int16 data, TQ3FileObject theFile);
TQ3Status			E3Int32_Read(TQ3Int32 *data, TQ3FileObject theFile);
TQ3Status			E3Int32_Write(TQ3Int32 data, TQ3FileObject theFile);
TQ3Status			E3Uns64_Read(TQ3Uns64 *data, TQ3FileObject theFile);
TQ3Status			E3Uns64_Write(TQ3Uns64 data, TQ3FileObject theFile);
TQ3Status			E3Int64_Read(TQ3Int64 *data, TQ3FileObject theFile);
TQ3Status			E3Int64_Write(TQ3Int64 data, TQ3FileObject theFile);
TQ3Status			E3Float32_Read(TQ3Float32 *data, TQ3FileObject theFile);
TQ3Status			E3Float32_Write(TQ3Float32 data, TQ3FileObject theFile);
TQ3Status			E3Float64_Read(TQ3Float64 *data, TQ3FileObject theFile);
TQ3Status			E3Float64_Write(TQ3Float64 data, TQ3FileObject theFile);
TQ3Size				E3Size_Pad(TQ3Size size);
TQ3Status			E3String_Read(char *data, unsigned long *length, TQ3FileObject theFile);
TQ3Status			E3String_Write(const char *data, TQ3FileObject theFile);
TQ3Status			E3RawData_Read(unsigned char *data, unsigned long size, TQ3FileObject theFile);
TQ3Status			E3RawData_Write(const unsigned char *data, unsigned long size, TQ3FileObject theFile);
TQ3Status			E3Point2D_Read(TQ3Point2D *point2D, TQ3FileObject theFile);
TQ3Status			E3Point2D_Write(const TQ3Point2D *point2D, TQ3FileObject theFile);
TQ3Status			E3Point3D_Read(TQ3Point3D *point3D, TQ3FileObject theFile);
TQ3Status			E3Point3D_Write(const TQ3Point3D *point3D, TQ3FileObject theFile);
TQ3Status			E3RationalPoint3D_Read(TQ3RationalPoint3D *point3D, TQ3FileObject theFile);
TQ3Status			E3RationalPoint3D_Write(const TQ3RationalPoint3D *point3D, TQ3FileObject theFile);
TQ3Status			E3RationalPoint4D_Read(TQ3RationalPoint4D *point4D, TQ3FileObject theFile);
TQ3Status			E3RationalPoint4D_Write(const TQ3RationalPoint4D *point4D, TQ3FileObject theFile);
TQ3Status			E3Vector2D_Read(TQ3Vector2D *vector2D, TQ3FileObject theFile);
TQ3Status			E3Vector2D_Write(const TQ3Vector2D *vector2D, TQ3FileObject theFile);
TQ3Status			E3Vector3D_Read(TQ3Vector3D *vector3D, TQ3FileObject theFile);
TQ3Status			E3Vector3D_Write(const TQ3Vector3D *vector3D, TQ3FileObject theFile);
TQ3Status			E3Matrix4x4_Read(TQ3Matrix4x4 *matrix4x4, TQ3FileObject theFile);
TQ3Status			E3Matrix4x4_Write(const TQ3Matrix4x4 *matrix4x4, TQ3FileObject theFile);
TQ3Status			E3Tangent2D_Read(TQ3Tangent2D *tangent2D, TQ3FileObject theFile);
TQ3Status			E3Tangent2D_Write(const TQ3Tangent2D *tangent2D, TQ3FileObject theFile);
TQ3Status			E3Tangent3D_Read(TQ3Tangent3D *tangent3D, TQ3FileObject theFile);
TQ3Status			E3Tangent3D_Write(const TQ3Tangent3D *tangent3D, TQ3FileObject theFile);
TQ3Status			E3Comment_Write(char *comment, TQ3FileObject theFile);

TQ3Status			E3Unknown_RegisterClass(void);
TQ3Status			E3Unknown_UnregisterClass(void);
TQ3UnknownObject	E3UnknownText_New(TQ3UnknownTextData* data);
TQ3ObjectType		E3Unknown_GetType(TQ3UnknownObject unknownObject);
TQ3Status			E3Unknown_GetDirtyState(TQ3UnknownObject unknownObject, TQ3Boolean *isDirty);
TQ3Status			E3Unknown_SetDirtyState(TQ3UnknownObject unknownObject, TQ3Boolean isDirty);
TQ3Status			E3UnknownText_GetData(TQ3UnknownObject unknownObject, TQ3UnknownTextData *unknownTextData);
TQ3Status			E3UnknownText_EmptyData(TQ3UnknownTextData *unknownTextData);
TQ3UnknownObject	E3UnknownBinary_New(TQ3UnknownBinaryData* data,const char *typeString);
TQ3Status			E3UnknownBinary_GetData(TQ3UnknownObject unknownObject, TQ3UnknownBinaryData *unknownBinaryData);
TQ3Status			E3UnknownBinary_EmptyData(TQ3UnknownBinaryData *unknownBinaryData);
TQ3Status			E3UnknownBinary_GetTypeString(TQ3UnknownObject unknownObject, char **typeString);
TQ3Status			E3UnknownBinary_EmptyTypeString(char **typeString);

TQ3ViewHintsObject	E3ViewHints_New(TQ3ViewObject view);
TQ3Status			E3ViewHints_SetRenderer(TQ3ViewHintsObject viewHints, TQ3RendererObject renderer);
TQ3Status			E3ViewHints_GetRenderer(TQ3ViewHintsObject viewHints, TQ3RendererObject *renderer);
TQ3Status			E3ViewHints_SetCamera(TQ3ViewHintsObject viewHints, TQ3CameraObject camera);
TQ3Status			E3ViewHints_GetCamera(TQ3ViewHintsObject viewHints, TQ3CameraObject *camera);
TQ3Status			E3ViewHints_SetLightGroup(TQ3ViewHintsObject viewHints, TQ3GroupObject lightGroup);
TQ3Status			E3ViewHints_GetLightGroup(TQ3ViewHintsObject viewHints, TQ3GroupObject *lightGroup);
TQ3Status			E3ViewHints_SetAttributeSet(TQ3ViewHintsObject viewHints, TQ3AttributeSet attributeSet);
TQ3Status			E3ViewHints_GetAttributeSet(TQ3ViewHintsObject viewHints, TQ3AttributeSet *attributeSet);
TQ3Status			E3ViewHints_SetDimensionsState(TQ3ViewHintsObject viewHints, TQ3Boolean isValid);
TQ3Status			E3ViewHints_GetDimensionsState(TQ3ViewHintsObject viewHints, TQ3Boolean *isValid);
TQ3Status			E3ViewHints_SetDimensions(TQ3ViewHintsObject viewHints, unsigned long width, unsigned long height);
TQ3Status			E3ViewHints_GetDimensions(TQ3ViewHintsObject viewHints, unsigned long *width, unsigned long *height);
TQ3Status			E3ViewHints_SetMaskState(TQ3ViewHintsObject viewHints, TQ3Boolean isValid);
TQ3Status			E3ViewHints_GetMaskState(TQ3ViewHintsObject viewHints, TQ3Boolean *isValid);
TQ3Status			E3ViewHints_SetMask(TQ3ViewHintsObject viewHints, const TQ3Bitmap *mask);
TQ3Status			E3ViewHints_GetMask(TQ3ViewHintsObject viewHints, TQ3Bitmap *mask);
TQ3Status			E3ViewHints_SetClearImageMethod(TQ3ViewHintsObject viewHints, TQ3DrawContextClearImageMethod clearMethod);
TQ3Status			E3ViewHints_GetClearImageMethod(TQ3ViewHintsObject viewHints, TQ3DrawContextClearImageMethod *clearMethod);
TQ3Status			E3ViewHints_SetClearImageColor(TQ3ViewHintsObject viewHints, const TQ3ColorARGB *color);
TQ3Status			E3ViewHints_GetClearImageColor(TQ3ViewHintsObject viewHints, TQ3ColorARGB *color);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

