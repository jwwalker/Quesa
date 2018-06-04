/*  NAME:
        E3FFW_3DMFBin_Writer.c

    DESCRIPTION:
        Quesa 3DMF Binary Writer.

    COPYRIGHT:
        Copyright (c) 1999-2015, Quesa Developers. All rights reserved.

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
#include "E3View.h"
#include "E3FFW_3DMFBin_Writer.h"
#include "E3Main.h"



static TQ3Status
e3ffw_3DMF_TraverseObject_CheckRef(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data		*fileFormatPrivate,
					TQ3Object		theObject,
					TQ3ObjectType		objectType,
					const void			*objectData,
					TQ3Boolean*			wroteReference );



//=============================================================================
//      e3ffw_3DMF_filter_in_toc : Adds the object to the TOC if needed and
//      returns a reference object
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_filter_in_toc(TE3FFormatW3DMF_Data *fileFormatPrivate,  TQ3Object theObject , TQ3Object *theReference)
{
	const TQ3Uns32 TOC_GROW_SIZE = 1024;

	TE3FFormat3DMF_TOC	*toc = fileFormatPrivate->toc;
	TQ3Uns32			tocSize, i;

	TQ3Boolean			createReference = kQ3False;
	TQ3Boolean			forceTOC = kQ3False;
	
	switch (fileFormatPrivate->fileMode)
		{
		case kQ3FFormatWriterType3DMFStreamBin:
		case kQ3FFormatWriterType3DMFStreamBinSwap:
			// do nothing
			*theReference = Q3Shared_GetReference(theObject);
			return (kQ3Success);
			
		case kQ3FFormatWriterType3DMFDatabaseStreamBin:
		case kQ3FFormatWriterType3DMFDatabaseStreamBinSwap:
			forceTOC = kQ3True;
			break;
			
		case kQ3FFormatWriterType3DMFDatabaseBin:
		case kQ3FFormatWriterType3DMFDatabaseBinSwap:
			forceTOC = kQ3True;
		case kQ3FFormatWriterType3DMFNormalBin:
		case kQ3FFormatWriterType3DMFNormalBinSwap:
			createReference = kQ3True;
			break;
			
		}
		
	// if this is the first time allocate the TOC
	
	if(toc == NULL)
		{
		tocSize = sizeof(TE3FFormat3DMF_TOC) + 
				(sizeof(TE3FFormat3DMF_TOCEntry) * (TOC_GROW_SIZE - 1));
				
		toc = fileFormatPrivate->toc = (TE3FFormat3DMF_TOC*) Q3Memory_AllocateClear(tocSize);
		if(toc == NULL)
			return (kQ3Failure);
			
		toc->refSeed = 1;
		toc->typeSeed = -1;
		
		}
	
	if (fileFormatPrivate->index == NULL)
	{
		fileFormatPrivate->index = new TE3FFormatW3DMF_Map;
	}
	
	
	// If the object is already in the table of contents, we want to find it,
	// and if it is not, we will add it.  We need only search the index once.
	
	TE3FFormatW3DMF_Map::value_type newRec( theObject, toc->nEntries );
	
	std::pair< TE3FFormatW3DMF_Map::iterator, bool > insertResult =
		fileFormatPrivate->index->insert( newRec );
		
	if (insertResult.second) // inserted a new entry, so it was not there before
	{
		// make room for the new TOC entry

		if ((toc->nEntries != 0) && (toc->nEntries % TOC_GROW_SIZE == 0))
		{
			tocSize = static_cast<TQ3Uns32>(sizeof(TE3FFormat3DMF_TOC) + 
			
					(sizeof(TE3FFormat3DMF_TOCEntry) * (toc->nEntries + TOC_GROW_SIZE - 1)));
			if(Q3Memory_Reallocate(&fileFormatPrivate->toc,tocSize) != kQ3Success)
				return (kQ3Failure);
				
			toc = fileFormatPrivate->toc;
		}
			
		if (forceTOC == kQ3True)
		{
			toc->tocEntries[toc->nEntries].refID = toc->refSeed;
			toc->refSeed++;
		}
		else
			toc->tocEntries[toc->nEntries].refID = 0;
			
		toc->tocEntries[toc->nEntries].object = Q3Shared_GetReference(theObject);
		toc->tocEntries[toc->nEntries].objType = fileFormatPrivate->lastObjectType;
		toc->tocEntries[toc->nEntries].objLocation.hi = 0;
		toc->tocEntries[toc->nEntries].objLocation.lo = 0; // will be filled in e3ffw_3DMF_write_objects
		
		fileFormatPrivate->lastTocIndex = toc->nEntries;
		
		toc->nEntries++;
		
		
		*theReference = Q3Shared_GetReference(theObject);
	}
	else	// the object was already there
	{
		i = insertResult.first->second;
		
		if (createReference == kQ3True)
		{
			if (toc->tocEntries[i].refID == 0)
			{
				toc->tocEntries[i].refID = toc->refSeed;
				toc->refSeed++;
			}
				
			*theReference = E3ClassTree::CreateInstance(kQ3ShapeTypeReference, kQ3False, &toc->tocEntries[i].refID);
		}
		else
		{
			*theReference = Q3Shared_GetReference(theObject);
		}
	}
		
	return (kQ3Success);
}




//=============================================================================
//      E3FFW_3DMF_type_Traverse : Traverse method for 'type' object.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_type_Traverse( TQ3Object object, void *data, TQ3ViewObject view )
	{
	TQ3ObjectType customType = *(TQ3ObjectType*) data ;
	
	
	// Find the class and class name.
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( customType ) ;
	if ( theClass != NULL )
		{
		const char* className = theClass->GetName () ;
		
		// Compute the object size.
		TQ3Uns32 size = static_cast<TQ3Uns32>(Q3Size_Pad( static_cast<TQ3Uns32>(strlen(className) + 1) ) + sizeof ( TQ3ObjectType ));
		
		// Put data on the stack.
		return Q3XView_SubmitWriteData ( view, size, data, NULL ) ;
		}
	
	return kQ3Failure ;
	}



//=============================================================================
//      E3FFW_3DMF_type_Write : Write method for 'type' object.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_type_Write( const void *data, TQ3FileObject file )
	{
	TQ3Status	status = kQ3Failure ;
	
	
	TQ3ObjectType customType = *(TQ3ObjectType*) data ;
	
	
	// Find the class and class name.
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( customType ) ;
	if ( theClass != NULL )
		{
		const char* className = theClass->GetName () ;
		
		// Write the data.
		status = Q3Uns32_Write ( customType, file ) ;
		
		if ( status == kQ3Success )
			status = Q3String_Write ( className, file ) ;
		}
	return status ;
	}




//=============================================================================
//      e3ffw_3DMF_write_custom_types : Write a 'type' object for each custom type.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_write_custom_types( TQ3ViewObject				theView,
								TE3FFormatW3DMF_Data		*fileFormatPrivate,
								E3ClassInfoPtr				theClass )
	{
	TQ3Status	status = kQ3Success ;
	
	
	Q3_REQUIRE_OR_RESULT( theClass != NULL, kQ3Failure ) ;


	// If this is a custom class with instances, we need to write a type object
	// for it.
	TQ3ObjectType theType = theClass->GetType () ;
	
	if ( ( theType < 0 ) && ( theClass->GetNumInstances () > 0 ) )
		status = E3FFW_3DMF_TraverseObject ( theView, fileFormatPrivate, NULL,
			kQ3ObjectTypeType, (const void*)&theType ) ;
	
	
	// Recurse the class hierarchy
	TQ3Uns32 numChildren = theClass->GetNumChildren () ;
	
	for ( TQ3Uns32 i = 0 ; ( status == kQ3Success ) &&  ( i < numChildren ) ; ++i )
		{
		E3ClassInfoPtr childClass = theClass->GetChild ( i ) ;
		
		status = e3ffw_3DMF_write_custom_types ( theView, fileFormatPrivate, childClass ) ;
		}
	

	return status ;
	}




//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFW_3DMF_StartFile : Start a file, write headers.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_StartFile(TQ3ViewObject				theView,
						TE3FFormatW3DMF_Data		*fileFormatPrivate,
						TQ3DrawContextObject	theDrawContext)
{
#pragma unused(theDrawContext)
  	TQ3Status status = fileFormatPrivate->baseData.currentStoragePosition > 0 ? kQ3Success :
  						E3FFW_3DMF_TraverseObject (theView, fileFormatPrivate, NULL, kQ3ObjectType3DMF, fileFormatPrivate);
	
	
	if (status == kQ3Success)
		{
		status = e3ffw_3DMF_write_custom_types( theView, fileFormatPrivate,
			E3ClassTree::GetClass (kQ3ObjectTypeRoot) );
		}
	
	
	return status;
}



//=============================================================================
//      E3FFW_3DMF_EndPass : finish the file.
//
//		note: I'm using this instead the End File, because at that point the view submit loop is closed
//				and we can't submit the TOC
//
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3FFW_3DMF_EndPass(TQ3ViewObject				theView,
						TE3FFormatW3DMF_Data		*fileFormatPrivate)
{
	TQ3Status				status = kQ3Success;
	TE3FFormat3DMF_TOC		*toc = fileFormatPrivate->toc;
	TQ3Uns64 				pos = {0,0};
	TQ3FileObject 			theFile = E3View_AccessFile (theView);
	
	if(toc != NULL) // write the toc
		{
		pos.lo = fileFormatPrivate->baseData.currentStoragePosition;
		status = E3FFW_3DMF_TraverseObject (theView, fileFormatPrivate, NULL, kQ3ObjectTypeTOC, fileFormatPrivate);
		
		if((status == kQ3Success) && (pos.lo != fileFormatPrivate->baseData.currentStoragePosition))// something has been written 
			{
				fileFormatPrivate->baseData.currentStoragePosition = 16;
				Q3Uns64_Write(pos, theFile);
			}
		
		}
	
	
	return kQ3ViewStatusDone;
}



//=============================================================================
//      E3FFW_3DMF_Group : handle groups.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Group(TQ3ViewObject       theView,
						void                *fileFormatPrivate,
						TQ3GroupObject   theGroup,
						TQ3ObjectType		objectType,
						const void		*objectData)
{
#pragma unused(objectType)
#pragma unused(objectData)
	TQ3Status							qd3dStatus;
	TQ3GroupPosition					position;
	TQ3Object							subObject;
	TQ3DisplayGroupState				groupState;
	TQ3Boolean							wroteReference;
	
	if(Q3Group_GetType (theGroup) == kQ3GroupTypeDisplay){
		Q3DisplayGroup_GetState (theGroup, &groupState);
		if((groupState & kQ3DisplayGroupStateMaskIsWritten) != kQ3DisplayGroupStateMaskIsWritten)
			return kQ3Success;
		}
		
 	// submit the group start tag
	qd3dStatus = e3ffw_3DMF_TraverseObject_CheckRef (theView,
		(TE3FFormatW3DMF_Data*)fileFormatPrivate, theGroup,
		Q3Object_GetLeafType (theGroup), theGroup->FindLeafInstanceData (), &wroteReference);

	// If we actually wrote a reference instead of a group start tag, then we don't
	// want to write contents and group end.
	if (wroteReference == kQ3True)
		{
		 return qd3dStatus;
		}
	
 	// submit the group contents
	for(Q3Group_GetFirstPosition (theGroup, &position);
		(position != NULL) && (qd3dStatus == kQ3Success);
		Q3Group_GetNextPosition (theGroup, &position))
		{
		qd3dStatus = Q3Group_GetPositionObject (theGroup, position, &subObject);
		if(qd3dStatus != kQ3Success)
			break;
		qd3dStatus = Q3Object_Submit (subObject, theView);
		
		Q3Object_Dispose (subObject);
		}
		
 	// submit the group end tag
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3FFW_3DMF_TraverseObject (theView,
			(TE3FFormatW3DMF_Data*)fileFormatPrivate, NULL, 0x656E6467 /* endg - EndGroup*/, NULL);


    return(qd3dStatus);
}





