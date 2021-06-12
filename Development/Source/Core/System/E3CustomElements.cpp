/*  NAME:
        E3CustomElements.cpp

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3CustomElements.h"
#include "E3HashTable.h"
#include "E3GeometryTriMesh.h"
#include "E3Main.h"
#include "E3Set.h"
#include "E3String.h"
#include "CQ3ObjectRef.h"

#include <vector>



//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TCEUrlDataPrivate {
	char *			url;
	TQ3StringObject	description;
	TCEUrlOptions 	options;
} TCEUrlDataPrivate;


struct TCETriangleStripPrivate
{
	TQ3Uns32		indexCount;
	TQ3Uns32		editIndex;
	const TQ3Uns32*	indexArray;
};


class E3NameElement : public E3Element  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeCustomElementName, E3NameElement, E3Element )
public :

	TQ3StringObject							instanceData ;
	} ;
	


class E3URLElement : public E3Element  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeCustomElementUrl, E3URLElement, E3Element )
public :

	TCEUrlDataPrivate						instanceData ;
	} ;
	



class E3BitDepthElement : public E3Element  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ElementTypeDepthBits, E3BitDepthElement, E3Element )
public :

	TQ3Uns32								instanceData ;
	} ;



class E3TextureAlphaTestElement : public E3Element  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ElementTypeTextureShaderAlphaTest, E3TextureAlphaTestElement, E3Element )
public :

	TQ3Float32								instanceData ;
	} ;


class E3SpecularElement : public E3Element  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeCustomElementSpecularMap, E3SpecularElement, E3Element )
public :

	TQ3TextureObject							instanceData ;
	} ;






//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kQ3ClassNameCustomElementDepthBits		"Quesa:DepthBitsElement"
#define kQ3ClassNameCustomElementAlphaTest		"Quesa:AlphaTestElement"
#define kQ3ClassNameCustomElementFlipRows		"Quesa:FlipRowsElement"





//=============================================================================
//      Static variables
//-----------------------------------------------------------------------------
static TQ3ElementType	sTriangleStripElementType = 0;
static TQ3ElementType	sFlippedRowsElementType = 0;
static TQ3ElementType	sFogMaxElementType = 0;
static TQ3ElementType	sHalfspaceFogElementType = 0;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//      e3nameelement_traverse : Traverse method (writing).
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_traverse (TQ3Object object, TQ3StringObject *string, TQ3ViewObject view)
{
#pragma unused(object)

	if (string == nullptr || *string == nullptr)
		return kQ3Success;

	if (Q3XView_SubmitWriteData(view, 0, nullptr, nullptr) == kQ3Failure)
		return kQ3Failure;

	return Q3Object_Submit(*string, view);
}





//=============================================================================
//      e3nameelement_readdata : Read data method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_readdata (TQ3Object object, TQ3FileObject file)
{
	TQ3Status		status;
	TQ3StringObject	string;

	string = Q3File_ReadObject(file);
	if (string == nullptr) 
		return kQ3Failure;
		
	status = Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementName, &string);
	Q3Object_Dispose(string);
	
	return status;
}





//=============================================================================
//      e3nameelement_copyadd : Copy add/get method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyadd ( const void* fromParam, void* toParam )
{
	TQ3StringObject* source = (TQ3StringObject*) fromParam;
	TQ3StringObject* dest = (TQ3StringObject*) toParam;
	
	*dest = Q3Shared_GetReference(*source);

	return (*dest != nullptr) ? kQ3Success : kQ3Failure;
}





//=============================================================================
//      e3nameelement_copyduplicate : Copy duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyduplicate( const void* inSrc, void* inDst )
{
	TQ3StringObject* source = (TQ3StringObject*) inSrc;
	TQ3StringObject* dest = (TQ3StringObject*) inDst;
	*dest = Q3Object_Duplicate(*source);

	return (*dest != nullptr) ? kQ3Success : kQ3Failure;
}





//=============================================================================
//      e3nameelement_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyreplace (TQ3StringObject *source, TQ3StringObject *dest)
{
	TQ3StringObject	temp;
	
	temp = Q3Shared_GetReference(*source);
	if (temp == nullptr) 
		return kQ3Failure;

	if (*dest)
		Q3Object_Dispose(*dest);
	
	*dest = temp;

	return kQ3Success;
}





//=============================================================================
//      e3nameelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_delete ( void * inStr )
{
	TQ3StringObject* string = (TQ3StringObject*) inStr;
	if (*string != nullptr) {
		Q3Object_Dispose(*string);
		*string = nullptr;
	}

	return kQ3Success;
}





//=============================================================================
//      e3nameelement_metahandler : Name element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3nameelement_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = nullptr;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) e3nameelement_traverse;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) e3nameelement_readdata;
			break;

		case kQ3XMethodTypeElementCopyAdd:
		case kQ3XMethodTypeElementCopyGet:
			theMethod = (TQ3XFunctionPointer) e3nameelement_copyadd;
			break;

		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) e3nameelement_copyduplicate;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3nameelement_copyreplace;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) e3nameelement_delete;
			break;
		}

	return(theMethod);
}





//=============================================================================
//      e3urlelement_traverse : Traverse method (writing).
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3urlelement_traverse (TQ3Object object, TCEUrlDataPrivate *urlData, TQ3ViewObject view)
{
	TQ3Size	size;
#pragma unused(object)


	if (urlData == nullptr || urlData->url == nullptr)
		return kQ3Success;

	size = Q3Size_Pad(static_cast<TQ3Size>(strlen(urlData->url) + 1));
	size += static_cast<TQ3Uns32>(sizeof(TQ3Uns32));

	if (Q3XView_SubmitWriteData (view, size, urlData, nullptr) == kQ3Failure)
		return kQ3Failure;

	if (urlData->description) {
		if (Q3Object_Submit(urlData->description, view) == kQ3Failure)
			return kQ3Failure;
	}

	return kQ3Success;
}





//=============================================================================
//      e3urlelement_write : Write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_write (TCEUrlDataPrivate *urlData, TQ3FileObject file)
{
	if (Q3String_Write (urlData->url, file) == kQ3Failure)
		return kQ3Failure;
		
	return Q3Uns32_Write ((TQ3Uns32)urlData->options, file);
}





//=============================================================================
//      e3urlelement_readdata : Read data method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_readdata (TQ3Object object, TQ3FileObject file)
{
	TCEUrlDataPrivate 	urlData;
	TQ3Uns32			length;
	char 				buffer[kQ3StringMaximumLength];
	
	if (Q3String_Read (buffer, &length, file) == kQ3Failure)
		return kQ3Failure;

	urlData.url = (char*)Q3Memory_Allocate(length + 1);
	SAFE_STRCPY(urlData.url, buffer, length + 1);
	
	if (Q3Uns32_Read ((TQ3Uns32*)&urlData.options, file) == kQ3Failure)
		return kQ3Failure;

	if (Q3File_IsEndOfContainer(file, nullptr) == kQ3False) {
		urlData.description = Q3File_ReadObject(file);
	}

	return Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementUrl, &urlData);
}





//=============================================================================
//      e3urlelement_copyadd : Copy add/get/duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_copyadd ( const void* fromParam, void* toParam )
{
	TCEUrlDataPrivate* source = (TCEUrlDataPrivate*) fromParam;
	TCEUrlDataPrivate* dest = (TCEUrlDataPrivate*) toParam;

	TQ3Uns32 dstSize = static_cast<TQ3Uns32>(strlen(source->url) + 1);
	dest->url = (char*)Q3Memory_Allocate(dstSize);
	if (dest->url == nullptr)
		return kQ3Failure;

	SAFE_STRCPY(dest->url, source->url, dstSize);

	if (source->description) {  // optional
		
		// In the case of duplication, this needs to be Q3Object_Duplicate
		// rather than Q3Shared_Duplicate.  In the case of Add and Get,
		// it doesn't matter much, since the strings being copied are
		// short-lived.
		dest->description = Q3Object_Duplicate( source->description );
		if (dest->description == nullptr) {
			return kQ3Failure;
		}
		
	} else {

		dest->description = nullptr;
	}
		
	dest->options = source->options;
	
	return kQ3Success;
}





//=============================================================================
//      e3urlelement_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_copyreplace (TCEUrlDataPrivate *source, TCEUrlDataPrivate *dest)
{
	TQ3StringObject	string = nullptr;
	TQ3Uns32 dstSize = static_cast<TQ3Uns32>(strlen(source->url) + 1);

	if (Q3Memory_Reallocate(&dest->url, dstSize) == kQ3Failure)
		return kQ3Failure;
		
	SAFE_STRCPY(dest->url, source->url, dstSize);

	if (source->description) {  // optional

		string = Q3Shared_GetReference (source->description);
		if (string == nullptr) {
			return kQ3Failure;
		}
		
		if (dest->description) {
			Q3Object_Dispose (dest->description);
		}

		dest->description = string;
		
	} else {

		dest->description = nullptr;
	}
		
	dest->options = source->options;
	
	return kQ3Success;
}





//=============================================================================
//      e3urlelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_delete (void *inData)
{
	TCEUrlDataPrivate* urlData = (TCEUrlDataPrivate*) inData;
	if (urlData->url != nullptr) {
	
		Q3Memory_Free (&urlData->url);
		urlData->url = nullptr;
	}
	
	if (urlData->description != nullptr) {
	
		Q3Object_Dispose (urlData->description);
		urlData->description = nullptr;
	}

	return kQ3Success;
}





//=============================================================================
//      e3urlelement_metahandler : URL element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3urlelement_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = nullptr;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) e3urlelement_traverse;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3urlelement_write;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) e3urlelement_readdata;
			break;

		case kQ3XMethodTypeElementCopyAdd:
		case kQ3XMethodTypeElementCopyGet:
		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) e3urlelement_copyadd;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3urlelement_copyreplace;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) e3urlelement_delete;
			break;
		}

	return(theMethod);
}




#pragma mark -

//=============================================================================
//      strip_element_copyadd : Copy add/duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
strip_element_copyadd( const void* fromAPIElementParam,
						void* toInternalElementParam )
{
	const TCETriangleStripPrivate* fromAPIElement = (const TCETriangleStripPrivate*) fromAPIElementParam;
	TCETriangleStripPrivate* toInternalElement = (TCETriangleStripPrivate*) toInternalElementParam;
	
	TQ3Status	status = kQ3Failure;
	toInternalElement->indexCount = fromAPIElement->indexCount;
	toInternalElement->editIndex = fromAPIElement->editIndex;
	
	if (toInternalElement->indexCount == 0)
	{
		toInternalElement->indexArray = nullptr;
		status = kQ3Success;
	}
	else
	{
		toInternalElement->indexArray = static_cast<TQ3Uns32*>( Q3Memory_Allocate(
			static_cast<TQ3Uns32>(toInternalElement->indexCount * sizeof(TQ3Uns32)) ) );
		if (toInternalElement->indexArray != nullptr)
		{
			Q3Memory_Copy( fromAPIElement->indexArray,
				const_cast<TQ3Uns32*>( toInternalElement->indexArray ),
				static_cast<TQ3Uns32>(toInternalElement->indexCount * sizeof(TQ3Uns32)) );
			status = kQ3Success;
		}
	}
	return status;
}





//=============================================================================
//      strip_element_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
strip_element_delete( void* param )
{
	TCETriangleStripPrivate* internalElement = (TCETriangleStripPrivate*) param;
	if (internalElement->indexArray != nullptr)
	{
		Q3Memory_Free( &internalElement->indexArray );
	}
	return kQ3Success;
}





//=============================================================================
//      strip_element_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
strip_element_copyreplace( const TCETriangleStripPrivate* fromAPIElement,
						TCETriangleStripPrivate* toInternalElement )
{
	strip_element_delete( toInternalElement );
	return strip_element_copyadd( fromAPIElement, toInternalElement );
}





//=============================================================================
//      strip_element_traverse : Traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
strip_element_traverse( TQ3Object object, TCETriangleStripPrivate *data,
						TQ3ViewObject view )
{
#pragma unused(object)
	TQ3Status	status = Q3XView_SubmitWriteData( view,
		static_cast<TQ3Uns32>((data->indexCount + 1) * sizeof(TQ3Uns32)), data, nullptr );
	return status;
}





//=============================================================================
//      strip_element_write : Write method.
//-----------------------------------------------------------------------------
static TQ3Status
strip_element_write( const void *data, TQ3FileObject theFile )
{
	const TCETriangleStripPrivate*	theData =
		reinterpret_cast<const TCETriangleStripPrivate*>(data);
	
	TQ3Status status = Q3Uns32_Write( theData->indexCount, theFile );
	
	for (TQ3Uns32 i = 0; (i < theData->indexCount) && (status == kQ3Success); ++i)
	{
		status = Q3Uns32_Write( theData->indexArray[i], theFile );
	}
	
	return status;
}





//=============================================================================
//      strip_element_readdata : Read data method.
//-----------------------------------------------------------------------------
static TQ3Status
strip_element_readdata( TQ3Object parentObject, TQ3FileObject theFile )
{
	TQ3Uns32	numIndices;
	TQ3Status	status = Q3Uns32_Read( &numIndices, theFile );
	
	if (status == kQ3Success)
	{
		std::vector<TQ3Uns32>	theArray( numIndices );
		
		if (numIndices > 0)
		{
			status = Q3Uns32_ReadArray( numIndices, &theArray[0], theFile );
		
			if (status == kQ3Success)
			{
				status = E3TriangleStripElement_SetData( parentObject,
					numIndices, &theArray[0] );
			}
		}
		else
		{
			status = E3TriangleStripElement_SetData( parentObject,
					numIndices, nullptr );
		}
	}
	
	return status;
}





//=============================================================================
//      strip_element_metahandler : Strip element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
strip_element_metahandler(TQ3XMethodType methodType)
{
	TQ3XFunctionPointer		theMethod = nullptr;

	switch (methodType)
	{
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) strip_element_traverse;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) strip_element_write;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) strip_element_readdata;
			break;

		case kQ3XMethodTypeElementCopyGet:
			// a straight memory copy will do
			break;

		case kQ3XMethodTypeElementCopyAdd:
		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) strip_element_copyadd;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) strip_element_copyreplace;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) strip_element_delete;
			break;
	}
	
	return theMethod;
}


#pragma mark -


static TQ3Status
alphatest_element_traverse( TQ3Object object, TQ3Float32 *data,
						TQ3ViewObject view )
{
#pragma unused(object)
	TQ3Status	status = Q3XView_SubmitWriteData( view,
		sizeof(TQ3Float32), data, nullptr );
	return status;
}


static TQ3Status
alphatest_element_write( TQ3Float32 *inData, TQ3FileObject file )
{
	TQ3Status	theStatus = Q3Float32_Write( *inData, file );
	return theStatus;
}


static TQ3Status alphatest_element_readdata(
	TQ3Object			ioSet,
	TQ3FileObject			file)
{
	TQ3Float32 threshold;
	
	TQ3Status	theStatus = Q3Float32_Read( &threshold, file );
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Object_AddElement( ioSet, kQ3ElementTypeTextureShaderAlphaTest,
			&threshold );
	}
	return theStatus;
}


static TQ3XFunctionPointer
e3alphatestelement_metahandler(TQ3XMethodType methodType)
{
	TQ3XFunctionPointer		theMethod = nullptr;

	switch (methodType)
	{
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) alphatest_element_traverse;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) alphatest_element_write;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) alphatest_element_readdata;
			break;
	}
	
	return theMethod;
}

#pragma mark -

static TQ3Status
FlipRowElement_traverse( TQ3Object object, void *data, TQ3ViewObject view )
{
#pragma unused(object, data)
	return Q3XView_SubmitWriteData(view, 0, nullptr, nullptr);
}

static TQ3Status
FlipRowElement_readdata( TQ3Object parentObject, TQ3FileObject file )
{
	E3TextureFlippedRowsElement_Add( parentObject );
	return kQ3Success;
}

static TQ3XFunctionPointer
e3texture_flipped_rows_element_metahandler(TQ3XMethodType methodType)
{
	TQ3XFunctionPointer		theMethod = nullptr;

	switch (methodType)
	{
		case kQ3XMethodTypeObjectClassVersion:
			theMethod = (TQ3XFunctionPointer)0x01008000;
			break;

		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) FlipRowElement_traverse;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) FlipRowElement_readdata;
			break;
	}
	
	return theMethod;
}


#pragma mark -

static TQ3Status
SpecularElement_delete( void *inParam )
{
	TQ3TextureObject* textureOb = (TQ3TextureObject*) inParam;
	
	if (*textureOb != nullptr)
	{
		Q3_ASSERT( (*textureOb != nullptr) && (*textureOb)->IsObjectValid() );
		Q3Object_Dispose(*textureOb);
		*textureOb = nullptr;
	}

	return kQ3Success;
}

static TQ3Status
SpecularElement_CopyAdd( const void* fromParam, void* toParam )
{
	const TQ3TextureObject* inFromAPIElement = (const TQ3TextureObject*) fromParam;
	TQ3TextureObject* ioToInternal = (TQ3TextureObject*) toParam;
	
	*ioToInternal = Q3Shared_GetReference( *inFromAPIElement );
	
	return (*ioToInternal != nullptr) ? kQ3Success : kQ3Failure;
}

static TQ3Status
SpecularElement_CopyReplace( const TQ3TextureObject* inFromAPIElement,
							TQ3TextureObject* ioToInternal )
{
	if (*ioToInternal != nullptr)
	{
		Q3_ASSERT( (*ioToInternal)->IsObjectValid() );
		Q3Object_Dispose( *ioToInternal );
	}
	*ioToInternal = Q3Shared_GetReference( *inFromAPIElement );
	
	return (*ioToInternal != nullptr) ? kQ3Success : kQ3Failure;
}

static TQ3Status
SpecularElement_CopyGet( const void* src,
	void* dst )
{
	const TQ3TextureObject* inFromInternal = (const TQ3TextureObject*) src;
	TQ3TextureObject* ioToExternal = (TQ3TextureObject*) dst;
	
	*ioToExternal = Q3Shared_GetReference( *inFromInternal );
	return kQ3Success;
}

static TQ3Status
SpecularElement_CopyDuplicate( TQ3TextureObject *source, TQ3TextureObject *dest )
{
	*dest = Q3Object_Duplicate(*source);

	return (*dest != nullptr) ? kQ3Success : kQ3Failure;
}

static TQ3Status
SpecularElement_traverse( TQ3Object object, TQ3TextureObject *texture, TQ3ViewObject view )
{
#pragma unused(object)

	if (texture == nullptr || *texture == nullptr)
		return kQ3Success;

	// No immediate data
	if (Q3XView_SubmitWriteData(view, 0, nullptr, nullptr) == kQ3Failure)
		return kQ3Failure;

	// submit child object
	return Q3Object_Submit(*texture, view);
}

static TQ3Status
SpecularElement_readdata( TQ3Object parentObject, TQ3FileObject file )
{
	TQ3TextureObject	texture;

	texture = Q3File_ReadObject(file);
	if (texture == nullptr) 
		return kQ3Failure;

	E3SpecularMapElement_Set( parentObject, texture );
	Q3Object_Dispose(texture);
	
	return kQ3Success;
}

static TQ3XFunctionPointer SpecularMetaHandler( TQ3XMethodType methodType )
{
	TQ3XFunctionPointer		theMethod = nullptr;
	
	switch (methodType)
	{
		case kQ3XMethodTypeElementCopyAdd:
			theMethod = (TQ3XFunctionPointer) SpecularElement_CopyAdd;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) SpecularElement_CopyReplace;
			break;

		case kQ3XMethodTypeElementCopyGet:
			theMethod = (TQ3XFunctionPointer) SpecularElement_CopyGet;
			break;

		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) SpecularElement_CopyDuplicate;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) SpecularElement_delete;
			break;

		case kQ3XMethodTypeObjectClassVersion:
			theMethod = (TQ3XFunctionPointer)0x01008000;
			break;

		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) SpecularElement_traverse;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) SpecularElement_readdata;
			break;

	}
	
	return theMethod;
}

#pragma mark -

static TQ3Status
FogMax_traverse( TQ3Object object, void *inData, TQ3ViewObject view )
{
#pragma unused(object)
	if (inData == nullptr)
		return kQ3Success;
	return Q3XView_SubmitWriteData( view, sizeof(float), inData, nullptr );
}

static TQ3Status
FogMax_write( float *inData, TQ3FileObject file )
{
	TQ3Status	theStatus = Q3Float32_Write( *inData, file );
	
	return theStatus;
}

static TQ3Status FogMax_readData(
	TQ3SetObject			ioSet,
	TQ3FileObject			file)
{
	float	theData;
		
	TQ3Status	theStatus = Q3Float32_Read( &theData, file );
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Set_Add( ioSet, sFogMaxElementType, &theData );
	}
	return theStatus;
}


static TQ3XFunctionPointer FogMax_MetaHandler( TQ3XMethodType methodType )
{
	TQ3XFunctionPointer		theMethod = nullptr;
	
	switch (methodType)
	{
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) FogMax_traverse;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) FogMax_write;
			break;
			
		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) FogMax_readData;
			break;
		
		case kQ3XMethodTypeObjectClassVersion:
			theMethod = (TQ3XFunctionPointer)0x01108000;
			break;
	}
	
	return theMethod;
}

#pragma mark -

static TQ3Status
HalfspaceFog_traverse( TQ3Object object, void *inData, TQ3ViewObject view )
{
#pragma unused(object)
	if (inData == nullptr)
		return kQ3Success;
	return Q3XView_SubmitWriteData( view, sizeof(TCEHalfspaceFogData), inData, nullptr );
}

static TQ3Status
HalfspaceFog_write( const TCEHalfspaceFogData *inData, TQ3FileObject file )
{
	TQ3Status	theStatus = Q3Float32_Write( inData->rate, file );
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Float32_Write( inData->plane.x, file );
	}
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Float32_Write( inData->plane.y, file );
	}
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Float32_Write( inData->plane.z, file );
	}
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Float32_Write( inData->plane.w, file );
	}
	
	return theStatus;
}

static TQ3Status HalfspaceFog_readData(
	TQ3SetObject			ioSet,
	TQ3FileObject			file)
{
	TCEHalfspaceFogData	theData;
		
	TQ3Status	theStatus = Q3Float32_Read( &theData.rate, file );
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Float32_Read( &theData.plane.x, file );
	}
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Float32_Read( &theData.plane.y, file );
	}
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Float32_Read( &theData.plane.z, file );
	}
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Float32_Read( &theData.plane.w, file );
	}
	
	if (theStatus == kQ3Success)
	{
		theStatus = Q3Set_Add( ioSet, sHalfspaceFogElementType, &theData );
	}
	return theStatus;
}

static TQ3XFunctionPointer HalfspaceFog_MetaHandler( TQ3XMethodType methodType )
{
	TQ3XFunctionPointer		theMethod = nullptr;
	
	switch (methodType)
	{
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) HalfspaceFog_traverse;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) HalfspaceFog_write;
			break;
			
		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) HalfspaceFog_readData;
			break;
		
		case kQ3XMethodTypeObjectClassVersion:
			theMethod = (TQ3XFunctionPointer)0x01108000;
			break;
	}
	
	return theMethod;
}


//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Customelements_RegisterClass : Register the classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3CustomElements_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the classes
	qd3dStatus = Q3_REGISTER_CLASS (	
				kQ3ClassNameCustomElementName,
				e3nameelement_metahandler,
				E3NameElement ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	
					kQ3ClassNameCustomElementUrl,
					e3urlelement_metahandler,
					E3URLElement ) ;
	
	if (qd3dStatus == kQ3Success)
	{
		if (nullptr == Q3XElementClass_Register( &sTriangleStripElementType,
			kQ3ClassNameCustomElementTriangleStrip,
			sizeof(TCETriangleStripPrivate),
			strip_element_metahandler ))
		{
			qd3dStatus = kQ3Failure;
		}
	}

	if (qd3dStatus == kQ3Success)
	{
		if (nullptr == Q3XElementClass_Register( &sFogMaxElementType,
			kQ3ClassNameCustomElementFogMaxOpacity,
			sizeof(float),
			FogMax_MetaHandler ))
		{
			qd3dStatus = kQ3Failure;
		}
	}
	
	if (qd3dStatus == kQ3Success)
	{
		if (nullptr == Q3XElementClass_Register( &sHalfspaceFogElementType,
			kQ3ClassNameCustomElementHalfspaceFog,
			sizeof(TCEHalfspaceFogData),
			HalfspaceFog_MetaHandler ))
		{
			qd3dStatus = kQ3Failure;
		}
	}

	// The depth bits element does not need a metahandler, because it is plain old data
	// and is never read or written (being attached to renderers, which aren't read or
	// written).
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	
					kQ3ClassNameCustomElementDepthBits,
					nullptr,
					E3BitDepthElement ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	
					kQ3ClassNameCustomElementAlphaTest,
					e3alphatestelement_metahandler,
					E3TextureAlphaTestElement );

	if (qd3dStatus == kQ3Success)
	{
		qd3dStatus = Q3_REGISTER_CLASS(
					kQ3ClassNameCustomElementSpecularMap,
					SpecularMetaHandler,
					E3SpecularElement );
	}

	if (qd3dStatus == kQ3Success)
	{
		if (nullptr == Q3XElementClass_Register( &sFlippedRowsElementType,
			kQ3ClassNameCustomElementFlipRows,
			0,
			e3texture_flipped_rows_element_metahandler ))
		{
			qd3dStatus = kQ3Failure;
		}
	}

	return(qd3dStatus);
}





//=============================================================================
//      E3Customelements_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3CustomElements_UnregisterClass(void)
{



	// Unregister the classes
	E3ClassTree::UnregisterClass(kQ3ObjectTypeCustomElementName, kQ3True);
	E3ClassTree::UnregisterClass(kQ3ObjectTypeCustomElementUrl,  kQ3True);
	E3ClassTree::UnregisterClass(kQ3ElementTypeDepthBits,  kQ3True);
	E3ClassTree::UnregisterClass(kQ3ElementTypeTextureShaderAlphaTest,  kQ3True);
	E3ClassTree::UnregisterClass(kQ3ObjectTypeCustomElementSpecularMap,  kQ3True);
	E3ClassTree::UnregisterClass(sFlippedRowsElementType,  kQ3True);
	E3ClassTree::UnregisterClass(sTriangleStripElementType,  kQ3True);
	E3ClassTree::UnregisterClass(sFogMaxElementType,  kQ3True);
	E3ClassTree::UnregisterClass(sHalfspaceFogElementType,  kQ3True);

	return(kQ3Success);
}





#pragma mark -

//=============================================================================
//      E3NameElement_SetData : : Attach a name to an object.
//-----------------------------------------------------------------------------
//		Note :	Passing nullptr for the name parameter removes the Name Element
//				from the object, if one is present.
//-----------------------------------------------------------------------------
TQ3Status
E3NameElement_SetData(TQ3Object object, const char *name)
{
	TQ3Status status;
	TQ3StringObject	string = nullptr;

	if (name == nullptr)
		return object->ClearElement( kQ3ObjectTypeCustomElementName );


	string = E3CString_New(name);
	if (string == nullptr)
		return kQ3Failure;
				
	status = object->AddElement( kQ3ObjectTypeCustomElementName, &string );

	string->Dispose();
	
	return status;
}





//=============================================================================
//      E3NameElement_GetData : Get the name  for an object.
//-----------------------------------------------------------------------------
//		Note :	The memory for the name must be disposed of by the caller.
//-----------------------------------------------------------------------------
TQ3Status
E3NameElement_GetData(TQ3Object object, char **name)
{
	TQ3Status status;
	TQ3StringObject	string = nullptr;



	// Initialise a return value
	*name = nullptr;

	
	if (Q3Object_ContainsElement(object, kQ3ObjectTypeCustomElementName)) {
	
		status = Q3Object_GetElement(object, kQ3ObjectTypeCustomElementName, &string);
		if (status == kQ3Failure)
			return status;
			
	} else {

		return kQ3Failure;
	}

	status = Q3CString_GetString(string, name);
	Q3Object_Dispose(string);
					
	return status;
}





//=============================================================================
//      E3NameElement_PeekData : Get the name of an object.
//-----------------------------------------------------------------------------
//		Note :	The returned pointer should be consider read-only and temporary.
//-----------------------------------------------------------------------------
TQ3Status	E3NameElement_PeekData(TQ3Object object, const char **name)
{
	TQ3Status status;
	TQ3StringObject	string = nullptr;



	// Initialise a return value
	*name = nullptr;


	status = object->GetElement( kQ3ObjectTypeCustomElementName, &string );
	
	if (status == kQ3Success)
	{
		*name = E3CString_PeekString( string );
		
		string->Dispose();
	}

	return status;
}



//=============================================================================
//      E3NameElement_EmptyData : Dispose of the memory allocated.
//-----------------------------------------------------------------------------
TQ3Status
E3NameElement_EmptyData(char **name)
{


	// Frees name and sets it to nullptr
	return Q3CString_EmptyData(name);
}





//=============================================================================
//      E3UrlElement_SetData : Set the URL for an object.
//-----------------------------------------------------------------------------
//		Note :	Passing nullptr for the name parameter removes the URL Element
//				from the object, if one is present.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3UrlElement_SetData(TQ3Object object, TCEUrlData *urlData)
{
	TQ3Status 			status;
	TCEUrlDataPrivate  	urlDataPrivate;
	TQ3StringObject		string = nullptr;

	// That's what QD3D does: you can't hold a description without url
	if (urlData == nullptr || urlData->url == nullptr ||urlData->url[0] == '\0')
		return Q3Object_ClearElement(object, kQ3ObjectTypeCustomElementUrl);


	// Validate our parameters
	Q3_ASSERT(urlData->options == kCEUrlOptionNone || urlData->options == kCEUrlOptionUseMap);
	

	urlDataPrivate.url = urlData->url;
	urlDataPrivate.options = urlData->options;
	urlDataPrivate.description = nullptr;

	char* desc = urlData->description;
	if (desc != nullptr) {

		string = Q3CString_New( desc );

		if (string == nullptr)
			return kQ3Failure;
		else
			urlDataPrivate.description = string;
	}

	status = Q3Object_AddElement(object, kQ3ObjectTypeCustomElementUrl, &urlDataPrivate);
		
	if (string != nullptr)
		Q3Object_Dispose(string);

	return status;
}





//=============================================================================
//      E3UrlElement_GetData : Get the URL for an object.
//-----------------------------------------------------------------------------
//		Note :	The memory for the name must be disposed of by the caller.
//-----------------------------------------------------------------------------
TQ3Status
E3UrlElement_GetData(TQ3Object object, TCEUrlData **urlData)
{
	TQ3Status 			status;
	TCEUrlDataPrivate  	urlDataPrivate;


	// Initialise a return value
	*urlData = nullptr;


	if (Q3Object_ContainsElement(object, kQ3ObjectTypeCustomElementUrl)) {
	
		status = Q3Object_GetElement(object, kQ3ObjectTypeCustomElementUrl, &urlDataPrivate);
		if (status == kQ3Failure)
			return status;
			
	} else {
	
		return kQ3Failure;
	}


	*urlData = (TCEUrlData*)Q3Memory_Allocate(sizeof(TCEUrlData));
	TQ3Uns32 dstSize = static_cast<TQ3Uns32>(strlen(urlDataPrivate.url) + 1);
	(**urlData).url = (char*)Q3Memory_Allocate(dstSize);
		
	SAFE_STRCPY((**urlData).url, urlDataPrivate.url, dstSize);
	
	(**urlData).options = urlDataPrivate.options;
	(**urlData).description = nullptr;

	if (urlDataPrivate.description) {
	
		status = Q3CString_GetString(urlDataPrivate.description, &(**urlData).description);
		Q3Object_Dispose(urlDataPrivate.description);
	}

	return status;
}





//=============================================================================
//      E3UrlElement_EmptyData : Dispose of the memory allocated.
//-----------------------------------------------------------------------------
TQ3Status
E3UrlElement_EmptyData(TCEUrlData **urlData)
{


	Q3Memory_Free(&(**urlData).url);

	if ((**urlData).description)
		Q3CString_EmptyData(&(**urlData).description);

	Q3Memory_Free(urlData);
	
	return kQ3Success; 
}




#pragma mark -



/*!
	@function	E3TriangleStripElement_SetData
	@abstract	Set a triangle strip for the object.
	@discussion	Triangle strips are used by the OpenGL renderer as a speed
				optimization for rendering TriMesh objects.  If you have not
				already provided a triangle strip for a TriMesh, the renderer
				will compute one, but this can take a little time.
				
				You can pass 0 for inNumIndices and nullptr for inIndices to
				indicate that you want to avoid using a triangle strip, perhaps
				because there is no efficient strip for this geometry.
				
				When you assign a triangle strip, the element also records the
				current edit index of the object.
				
				<em>This function is not available in QD3D.</em>
	@param		ioObject		An object, normally a TriMesh.
	@param		inNumIndices	Count of indices in the following array.
	@param		inIndices		Array of vertex indices making the strip.
	@result		Success or failure of the operation.
*/
TQ3Status	E3TriangleStripElement_SetData(
	TQ3Object ioObject,
	TQ3Uns32 inNumIndices,
	const TQ3Uns32* inIndices
)
{
	TQ3GeometryObject theGeom = ioObject;
	CQ3ObjectRef nakedGeom;
	if (Q3Object_IsType( theGeom, kQ3GeometryTypeTriMesh ))
	{
		nakedGeom = CQ3ObjectRef( E3TriMesh_GetNakedGeometry( theGeom ) );
		theGeom = nakedGeom.get();
	}
	
	// Lock the edit index, so that adding an element won't change it.
	StLockEditIndex lockIndex( theGeom );
	
	TQ3Uns32	editIndex = Q3Shared_GetEditIndex( theGeom );
	
	TCETriangleStripPrivate	theData = {
		inNumIndices,
		editIndex,
		inIndices
	};
	TQ3Status status = Q3Object_AddElement( theGeom, sTriangleStripElementType,
		&theData );


	return status;
}

