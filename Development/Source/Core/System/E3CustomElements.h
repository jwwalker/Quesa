/*  NAME:
        E3CustomElements.h

    DESCRIPTION:
        Header file for E3CustomElements.c.

    COPYRIGHT:
        Copyright (c) 1999-2025, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#ifndef E3CUSTOMELEMENTS_HDR
#define E3CUSTOMELEMENTS_HDR
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
TQ3Status	E3CustomElements_RegisterClass(void);
TQ3Status	E3CustomElements_UnregisterClass(void);

TQ3Status	E3NameElement_SetData(TQ3Object object, const char *name);
TQ3Status	E3NameElement_GetData(TQ3Object object, char **name);
TQ3Status	E3NameElement_PeekData(TQ3Object object, const char **name);
TQ3Status	E3NameElement_EmptyData(char **name);
TQ3Status	E3UrlElement_SetData(TQ3Object object, TCEUrlData *urlData);
TQ3Status	E3UrlElement_GetData(TQ3Object object, TCEUrlData **urlData);
TQ3Status	E3UrlElement_EmptyData(TCEUrlData **urlData);

TQ3Status	E3TriangleStripElement_SetData(
	TQ3Object ioObject,
	TQ3Uns32 inNumIndices,
	const TQ3Uns32* inIndices
);
TQ3Status	E3TriangleStripElement_GetData(
	TQ3Object inObject,
	TQ3Uns32* outNumIndices,
	const TQ3Uns32** outIndices
);
void		E3TriangleStripElement_Remove( TQ3Object ioObject );

void		E3TextureFlippedRowsElement_Add( TQ3TextureObject inTexture );
TQ3Boolean	E3TextureFlippedRowsElement_IsPresent( TQ3TextureObject inTexture );
void		E3TextureFlippedRowsElement_Remove( TQ3TextureObject inTexture );

float		E3FogMaxElement_Get( TQ3StyleObject inFogStyle );
void		E3FogMaxElement_Set( TQ3StyleObject ioFogStyle, float inMaxOpacity );

TQ3Status	E3HalfspaceFogElement_Get( TQ3StyleObject inFogStyle,
										TCEHalfspaceFogData* outData );
void		E3HalfspaceFogElement_Set( TQ3StyleObject inFogStyle,
										const TCEHalfspaceFogData* inData );


/*!
	@function	E3SpecularMapElement_Copy
	@abstract	Retrieve a specular map texture from an object.
	@param		shader		An object, normally a surface shader.
	@result		A new reference to a texture, or nullptr.
*/
TQ3TextureObject	E3SpecularMapElement_Copy( TQ3ShaderObject shader );

/*!
	@function	E3SpecularMapElement_Set
	@abstract	Set or remove a specular map.
	@param		shader		A surface shader.
	@param		texture		A texture object, or nullptr to remove.
*/
void				E3SpecularMapElement_Set( TQ3ShaderObject shader, TQ3TextureObject texture );

/*!
	@function	E3NormalMapElement_Copy
	@abstract		Retrieve a normal map texture from an object.
	@param		inShader	A surface shader.
	@result		A new reference to a texture, or NULL.
*/
TQ3TextureObject E3NormalMapElement_Copy( TQ3ShaderObject inShader );

/*!
	@function	E3NormalMapElement_Set
	@abstract		Set or remove a normal map.
	@param		ioShader	A surface shader.
	@param		inTexture	A texture object, or nullptr to remove.
*/
void E3NormalMapElement_Set( TQ3ShaderObject ioShader,
	TQ3TextureObject inTexture );


const void*	E3Object_GetPropertyAddress( TQ3Object object,
										TQ3ObjectType propType,
										TQ3Uns32* actualSize );
TQ3Status	E3Object_GetProperty( TQ3Object object, TQ3ObjectType propType,
								TQ3Uns32 bufferSize, TQ3Uns32* actualSize, void* buffer );
TQ3Status	E3Object_RemoveProperty( TQ3Object object, TQ3ObjectType propType );
TQ3Status	E3Object_SetProperty( TQ3Object object, TQ3ObjectType propType,
								TQ3Uns32 dataSize, const void* data );

//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