//=============================================================================
//      E3FFW_3DMFBin_Cancel: Cancel a pass.
//-----------------------------------------------------------------------------
void
E3FFW_3DMF_Cancel(TQ3ViewObject theView, TE3FFormatW3DMF_Data *fileFormatPrivate)
{
#pragma unused(theView)
#pragma unused(fileFormatPrivate)
	// To Be implemented
}



//=============================================================================
//      E3FFW_3DMF_Close: Close the format.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Close( TQ3FileFormatObject format, TQ3Boolean abort )
{
	TE3FFormatW3DMF_Data*	instanceData = (TE3FFormatW3DMF_Data*) format->FindLeafInstanceData () ;
	TQ3Status				status = kQ3Success;
	TE3FFormat3DMF_TOC		*toc = instanceData->toc;
	TQ3Uns32				i;
	
	if(toc != NULL) // delete the toc
		{
		for(i = 0; i < toc->nEntries; i++)
			{
			if(toc->tocEntries[i].object != NULL)
				{
				Q3Object_Dispose(toc->tocEntries[i].object);
				}
			}
		Q3Memory_Free(&instanceData->toc);
		}
	
	if (instanceData->index != NULL)
	{
		delete instanceData->index;
	}
		
			
	return status;
}


//=============================================================================
//      e3ffw_3DMF_write_objects: unroll the stack and do the real write.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_write_objects(TE3FFormatW3DMF_Data *instanceData, TQ3FileObject theFile)
{	TQ3Status		qd3dStatus = kQ3Success;
	
	TQ3Size					size;
	TQ3Uns32				i,j;
	TQ3ObjectType			container;
	TQ3Uns32				lastLevel;
#if Q3_DEBUG
	TQ3Uns32 pos;
#endif

	for(i=0; i<instanceData->stackCount; i++){
	
		container = 0;
		
		if(qd3dStatus == kQ3Success){
		
			if(instanceData->stack[i].tocIndex != kQ3ArrayIndexNULL)
				{ // fill in the object position in the TOC
				instanceData->toc->tocEntries[instanceData->stack[i].tocIndex].objLocation.lo =
							 instanceData->baseData.currentStoragePosition;
				}


			//
			// check for groups
			//
			switch (instanceData->stack[i].objectType) {
				case kQ3GroupTypeDisplay:
				case kQ3DisplayGroupTypeOrdered:
				case kQ3DisplayGroupTypeIOProxy:
				case kQ3GroupTypeInfo:
				case kQ3ShapeTypeGroup:
					container = 0x62676E67; /* bgng - BeginGroup */
					Q3_ASSERT(instanceData->stack[i].level == 0);
					break;
				case kQ3GroupTypeLight:
					container = 0x62676E67; /* bgng - BeginGroup */
					Q3_ASSERT(instanceData->stack[i].level == 1); // always seems to be 1 for a light group inside a view hints object
					break;
				default:
					if(i < instanceData->stackCount-1)
						if(instanceData->stack[i+1].level > instanceData->stack[i].level)
							container = 0x636E7472; /* cntr - Container */
			}
			
					
					
			if(container != 0){
					
				qd3dStatus = Q3Uns32_Write(container,theFile);
				if(qd3dStatus == kQ3Success){
				
					//compute container size
					size = instanceData->stack[i].size; //the container object size
					j=i+1;
					lastLevel = instanceData->stack[i].level;
					while(j < instanceData->stackCount)
						if(instanceData->stack[j].level <= instanceData->stack[i].level)
							break;
						else{
							if(instanceData->stack[j].level > lastLevel)
								size += 8; // the container object type & size
							size += instanceData->stack[j].size; //the contained object size
							size += 8; // the contained object type & size
							lastLevel = instanceData->stack[j].level;
							j++;
							}
					
					// If the container only contains one thing, e.g., a bgng
					// contains a dspg and nothing else, we still need to account
					// for the type and size bytes of that one thing.
					// I'm not sure that this is the correct fix. - JWW
					if (j == i + 1)
						size += 8;
					
					//write container size
					qd3dStatus = Q3Uns32_Write(size,theFile);
					}
				}
				
					
				
				//write class
				if(qd3dStatus == kQ3Success)
					qd3dStatus = Q3Uns32_Write(instanceData->stack[i].objectType,theFile);
				//write size
				if(qd3dStatus == kQ3Success)
					qd3dStatus = Q3Uns32_Write(instanceData->stack[i].size,theFile);
	#if Q3_DEBUG
				//get storage pos
				pos = instanceData->baseData.currentStoragePosition;
	#endif
				//call write method
				if((instanceData->stack[i].writeMethod != NULL) && (qd3dStatus == kQ3Success))
					qd3dStatus = instanceData->stack[i].writeMethod(instanceData->stack[i].data,theFile);
					
				//assert storage position
				//instanceData->baseData.currentStoragePosition = Q3Size_Pad(instanceData->baseData.currentStoragePosition);
				// Due to the TriMesh compression scheme, things can end up on unpadded
				// boundaries.
				Q3_ASSERT(pos+instanceData->stack[i].size == instanceData->baseData.currentStoragePosition);
				}
			//call deletedata if any
			if (instanceData->stack[i].theObject != NULL && Q3Object_IsType(instanceData->stack[i].theObject, kQ3ObjectTypeShared))
				E3Shared_Replace(&instanceData->stack[i].theObject,NULL);
			else
				instanceData->stack[i].theObject = NULL;
				
			if(instanceData->stack[i].deleteData != NULL)
				instanceData->stack[i].deleteData(instanceData->stack[i].data);
			
		}
	
	return (qd3dStatus);
}