/*!
	@function	E3TriangleStripElement_GetData
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
TQ3Status	E3TriangleStripElement_GetData(
	TQ3Object inObject,
	TQ3Uns32* outNumIndices,
	const TQ3Uns32** outIndices
)
{
	TQ3GeometryObject theGeom = inObject;
	CQ3ObjectRef nakedGeom;
	if (Q3Object_IsType( theGeom, kQ3GeometryTypeTriMesh ))
	{
		nakedGeom = CQ3ObjectRef( E3TriMesh_GetNakedGeometry( theGeom ) );
		theGeom = nakedGeom.get();
	}
	TCETriangleStripPrivate	theData;
	TQ3Status	status = Q3Object_GetElement( theGeom,
		sTriangleStripElementType, &theData );
	if (status == kQ3Success)
	{
		TQ3Uns32	curIndex = Q3Shared_GetEditIndex( theGeom );
		
		if (curIndex == theData.editIndex)
		{
			*outNumIndices = theData.indexCount;
			*outIndices = theData.indexArray;
		}
		else
		{
			status = kQ3Failure;	// stale cache
		}
	}
	return status;
}



/*!
	@function	E3TriangleStripElement_Remove
	@abstract	Remove a triangle strip element if it exists.
	@param		ioObject		An object, normally a TriMesh.
*/
void		E3TriangleStripElement_Remove( TQ3Object ioObject )
{
	TQ3GeometryObject theGeom = ioObject;
	CQ3ObjectRef nakedGeom;
	if (Q3Object_IsType( theGeom, kQ3GeometryTypeTriMesh ))
	{
		nakedGeom = CQ3ObjectRef( E3TriMesh_GetNakedGeometry( theGeom ) );
		theGeom = nakedGeom.get();
	}
	Q3Object_ClearElement( theGeom, sTriangleStripElementType );
}

