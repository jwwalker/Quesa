/*! @header QuesaCustomElements.h
        Declares the Quesa custom elements inherited from QD3D.
 */
/*  NAME:
        QuesaCustomElements.h

    DESCRIPTION:
        Quesa public header.

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
	@constant	CEcWireElementName
	@discussion	Class name of the wire element.
	
				Ordinarily you will not need to use the class name, because you
				can manipulate name elements using the functions
				CEWireElement_SetData, CEWireElement_SetData, and
				CEWireElement_EmptyData.
*/
#define CEcWireElementName                      "Apple Computer, Inc.:WireElement"


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


#if QUESA_ALLOW_QD3D_EXTENSIONS
/*!
 *	@enum
 *		TQ3EventCallbackElementTypes
 *	@discussion
 *		Custom elements containing callback functions can be attached to objects,
 *		in order that client code can be notified by Quesa of certain events.
 *		The callback function has the type TQ3ObjectEventCallback.
 *
 *		<em>This functionality is not available in QD3D.</em>
 *
 *		Example of installing an event callback:
 *
 *		<blockquote><pre><code>
 *		TQ3ObjectEventCallback	callback = MyCallback;
 *		Q3Object_AddElement( theView,
 *		&nbsp;		kQ3CallbackElementTypeBeforeRender,
 *		&nbsp;		&amp;callback );
 *		</code></pre></blockquote>
 *
 *		The before and after rendering callbacks should be attached to the view
 *		object.  They are called not only for retained objects that you explicitly submit
 *		for rendering, but also for each object in the hierarchy of a group that
 *		you submit.
 *
 *	@constant	kQ3CallbackElementTypeBeforeRender
 *						The callback will be called just before each retained object
 *						is passed to the renderer.  If the callback returns
 *						kQ3Failure, then the object will not be passed to the renderer.
 *	@constant	kQ3CallbackElementTypeAfterRender
 *						The callback will be called just after each retained object
 *						is passed to the renderer.  The callback's status result
 *						is ignored.
 *	@constant	kQ3CallbackElementTypeBeforePick
 *						The callback will be called just before each retained object
 *						is submitted for picking.  If the callback returns
 *						kQ3Failure, then the object will not be submitted.
 *	@constant	kQ3CallbackElementTypeAfterPick
 *						The callback will be called just after each retained object
 *						is submitted for picking.  The callback's status result
 *						is ignored.
 */
typedef enum {
	kQ3CallbackElementTypeBeforeRender			=	Q3_OBJECT_TYPE(0xF0, 'b', 'r', 'e'),
	kQ3CallbackElementTypeAfterRender			=	Q3_OBJECT_TYPE(0xF0, 'a', 'r', 'e'),
	kQ3CallbackElementTypeBeforePick			=	Q3_OBJECT_TYPE(0xF0, 'b', 'p', 'k'),
	kQ3CallbackElementTypeAfterPick				=	Q3_OBJECT_TYPE(0xF0, 'a', 'p', 'k')
} TQ3EventCallbackElementTypes;

#endif



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


#if QUESA_ALLOW_QD3D_EXTENSIONS

/*!
  *	@typedef
  *		TQ3ObjectEventCallback
  *	@discussion
  *		Applications can obtain callbacks as objects are processed
  *		by Quesa.  Callbacks are installed as custom elements.  In some cases,
  *		it may be necessary to attach the element to each object of interest,
  *		while in other cases the element may be attached to another object
  *		such as a view.  See the discussions of element types such as
  *		kQ3ElementTypeBeforeRender for details about specific callbacks.
  *
  *		Callbacks should return kQ3Success to continue processing the
  *		object, or kQ3Failure to cancel the operation (if possible).
  *		Some events, e.g., those which indicate the completion of an
  *		event, can not be cancelled and the result will be ignored.
  *
  *		Event callbacks are only available for retained mode objects.
  *
  *		<em>This functionality is not available in QD3D.</em>
  *
  *    @param    theObject        The object being processed.
  *    @param    theEventType     The event occurring to the object.
  *    @param    theHost          The object to which the callback element is attached.
  *    @result                    Whether to continue with the event.
*/
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3ObjectEventCallback)(
                            TQ3Object       theObject,
                            TQ3ElementType  theEventType,
                            TQ3Object   	theHost );
#endif



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
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





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