//=============================================================================
//      E3FFW_3DMF_TraverseObject: Do the traverse.
//-----------------------------------------------------------------------------
//      this is the bottleneck for any 3DMF writing,
//		any Quesa object pass here to be written
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_TraverseObject(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data		*fileFormatPrivate,
					TQ3Object		theObject,
					TQ3ObjectType		objectType,
					const void			*objectData)
{
	TQ3Boolean	wroteRef;
	return e3ffw_3DMF_TraverseObject_CheckRef( theView, fileFormatPrivate, theObject,
		 objectType, objectData, &wroteRef );
}							



//=============================================================================
//      e3ffw_3DMF_TraverseObject_CheckRef: Do the traverse.
//-----------------------------------------------------------------------------
//      This is identical to E3FFW_3DMF_TraverseObject, except that it informs
//		the caller whether it wrote a reference object.
//		E3FFW_3DMF_Group needs to know.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_TraverseObject_CheckRef(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data		*fileFormatPrivate,
					TQ3Object		theObject,
					TQ3ObjectType		objectType,
					const void			*objectData,
					TQ3Boolean*			wroteReference )
							
{	TQ3Status		qd3dStatus = kQ3Success;
	TQ3Object		submittedObject;
	TQ3ObjectType	old_lastObjectType;
	TQ3Object		old_lastObject;
	TQ3Uns32		old_lastTocIndex;
	E3ClassInfoPtr theClass = NULL;
	TQ3XObjectTraverseMethod traverse;
	TQ3FileObject theFile = E3View_AccessFile (theView);
	
	*wroteReference = kQ3False;	// until further notice
	
	old_lastTocIndex = fileFormatPrivate->lastTocIndex;
	old_lastObject = fileFormatPrivate->lastObject;
	old_lastObjectType = fileFormatPrivate->lastObjectType;

//	fileFormatPrivate->lastObject = theObject; // called below since it can be substituted by a reference
	fileFormatPrivate->lastObjectType = objectType;
	fileFormatPrivate->lastTocIndex = kQ3ArrayIndexNULL;
	
	submittedObject = theObject;
	
	//find the object traverse method
	if(theObject != NULL){
		if(Q3Object_IsType(theObject, kQ3ObjectTypeShared))
			{
			if(e3ffw_3DMF_filter_in_toc(fileFormatPrivate, theObject, &submittedObject) != kQ3Success)
				return (kQ3Failure);
			}
			// will add a reference to the object
			// to retain the eventually made reference object
		if(submittedObject != theObject)
			{
			fileFormatPrivate->lastObjectType = Q3Object_GetLeafType(submittedObject);
			objectData = submittedObject->FindLeafInstanceData () ;
			}
			
		theClass = submittedObject->GetClass () ;
		}
	else
		theClass = E3ClassTree::GetClass ( objectType ) ;
	
	if (theClass == NULL)
		goto exit;

	
	traverse = (TQ3XObjectTraverseMethod)
					theClass->GetMethod ( kQ3XMethodTypeObjectTraverse ) ;
	if (traverse == NULL)
		goto exit;

	// mark our level
	fileFormatPrivate->baseData.groupDeepCounter++;
	fileFormatPrivate->lastObject = submittedObject;
	
	// Call the method
	qd3dStatus = traverse(submittedObject, (void*)objectData, theView);
	
	// If this is a shape, submit any custom elements attached to it.
	// This saves each shape traversal method from worrying about it.
	if ( (qd3dStatus == kQ3Success) && (submittedObject != NULL) &&
		Q3Object_IsType( submittedObject, kQ3SharedTypeShape ) )
		{
		qd3dStatus = E3Shape_SubmitElements( submittedObject, theView );
		}
	
	fileFormatPrivate->baseData.groupDeepCounter--;
	
	if((fileFormatPrivate->baseData.groupDeepCounter == 0) && (qd3dStatus == kQ3Success)){ // we're again in the root object
		if(fileFormatPrivate->stackCount != 0){
			qd3dStatus = e3ffw_3DMF_write_objects (fileFormatPrivate,theFile);
			// clean the stack
			fileFormatPrivate->stackCount = 0;
			Q3Memory_Free(&fileFormatPrivate->stack);
			}
		}
exit:

	*wroteReference = (fileFormatPrivate->lastObjectType == kQ3ShapeTypeReference)? kQ3True : kQ3False;
	
	fileFormatPrivate->lastObjectType = old_lastObjectType;
	fileFormatPrivate->lastObject = old_lastObject;
	fileFormatPrivate->lastTocIndex = old_lastTocIndex;

	if(submittedObject != NULL && Q3Object_IsType(submittedObject, kQ3ObjectTypeShared))
		{
		// remove the reference made by e3ffw_3DMF_filter_in_toc
		Q3Object_Dispose(submittedObject);
		}
	return (qd3dStatus);
}



