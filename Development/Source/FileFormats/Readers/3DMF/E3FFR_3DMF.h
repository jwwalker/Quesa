/*  NAME:
        E3FFR_3DMF.h

    DESCRIPTION:
        Header file for E3FFR_3DMF.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#ifndef E3FFR_3DMF_HDR
#define E3FFR_3DMF_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3IOFileFormat.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
typedef struct {
	TQ3Uns32						refID;
	TQ3Uns64						objLocation;
	TQ3ObjectType					objType;
	TQ3Object						object;
} TE3FFormat3DMF_TOCEntry;

typedef struct {
	TQ3Uns32						refSeed;
	TQ3Int32						typeSeed;
	TQ3Uns32						nEntries;
	TE3FFormat3DMF_TOCEntry			tocEntries[1];// better a BTree?
} TE3FFormat3DMF_TOC;

typedef struct {
	TQ3FFormatBaseData				baseData;
	TE3FFormat3DMF_TOC				*toc;
	TQ3FileMode						fileMode;
	TQ3Boolean						noMoreObjectData;
	TQ3Boolean						inContainer;
	TQ3TriMeshData*					currentTriMesh;
} TE3FFormat3DMF_Data;

// Stack data
typedef struct {
	TQ3Uns32						level;
	TQ3Object						theObject;
	TQ3ObjectType					objectType;
	TQ3Size							size;
	TQ3XObjectWriteMethod			writeMethod;
	void							*data;
	TQ3XDataDeleteMethod 			deleteData;
} TQ33DMFWStackItem;

typedef struct {
	TQ3FFormatBaseData				baseData;
	TE3FFormat3DMF_TOC				*toc;
	TQ3FileMode								fileMode;
	TQ3ObjectType							lastObjectType;
	TQ3Object									lastObject;
	// objects stack
	TQ3Uns32									stackCount;
	TQ33DMFWStackItem					*stack;

} TE3FFormatW3DMF_Data;


typedef struct {
	TQ3Uns32			attributeSetCounter;
	TQ3AttributeSet		*attributeSetArray;
} TE3FFormat3DMF_AttributeSetList_Data;




//=============================================================================
// protected virtual methods (sorta of...)
//-----------------------------------------------------------------------------
enum{
	kE3XMethodType_3DMF_ReadNextElement = Q3_FOUR_CHARACTER_CONSTANT('3', 'F', 'r', 'e'),
	kE3XMethodType_3DMF_ReadFlag = Q3_FOUR_CHARACTER_CONSTANT('3', 'F', 'r', 'f')
	};
typedef CALLBACK_API_C(void, TQ3XFFormat_3DMF_ReadNextElementMethod)(TQ3AttributeSet parent,TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3Status, TQ3XFFormat_3DMF_ReadFlagMethod)(TQ3Uns32* flag,TQ3FileObject file, TQ3ObjectType hint);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status					E3FFormat_3DMF_Reader_RegisterClass(void);
TQ3Status					E3FFormat_3DMF_Reader_UnregisterClass(void);
TQ3Status					E3FFW_3DMF_Register(void);
TQ3Status					E3FFW_3DMF_Unregister(void);

TQ3Object           E3FFormat_3DMF_FaceAttributeSetList_New(TQ3Size size);
TQ3Object           E3FFormat_3DMF_VertexAttributeSetList_New(TQ3Size size);
TQ3Object           E3FFormat_3DMF_GeomAttributeSetList_New(TQ3Size size);
TQ3AttributeSet			E3FFormat_3DMF_AttributeSetList_Get(TQ3Object theAttributeSetList, TQ3Uns32 index);
TQ3Status						E3FFormat_3DMF_AttributeSetList_Set(TQ3Object theAttributeSetList ,TQ3Uns32 index,
										TQ3AttributeSet theAttributeSet);

TQ3GeneralPolygonShapeHint	E3FFormat_3DMF_GeneralPolygonHint_Get(TQ3Object theObject);
TQ3DisplayGroupState		E3FFormat_3DMF_DisplayGroupState_Get(TQ3Object theObject);

void 						E3FFormat_3DMF_ReadNextElement(TQ3AttributeSet parent,TQ3FileObject theFile);
TQ3Status               	E3FFormat_3DMF_ReadFlag(TQ3Uns32* flag,TQ3FileObject theFile, TQ3ObjectType hint);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

