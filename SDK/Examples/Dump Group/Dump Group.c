/*  NAME:
        Dump Group.c
        
    DESCRIPTION:
        Dumps the structure of a 3DMF file.
        
        Based on original QD3D example app by Nick Thompson and Robert
        Dierkes, modified by Dan Zinngrabe and Dair Grant for Quesa.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Qut.h"

#if QUESA_OS_MACINTOSH && (!TARGET_RT_MAC_MACHO)
	#include <console.h>
#endif





//=============================================================================
//      MyNewFileObject : Create a file object from a path.
//-----------------------------------------------------------------------------
//		Note :	Uses the Quesa path storage objects, which are an extension to
//				QD3D (QD3D has Unix Path storage objects, but they're only
//				available under Unix).
//-----------------------------------------------------------------------------
static TQ3FileObject
MyNewFileObject( char *pPathToMetafile )
{	TQ3StorageObject	myStorageObj;
	TQ3FileObject		myFileObj;



	/* Create new storage object and new file object */
	if (((myStorageObj = Q3PathStorage_New( pPathToMetafile )) == NULL) 
		|| ((myFileObj = Q3File_New()) == NULL)) 
	{
		if (myStorageObj != NULL) 
			Q3Object_Dispose(myStorageObj);
		return(NULL);
	}


	/* Set the storage for the file object */
	Q3File_SetStorage(myFileObj, myStorageObj);
	Q3Object_Dispose(myStorageObj);

	return (myFileObj);
}





//=============================================================================
//      MyReadObjectsFromFileIntoGroup : Read a file into a group.
//-----------------------------------------------------------------------------
static TQ3Status
MyReadObjectsFromFileIntoGroup( TQ3FileObject theFile,TQ3GroupObject myGroup)
{


	if(theFile != NULL) {
		TQ3Object			myTempObj ;
		TQ3Boolean			isEOF ;


		/* read all objects from the file into the group */
		do {
			myTempObj = Q3File_ReadObject( theFile );
			
			if( myTempObj != NULL ) {
			
				if( !Q3Object_IsType( myTempObj, kQ3SharedTypeViewHints ) )
					Q3Group_AddObject( myGroup, myTempObj ) ;

				Q3Object_Dispose( myTempObj ) ;
			}


			/* check to see if we reached the end of file yet */
			isEOF = Q3File_IsEndOfFile( theFile );
		} while (isEOF == kQ3False);	
	}


	if( myGroup != NULL )
		return kQ3Success ;
	else
		return kQ3Failure ;
}





//=============================================================================
//      MyNewModelFromFile : Read a model from a file.
//-----------------------------------------------------------------------------
static TQ3GroupObject
MyNewModelFromFile(char *pPathToMetafile)
{	TQ3GroupObject		myGroup = NULL ;
	TQ3FileMode			myFileMode = 0;
	TQ3FileObject		theFile;



	/*	Create a group for the complete model. */
	if ((myGroup = Q3Group_New()) == NULL )
		return NULL;



	/* Create a file object */
	theFile = MyNewFileObject( pPathToMetafile ) ;



	/* Read the objects from the file */	
	if( Q3File_OpenRead( theFile, &myFileMode ) != kQ3Success)
		return  NULL ;

	if( MyReadObjectsFromFileIntoGroup( theFile, myGroup ) == 0)
		printf("Metafile data read is null") ;



	/* Clean up */
	Q3File_Close(theFile);
	Q3Object_Dispose(theFile);
	
	return myGroup ;
}





//=============================================================================
//      PrintAttributeSetType : Convert an attribute type to a string.
//-----------------------------------------------------------------------------
static TQ3Status
PrintAttributeSetType(TQ3Object object)
{
	TQ3AttributeType	attrType;
	TQ3AttributeType	attributeTypes[] = {
							kQ3AttributeTypeNone,
							kQ3AttributeTypeSurfaceUV,
							kQ3AttributeTypeShadingUV,
							kQ3AttributeTypeNormal,
							kQ3AttributeTypeAmbientCoefficient,
							kQ3AttributeTypeDiffuseColor,
							kQ3AttributeTypeSpecularColor,
							kQ3AttributeTypeSpecularControl,
							kQ3AttributeTypeTransparencyColor,
							kQ3AttributeTypeSurfaceTangent,
							kQ3AttributeTypeHighlightState,
							kQ3AttributeTypeSurfaceShader
						};
	char				*attributeTypeNames[] = {
							"None",
							"SurfaceUV",
							"ShadingUV",
							"Normal",
							"AmbientCoefficient",
							"DiffuseColor",
							"SpecularColor",
							"SpecularControl",
							"TransparencyColor",
							"SurfaceTangent",
							"HighlightState",
							"SurfaceShader"
						};
	TQ3Int32			i, total, count;
	TQ3Status			status;

	if (Q3Object_IsType(object, kQ3SetTypeAttribute) == kQ3False) {
		return kQ3Failure;
	}

	count = 0;
	total = sizeof(attributeTypes) / sizeof(TQ3AttributeType);
	attrType = kQ3AttributeTypeNone;

	while (1) {
		status = Q3AttributeSet_GetNextAttributeType(object, &attrType);
		if (status == kQ3Failure) {
			break;
		}

		if (attrType == kQ3AttributeTypeNone) {
			break;
		}

		printf((count == 0) ? "  [" : ", ");

		for (i = 0; i < total; i++) {
			if (attrType == attributeTypes[i]) {
				break;
			}
		}

		if (i < total) {
			printf("%s", attributeTypeNames[i]);
		}
		else {
			printf("Custom(%ld)", (TQ3Uns32) attrType);
		}

		count++;
	}

	if (count) {
		printf("]");
	}
	else {
		printf("  [None]");
	}

	return status;
}