//=============================================================================
//      E3XView_SubmitWriteData : collects data from traverse methods to pass it to write methods.
//-----------------------------------------------------------------------------
//		Note : Used only by the 3DMF writer.
//-----------------------------------------------------------------------------
TQ3Status
E3XView_SubmitWriteData(TQ3ViewObject view, TQ3Size size, void *data, TQ3XDataDeleteMethod deleteData)
{
	TQ3FileFormatObject		theFormat;
	E3ClassInfoPtr 			theClass = NULL;
	TE3FFormatW3DMF_Data	*instanceData;
	TQ33DMFWStackItem		*newItem;
	TQ3Status				qd3dStatus;
	TQ3XObjectWriteMethod	writeMethod = NULL;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(view);

	theFormat = E3View_AccessFileFormat(view);

	Q3_REQUIRE_OR_RESULT(E3View_GetViewMode(view) == kQ3ViewModeWriting, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theFormat), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theFormat, kQ3FileFormatTypeWriter), kQ3Failure);
	
	instanceData = (TE3FFormatW3DMF_Data *) theFormat->FindLeafInstanceData () ;

	if(size != 0){
		// retrieve the write method
		theClass = E3ClassTree::GetClass ( instanceData->lastObjectType ) ;
		Q3_ASSERT_VALID_PTR(theClass);
	
		writeMethod = (TQ3XObjectWriteMethod) theClass->GetMethod ( kQ3XMethodTypeObjectWrite ) ;
		if (writeMethod == NULL)
			return(kQ3Failure);
		}

	// Grow the view stack to the hold the new item
	qd3dStatus = Q3Memory_Reallocate(&instanceData->stack,
									  static_cast<TQ3Uns32>(sizeof(TQ33DMFWStackItem) * (instanceData->stackCount+1)));
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);
	
	newItem = &instanceData->stack[instanceData->stackCount];
	
	// initialize the stack item
	
	Q3_ASSERT(instanceData->baseData.groupDeepCounter>0);
	
	newItem->level = instanceData->baseData.groupDeepCounter-1;
	newItem->objectType = instanceData->lastObjectType;
	// retain objects that could be created and disposed on the fly in a parents' traverse method
	if (instanceData->lastObject != NULL && Q3Object_IsType(instanceData->lastObject, kQ3ObjectTypeShared))
		E3Shared_Acquire (&newItem->theObject, instanceData->lastObject);
	else
		newItem->theObject = instanceData->lastObject;
		
	newItem->writeMethod = writeMethod;
	newItem->size = size;
	newItem->tocIndex = instanceData->lastTocIndex;
	newItem->data = data;
	newItem->deleteData = deleteData;
	
	// Increment the stack count
	instanceData->stackCount++;		

	return(kQ3Success);
}





