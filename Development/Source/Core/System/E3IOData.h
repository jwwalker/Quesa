/*  NAME:
        E3IOData.h

    DESCRIPTION:
        Header file for E3IOData.c.

    COPYRIGHT:
        Copyright (c) 1999-2009, Quesa Developers. All rights reserved.

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
#ifndef E3IODATA_HDR
#define E3IODATA_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here


#include "E3IO.h"


//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TE3UnknownBinary_Data {
	char					*typeString;
	TQ3UnknownBinaryData	data;
} TE3UnknownBinary_Data;



struct E3ViewHintsData
{
	TQ3RendererObject				renderer ;
	TQ3CameraObject					camera ;
	TQ3AttributeSet					attributeSet ;
	TQ3GroupObject					lightGroup ;
	TQ3Boolean						isValidSetDimensions ;
	TQ3Uns32						widthDimensions ;
	TQ3Uns32						heightDimensions ;
	TQ3Boolean						isValidSetMaskState ;
	TQ3Bitmap						mask ;
	TQ3DrawContextClearImageMethod	clearMethod ;
	TQ3ColorARGB					clearImageColor ;
};




class E3ViewHints : public E3Shared
	{
Q3_CLASS_ENUMS ( kQ3SharedTypeViewHints, E3ViewHints, E3Shared )

	E3ViewHintsData					instanceData;
	
public :

//	static TQ3Boolean		IsOfMyClass ( TQ3Object object ) ;

	TQ3Status						SetRenderer ( TQ3RendererObject renderer ) ;
	TQ3Status						GetRenderer ( TQ3RendererObject *renderer ) ;
	TQ3Status						SetCamera ( TQ3CameraObject camera ) ;
	TQ3Status						GetCamera ( TQ3CameraObject* camera ) ;
	TQ3Status						SetLightGroup ( TQ3GroupObject lightGroup ) ;
	TQ3Status						GetLightGroup ( TQ3GroupObject* lightGroup ) ;
	TQ3Status						SetAttributeSet ( TQ3AttributeSet attributeSet ) ;
	TQ3Status						GetAttributeSet ( TQ3AttributeSet* attributeSet ) ;
	TQ3Status						SetDimensionsState ( TQ3Boolean isValid ) ;
	TQ3Status						GetDimensionsState ( TQ3Boolean* isValid ) ;
	TQ3Status						SetDimensions ( TQ3Uns32 width, TQ3Uns32 height ) ;
	TQ3Status						GetDimensions ( TQ3Uns32* width, TQ3Uns32* height ) ;
	TQ3Status						SetMaskState ( TQ3Boolean isValid ) ;
	TQ3Status						GetMaskState ( TQ3Boolean* isValid ) ;
	TQ3Status						SetMask ( const TQ3Bitmap* mask ) ;
	TQ3Status						GetMask ( TQ3Bitmap* mask ) ;
	TQ3Status						SetClearImageMethod ( TQ3DrawContextClearImageMethod clearMethod ) ;
	TQ3Status						GetClearImageMethod ( TQ3DrawContextClearImageMethod* clearMethod ) ;
	TQ3Status						SetClearImageColor ( const TQ3ColorARGB* color ) ;
	TQ3Status						GetClearImageColor ( TQ3ColorARGB* color ) ;

	friend TQ3Status				e3viewhints_new ( E3ViewHints* theObject, void* privateData, const void* paramData ) ;
	friend void						e3viewhints_delete ( E3ViewHints* theObject, void* privateData ) ;
	friend TQ3Status				E3ViewHints_RegisterClass ( void );
	} ;
	


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3Shared_ClearEditTracking(TQ3SharedObject sharedObject);
TQ3Boolean			E3Shared_GetEditTrackingState(TQ3SharedObject sharedObject);
TQ3Status			E3NewLine_Write(E3File* theFile);

TQ3Status			E3Uns8_Read(TQ3Uns8 *data, E3File* theFile);
TQ3Status			E3Uns8_ReadArray(TQ3Uns32 numNums, TQ3Uns8 *data, E3File* theFile);
TQ3Status			E3Uns8_Write(TQ3Uns8 data, E3File* theFile);
TQ3Status			E3Uns16_Read(TQ3Uns16 *data, E3File* theFile);
TQ3Status			E3Uns16_ReadArray(TQ3Uns32 numNums, TQ3Uns16 *data, E3File* theFile);
TQ3Status			E3Uns16_Write(TQ3Uns16 data, E3File* theFile);
TQ3Status			E3Uns32_Read(TQ3Uns32 *data, E3File* theFile);
TQ3Status			E3Uns32_ReadArray(TQ3Uns32 numNums, TQ3Uns32 *data, E3File* theFile);
TQ3Status			E3Uns32_Write(TQ3Uns32 data, E3File* theFile);
TQ3Status			E3Int8_Read(TQ3Int8 *data, E3File* theFile);
TQ3Status			E3Int8_Write(TQ3Int8 data, E3File* theFile);
TQ3Status			E3Int16_Read(TQ3Int16 *data, E3File* theFile);
TQ3Status			E3Int16_Write(TQ3Int16 data, E3File* theFile);
TQ3Status			E3Int32_Read(TQ3Int32 *data, E3File* theFile);
TQ3Status			E3Int32_Write(TQ3Int32 data, E3File* theFile);
TQ3Status			E3Uns64_Read(TQ3Uns64 *data, E3File* theFile);
TQ3Status			E3Uns64_Write(TQ3Uns64 data, E3File* theFile);
TQ3Status			E3Int64_Read(TQ3Int64 *data, E3File* theFile);
TQ3Status			E3Int64_Write(TQ3Int64 data, E3File* theFile);
TQ3Status			E3Float32_Read(TQ3Float32 *data, E3File* theFile);
TQ3Status			E3Float32_ReadArray( TQ3Uns32 numFloats, TQ3Float32* theFloats, E3File* theFile);
TQ3Status			E3Float32_Write(TQ3Float32 data, E3File* theFile);
TQ3Status			E3Float64_Read(TQ3Float64 *data, E3File* theFile);
TQ3Status			E3Float64_Write(TQ3Float64 data, E3File* theFile);
TQ3Size				E3Size_Pad(TQ3Size size);
TQ3Status			E3String_Read(char *data, TQ3Uns32 *length, E3File* theFile);
TQ3Status			E3String_ReadUnlimited(char *data, TQ3Uns32 *ioLength, E3File* theFile);
TQ3Status			E3String_Write(const char *data, E3File* theFile);
TQ3Status			E3String_WriteUnlimited(const char *data, E3File* theFile);
TQ3Status			E3RawData_Read(unsigned char *data, TQ3Uns32 size, E3File* theFile);
TQ3Status			E3RawData_Write(const unsigned char *data, TQ3Uns32 size, E3File* theFile);
TQ3Status			E3Point2D_Read(TQ3Point2D *point2D, E3File* theFile);
TQ3Status			E3Point2D_Write(const TQ3Point2D *point2D, E3File* theFile);
TQ3Status			E3Point3D_Read(TQ3Point3D *point3D, E3File* theFile);
TQ3Status			E3Point3D_Write(const TQ3Point3D *point3D, E3File* theFile);
TQ3Status			E3RationalPoint3D_Read(TQ3RationalPoint3D *point3D, E3File* theFile);
TQ3Status			E3RationalPoint3D_Write(const TQ3RationalPoint3D *point3D, E3File* theFile);
TQ3Status			E3RationalPoint4D_Read(TQ3RationalPoint4D *point4D, E3File* theFile);
TQ3Status			E3RationalPoint4D_Write(const TQ3RationalPoint4D *point4D, E3File* theFile);
TQ3Status			E3Vector2D_Read(TQ3Vector2D *vector2D, E3File* theFile);
TQ3Status			E3Vector2D_Write(const TQ3Vector2D *vector2D, E3File* theFile);
TQ3Status			E3Vector3D_Read(TQ3Vector3D *vector3D, E3File* theFile);
TQ3Status			E3Vector3D_Write(const TQ3Vector3D *vector3D, E3File* theFile);
TQ3Status			E3Matrix4x4_Read(TQ3Matrix4x4 *matrix4x4, E3File* theFile);
TQ3Status			E3Matrix4x4_Write(const TQ3Matrix4x4 *matrix4x4, E3File* theFile);
TQ3Status			E3Tangent2D_Read(TQ3Tangent2D *tangent2D, E3File* theFile);
TQ3Status			E3Tangent2D_Write(const TQ3Tangent2D *tangent2D, E3File* theFile);
TQ3Status			E3Tangent3D_Read(TQ3Tangent3D *tangent3D, E3File* theFile);
TQ3Status			E3Tangent3D_Write(const TQ3Tangent3D *tangent3D, E3File* theFile);
TQ3Status			E3Comment_Write(char *comment, E3File* theFile);

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

TQ3Status			E3ViewHints_RegisterClass ( void ) ;
TQ3Status			E3ViewHints_UnregisterClass ( void ) ;
TQ3ViewHintsObject	E3ViewHints_New(TQ3ViewObject view);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

