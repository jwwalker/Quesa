/*  NAME:
        E3Main.h

    DESCRIPTION:
        Header file for E3Main.c.

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
#ifndef E3MAIN_HDR
#define E3MAIN_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here


#include "E3Memory.h"


//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif


//=============================================================================
//      Macros
//-----------------------------------------------------------------------------

// N.B although we don't actually use _class yet we might one day
#define Q3_CLASS_ENUMS(_type, _class, _parentClass) 			\
public :														\
	enum														\
		{														\
		eClassDepth = _parentClass::eClassDepth + 1 ,			\
		eClassType = _type ,									\
		eParentType = _parentClass::eClassType					\
		} ;														\
private :		



#define Q3_OBJECT_IS_CLASS(_object, _class) ((_object)->IsClass ( _class::eClassType, _class::eClassDepth ))


#define Q3_REGISTER_CLASS(_parentType, _infoClass, _Name, _metaHandler, _instanceClass )	\
	E3ClassTree::RegisterClass	( new ( std::nothrow ) _infoClass							\
										(													\
										_metaHandler, 										\
										E3ClassTree::GetClass ( _parentType )				\
										),													\
								_instanceClass::eClassType,									\
								_Name,														\
								sizeof ( _instanceClass )									\
								)
	
//=============================================================================
//      Types
//-----------------------------------------------------------------------------



class E3Root : public E3ClassInfo
	{
	TQ3XObjectDisposeMethod			disposeMethod ;

public :

									E3Root	(
											TQ3XMetaHandler	newClassMetaHandler,
											E3ClassInfo*	newParent
			 								) ; // constructor	
			 								
	friend class OpaqueTQ3Object ;
	} ;




// root object data
struct OpaqueTQ3Object
	{
	enum
		{
		eClassDepth = 0 ,
		eClassType = kQ3ObjectTypeRoot
		} ;
private :

	TQ3ObjectType				quesaTag ;
	E3ClassInfoPtr				theClass ;
	TQ3SetObject				theSet ;
	
#if Q3_DEBUG
	TQ3Object					prev ;
	TQ3Object					next ;
	struct TQ3StackCrawlRec*	stackCrawl ;
friend TQ3Status E3Memory_ForgetRecording ( void ) ;
friend TQ3Uns32 E3Memory_CountRecords ( void ) ;
friend TQ3Object E3Memory_NextRecordedObject ( TQ3Object inObject ) ;
friend TQ3Status E3Memory_DumpRecording ( const char* fileName, const char* memo ) ;
#endif


// Private methods :

	void						Verify () ;
	TQ3Status					InitialiseInstanceData (	E3ClassInfoPtr	theClass ,
															TQ3Boolean		sharedParams ,
												   			const void		*paramData ) ;
	void						DeleteInstanceData ( E3ClassInfoPtr	theClass ) ;
	TQ3Status					DuplicateInstanceData ( TQ3Object		newObject ,
														E3ClassInfoPtr	theClass ) ;



public :


	TQ3Status					Dispose ( void ) ;
	void						DestroyInstance ( void ) ;
	TQ3Object					DuplicateInstance ( void ) ;
	void*						FindLeafInstanceData ( void ) ;
	TQ3ObjectType				GetObjectType ( TQ3ObjectType baseType ) ;
	TQ3ObjectType				GetLeafType ( void ) { return GetClass ()->GetType () ; }
	TQ3Object					GetLeafObject ( void ) ;
	TQ3Boolean					IsObjectValid ( void ) { return (TQ3Boolean) ( quesaTag == kQ3ObjectTypeQuesa ) ; }
	TQ3Boolean					IsClass ( TQ3ObjectType queryClass, TQ3Int32 queryDepth )
												{ return (TQ3Boolean) ( theClass->ownAndParentTypes [ queryDepth ] == queryClass ) ; }


	// Locate a class
	E3ClassInfoPtr				GetClass ( void ) ;
	TQ3XFunctionPointer 		GetMethod ( TQ3XMethodType methodType ) ;



	TQ3Status  					AddElement ( TQ3ElementType theType, const void *theData ) ;
	TQ3Status					GetElement ( TQ3ElementType theType, void *theData ) ;
	TQ3Boolean					ContainsElement ( TQ3ElementType theType ) ;
	TQ3Status					GetNextElementType ( TQ3ElementType *theType ) ;
	TQ3Status					EmptyElements ( void ) ;
	TQ3Status					ClearElement ( TQ3ElementType theType )	;
	TQ3Status					GetSet ( TQ3SetObject *set ) ;
	TQ3Status					SetSet ( TQ3SetObject set ) ;
	TQ3Status					SubmitElements ( TQ3ViewObject inView ) ;
	
	
	friend class E3ClassTree ;
	friend class E3ClassInfo ;
	friend TQ3Status			e3root_new ( TQ3Object theObject, void *privateData, void *paramData ) ;
	friend TQ3Status			e3root_duplicate ( TQ3Object fromObject, const void *fromPrivateData, TQ3Object toObject, void *toPrivateData) ;
	friend void					e3root_delete ( TQ3Object theObject, void *privateData ) ;
	} ;



// Shared object data
class E3Shared : public OpaqueTQ3Object
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeShared, E3Shared, OpaqueTQ3Object )
	TQ3Uns32		refCount;
	TQ3Uns32		editIndex;
	
	friend TQ3Status	e3shared_new ( E3Shared* theObject, void *privateData, void *paramData ) ;
	friend void			e3shared_dispose ( E3Shared* theObject ) ;
	friend TQ3Status	e3shared_duplicate (	TQ3Object fromObject,
												const void *fromPrivateData,
						 						TQ3Object toObject,
						 						void *toPrivateData ) ;
						 						
public :


	E3Shared*			GetReference ( void ) ;
	TQ3Boolean			IsReferenced ( void ) ;
	TQ3Uns32			GetReferenceCount ( void ) ;	
	TQ3Uns32			GetEditIndex ( void ) ;
	TQ3Status			Edited ( void ) ;
	} ;



class E3ShapeData : public E3Shared
	{
Q3_CLASS_ENUMS ( kQ3SharedTypeShape, E3ShapeData, E3Shared )
	// Currently empty
	} ;





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

TQ3Status			E3Object_CleanDispose(TQ3Object *object);
TQ3Object			E3Object_Duplicate(TQ3Object theObject);
TQ3Status			E3Object_Submit(TQ3Object theObject, TQ3ViewObject theView);
TQ3Boolean			E3Object_IsDrawable(TQ3Object theObject);
TQ3Boolean			E3Object_IsWritable(TQ3Object theObject, TQ3FileObject theFile);
TQ3ObjectType		E3Object_GetType(TQ3Object theObject);
TQ3Boolean			E3Object_IsType(TQ3Object theObject, TQ3ObjectType theType);

TQ3ObjectType		E3Shared_GetType(TQ3SharedObject sharedObject);
inline TQ3Status	E3Shared_Edited ( TQ3SharedObject sharedObject )
	{ return ( (E3Shared*) sharedObject )->Edited () ; }

TQ3ObjectType		E3Shape_GetType(TQ3ShapeObject theShape);
TQ3Status			E3Shape_GetSet(TQ3ShapeObject theShape, TQ3SetObject *theSet);
TQ3Status			E3Shape_SetSet(TQ3ShapeObject theShape, TQ3SetObject theSet);
inline TQ3Status	E3Shape_SubmitElements( TQ3ShapeObject inShape, TQ3ViewObject inView )
	{ return inShape->SubmitElements ( inView ) ; }

TQ3Status			E3Bitmap_Empty(TQ3Bitmap *theBitmap);
TQ3Uns32			E3Bitmap_GetImageSize(TQ3Uns32 theWidth, TQ3Uns32 theHeight);
TQ3Boolean			E3Bitmap_GetBit(const TQ3Bitmap *theBitmap, TQ3Uns32 x, TQ3Uns32 y);
TQ3Status			E3Bitmap_SetBit(TQ3Bitmap *theBitmap, TQ3Uns32 x, TQ3Uns32 y, TQ3Boolean theState);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

