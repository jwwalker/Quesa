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

TQ3Status	E3NameElement_SetData(TQ3Object _Nonnull object, const char * _Nullable name);
TQ3Status	E3NameElement_GetData(TQ3Object _Nonnull object, char * _Nonnull * _Nonnull name);
TQ3Status	E3NameElement_PeekData(TQ3Object _Nonnull object, const char * _Nullable * _Nonnull name);
TQ3Status	E3NameElement_EmptyData(char * _Nonnull * _Nonnull name);
TQ3Status	E3UrlElement_SetData(TQ3Object _Nonnull object, TCEUrlData * _Nonnull urlData);
TQ3Status	E3UrlElement_GetData(TQ3Object _Nonnull object, TCEUrlData * _Nullable * _Nonnull urlData);
TQ3Status	E3UrlElement_EmptyData(TCEUrlData * _Nullable * _Nonnull urlData);

TQ3Status	E3TriangleStripElement_SetData(
	TQ3Object _Nonnull ioObject,
	TQ3Uns32 inNumIndices,
	const TQ3Uns32* _Nullable inIndices
);
TQ3Status	E3TriangleStripElement_GetData(
	TQ3Object _Nonnull inObject,
	TQ3Uns32* _Nonnull outNumIndices,
	const TQ3Uns32* _Nullable * _Nonnull outIndices
);
void		E3TriangleStripElement_Remove( TQ3Object _Nonnull ioObject );

void		E3TextureFlippedRowsElement_Add( TQ3TextureObject _Nonnull inTexture );
TQ3Boolean	E3TextureFlippedRowsElement_IsPresent( TQ3TextureObject _Nonnull inTexture );
void		E3TextureFlippedRowsElement_Remove( TQ3TextureObject _Nonnull inTexture );

float		E3FogMaxElement_Get( TQ3StyleObject _Nonnull inFogStyle );
void		E3FogMaxElement_Set( TQ3StyleObject _Nonnull ioFogStyle, float inMaxOpacity );

TQ3Status	E3HalfspaceFogElement_Get( TQ3StyleObject _Nonnull inFogStyle,
										TCEHalfspaceFogData* _Nonnull outData );
void		E3HalfspaceFogElement_Set( TQ3StyleObject _Nonnull inFogStyle,
										const TCEHalfspaceFogData* _Nullable inData );


/*!
	@function	E3SpecularMapElement_Copy
	@abstract	Retrieve a specular map texture from an object.
	@param		shader		An object, normally a surface shader.
	@result		A new reference to a texture, or nullptr.
*/
TQ3TextureObject _Nullable	E3SpecularMapElement_Copy( TQ3ShaderObject _Nonnull shader );

/*!
	@function	E3SpecularMapElement_Set
	@abstract	Set or remove a specular map.
	@param		shader		A surface shader.
	@param		texture		A texture object, or nullptr to remove.
*/
void				E3SpecularMapElement_Set( TQ3ShaderObject _Nonnull shader,
						TQ3TextureObject _Nullable texture );

/*!
	@function	E3NormalMapElement_Copy
	@abstract		Retrieve a normal map texture from an object.
	@param		inShader	A surface shader.
	@result		A new reference to a texture, or NULL.
*/
TQ3TextureObject _Nullable E3NormalMapElement_Copy( TQ3ShaderObject _Nonnull inShader );

/*!
	@function	E3NormalMapElement_Set
	@abstract		Set or remove a normal map.
	@param		ioShader	A surface shader.
	@param		inTexture	A texture object, or nullptr to remove.
*/
void E3NormalMapElement_Set( TQ3ShaderObject _Nonnull ioShader,
	TQ3TextureObject _Nullable inTexture );

/*!
	@function	E3EmissiveMapElement_Copy
	@abstract	Retrieve a nemissive map texture from an object.
	@param		shader		An object, normally a surface shader.
	@result		A new reference to a texture, or nullptr.
*/
TQ3TextureObject _Nullable	E3EmissiveMapElement_Copy(
		TQ3ShaderObject _Nonnull shader );

/*!
	@function	E3EmissiveMapElement_Set
	@abstract	Set or remove an emissive map.
	@param		shader		A surface shader.
	@param		texture		A texture object, or nullptr to remove.
*/
void	E3EmissiveMapElement_Set( TQ3ShaderObject _Nonnull shader,
								TQ3TextureObject _Nullable texture );

/*!
	@function	E3MetallicRoughMapElement_Copy
	@abstract	Retrieve a metallic/roughness map texture from an object.
	@param		shader		An object, normally a surface shader.
	@result		A new reference to a texture, or nullptr.
*/
TQ3TextureObject _Nullable	E3MetallicRoughMapElement_Copy(
		TQ3ShaderObject _Nonnull shader );

/*!
	@function	E3MetallicRoughMapElement_Set
	@abstract	Set or remove a metallic/roughness map.
	@param		shader		A surface shader.
	@param		texture		A texture object, or nullptr to remove.
*/
void	E3MetallicRoughMapElement_Set( TQ3ShaderObject _Nonnull shader,
								TQ3TextureObject _Nullable texture );



TQ3Status	E3Object_GetProperty( TQ3Object _Nonnull object, TQ3ObjectType propType,
								TQ3Uns32 bufferSize, TQ3Uns32* _Nullable actualSize,
								void* _Nullable buffer );
TQ3Status	E3Object_RemoveProperty( TQ3Object _Nonnull object, TQ3ObjectType propType );
TQ3Status	E3Object_SetProperty( TQ3Object _Nonnull object, TQ3ObjectType propType,
								TQ3Uns32 dataSize, const void* _Nonnull data );

//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

