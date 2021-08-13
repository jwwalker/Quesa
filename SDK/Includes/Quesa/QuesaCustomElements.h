/*! @header QuesaCustomElements.h
        Declares the standard Quesa custom elements, most inherited from QD3D.
        
	@ignore	_Nullable
	@ignore _Nonnull
	@ignore	_Null_unspecified
 */
/*  NAME:
        QuesaCustomElements.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

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
#ifndef QUESA_CUSTOMELEMENTS_HDR
#define QUESA_CUSTOMELEMENTS_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DCUSTOMELEMENTS__)
#error
#endif

#define __QD3DCUSTOMELEMENTS__



//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Custom element names (as defined by QD3D)
/*!
	@constant	CEcNameElementName
	@discussion	Class name of the name element.
	
				Ordinarily you will not need to use the class name, because you
				can manipulate name elements using the functions
				CENameElement_SetData, CENameElement_GetData, and
				CENameElement_EmptyData.
*/
#define CEcNameElementName                      "Apple Computer, Inc.:NameElement"

/*!
	@constant	CEcUrlElementName
	@discussion	Class name of the URL element.
	
				Ordinarily you will not need to use the class name, because you
				can manipulate name elements using the functions
				CEUrlElement_SetData, CEUrlElement_GetData, and
				CEUrlElement_EmptyData.
*/
#define CEcUrlElementName                       "Apple Computer, Inc.:URLElement"

/*!
	@constant	kQ3ClassNameCustomElementTriangleStrip
	@abstract	Class name of the triangle strip custom element.
	@discussion	Ordinarily you will not need to use the class name, because you
				can manipulate triangle strip elements using the functions
				CETriangleStripElement_SetData, CETriangleStripElement_GetData.
*/
#define	kQ3ClassNameCustomElementTriangleStrip	"Quesa:TriangleStripElement"

/*!
	@constant	kQ3ClassNameCustomElementSpecularMap
	@abstract	Class name of the specular map custom element.
	@discussion	Ordinarily you will not need to use the class name, because you
				can get, set, and clear specular maps using the functions
				CESpecularMapElement_CopyData, CESpecularMapElement_SetData.
*/
#define	kQ3ClassNameCustomElementSpecularMap	"Quesa:ShininessElement"

/*!
	@constant	kQ3ClassNameCustomElementFogMaxOpacity
	@abstract	Class name of the fog maximum opacity custom element.
	@discussion	Ordinarily you will not need to use the class name, because you
				can get, set, and clear max opacities using the functions
				CEFogMaxElement_Get, CEFogMaxElement_Set.
*/
#define	kQ3ClassNameCustomElementFogMaxOpacity	"Quesa:FogMaxDensity"

/*!
	@constant	kQ3ClassNameCustomElementHalfspaceFog
	@abstract	Class name of the halfspace fog custom element.
	@discussion	Ordinarily you will not need to use the class name, because you
				can get, set, and clear max opacities using the functions
				CEHalfspaceFogElement_GetData, CEHalfspaceFogElement_SetData.
*/
#define	kQ3ClassNameCustomElementHalfspaceFog	"Quesa:HalfspaceFog"


/*!
 *  @enum
 *      TCEUrlOptions
 *  @discussion
 *      URL options.
 *
 *  @constant kCEUrlOptionNone          No options set.
 *  @constant kCEUrlOptionUseMap        Map option set (meaning unknown).
 */
typedef enum TCEUrlOptions QUESA_ENUM_BASE(TQ3Uns32) {
    kCEUrlOptionNone                            = 0,
    kCEUrlOptionUseMap                          = 1,
    kCEUrlOptionSize32                          = 0xFFFFFFFF
} TCEUrlOptions;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *  @struct
 *      TCEUrlData
 *  @discussion
 *      Describes the data for a URL.
 
 *  @field url              C string containing the full (scheme + path) URL.
 *  @field description      Human readable description of the URL.
 *  @field options          Options for the URL.
 */
typedef struct TCEUrlData {
    char                                        * _Nonnull url;
    char                                        * _Nullable description;
    TCEUrlOptions                               options;
} TCEUrlData;


/*!
	@struct		TCEHalfspaceFogData
	@abstract	Data specifying half-space fog.
	@discussion	A point P is in the foggy halfspace if the dot product of P and
				the plane is negative.  For example, if you want fog to exist
				where y < 5, you could use a plane (0, 1, 0, -5), and to have
				fog where y > 5, you could use (0, -1, 0, 5).
	
	@field		rate		A measure of the rate of increase in fog as you
							move farther from the plane.
	@field		plane		A plane defining a halfspace in world coordinates,
							see discussion.
*/
typedef struct TCEHalfspaceFogData
{
	float                   rate;
	TQ3RationalPoint4D      plane;
} TCEHalfspaceFogData;



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

/*!
	@functiongroup	Name element
*/

