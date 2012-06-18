/*! @header QuesaCustomElements.h
        Declares the standard Quesa custom elements, most inherited from QD3D.
 */
/*  NAME:
        QuesaCustomElements.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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

#if QUESA_SUPPORT_QUICKTIME
	#include <QuickTime/QuickTime.h>
#endif



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
	@constant	CEcWireElementName
	@discussion	Class name of the wire element.
	
				Ordinarily you will not need to use the class name, because you
				can manipulate name elements using the functions
				CEWireElement_SetData, CEWireElement_GetData, and
				CEWireElement_EmptyData.
*/
#define CEcWireElementName                      "Apple Computer, Inc.:WireElement"

/*!
	@constant	kQ3ClassNameCustomElementTriangleStrip
	@abstract	Class name of the triangle strip custom element.
	@discussion	Ordinarily you will not need to use the class name, because you
				can manipulate triangle strip elements using the functions
				CETriangleStripElement_SetData, CETriangleStripElement_GetData.
*/
#define	kQ3ClassNameCustomElementTriangleStrip	"Quesa:TriangleStripElement"

/*!
 *  @enum
 *      TCEUrlOptions
 *  @discussion
 *      URL options.
 *
 *  @constant kCEUrlOptionNone          No options set.
 *  @constant kCEUrlOptionUseMap        Map option set (meaning unknown).
 */
typedef enum {
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
    char                                        *url;
    char                                        *description;
    TCEUrlOptions                               options;
} TCEUrlData;





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
 *  @param name             The C string to associate with the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CENameElement_SetData (
    TQ3Object                     object,
    const char                    *name
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
    TQ3Object                     object,
    char                          **name
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
    TQ3Object                     object,
    const char                    **name
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
    char                          **name
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
    TQ3Object                     object,
    TCEUrlData                    *urlData
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
    TQ3Object                     object,
    TCEUrlData                    **urlData
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
    TCEUrlData                    **urlData
);


/*!
	@functiongroup	Wire element
*/

#if QUESA_SUPPORT_QUICKTIME

/*!
 *  @function
 *      CEWireElement_SetData
 *  @discussion
 *      Set the QuickTime wired element for an object.
 *
 *  @param object           The object to assign the element to.
 *  @param wireData         The QuickTime wired element to associate with the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CEWireElement_SetData (
    TQ3Object                     object,
    QTAtomContainer               wireData
);



/*!
 *  @function
 *      CEWireElement_GetData
 *  @discussion
 *      Get the QuickTime wired element for an object.
 *
 *      The wireData parameter will receive a QTAtomContainer, or NULL if no
 *      wired element has been associated with the object. If a non-NULL pointer
 *      is returned, it must be disposed of with CEWireElement_EmptyData.
 *
 *  @param object           The object to query.
 *  @param wireData         Receives the QuickTime wired element associated with the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CEWireElement_GetData (
    TQ3Object                     object,
    QTAtomContainer               *wireData
);



/*!
 *  @function
 *      CEWireElement_EmptyData
 *  @discussion
 *      Release the data returned by CEWireElement_GetData.
 *
 *  @param wireData         A pointer previously returned by CEWireElement_GetData.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
CEWireElement_EmptyData (
    QTAtomContainer               *wireData
);

#endif // QUESA_SUPPORT_QUICKTIME

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
	TQ3Object ioObject,
	TQ3Uns32 inNumIndices,
	const TQ3Uns32* inIndices
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
	TQ3Object inObject,
	TQ3Uns32* outNumIndices,
	const TQ3Uns32** outIndices
);



/*!
	@function	CETriangleStripElement_Remove
	@abstract	Remove a triangle strip element if it exists.
	@param		ioObject		An object, normally a TriMesh.
*/
Q3_EXTERN_API_C (void)
CETriangleStripElement_Remove(
	TQ3Object ioObject
);

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