//=============================================================================
//      PrintGroupElementsAndRecurse : Dump a group and recurse.
//-----------------------------------------------------------------------------
static void
PrintGroupElementsAndRecurse( TQ3GroupObject theGroup, TQ3Uns32 depth )
{
 
	TQ3GroupPosition			thePosition ;
	TQ3Status					theStatus ;

	TQ3Object					thisObject ;
	TQ3ObjectType				thisObjectType ;
	TQ3ObjectClassNameString 	objectClassString ;
	
	TQ3Uns32					i ;
	TQ3Uns32					numberOfObjects ;



	/* sanity checks, and totals... */
	if( depth < 1 ) 
		depth = 1 ;	/* we must always be called with a depth of at least 1 */
	
	if ( Q3Object_IsType( theGroup, kQ3ShapeTypeGroup) )
	{
		/* print the group name here */
		thisObjectType = Q3Object_GetLeafType ( theGroup ) ;
		
		Q3ObjectHierarchy_GetStringFromType( thisObjectType,  objectClassString) ;
		Q3Group_CountObjects( theGroup, &numberOfObjects ) ;
		
		/* really this group is at the level before this, so subtract 1 from the total */
		printf("%4ld  ", (TQ3Uns32) (depth - 1)) ;
		for( i = 0; i < (depth - 1); i++ )
			printf("  ") ;
			
		printf( "%s (contains %ld objects)\n",	objectClassString,  numberOfObjects) ;	
	}
	else
	{
		printf( "PrintGroupElementsAndRecurse called in error, theGroup ain't a group") ;
		return ;
	}



	/* get the first object from the group and iterate through */
	theStatus = Q3Group_GetFirstPosition ( theGroup, &thePosition ) ;

	while( thePosition != NULL && theStatus == kQ3Success )
	{
	
		theStatus = Q3Group_GetPositionObject ( theGroup, thePosition, &thisObject ) ;
		
		if(theStatus == kQ3Failure )
		{
			printf("GetPositionObject failed, Aborting") ;
			return ;
		}
		
		
		if ( Q3Object_IsType( thisObject, kQ3ShapeTypeGroup) )
		{
			PrintGroupElementsAndRecurse( thisObject, depth + 1 ) ;
		}
		else
		{
			thisObjectType = Q3Object_GetLeafType ( thisObject ) ;
			
			Q3ObjectHierarchy_GetStringFromType( thisObjectType,  objectClassString) ;
			
			printf("%4ld  ", depth) ;
			for( i = 0; i < depth; i++ )
				printf("  ") ;
				
			printf( "%s",	objectClassString ) ;	

			if (thisObjectType == kQ3SetTypeAttribute) {
				PrintAttributeSetType(thisObject);
			}
				
			printf( "\n" ) ;	
		}
			
		Q3Object_Dispose( thisObject ) ;
		
		/* finally get the next object in the group */
		theStatus = Q3Group_GetNextPosition(theGroup, &thePosition) ;
	}
}





//=============================================================================
//      main : Program entry point.
//-----------------------------------------------------------------------------
int main(int argc, char * argv[])
{	char				*pathToFile;
	TQ3Status 			qd3dStatus;
	TQ3GroupObject		theGroup;



	/* Special Mac hook */
#if QUESA_OS_MACINTOSH && (!TARGET_RT_MAC_MACHO)
	argc = ccommand(&argv);
#endif



	/* Handle parameters */
	if (argc == 1)
		{
#if QUESA_OS_MACINTOSH && (!TARGET_RT_MAC_MACHO)
		pathToFile = "::Support Files:Models:3DMF:F18.3dmf";
#elif QUESA_OS_WIN32
		pathToFile = "..\\Support Files\\Models\\3DMF\\F18.3dmf";
#else
		pathToFile = "../Support Files/Models/3DMF/F18.3dmf";
#endif

		printf("dumpgroup <file>\n");
		printf("   - dumps the structure of a 3DMF file\n");
		printf("   - no arguments given, using '%s'\n", pathToFile);
		printf("\n");
		}
	else
		pathToFile = argv[1];



	/* Initialise QD3D */
	qd3dStatus = Q3Initialize();
	if (qd3dStatus != kQ3Success)
		{
		printf("Unable to initialise QD3D\n");
		return(-1);
		}



	/* Read the file in and dump it out */
	theGroup = MyNewModelFromFile(pathToFile);
	if (theGroup == NULL)
		printf("Unable to read group from '%s'\n", pathToFile);
	else
		{
		PrintGroupElementsAndRecurse(theGroup, 0);
		Q3Object_Dispose(theGroup);
		}



	/* Clean up */
	qd3dStatus = Q3Exit();

	return(0);
}