//=============================================================================
//      E3XView_SubmitSubObjectData : short circuit to   - create an object -> 
//				submit -> traverse -> E3XView_SubmitWriteData.
//-----------------------------------------------------------------------------
//		Note : Used only by the 3DMF writer.
//-----------------------------------------------------------------------------
TQ3Status
E3XView_SubmitSubObjectData(TQ3ViewObject view, TQ3XObjectClass objectClass, TQ3Uns32 size, void *data, TQ3XDataDeleteMethod deleteData)
{	TQ3Status				qd3dStatus = kQ3Success;
	TQ3ObjectType			old_lastObjectType;
	TQ3FileFormatObject		theFormat;
	TE3FFormatW3DMF_Data	*instanceData;
	TQ3ObjectType			objectType;
	TQ3Uns32		old_lastTocIndex;

	TQ3FileObject theFile = E3View_AccessFile (view);
	
	theFormat = E3View_AccessFileFormat(view);
	instanceData = (TE3FFormatW3DMF_Data *) theFormat->FindLeafInstanceData ();
	
	qd3dStatus = Q3XObjectClass_GetType (objectClass, &objectType);

	if(qd3dStatus != kQ3Success)
		return qd3dStatus;
			
			
	// mark our level
	instanceData->baseData.groupDeepCounter++;
	old_lastObjectType = instanceData->lastObjectType;
	old_lastTocIndex = instanceData->lastTocIndex;
	instanceData->lastObjectType = objectType;
	instanceData->lastTocIndex = kQ3ArrayIndexNULL;
	
	// push data on the stack
	qd3dStatus = E3XView_SubmitWriteData (view, size, data, deleteData);
	
	instanceData->baseData.groupDeepCounter--;
	instanceData->lastObjectType = old_lastObjectType;
	instanceData->lastTocIndex = old_lastTocIndex;
	
	if((instanceData->baseData.groupDeepCounter == 0) && (qd3dStatus == kQ3Success)){
		if(instanceData->stackCount != 0){
			qd3dStatus = e3ffw_3DMF_write_objects (instanceData,theFile);
			// clean the stack
			instanceData->stackCount = 0;
			Q3Memory_Free(&instanceData->stack);
			}
		}
	return (qd3dStatus);
	
	
}



