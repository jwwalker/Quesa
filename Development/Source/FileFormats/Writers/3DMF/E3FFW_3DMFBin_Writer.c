/*  NAME:
        E3FFW_3DMFBin_Writer.c

    DESCRIPTION:
        Quesa 3DMF Binary Writer.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3View.h"
#include "E3FFW_3DMFBin_Writer.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFW_3DMFBin_StartFrame : Start a frame.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_StartFile(TQ3ViewObject				theView,
						TE3FFormatW3DMF_Data		*instanceData,
						TQ3DrawContextObject	theDrawContext)
{
#pragma unused(theDrawContext)
	
	return E3FFW_3DMF_TraverseObject (theView, instanceData, NULL, kQ3ObjectType3DMF, NULL);
	
}








//=============================================================================
//      E3FFW_3DMFBin_Cancel: Cancel a pass.
//-----------------------------------------------------------------------------
void
E3FFW_3DMF_Cancel(TQ3ViewObject theView, TE3FFormatW3DMF_Data *instanceData)
{
#pragma unused(theView)
#pragma unused(instanceData)
	// To Be implemented
}


//=============================================================================
//      e3ffw_3DMF_write_objects: unroll the stack and do the real write.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_write_objects(TE3FFormatW3DMF_Data *instanceData, TQ3FileObject theFile)
{	TQ3Status		qd3dStatus = kQ3Success;
	
	TQ3Size size;
	int i,j;
	E3ClassInfoPtr theClass = NULL;
#if Q3_DEBUG
	TQ3Uns32 pos;
#endif

	for(i=0; i<instanceData->stackCount; i++){
		if(qd3dStatus == kQ3Success){
			if(i < instanceData->stackCount-1){
				if(instanceData->stack[i+1].level > instanceData->stack[i].level){
				
					qd3dStatus = Q3Uns32_Write(0x636E7472 /* cntr - Container */,theFile);
					if(qd3dStatus == kQ3Success){
					
						//compute container size
						size = instanceData->stack[i].level; //the container object size
						size += 8; // the container object type & size
						j=i+1;
						while(j < instanceData->stackCount)
							if(instanceData->stack[j].level > instanceData->stack[i].level)
								break;
							else{
								size += instanceData->stack[j].level; //the contained object size
								size += 8; // the contained object type & size
								j++;
								}
						//write container size
						qd3dStatus = Q3Uns32_Write(size,theFile);
						}
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
				Q3_ASSERT(pos+instanceData->stack[i].size == instanceData->baseData.currentStoragePosition);
				}
			//call deletedata if any
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
					TE3FFormatW3DMF_Data		*instanceData,
					TQ3Object		theObject,
					TQ3ObjectType		objectType,
					void			*objectData)
							
{	TQ3Status		qd3dStatus = kQ3Success;
	TQ3ObjectType	old_lastObjectType;
	E3ClassInfoPtr theClass = NULL;
	TQ3XObjectTraverseMethod traverse;
	TQ3FileObject theFile = E3View_AccessFile (theView);
	
	
	//find the object traverse method
	if(theObject != NULL)
		theClass = theObject->theClass;
	else
		theClass = E3ClassTree_GetClassByType(objectType);
	
	if (theClass == NULL)
		return(kQ3Success);

	traverse = (TQ3XObjectTraverseMethod)
					E3ClassTree_GetMethod(theClass,
										  kQ3XMethodTypeObjectTraverse);
	if (traverse == NULL)
		return(kQ3Success);

	// mark our level
	instanceData->baseData.groupDeepCounter++;
	old_lastObjectType = instanceData->lastObjectType;
	instanceData->lastObjectType = objectType;
	
	// Call the method
	qd3dStatus = traverse(theObject, objectData, theView);
	
	instanceData->baseData.groupDeepCounter--;
	instanceData->lastObjectType = old_lastObjectType;
	
	if(instanceData->baseData.groupDeepCounter == 0){ // we're again in the root object
		if(instanceData->stackCount != 0){
			qd3dStatus = e3ffw_3DMF_write_objects (instanceData,theFile);
			// clean the stack
			instanceData->stackCount = 0;
			E3Memory_Free(&instanceData->stack);
			}
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
	E3ClassInfoPtr theClass = NULL;
	TE3FFormatW3DMF_Data	*instanceData;
	TQ33DMFWStackItem		*newItem;
	TQ3Status				qd3dStatus;
	TQ3XObjectWriteMethod	writeMethod = NULL;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(view);

	theFormat = E3View_AccessFileFormat(view);

	Q3_REQUIRE_OR_RESULT(E3View_GetViewMode(view) == kQ3ViewModeWriting, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theFormat), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theFormat, kQ3FFormatWriterType3DMFStreamBin), kQ3Failure);
	
	instanceData = (TE3FFormatW3DMF_Data *) theFormat->instanceData;

	if(size != 0){
		// retrive the write method
		theClass = E3ClassTree_GetClassByType(instanceData->lastObjectType);
		Q3_ASSERT_VALID_PTR(theClass);
	
		writeMethod = (TQ3XObjectWriteMethod)
					E3ClassTree_GetMethod(theClass,
										  kQ3XMethodTypeObjectWrite);
		if (writeMethod == NULL)
			return(kQ3Failure);
		}

	// Grow the view stack to the hold the new item
	qd3dStatus = E3Memory_Reallocate(&instanceData->stack,
									  sizeof(TQ33DMFWStackItem) * (instanceData->stackCount+1));
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);
	
	newItem = &instanceData->stack[instanceData->stackCount];
	
	// initialize the stack item
	
	Q3_ASSERT(instanceData->baseData.groupDeepCounter>0);
	
	newItem->level = instanceData->baseData.groupDeepCounter-1;
	newItem->objectType = instanceData->lastObjectType;
	newItem->writeMethod = writeMethod;
	newItem->size = size;
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
{


	// To be implemented...
	return(kQ3Failure);
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
	#pragma unused(data)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 16, NULL, NULL);
	
	return qd3dstatus;
}

//=============================================================================
//      E3FFW_3DMF_Traverse: The write method for the 3DMF root class.
//-----------------------------------------------------------------------------

TQ3Status
E3FFW_3DMF_Write(const void *object,
				TQ3FileObject theFile)
{
	#pragma unused(object)


	TQ3Uns16 majorVersion = 1;
	TQ3Uns16 minorVersion = 6;
	TQ3FileMode fileMode = kQ3FileModeStream;
	TQ3Uns64 toc = {0,0};

	TQ3Status writeStatus;
	
	writeStatus = Q3Uns16_Write(majorVersion,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns16_Write(minorVersion,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(fileMode,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns64_Write(toc,theFile);
	
	
	return(writeStatus);
}