#pragma mark -

void		E3TextureFlippedRowsElement_Add( TQ3TextureObject inTexture )
{
	int dummy;
	Q3Object_AddElement( inTexture, sFlippedRowsElementType, &dummy );
}

TQ3Boolean	E3TextureFlippedRowsElement_IsPresent( TQ3TextureObject inTexture )
{
	return Q3Object_ContainsElement( inTexture, sFlippedRowsElementType );
}

void		E3TextureFlippedRowsElement_Remove( TQ3TextureObject inTexture )
{
	Q3Object_ClearElement( inTexture, sFlippedRowsElementType );
}


#pragma mark -

float		E3FogMaxElement_Get( TQ3StyleObject inFogStyle )
{
	float theOpacity;
	TQ3Status getStatus = Q3Object_GetElement( inFogStyle, sFogMaxElementType,
		&theOpacity );
	if (getStatus == kQ3Failure)
	{
		theOpacity = 1.0f;
	}
	return theOpacity;
}

void		E3FogMaxElement_Set( TQ3StyleObject ioFogStyle, float inMaxOpacity )
{
	Q3Object_AddElement( ioFogStyle, sFogMaxElementType, &inMaxOpacity );
}

#pragma mark -

TQ3Status	E3HalfspaceFogElement_Get( TQ3StyleObject inFogStyle,
										TCEHalfspaceFogData* outData )
{
	TQ3Status getStatus = Q3Object_GetElement( inFogStyle,
		sHalfspaceFogElementType, outData );
	return getStatus;
}