//=============================================================================
//      E3FFW_3DMF_Void_Traverse: The traverse method for zero data classes.
//-----------------------------------------------------------------------------

TQ3Status
E3FFW_3DMF_Void_Traverse(TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	#pragma unused(data)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 0, NULL, NULL);
	
	return qd3dstatus;
}



//=============================================================================
//      E3FFW_3DMF_32_Traverse: The Generic traverse method for 32 bits Data.
//-----------------------------------------------------------------------------

TQ3Status
E3FFW_3DMF_32_Traverse(TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 4, data, NULL);
	
	return qd3dstatus;
}



//=============================================================================
//      E3FFW_3DMF_32_Write: The Generic write method for 32 bits Data.
//-----------------------------------------------------------------------------

TQ3Status
E3FFW_3DMF_32_Write(const void *object,
				TQ3FileObject theFile)
{
	
	return Q3Uns32_Write(*((TQ3Uns32*)object),theFile);
	
}



//=============================================================================
//      E3FFW_3DMF_Default_Delete: The Generic delete method.
//-----------------------------------------------------------------------------

void
E3FFW_3DMF_Default_Delete(void *data)
{	void 	**dataPtr = &data;

	 Q3Memory_Free(dataPtr);
	
}



//=============================================================================
//      E3FFW_3DMF_Traverse: The traverse method for the 3DMF root class.
//-----------------------------------------------------------------------------

