/*  NAME:
        E3FFR_3DMF.h

    DESCRIPTION:
        Header file for E3FFR_3DMF.c.

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
typedef struct TE3FFormat3DMF_TOCEntry {
	TQ3Uns32						refID;
	TQ3Uns64						objLocation;
	TQ3ObjectType					objType;
	TQ3Object						object;
} TE3FFormat3DMF_TOCEntry;

typedef struct TE3FFormat3DMF_TOC {
	TQ3Uns32						refSeed;
	TQ3Int32						typeSeed;
	TQ3Uns32						nEntries;
	TQ3Uns32						nUsedEntries;
	TE3FFormat3DMF_TOCEntry			tocEntries[1];// better a BTree?
} TE3FFormat3DMF_TOC;

typedef struct TE3FFormat3DMF_Data {
	TQ3FFormatBaseData				baseData;
	TE3FFormat3DMF_TOC				*toc;
	TQ3FileMode						fileMode;
	TQ3Boolean						noMoreObjectData;
	TQ3Boolean						inContainer;
	TQ3TriMeshData*					currentTriMesh;
} TE3FFormat3DMF_Data;

// Stack data
typedef struct TQ33DMFWStackItem {
	TQ3Uns32						level;
	TQ3Object						theObject;
	TQ3ObjectType					objectType;
	TQ3Size							size;
	TQ3Uns32						tocIndex;
	TQ3XObjectWriteMethod			writeMethod;
	void							*data;
	TQ3XDataDeleteMethod 			deleteData;
} TQ33DMFWStackItem;

typedef struct TE3FFormatW3DMF_Data {
	TQ3FFormatBaseData				baseData;
	TE3FFormat3DMF_TOC				*toc;
	TQ3FileMode						fileMode;
	TQ3ObjectType					lastObjectType;
	TQ3Object						lastObject;
	TQ3Uns32						lastTocIndex;
	// objects stack
	TQ3Uns32						stackCount;
	TQ33DMFWStackItem				*stack;
} TE3FFormatW3DMF_Data;


typedef struct TE3FFormat3DMF_AttributeSetList_Data {
	TQ3Uns32						attributeSetCounter;
	TQ3AttributeSet					*attributeSetArray;
} TE3FFormat3DMF_AttributeSetList_Data;


typedef struct TE3FFormat3DMF_AttributeArray_Data {
	TQ3TriMeshAttributeData*		attributeData;
	TQ3Uns32						whichArray;
	TQ3Uns32						whichAttr;
	TQ3Uns32						arraySize;
	TQ3Uns32						attributeSize;
} TE3FFormat3DMF_AttributeArray_Data;


typedef struct TE3FFormat3DMF_MeshCorner_Data {
	TQ3Uns32						vertexIndex;
	TQ3Uns32						nFaces;
	TQ3Uns32*						faces;
	TQ3AttributeSet					attributeSet;
} TE3FFormat3DMF_MeshCorner_Data;


typedef struct TE3FFormat3DMF_MeshCorners_Data {
	TQ3Uns32						nCorners;
	TE3FFormat3DMF_MeshCorner_Data*	corners;
} TE3FFormat3DMF_MeshCorners_Data;


typedef struct TE3FFormat3DMF_MeshEdge_Data {
	TQ3Uns32						vertexIndex1;
	TQ3Uns32						vertexIndex2;
	TQ3AttributeSet					attributeSet;
} TE3FFormat3DMF_MeshEdge_Data;


typedef struct TE3FFormat3DMF_MeshEdges_Data {
	TQ3Uns32						nEdges;
	TE3FFormat3DMF_MeshEdge_Data*	edges;
} TE3FFormat3DMF_MeshEdges_Data;



//=============================================================================
// protected virtual methods (sorta of...)
//-----------------------------------------------------------------------------
enum{
	kE3XMethodType_3DMF_ReadNextElement = Q3_FOUR_CHARACTER_CONSTANT('3', 'F', 'r', 'e'),
	kE3XMethodType_3DMF_ReadFlag = Q3_FOUR_CHARACTER_CONSTANT('3', 'F', 'r', 'f')
	};
typedef Q3_CALLBACK_API_C(void, TQ3XFFormat_3DMF_ReadNextElementMethod)(TQ3AttributeSet parent,TQ3FileObject theFile);
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormat_3DMF_ReadFlagMethod)(TQ3Uns32* flag,TQ3FileObject file, TQ3ObjectType hint);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status					E3FFormat_3DMF_Reader_RegisterClass(void);
TQ3Status					E3FFormat_3DMF_Reader_UnregisterClass(void);
TQ3Status					E3FFW_3DMF_Register(void);
TQ3Status					E3FFW_3DMF_Unregister(void);

TQ3Object           		E3FFormat_3DMF_FaceAttributeSetList_New(TQ3Size size);
TQ3Object           		E3FFormat_3DMF_VertexAttributeSetList_New(TQ3Size size);
TQ3Object           		E3FFormat_3DMF_GeomAttributeSetList_New(TQ3Size size);
TQ3AttributeSet				E3FFormat_3DMF_AttributeSetList_Get(TQ3Object theAttributeSetList, TQ3Uns32 index);
TQ3Status					E3FFormat_3DMF_AttributeSetList_Set(TQ3Object theAttributeSetList ,TQ3Uns32 index,
							TQ3AttributeSet theAttributeSet);

TQ3Object					E3FFormat_3DMF_MeshCorners_New(TQ3MeshData* meshData);
void						E3FFormat_3DMF_MeshCorners_Assign(TQ3Object theMeshCorners, TQ3GeometryObject theMesh,
																TQ3Uns32 nFaces, TQ3MeshFace* faces,
																TQ3Uns32 nVertices, TQ3MeshVertex* vertices);
void						E3FFormat_3DMF_MeshEdges_Assign(TQ3Object theMeshEdges, TQ3GeometryObject theMesh,
																TQ3Uns32 nVertices, TQ3MeshVertex* vertices);

TQ3GeneralPolygonShapeHint	E3FFormat_3DMF_GeneralPolygonHint_Get(TQ3Object theObject);
TQ3EndCap					E3FFormat_3DMF_GeometryCapsMask_Get(TQ3Object theObject);
TQ3DisplayGroupState		E3FFormat_3DMF_DisplayGroupState_Get(TQ3Object theObject);
TQ3AttributeSet				E3FFormat_3DMF_CapsAttributes_Get(TQ3Object theObject);

TQ3Status               	E3FFormat_3DMF_ReadFlag(TQ3Uns32* flag,TQ3FileObject theFile, TQ3ObjectType hint);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

