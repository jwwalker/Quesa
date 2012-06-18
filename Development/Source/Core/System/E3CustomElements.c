/*  NAME:
        E3CustomElements.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3CustomElements.h"
#include "E3HashTable.h"
#include "E3Main.h"
#include "E3Set.h"
#include "E3String.h"

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
	


#if QUESA_SUPPORT_QUICKTIME
class E3WireElement : public E3Element  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeCustomElementWire, E3WireElement, E3Element )
public :

	QTAtomContainer							instanceData ;
	} ;
#endif
	


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
	







//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kQ3ClassNameCustomElementDepthBits		"Quesa:DepthBitsElement"
#define kQ3ClassNameCustomElementBeforeRender	"Quesa:BeforeRenderCallback"
#define kQ3ClassNameCustomElementAfterRender	"Quesa:AfterRenderCallback"
#define kQ3ClassNameCustomElementBeforePick		"Quesa:BeforePickCallback"
#define kQ3ClassNameCustomElementAfterPick		"Quesa:AfterPickCallback"
#define kQ3ClassNameCustomElementAlphaTest		"Quesa:AlphaTestElement"





//=============================================================================
//      Static variables
//-----------------------------------------------------------------------------
static TQ3ElementType	sTriangleStripElementType = 0;





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

	if (string == NULL || *string == NULL)
		return kQ3Success;

	if (Q3XView_SubmitWriteData(view, 0, NULL, NULL) == kQ3Failure)
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
	if (string == NULL) 
		return kQ3Failure;
		
	status = Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementName, &string);
	Q3Object_Dispose(string);
	
	return status;
}





//=============================================================================
//      e3nameelement_copyadd : Copy add/get method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyadd (TQ3StringObject *source, TQ3StringObject *dest)
{
	*dest = Q3Shared_GetReference(*source);

	return (*dest != NULL) ? kQ3Success : kQ3Failure;
}





//=============================================================================
//      e3nameelement_copyduplicate : Copy duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyduplicate( TQ3StringObject *source, TQ3StringObject *dest)
{
	*dest = Q3Object_Duplicate(*source);

	return (*dest != NULL) ? kQ3Success : kQ3Failure;
}





//=============================================================================
//      e3nameelement_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyreplace (TQ3StringObject *source, TQ3StringObject *dest)
{
	TQ3StringObject	temp;
	
	temp = Q3Shared_GetReference(*source);
	if (temp == NULL) 
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
e3nameelement_delete (TQ3StringObject *string)
{
	if (*string != NULL) {
		Q3Object_Dispose(*string);
		*string = NULL;
	}

	return kQ3Success;
}





//=============================================================================
//      e3nameelement_metahandler : Name element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3nameelement_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



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


	if (urlData == NULL || urlData->url == NULL)
		return kQ3Success;

	size = Q3Size_Pad(strlen(urlData->url) + 1);
	size += sizeof(TQ3Uns32);

	if (Q3XView_SubmitWriteData (view, size, urlData, NULL) == kQ3Failure)
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
	strcpy (urlData.url, buffer);
	
	if (Q3Uns32_Read ((TQ3Uns32*)&urlData.options, file) == kQ3Failure)
		return kQ3Failure;

	if (Q3File_IsEndOfContainer(file, NULL) == kQ3False) {
		urlData.description = Q3File_ReadObject(file);
	}

	return Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementUrl, &urlData);
}





//=============================================================================
//      e3urlelement_copyadd : Copy add/get/duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_copyadd (TCEUrlDataPrivate *source, TCEUrlDataPrivate *dest)
{
	dest->url = (char*)Q3Memory_Allocate(strlen(source->url) + 1);
	if (dest->url == NULL)
		return kQ3Failure;

	strcpy (dest->url, source->url);

	if (source->description) {  // optional
		
		// In the case of duplication, this needs to be Q3Object_Duplicate
		// rather than Q3Shared_Duplicate.  In the case of Add and Get,
		// it doesn't matter much, since the strings being copied are
		// short-lived.
		dest->description = Q3Object_Duplicate( source->description );
		if (dest->description == NULL) {
			return kQ3Failure;
		}
		
	} else {

		dest->description = NULL;
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
	TQ3StringObject	string = NULL;

	if (Q3Memory_Reallocate(&dest->url ,strlen(source->url) + 1) == kQ3Failure)
		return kQ3Failure;
		
	strcpy (dest->url, source->url);

	if (source->description) {  // optional

		string = Q3Shared_GetReference (source->description);
		if (string == NULL) {
			return kQ3Failure;
		}
		
		if (dest->description) {
			Q3Object_Dispose (dest->description);
		}

		dest->description = string;
		
	} else {

		dest->description = NULL;
	}
		
	dest->options = source->options;
	
	return kQ3Success;
}





//=============================================================================
//      e3urlelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_delete (TCEUrlDataPrivate *urlData)
{
	if (urlData->url != NULL) {
	
		Q3Memory_Free (&urlData->url);
		urlData->url = NULL;
	}
	
	if (urlData->description != NULL) {
	
		Q3Object_Dispose (urlData->description);
		urlData->description = NULL;
	}

	return kQ3Success;
}





//=============================================================================
//      e3urlelement_metahandler : URL element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3urlelement_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



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





//=============================================================================
//      e3wireelement_traverse : Traverse method (writing).
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_QUICKTIME
#pragma mark -
static TQ3Status
e3wireelement_traverse (TQ3Object object, QTAtomContainer *atom, TQ3ViewObject view)
{
#pragma unused(object, atom, view)



	// To be implemented...
	return kQ3Failure;
}





//=============================================================================
//      e3wireelement_write : Write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_write (QTAtomContainer *urlData, TQ3FileObject file)
{
#pragma unused( urlData, file )

	// To be implemented...
	return kQ3Failure;
}





//=============================================================================
//      e3wireelement_readdata : Read data method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_readdata (TQ3Object object, TQ3FileObject file)
{
#pragma unused( object, file )

	// To be implemented...
	return kQ3Failure;
}





//=============================================================================
//      e3wireelement_copyadd : Copy add/get/duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_copyadd(QTAtomContainer *source, QTAtomContainer *dest)
{


	// Copy the element
	*dest = *source;

	return(kQ3Success);
}





//=============================================================================
//      e3wireelement_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_copyreplace(QTAtomContainer *source, QTAtomContainer *dest)
{


	// Copy the element
	*dest = *source;

	return(kQ3Success);
}





//=============================================================================
//      e3wireelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_delete(QTAtomContainer *atom)
{
#pragma unused( atom )


	// We don't currently copy the atom container, so nothing to do
	return(kQ3Success);
}





//=============================================================================
//      e3wireelement_metahandler : Wire element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3wireelement_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) e3wireelement_traverse;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3wireelement_write;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) e3wireelement_readdata;
			break;

		case kQ3XMethodTypeElementCopyAdd:
		case kQ3XMethodTypeElementCopyGet:
		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) e3wireelement_copyadd;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3wireelement_copyreplace;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) e3wireelement_delete;
			break;
		}

	return(theMethod);
}
#endif




#pragma mark -

//=============================================================================
//      strip_element_copyadd : Copy add/duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
strip_element_copyadd( const TCETriangleStripPrivate* fromAPIElement,
						TCETriangleStripPrivate* toInternalElement )
{
	TQ3Status	status = kQ3Failure;
	toInternalElement->indexCount = fromAPIElement->indexCount;
	toInternalElement->editIndex = fromAPIElement->editIndex;
	
	if (toInternalElement->indexCount == 0)
	{
		toInternalElement->indexArray = NULL;
		status = kQ3Success;
	}
	else
	{
		toInternalElement->indexArray = static_cast<TQ3Uns32*>( Q3Memory_Allocate(
			toInternalElement->indexCount * sizeof(TQ3Uns32) ) );
		if (toInternalElement->indexArray != NULL)
		{
			Q3Memory_Copy( fromAPIElement->indexArray,
				const_cast<TQ3Uns32*>( toInternalElement->indexArray ),
				toInternalElement->indexCount * sizeof(TQ3Uns32) );
			status = kQ3Success;
		}
	}
	return status;
}





//=============================================================================
//      strip_element_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
strip_element_delete( TCETriangleStripPrivate* internalElement )
{
	if (internalElement->indexArray != NULL)
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
		(data->indexCount + 1) * sizeof(TQ3Uns32), data, NULL );
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
					numIndices, NULL );
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
	TQ3XFunctionPointer		theMethod = NULL;

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
		sizeof(TQ3Float32), data, NULL );
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
	TQ3XFunctionPointer		theMethod = NULL;

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
		if (NULL == Q3XElementClass_Register( &sTriangleStripElementType,
			kQ3ClassNameCustomElementTriangleStrip,
			sizeof(TCETriangleStripPrivate),
			strip_element_metahandler ))
		{
			qd3dStatus = kQ3Failure;
		}
	}

#if QUESA_SUPPORT_QUICKTIME
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	
					kQ3ClassNameCustomElementWire,
					e3wireelement_metahandler,
					E3WireElement ) ;
#endif

	// The depth bits element does not need a metahandler, because it is plain old data
	// and is never read or written (being attached to renderers, which aren't read or
	// written).
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	
					kQ3ClassNameCustomElementDepthBits,
					NULL,
					E3BitDepthElement ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	
					kQ3ClassNameCustomElementAlphaTest,
					e3alphatestelement_metahandler,
					E3TextureAlphaTestElement );


	return(qd3dStatus);
}





//=============================================================================
//      E3Customelements_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3CustomElements_UnregisterClass(void)
{



	// Unregister the classes
#if QUESA_SUPPORT_QUICKTIME
	E3ClassTree::UnregisterClass(kQ3ObjectTypeCustomElementWire, kQ3True);
#endif

	E3ClassTree::UnregisterClass(kQ3ObjectTypeCustomElementName, kQ3True);
	E3ClassTree::UnregisterClass(kQ3ObjectTypeCustomElementUrl,  kQ3True);
	E3ClassTree::UnregisterClass(kQ3ElementTypeDepthBits,  kQ3True);
	E3ClassTree::UnregisterClass(kQ3ElementTypeTextureShaderAlphaTest,  kQ3True);

	return(kQ3Success);
}





#pragma mark -

//=============================================================================
//      E3NameElement_SetData : : Attach a name to an object.
//-----------------------------------------------------------------------------
//		Note :	Passing NULL for the name parameter removes the Name Element
//				from the object, if one is present.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3NameElement_SetData(TQ3Object object, const char *name)
{
	TQ3Status status;
	TQ3StringObject	string = NULL;

	if (name == NULL)
		return Q3Object_ClearElement(object, kQ3ObjectTypeCustomElementName);


	string = Q3CString_New(name);
	if (string == NULL)
		return kQ3Failure;
				
	status = Q3Object_AddElement(object, kQ3ObjectTypeCustomElementName, &string);
	Q3Object_Dispose(string);
	
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
	TQ3StringObject	string = NULL;



	// Initialise a return value
	*name = NULL;

	
	if (Q3Object_ContainsElement(object, kQ3ObjectTypeCustomElementName)) {
	
		status = Q3Object_GetElement(object, kQ3ObjectTypeCustomElementName, (TQ3StringObject)&string);
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
	TQ3StringObject	string = NULL;



	// Initialise a return value
	*name = NULL;


	status = Q3Object_GetElement( object, kQ3ObjectTypeCustomElementName,
		&string );
	
	if (status == kQ3Success)
	{
		*name = E3CString_PeekString( string );
		
		Q3Object_Dispose(string);
	}
					
	return status;
}



//=============================================================================
//      E3NameElement_EmptyData : Dispose of the memory allocated.
//-----------------------------------------------------------------------------
TQ3Status
E3NameElement_EmptyData(char **name)
{


	// Frees name and sets it to NULL
	return Q3CString_EmptyData(name);
}





//=============================================================================
//      E3UrlElement_SetData : Set the URL for an object.
//-----------------------------------------------------------------------------
//		Note :	Passing NULL for the name parameter removes the URL Element
//				from the object, if one is present.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3UrlElement_SetData(TQ3Object object, TCEUrlData *urlData)
{
	TQ3Status 			status;
	TCEUrlDataPrivate  	urlDataPrivate;
	TQ3StringObject		string = NULL;

	// That's what QD3D does: you can't hold a description without url
	if (urlData == NULL || urlData->url == NULL ||urlData->url[0] == '\0')
		return Q3Object_ClearElement(object, kQ3ObjectTypeCustomElementUrl);


	// Validate our parameters
	Q3_ASSERT(urlData->options == kCEUrlOptionNone || urlData->options == kCEUrlOptionUseMap);
	

	urlDataPrivate.url = urlData->url;
	urlDataPrivate.options = urlData->options;
	urlDataPrivate.description = NULL;

	if (urlData->description != NULL) {

		string = Q3CString_New (urlData->description);

		if (string == NULL)
			return kQ3Failure;
		else
			urlDataPrivate.description = string;
	}

	status = Q3Object_AddElement(object, kQ3ObjectTypeCustomElementUrl, &urlDataPrivate);
		
	if (string != NULL)
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
	*urlData = NULL;


	if (Q3Object_ContainsElement(object, kQ3ObjectTypeCustomElementUrl)) {
	
		status = Q3Object_GetElement(object, kQ3ObjectTypeCustomElementUrl, &urlDataPrivate);
		if (status == kQ3Failure)
			return status;
			
	} else {
	
		return kQ3Failure;
	}


	*urlData = (TCEUrlData*)Q3Memory_Allocate(sizeof(TCEUrlData));
	(**urlData).url = (char*)Q3Memory_Allocate(strlen(urlDataPrivate.url) + 1);
		
	strcpy ((**urlData).url, urlDataPrivate.url);
	
	(**urlData).options = urlDataPrivate.options;
	(**urlData).description = NULL;

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





//=============================================================================
//      E3WireElement_SetData : Set the atom container element.
//-----------------------------------------------------------------------------
//		Note : We don't currently copy atom containers.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_SUPPORT_QUICKTIME
TQ3Status
E3WireElement_SetData(TQ3Object object, QTAtomContainer wireData)
{	TQ3Status		qd3dStatus;



	// Add the element
	qd3dStatus = Q3Object_AddElement(object, kQ3ObjectTypeCustomElementWire, &wireData);

	return(qd3dStatus);
}





//=============================================================================
//      E3WireElement_GetData : Get the atom container element.
//-----------------------------------------------------------------------------
//		Note : We don't currently copy atom containers.
//-----------------------------------------------------------------------------
TQ3Status
E3WireElement_GetData(TQ3Object object, QTAtomContainer *wireData)
{	TQ3Status			qd3dStatus;



	// Initialise a return value
	*wireData = NULL;



	// Get the element
	qd3dStatus = kQ3Success;
	if (Q3Object_ContainsElement(object, kQ3ObjectTypeCustomElementWire))
		qd3dStatus = Q3Object_GetElement(object, kQ3ObjectTypeCustomElementWire, wireData);

	return(qd3dStatus);
}





//=============================================================================
//      E3WireElement_EmptyData : Release an atom container element.
//-----------------------------------------------------------------------------
//		Note :	We don't currently copy atom containers, so for now we just
//				reset the pointer.
//-----------------------------------------------------------------------------
TQ3Status
E3WireElement_EmptyData(QTAtomContainer *wireData)
{


	// Reset the pointer
	*wireData = NULL;

	return(kQ3Success);
}
#endif // QUESA_SUPPORT_QUICKTIME

#pragma mark -



/*!
	@function	E3TriangleStripElement_SetData
	@abstract	Set a triangle strip for the object.
	@discussion	Triangle strips are used by the OpenGL renderer as a speed
				optimization for rendering TriMesh objects.  If you have not
				already provided a triangle strip for a TriMesh, the renderer
				will compute one, but this can take a little time.
				
				You can pass 0 for inNumIndices and NULL for inIndices to
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
	TQ3Uns32	editIndex = Q3Shared_GetEditIndex( ioObject );
	
	TCETriangleStripPrivate	theData = {
		inNumIndices,
		 editIndex,
		inIndices
	};
	TQ3Status status = Q3Object_AddElement( ioObject, sTriangleStripElementType,
		&theData );
	
	
	// Adding the element bumped the edit index, set it back.
	Q3Shared_SetEditIndex( ioObject, editIndex );


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
	TCETriangleStripPrivate	theData;
	TQ3Status	status = Q3Object_GetElement( inObject,
		sTriangleStripElementType, &theData );
	if (status == kQ3Success)
	{
		TQ3Uns32	curIndex = Q3Shared_GetEditIndex( inObject );
		
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
	Q3Object_ClearElement( ioObject, sTriangleStripElementType );
}