TQ3Status
E3FFW_3DMF_Traverse(TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 16, data, NULL);
	
	return qd3dstatus;
}



//=============================================================================
//      E3FFW_3DMF_Write: The write method for the 3DMF root class.
//-----------------------------------------------------------------------------

TQ3Status
E3FFW_3DMF_Write(const void *object,
				TQ3FileObject theFile)
{
	TE3FFormatW3DMF_Data		*fileFormatPrivate = (TE3FFormatW3DMF_Data*)object;

	TQ3Uns16 majorVersion = 1;
	TQ3Uns16 minorVersion = 6;
	TQ3FileMode fileMode = kQ3FileModeNormal;
	TQ3Uns64 toc = {0,0};
	TQ3Status writeStatus;
	
	switch(fileFormatPrivate->fileMode)
		{
		case kQ3FFormatWriterType3DMFNormalBin:
			fileMode = kQ3FileModeNormal;
			break;
		case kQ3FFormatWriterType3DMFStreamBin:
			fileMode = kQ3FileModeStream;
			break;
		case kQ3FFormatWriterType3DMFDatabaseBin:
			fileMode = kQ3FileModeDatabase;
			break;
		case kQ3FFormatWriterType3DMFDatabaseStreamBin:
			fileMode = kQ3FileModeStream + kQ3FileModeDatabase;
			break;
		}

	
	writeStatus = Q3Uns16_Write(majorVersion,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns16_Write(minorVersion,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(fileMode,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns64_Write(toc,theFile);
	
	
	return(writeStatus);
}




//=============================================================================
//      E3FFW_3DMF_TOC_Traverse: The traverse method for the 3DMF TOC class.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_TOC_Traverse(TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	TE3FFormatW3DMF_Data		*fileFormatPrivate = (TE3FFormatW3DMF_Data*)data;
	TE3FFormat3DMF_TOC		*toc = fileFormatPrivate->toc;
	TQ3Uns32				tocSize = 0;
	TQ3Uns32				i;
	TQ3Uns32				tocEntrySize = 16;
	
	//compute size
	for(i = 0; i < toc->nEntries; i++)
		{
		if(toc->tocEntries[i].refID != 0)
			{
			tocSize++;
			}
		}
		
	if(tocSize == 0) // no TOC
		return (kQ3Success);
	
	toc->nUsedEntries = tocSize;
	tocSize *= tocEntrySize;
	tocSize += 28;
	
	return Q3XView_SubmitWriteData (view, tocSize, data, NULL);
	
}



//=============================================================================
//      E3FFW_3DMF_TOC_Write: The write method for the 3DMF TOC class.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_TOC_Write(const void *object,
				TQ3FileObject theFile)
{
	TE3FFormatW3DMF_Data	*fileFormatPrivate = (TE3FFormatW3DMF_Data*)object;
	TE3FFormat3DMF_TOC		*toc = fileFormatPrivate->toc;

	TQ3Uns64 		nextToc = {0,0};
	TQ3Status 		writeStatus;
	TQ3Uns32		i;
	

	
	writeStatus = Q3Uns64_Write(nextToc,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(toc->refSeed,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(toc->typeSeed,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(1,theFile); //tocEntryType QD3D 1.5 3DMF
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(16,theFile); //tocEntrySize QD3D 1.5 3DMF
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(toc->nUsedEntries,theFile);
	
	for(i = 0; i < toc->nEntries && (writeStatus == kQ3Success); i++)
		{
		if(toc->tocEntries[i].refID != 0)
			{
			if(writeStatus == kQ3Success)
				writeStatus = Q3Uns32_Write(toc->tocEntries[i].refID,theFile);
			if(writeStatus == kQ3Success)
				writeStatus = Q3Uns64_Write(toc->tocEntries[i].objLocation,theFile);
			if(writeStatus == kQ3Success)
				writeStatus = Q3Uns32_Write(toc->tocEntries[i].objType,theFile);
			}
					
		}
		
	
	return(writeStatus);
}





//=============================================================================
//      E3FFW_3DMF_DisplayGroup_Traverse: The traverse method for the DisplayGroup class.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_DisplayGroup_Traverse(TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(data)
	TQ3DisplayGroupState			state;
	TQ3DisplayGroupState			*writeState;
	TQ3XObjectClass					displayGroupStateClass;
	TQ3DisplayGroupState			defaultState = kQ3DisplayGroupStateMaskIsDrawn |
				kQ3DisplayGroupStateMaskIsPicked |
				kQ3DisplayGroupStateMaskUseBoundingBox |
				kQ3DisplayGroupStateMaskUseBoundingSphere |
				kQ3DisplayGroupStateMaskIsWritten;
	
	
	TQ3Status qd3dStatus = Q3XView_SubmitWriteData (view, 0, NULL, NULL);
	
	if(qd3dStatus != kQ3Success)
		return qd3dStatus;
			
	if(object != NULL){
		qd3dStatus = Q3DisplayGroup_GetState(object, &state);
		
		if(qd3dStatus != kQ3Success)
			return qd3dStatus;
		
		if(state != defaultState){
			writeState = (TQ3DisplayGroupState*)Q3Memory_Allocate(sizeof(TQ3DisplayGroupState));
			
			if(writeState){
					
				*writeState = 0;
				
				// Why the memory layout of the group state flags is different from those in disk  ????
				
				
				if((state & kQ3DisplayGroupStateMaskIsInline) == kQ3DisplayGroupStateMaskIsInline)
					*writeState |= 0x01; // inline
				
				if((state & kQ3DisplayGroupStateMaskIsDrawn) != kQ3DisplayGroupStateMaskIsDrawn)
					*writeState |= 0x02; // dont draw
				
				if((state & kQ3DisplayGroupStateMaskUseBoundingBox) != kQ3DisplayGroupStateMaskUseBoundingBox)
					*writeState |= 0x04; // no Bounding Box
				
				if((state & kQ3DisplayGroupStateMaskUseBoundingSphere) != kQ3DisplayGroupStateMaskUseBoundingSphere)
					*writeState |= 0x08; // no Bounding Sphere
				
				if((state & kQ3DisplayGroupStateMaskIsPicked) != kQ3DisplayGroupStateMaskIsPicked)
					*writeState |= 0x10; // dont pick
			
				if((state & kQ3DisplayGroupStateMaskIsNotForBounding) == kQ3DisplayGroupStateMaskIsNotForBounding)
					*writeState |= 0x20; // dont bound
					
				displayGroupStateClass = Q3XObjectHierarchy_FindClassByType (kQ3ObjectTypeDisplayGroupState);
				
				if(displayGroupStateClass != NULL)
					qd3dStatus = Q3XView_SubmitSubObjectData (view, displayGroupStateClass, 4, writeState, E3FFW_3DMF_Default_Delete);
				}
			else{
				return kQ3Failure;
				}
			}
		
		TQ3BoundingBox	bBox;
		if (kQ3Success == Q3DisplayGroup_GetBoundingBox( object, &bBox ))
			{
			TQ3BoundingBox*	writeBoxData = (TQ3BoundingBox*) Q3Memory_Allocate( sizeof(TQ3BoundingBox) );
			
			if (writeBoxData)
				{
				*writeBoxData = bBox;

				TQ3XObjectClass displayGroupBoxClass = Q3XObjectHierarchy_FindClassByType( kQ3ObjectTypeDisplayGroupBBox );
				
				// Note that we do not write the bounding box isEmpty flag, hence we do not use
				// sizeof( TQ3BoundingBox ) as the size.
				if (displayGroupBoxClass != NULL)
					qd3dStatus = Q3XView_SubmitSubObjectData( view, displayGroupBoxClass,
						2 * sizeof(TQ3Point3D), writeBoxData, E3FFW_3DMF_Default_Delete );
				}
			}
		}
		
	return qd3dStatus;
}



//=============================================================================
//      e3ffw_3DMF_CString_DeleteData: Release string data after CString write.
//-----------------------------------------------------------------------------
static void e3ffw_3DMF_CString_DeleteData( void* data )
{
	char*	theString = (char*) data;
	Q3CString_EmptyData( &theString );
}



//=============================================================================
//      E3FFW_3DMF_CString_Traverse: The traverse method for the CString class.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_CString_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view)
{
	#pragma unused(data)
	char*	theString = NULL;
	TQ3Status	theStatus;
	TQ3Size	size;
	
	theStatus = Q3CString_GetString( object, &theString );
	if (theStatus == kQ3Success)
		{
		size = Q3Size_Pad(static_cast<TQ3Uns32>(strlen(theString) + 1));
		theStatus = Q3XView_SubmitWriteData( view, size, theString,
			e3ffw_3DMF_CString_DeleteData );
		}
	
	return theStatus;
}



//=============================================================================
//      E3FFW_3DMF_CString_Write: The write method for the CString class.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_CString_Write( const void *theString,  TQ3FileObject theFile )
{
#if QUESA_ALLOW_QD3D_EXTENSIONS
	return Q3String_WriteUnlimited( (const char*) theString, theFile );
#else
	return Q3String_Write( theString, theFile );
#endif
}