void		E3HalfspaceFogElement_Set( TQ3StyleObject inFogStyle,
										const TCEHalfspaceFogData* inData )
{
	if (inData == nullptr)
	{
		Q3Object_ClearElement( inFogStyle, sHalfspaceFogElementType );
	}
	else
	{
		Q3Object_AddElement( inFogStyle, sHalfspaceFogElementType, inData );
	}
}

#pragma mark -


/*!
	@function	E3SpecularMapElement_Copy
	@abstract	Retrieve a specular map texture from an object.
	@param		shader		An object, normally a surface shader.
	@result		A new reference to a texture, or nullptr.
*/
TQ3TextureObject	E3SpecularMapElement_Copy( TQ3ShaderObject shader )
{
	TQ3TextureObject theTexture;
	TQ3Status status = Q3Object_GetElement( shader, kQ3ObjectTypeCustomElementSpecularMap, &theTexture );
	if (status == kQ3Failure)
	{
		theTexture = nullptr;
	}
	return theTexture;
}

/*!
	@function	E3SpecularMapElement_Set
	@abstract	Set or remove a specular map.
	@param		shader		A surface shader.
	@param		texture		A texture object, or nullptr to remove.
*/
void	E3SpecularMapElement_Set( TQ3ShaderObject shader, TQ3TextureObject texture )
{
	if (texture == nullptr)
	{
		Q3Object_ClearElement( shader, kQ3ObjectTypeCustomElementSpecularMap );
	}
	else
	{
		Q3Object_AddElement( shader, kQ3ObjectTypeCustomElementSpecularMap, &texture );
	}
}