/*!
 *  @function
 *      CENameElement_SetData
 *  @discussion
 *      Set the name element for an object.
 *
 *      The name parameter should point to a C string, and will be copied
 *      by CENameElement_SetData.
 *
 *  @param object           The object to assign the name to.
 *  @param name             The C string to associate with the object, or NULL to clear.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CENameElement_SetData (
    TQ3Object _Nonnull                     object,
    const char * _Nullable                 name
);



/*!
 *  @function
 *      CENameElement_GetData  
 *  @discussion
 *      Get the name element for an object.
 *
 *      The name parameter will receive a C string, or NULL if no name has
 *      been associated with the object. If a non-NULL pointer is returned,
 *      it must be disposed of with CENameElement_EmptyData.
 *
 *  @param object           The object to query.
 *  @param name             Receives the name associated with the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CENameElement_GetData (
    TQ3Object _Nonnull                     object,
    char * _Nullable * _Nonnull            name
);



/*!
 *  @function
 *      CENameElement_PeekData
 *  @discussion
 *      Get the name element for an object.
 *
 *      The name parameter will receive a C string, or NULL if no name has
 *      been associated with the object. If a non-NULL pointer is returned,
 *      it should be considered read-only and temporary.  Unlike
 *      CENameElement_GetData, this function returns the actual string within
 *      the element, not a copy.
 *
 *	@availability			This function is not available in QD3D.
 *  @param object           The object to query.
 *  @param name             Receives the name associated with the object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
CENameElement_PeekData (
    TQ3Object _Nonnull                     object,
    const char * _Nullable * _Nonnull      name
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


/*!
 *  @function
 *      CENameElement_EmptyData
 *  @discussion
 *      Release the data returned by CENameElement_GetData.
 *
 *  @param name             A pointer previously returned by CENameElement_GetData.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CENameElement_EmptyData (
    char * _Nonnull * _Nonnull    name
);


/*!
	@functiongroup	URL element
*/

/*!
 *  @function
 *      CEUrlElement_SetData
 *  @discussion
 *      Set the URL element for an object.
 *
 *      The URL element data will be copied by CEUrlElement_SetData.
 *
 *  @param object           The object to assign the URL element to.
 *  @param urlData          The URL data to associate with the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CEUrlElement_SetData (
    TQ3Object _Nonnull                     object,
    TCEUrlData * _Nonnull                  urlData
);



/*!
 *  @function
 *      CEUrlElement_GetData
 *  @discussion
 *      Get the URL element for an object.
 *
 *      The urlData parameter will receive a pointer to the URL data, or NULL
 *      if no URL element has been associated with the object. If a non-NULL
 *      pointer is returned, it must be disposed of with CEUrlElement_EmptyData.
 *
 *  @param object           The object to retrieve the URL element from.
 *  @param urlData          Receives the URL data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CEUrlElement_GetData (
    TQ3Object _Nonnull                     object,
    TCEUrlData * _Nullable * _Nonnull      urlData
);



/*!
 *  @function
 *      CEUrlElement_EmptyData
 *  @discussion
 *      Release the data returned by CEUrlElement_GetData.
 *
 *  @param urlData          A pointer previously returned by CEUrlElement_GetData.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CEUrlElement_EmptyData (
    TCEUrlData * _Nullable * _Nonnull urlData
);


/*!
	@functiongroup	Triangle strip element
*/

/*!
	@function	CETriangleStripElement_SetData
	@abstract	Set a triangle strip for the object.
	@discussion	Triangle strips are used by the OpenGL renderer as a speed
				optimization for rendering TriMesh objects.  If you have not
				already provided a triangle strip for a TriMesh, the renderer
				will compute one, but this can take a little time.
				
				When you assign a triangle strip, the element also records the
				current edit index of the object.
				
				You can pass 0 for inNumIndices and NULL for inIndices to
				indicate that you want to avoid using a triangle strip, perhaps
				because there is no efficient strip for this geometry.
				
				<em>This function is not available in QD3D.</em>
	@param		ioObject		An object, normally a TriMesh.
	@param		inNumIndices	Count of indices in the following array.
	@param		inIndices		Array of vertex indices making the strip.
	@result		Success or failure of the operation.
*/
Q3_EXTERN_API_C ( TQ3Status )
CETriangleStripElement_SetData(
	TQ3Object _Nonnull ioObject,
	TQ3Uns32 inNumIndices,
	const TQ3Uns32* _Nullable inIndices
);



/*!
	@function	CETriangleStripElement_GetData
	@abstract	Get a triangle strip for the object.
	@discussion	Triangle strips are used by the OpenGL renderer as a speed
				optimization for rendering TriMesh objects.
				
				If the current edit index of the object is not the same as
				when a strip was assigned, the strip will be considered stale
				and this function will return kQ3Failure.
				
				This function returns the actual triangle strip data within
				the element, not a copy.  The data should be considered
				read-only and temporary.
				
				<em>This function is not available in QD3D.</em>
	@param		inObject		An object, normally a TriMesh.
	@param		outNumIndices	Receives count of indices.
	@param		outIndices		Receives address of array of vertex indices.
	@result		Success or failure of the operation.
*/
Q3_EXTERN_API_C ( TQ3Status )
CETriangleStripElement_GetData(
	TQ3Object _Nonnull inObject,
	TQ3Uns32* _Nonnull outNumIndices,
	const TQ3Uns32* _Nullable * _Nonnull outIndices
);



