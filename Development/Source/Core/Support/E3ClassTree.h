/*  NAME:
        E3ClassTree.h

    DESCRIPTION:
        Header file for E3ClassTree.c.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#ifndef E3CLASSTREE_HDR
#define E3CLASSTREE_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here


#include "E3HashTable.h"


//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------

class OpaqueTQ3Object ;

// Nodes in the class tree have all their fields private
typedef struct E3ClassInfo *E3ClassInfoPtr ;

// A single node within the class tree
class E3ClassInfo
	{
	// Class
	TQ3ObjectType		classType ;
	char				*className ;
	TQ3XMetaHandler		classMetaHandler ;
	E3HashTablePtr		methodTable ;


	// Instances
	TQ3Uns32			numInstances ;
	TQ3Uns32			instanceSize ;
	TQ3Boolean			includesParentData ;	


	// Parent/children
	TQ3Uns32			numChildren ;
	E3ClassInfoPtr		theParent ;
	E3ClassInfoPtr		*theChildren ;
	
	
	
	static TQ3Status	Attach ( E3ClassInfoPtr theChild, E3ClassInfoPtr theParent ) ;
	void				Detach ( void ) ;	
	E3ClassInfoPtr		Find ( const char *className ) ;
	TQ3XFunctionPointer	Find_Method ( TQ3XMethodType methodType, TQ3Boolean canInherit ) ;
	void				Dump_Class ( FILE *theFile, TQ3Uns32 indent ) ;

public :

	// Retrieve the information for a class
	E3ClassInfoPtr		GetParent ( void ) ;
	TQ3Uns32			GetNumChildren ( void ) ;
	E3ClassInfoPtr		GetChild ( TQ3Uns32 childIndex ) ;
	TQ3Boolean			IsType ( TQ3ObjectType theType ) ;

	TQ3ObjectType		GetType () ;
	const char*			GetName ( void ) ;
	TQ3XMetaHandler		GetMetaHandler ( void ) ;
	TQ3Uns32			GetInstanceSize ( void ) ;
	TQ3Uns32			GetNumInstances ( void ) ;
	TQ3XFunctionPointer GetMethod ( TQ3XMethodType methodType ) ;
	void				AddMethod ( TQ3XMethodType methodType, TQ3XFunctionPointer theMethod ) ;

	
	friend class E3ClassTree ;
	friend class E3InstanceNode ;
	} ;




class E3ClassTree
	{
public :
	static void				Destroy ( void ) ;


	// Get the next available class type (for extensions)
	static TQ3ObjectType	GetNextClassType ( void ) ;


	// Register and unregister a class
	static TQ3Status		RegisterClass (	TQ3ObjectType		parentClassType,
											TQ3ObjectType		classType,
											const char			*className,
											TQ3XMetaHandler		classMetaHandler,
											TQ3Int32			instanceSize ) ;
	static TQ3Status		UnregisterClass ( TQ3ObjectType classType, TQ3Boolean isRequired ) ;


	// Create, destroy, duplicate, and locate an instance of a class
	static TQ3Object		CreateInstance (	TQ3ObjectType	classType,
												TQ3Boolean		sharedParams,
												const void		*paramData,
												TQ3Boolean		leaf ) ;
	static TQ3Object		CreateInstance (	TQ3ObjectType	classType,
												TQ3Boolean		sharedParams,
												const void		*paramData )
		{ return CreateInstance ( classType , sharedParams , paramData , kQ3True ) ; }

	// Locate a class
	static E3ClassInfoPtr	GetClass ( TQ3ObjectType classType ) ;
	static E3ClassInfoPtr	GetClass ( const char *className ) ;
	static E3ClassInfoPtr	GetClass ( TQ3Object theObject ) ;
	static void				AddMethod ( TQ3ObjectType classType, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod ) ;
	static void				Dump ( void ) ;


	
	friend class E3ClassInfo ;
	} ;




class E3InstanceNode // To be merged into OpaqueTQ3Object later
	{
	friend class E3InstanceNode;
	TQ3ObjectType		quesaTag ;
	E3ClassInfoPtr		theClass ;
	void				*instanceData ; // To be removed later
	E3InstanceNode*		parentObject ; // To be removed later
#if Q3_DEBUG
	E3InstanceNode*		childObject ; // To be removed later
#endif



// Private methods :

	void				Verify () ;
	TQ3Status			InitialiseInstanceData (	E3ClassInfoPtr	theClass ,
													TQ3Boolean		sharedParams ,
												    const void		*paramData ) ;
	void				DeleteInstanceData ( E3ClassInfoPtr	theClass ) ;
	TQ3Status			DuplicateInstanceData ( TQ3Object		newObject ,
												E3ClassInfoPtr	theClass ) ;



public :

	void				DestroyInstance ( void ) ;
	TQ3Object			DuplicateInstance ( void ) ;
	void*				FindLeafInstanceData ( void ) ;
	TQ3ObjectType		GetObjectType ( TQ3ObjectType baseType ) ;
	TQ3Object			GetLeafObject ( void ) ;
	TQ3Boolean			IsObjectValid ( void ) ;


	// Locate a class
	E3ClassInfoPtr		GetClass ( void ) ;
	TQ3XFunctionPointer GetMethod ( TQ3XMethodType methodType ) ;
	
	
	friend class E3ClassInfo ;
	friend class E3ClassTree ;
	friend class OpaqueTQ3Object ;
	} ;




// root object data
class OpaqueTQ3Object : public E3InstanceNode
	{
	TQ3SetObject				theSet ;
	E3InstanceNode*				leafInstanceData ; // Temporary
	
#if Q3_DEBUG
public :
	TQ3Object					prev ;
	TQ3Object					next ;
	struct TQ3StackCrawlRec*	stackCrawl ;
private :
#endif


public :

	void				DestroyInstance ( void ) { leafInstanceData->DestroyInstance () ; }
	TQ3Object			DuplicateInstance ( void ) ;
	void*				FindLeafInstanceData ( void ) { return leafInstanceData->FindLeafInstanceData () ; }
	TQ3ObjectType		GetObjectType ( TQ3ObjectType baseType ) { return leafInstanceData->GetObjectType ( baseType ) ; }
	TQ3Object			GetLeafObject ( void ) { return leafInstanceData->GetLeafObject () ; }
	TQ3Boolean			IsObjectValid ( void ) { return leafInstanceData->IsObjectValid () ; }


	// Locate a class
	E3ClassInfoPtr		GetClass ( void ) { return leafInstanceData->GetClass () ; }
	TQ3XFunctionPointer GetMethod ( TQ3XMethodType methodType ) { return leafInstanceData->GetMethod ( methodType ) ; }

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
	friend TQ3Status			e3root_new ( TQ3Object theObject, void *privateData, void *paramData ) ;
	friend TQ3Status			e3root_duplicate ( TQ3Object fromObject, const void *fromPrivateData, TQ3Object toObject, void *toPrivateData) ;
	friend void					e3root_delete ( TQ3Object theObject, void *privateData ) ;
	} ;


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Destroy the class tree
inline void			E3ClassTree_Destroy ( void ) {	E3ClassTree::Destroy () ; }


// Get the next available class type (for extensions)
inline
TQ3ObjectType		E3ClassTree_GetNextClassType ( void ) { return E3ClassTree::GetNextClassType () ; }


// Register and unregister a class
inline TQ3Status	E3ClassTree_RegisterClass(TQ3ObjectType			parentClassType,
												TQ3ObjectType		classType,
												const char			*className,
												TQ3XMetaHandler		classMetaHandler,
												TQ3Int32			instanceSize)
	{ return E3ClassTree::RegisterClass (	parentClassType,
											classType,
											className,
											classMetaHandler,
											instanceSize ) ; }
inline TQ3Status	E3ClassTree_UnregisterClass(TQ3ObjectType classType, TQ3Boolean isRequired)
	{ return E3ClassTree::UnregisterClass( classType, isRequired); }

// Create, destroy, duplicate, and locate an instance of a class
inline TQ3Object	E3ClassTree_CreateInstance(TQ3ObjectType	classType,
												TQ3Boolean		sharedParams,
												const void		*paramData)
	{ return E3ClassTree::CreateInstance (	classType,
											sharedParams,
											paramData,
											kQ3True ) ; }
inline void			E3ClassTree_DestroyInstance(TQ3Object theObject)
	{ theObject->DestroyInstance () ; }
inline
TQ3ObjectType		E3ClassTree_GetObjectType(TQ3Object theObject, TQ3ObjectType baseType)
	{ return theObject->GetObjectType ( baseType ) ; }
inline TQ3Object	E3ClassTree_GetLeafObject(TQ3Object theObject)
	{ return theObject->GetLeafObject () ; }
inline TQ3Boolean	E3ClassTree_IsObjectValid(TQ3Object theObject)
	{ return theObject->IsObjectValid () ; }


// Locate a class
inline
E3ClassInfoPtr		E3ClassTree_GetClassByType ( TQ3ObjectType classType )
	{ return E3ClassTree::GetClass ( classType ) ; }
inline
E3ClassInfoPtr		E3ClassTree_GetClassByName ( const char *className )
	{ return E3ClassTree::GetClass ( className ) ; }
inline
E3ClassInfoPtr		E3ClassTree_GetClassByObject ( TQ3Object theObject )
	{ return theObject->GetClass () ; }


// Retrieve the information for a class
inline
E3ClassInfoPtr		E3ClassTree_GetParent(E3ClassInfoPtr theClass)
	{ return theClass->GetParent () ; }
inline
TQ3Uns32			E3ClassTree_GetNumChildren(E3ClassInfoPtr theClass)
	{ return theClass->GetNumChildren () ; }
inline
E3ClassInfoPtr		E3ClassTree_GetChild(E3ClassInfoPtr theClass, TQ3Uns32 childIndex)
	{ return theClass->GetChild ( childIndex ) ; }
inline
TQ3Boolean			E3ClassTree_IsType(E3ClassInfoPtr theClass, TQ3ObjectType theType)
	{ return theClass->IsType ( theType ) ; }

inline
TQ3ObjectType		E3ClassTree_GetType(E3ClassInfoPtr theClass)
	{ return theClass->GetType () ; }
inline
const char*			E3ClassTree_GetName(E3ClassInfoPtr theClass)
	{ return theClass->GetName () ; }
inline
TQ3XMetaHandler		E3ClassTree_GetMetaHandler(E3ClassInfoPtr theClass)
	{ return theClass->GetMetaHandler () ; }
inline
TQ3Uns32			E3ClassTree_GetInstanceSize(E3ClassInfoPtr theClass)
	{ return theClass->GetInstanceSize () ; }
inline
TQ3Uns32			E3ClassTree_GetNumInstances(E3ClassInfoPtr theClass)
	{ return theClass->GetNumInstances () ; }
inline
TQ3XFunctionPointer E3ClassTree_GetMethod(E3ClassInfoPtr theClass, TQ3XMethodType methodType)
	{ return theClass->GetMethod ( methodType ) ; }
inline
TQ3XFunctionPointer E3ClassTree_GetMethodByObject(TQ3Object theObject, TQ3XMethodType methodType)
	{ return theObject->GetMethod ( methodType ) ; }
inline
void				E3ClassTree_AddMethod(E3ClassInfoPtr theClass, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod)
	{ theClass->AddMethod ( methodType , theMethod ) ; }
inline
void				E3ClassTree_AddMethodByType(TQ3ObjectType classType, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod)
	{ E3ClassTree::AddMethod ( classType , methodType , theMethod ) ; }


// Dump some stats on the class tree
inline
void				E3ClassTree_Dump(void)
	{ E3ClassTree::Dump () ; }





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

