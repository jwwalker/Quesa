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
	TQ3Uns32			instanceSize ; // Includes all parents instance data


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
	TQ3Object			CreateInstance ( TQ3Boolean sharedParams, const void* paramData ) ;

	
	friend class E3ClassTree ;
	friend class OpaqueTQ3Object ;
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
	static TQ3Status		RegisterExternalClass (	TQ3ObjectType		parentClassType,
											TQ3ObjectType		classType,
											const char			*className,
											TQ3XMetaHandler		classMetaHandler,
											TQ3Int32			instanceSize ) ;
	static TQ3Status		UnregisterClass ( TQ3ObjectType classType, TQ3Boolean isRequired ) ;


	// Create, destroy, duplicate, and locate an instance of a class
	static TQ3Object		CreateInstance (	TQ3ObjectType	classType,
												TQ3Boolean		sharedParams,
												const void		*paramData ) ;

	// Locate a class
	static E3ClassInfoPtr	GetClass ( TQ3ObjectType classType ) ;
	static E3ClassInfoPtr	GetClass ( const char *className ) ;
	static E3ClassInfoPtr	GetClass ( TQ3Object theObject ) ;
	static void				AddMethod ( TQ3ObjectType classType, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod ) ;
	static void				Dump ( void ) ;


	
	friend class E3ClassInfo ;
	} ;




// root object data
class OpaqueTQ3Object
	{
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


	void						DestroyInstance ( void ) ;
	TQ3Object					DuplicateInstance ( void ) ;
	void*						FindLeafInstanceData ( void ) ;
	TQ3ObjectType				GetObjectType ( TQ3ObjectType baseType ) ;
	TQ3Object					GetLeafObject ( void ) ;
	TQ3Boolean					IsObjectValid ( void ) ;


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



//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