/*!
	@function	CETriangleStripElement_Remove
	@abstract	Remove a triangle strip element if it exists.
	@param		ioObject		An object, normally a TriMesh.
*/
Q3_EXTERN_API_C (void)
CETriangleStripElement_Remove(
	TQ3Object _Nonnull ioObject
);

/*!
	@functiongroup	Specular Map element
*/

/*!
	@function	CESpecularMapElement_Copy
	@abstract	Retrieve a shininess map texture from an object.
	@param		inShader	A surface shader.
	@result		A new reference to a texture, or NULL.
*/
Q3_EXTERN_API_C( TQ3TextureObject _Nullable )
CESpecularMapElement_Copy( TQ3ShaderObject _Nonnull inShader );

/*!
	@function	CESpecularMapElement_Set
	@abstract	Set or remove a shininess map.
	@discussion	The specular map texture must be an RGBA texture.
				The RGB part is the specular color, while the alpha
				channel serves as specular control.  The alpha is not
				the same as Quesa's usual specular control, since
				specular control is an unbounded floating-point number,
				while the alpha value is bounded. To be exact,
				an alpha value in the range [0.0, 1.0] is multiplied
				by 128.0 and used as the OpenGL specular exponent.
				
				A specular map may only be used with the OpenGL
				renderer, and requires that per-pixel lighting be
				enabled.
	@param		ioShader	A surface shader.
	@param		inTexture	A texture object, or nullptr to remove.
*/
Q3_EXTERN_API_C( void )
CESpecularMapElement_Set( TQ3ShaderObject _Nonnull ioShader, TQ3TextureObject _Nullable inTexture );

/*!
	@functiongroup	Flipped Rows element
*/

/*!
	@function	CETextureFlippedRowsElement_Add
	@abstract	Mark a texture as having image data with rows in the opposite
				of the usual order, for better compatibility with OpenGL.
	@param		inTexture	A texture object.
*/
Q3_EXTERN_API_C( void )
CETextureFlippedRowsElement_Add( TQ3TextureObject _Nonnull inTexture );

/*!
	@function	CETextureFlippedRowsElement_IsPresent
	@abstract	Test whether a texture has been marked as having flipped rows.
	@param		inTexture	A texture object.
	@result		True if the rows are flipped.
*/
Q3_EXTERN_API_C( TQ3Boolean )
CETextureFlippedRowsElement_IsPresent( TQ3TextureObject _Nonnull inTexture );


/*!
	@function	CETextureFlippedRowsElement_Remove
	@abstract	Remove a flipped-rows mark from a texture.
	@param		inTexture	A texture object.
*/
Q3_EXTERN_API_C( void )
CETextureFlippedRowsElement_Remove( TQ3TextureObject _Nonnull inTexture );


/*!
	@functiongroup	Fog Max Opacity element
*/

/*!
	@function	CEFogMaxElement_Get
	@abstract	Get the maximum fog opacity for a fog style.
	@param		inFogStyle		A fog style object.
	@result		The maximum fog opacity, in the range [0, 1].  Defaults to 1
				if it has not been defined on the fog style.
*/
Q3_EXTERN_API_C( float )
CEFogMaxElement_Get( TQ3StyleObject _Nullable inFogStyle );

/*!
	@function	CEFogMaxElement_Set
	@abstract	Set the maximum fog opacity for a fog style.
	@discussion	Whichever fog density function you choose, you end up doing a
				blend,
				resultColor = opacity * fogColor + (1-opacity) * fragmentColor.
				By setting an upper limit on opacity, you ensure that the
				geometry fragments are not completely effaced.
	@param		ioFogStyle		A fog style object.
	@param		inMaxOpacity	Maximum fog density, in the range [0, 1].
								Passing 1 is equivalent to no limit on fog
								density.
*/
Q3_EXTERN_API_C( void )
CEFogMaxElement_Set( TQ3StyleObject _Nonnull ioFogStyle, float inMaxOpacity );


/*!
	@functiongroup		Halfspace Fog element
*/


/*!
	@function	CEHalfspaceFogElement_GetData
	@abstract	Get halfspace fog data attached to a fog style.
	@param		inFogStyle		A fog style object.
	@param		outData			Receives halfspace fog data.
	@result		Success or failure of the operation, e.g., failure if the
				element does not exist.
*/
Q3_EXTERN_API_C( TQ3Status )
CEHalfspaceFogElement_GetData( TQ3StyleObject _Nullable inFogStyle,
								TCEHalfspaceFogData* _Nonnull outData );

/*!
	@function	CEHalfspaceFogElement_SetData
	@abstract	Add or remove halfspace fog parameters on a fog style.
	@param		ioFogStyle		A fog style object.
	@param		inData			Halfspace fog data, or NULL to remove.
*/
Q3_EXTERN_API_C( void )
CEHalfspaceFogElement_SetData( TQ3StyleObject _Nonnull ioFogStyle,
								const TCEHalfspaceFogData* _Nullable inData );


// Work around a HeaderDoc bug
/*!
	@functiongroup
*/

//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


