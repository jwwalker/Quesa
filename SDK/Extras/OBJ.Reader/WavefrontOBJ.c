/*  NAME:
        WAVEFRONTOBJ.c

    DESCRIPTION:
        implementation WaveFront .OBJ Importer Plug-In for Quesa.
        
    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://quesa.designcommunity.com/>

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

//adapted with permission from

/****************************/
/*   	WAVEFRONTOBJ.C 		*/
/* (c)2000 Pangea Software  */
/* By Brian Greenstone      */
/****************************/


/****************************/
/*    EXTERNALS             */
/****************************/

#include <Quesa.h>
#include <QuesaGeometry.h>
#include <QuesaMemory.h>
#include <QuesaStorage.h>
#include <QuesaGroup.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "wavefrontobj.h"

//extern	ObjNode					*gModelObjNode;

/****************************/
/*    PROTOTYPES            */
/****************************/

static TQ3Status DecomposeOBJ(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status LoadOBJFile(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status SkipSpaces(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status NextLine(TE3FFormat_OBJ_Data* formatInstanceData);
static int GetToken(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status SkipUnwanted(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status ReadOBJVertexData(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status ReadWord(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status ReadFloat(TE3FFormat_OBJ_Data* formatInstanceData,float *value);
static TQ3Status ReadInt(TE3FFormat_OBJ_Data* formatInstanceData,long *value);
static TQ3Status ReadOBJFace(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status ReadOBJVertexNormalData(TE3FFormat_OBJ_Data* formatInstanceData);
static TQ3Status ReadOBJUVData(TE3FFormat_OBJ_Data* formatInstanceData);
static void NewGroup(TE3FFormat_OBJ_Data* formatInstanceData);
static void BuildQD3DMesh(TE3FFormat_OBJ_Data* formatInstanceData);


/****************************/
/*    CONSTANTS             */
/****************************/

enum
{
	TOKEN_END		=	0,
	TOKEN_GROUP,
	TOKEN_VERTEX,
	TOKEN_UV,
	TOKEN_USEMTL,
	TOKEN_MTLLIB,
	TOKEN_FACE,
	TOKEN_OBJECT,
	TOKEN_VERTEX_NORMAL,
	TOKEN_SHADE,
	
	TOKEN_UNKNOWN
};





/******************* IMPORT OBJ FILE ***********************/

void ImportObjFile(TE3FFormat_OBJ_Data* formatInstanceData)
{
int		i;

	formatInstanceData->gNextVertIsNewGroup = kQ3True;

	formatInstanceData->gNumGroups = 0;
	formatInstanceData->gCurrentGroup = -1;
	
	formatInstanceData->gNumPointsInCurrentGroup = 0;
	formatInstanceData->gPointIndexBase = 1;

	formatInstanceData->gNumUVsInCurrentGroup = 0;
	formatInstanceData->gUVIndexBase = 1;

	formatInstanceData->gNumNormalsInCurrentGroup = 0;
	formatInstanceData->gNormalIndexBase = 1;
	

			/* INITIALIZE STUFF */
			


			/* SELECT AND LOAD THE FILE */
			
    formatInstanceData->model = Q3DisplayGroup_New();
	
	if(formatInstanceData->model == NULL)
		return;

	LoadOBJFile(formatInstanceData);



		/* DECOMPOSE THE OBJ FILE INTO TRIANGLES */

	if (DecomposeOBJ(formatInstanceData) == kQ3Success)
		/* BUILD QD3D MESH */
			BuildQD3DMesh(formatInstanceData);
	
	
			/* CLEANUP */
			
	Q3Memory_Free(&formatInstanceData->gTheOBJData);
	

	for (i = 0; i < formatInstanceData->gNumGroups; i++){
		Q3Memory_Free(&formatInstanceData->gGroups[i]);
		}

}



/********************** LOAD OBJ FILE **************************/

static TQ3Status LoadOBJFile(TE3FFormat_OBJ_Data* formatInstanceData)
{
TQ3Status	iErr;



		
	Q3Memory_Free(&formatInstanceData->gTheOBJData);		// see if nuke old data


			/****************************/
			/*   LOAD THE OBJ FILE      */
			/****************************/

	
	iErr = Q3Storage_GetSize(formatInstanceData->baseData.storage, &formatInstanceData->gFileSize);						// get size of file
	if (iErr != kQ3Success)
	{
		goto bail;
	}
	
	formatInstanceData->gTheOBJData = Q3Memory_Allocate(formatInstanceData->gFileSize);						// alloc memory for the file's data
	if (formatInstanceData->gTheOBJData == nil)
	{
		goto bail;
	}
	iErr = Q3Storage_GetData (formatInstanceData->baseData.storage, 0, formatInstanceData->gFileSize,
						formatInstanceData->gTheOBJData, &formatInstanceData->gFileSize);
	if (iErr != kQ3Success)
	{
		goto bail;
	}

	return(kQ3Success);
	
	
bail:
	Q3Memory_Free(&formatInstanceData->gTheOBJData);
		
	return(kQ3Failure);

}


/********************** DECOMPOSE OBJ *****************************/

static TQ3Status DecomposeOBJ(TE3FFormat_OBJ_Data* formatInstanceData)
{
//FSSpec	spec;
//OSErr	iErr;
	



			/* POINT TO 1ST TOKEN */
							
	formatInstanceData->gFileIndex = 0;									// start @ front of file
	if (SkipUnwanted(formatInstanceData) != kQ3Success)
		goto badfile;
		
	if (formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == '#') // see if 1st line is a comment
	{
		if (NextLine(formatInstanceData) != kQ3Success)
		{
badfile:		
			//DoAlert("\pDecomposeOBJ: the file doesnt contain anything useful");
			return(kQ3Failure);								// nothing found
		}
	}	


		/*******************************/
		/* SCAN AND PROCESS EACH TOKEN */
		/*******************************/

	do
	{
		switch(GetToken(formatInstanceData))
		{
			case	TOKEN_END:
					goto eof;
					
			case	TOKEN_GROUP:
					break;
					
			case	TOKEN_OBJECT:
					break;
					
			case	TOKEN_VERTEX:
					if (ReadOBJVertexData(formatInstanceData) != kQ3Success)
						return(kQ3Failure);
					break;
					
			case	TOKEN_VERTEX_NORMAL:
					if (ReadOBJVertexNormalData(formatInstanceData) != kQ3Success)
						return(kQ3Failure);
					break;
					
			case	TOKEN_UV:
					if (ReadOBJUVData(formatInstanceData) != kQ3Success)
						return(kQ3Failure);
					break;
					
			case	TOKEN_USEMTL:
					if (ReadWord(formatInstanceData) != kQ3Success)				// read the name of the material to activate
						return(kQ3Failure);
/*					SelectActiveMTLMaterial(&gWord[0]);
					gGroups[gCurrentGroup]->materialNum = gCurrentActiveMTLMaterial;*/
					break;
					
			case	TOKEN_MTLLIB:
					if (ReadWord(formatInstanceData) != kQ3Success)				// read the filename of the material lib file
						return(kQ3Failure);
										
					/*iErr = FSMakeFSSpec(gTheOBJFileSpec.vRefNum,	// create FSSpec for the file
										 gTheOBJFileSpec.parID,
										 gWord, &spec);	
					if (iErr != noErr)
					{
						DoAlert("\pCannot find .MTL library needed by this .OBJ file");
						break;
					}
					
					ProcessMTLFile(&spec);		// read the materials out of the file
					*/			
					break;
			case	TOKEN_FACE:
					if (ReadOBJFace(formatInstanceData) != kQ3Success)
						return(kQ3Failure);
					break;
					
			case	TOKEN_SHADE:
					if (ReadWord(formatInstanceData) != kQ3Success)				// read on or off
						return(kQ3Failure);
					break;
				
		}
	}while (NextLine(formatInstanceData) == kQ3Success);
eof:

	
	
	return(kQ3Success);
}


/****************** READ OBJ VERTEX DATA ************************/
//
// The current token is "v", so read 3 floats and store into vertex array
//

static TQ3Status ReadOBJVertexData(TE3FFormat_OBJ_Data* formatInstanceData)
{
float	x,y,z;
OBJGroupType	*group;
int		n;


	if (formatInstanceData->gNextVertIsNewGroup)							// see if start new group
	{
		NewGroup(formatInstanceData);
		formatInstanceData->gNextVertIsNewGroup = kQ3False;
	}

	group = formatInstanceData->gGroups[formatInstanceData->gCurrentGroup];						// get ptr to group
	

		/* CHECK IF OVERFLOW */
		
	if (group->numPoints >= MAX_OBJ_VERTS)
	{
		//DoAlert("\pReadOBJVertexData: vertex array overflow");
		return(kQ3Failure);
	}


		/* TRY TO READ 3 FLOATS */
		
	if (ReadFloat(formatInstanceData,&x) != kQ3Success)
		return(kQ3Failure);
	if (ReadFloat(formatInstanceData,&y) != kQ3Success)
		return(kQ3Failure);
	if (ReadFloat(formatInstanceData,&z) != kQ3Success)
		return(kQ3Failure);


		/* STORE FLOATS */
		
	n = group->numPoints;
		
	group->points[n].x = x;
	group->points[n].y = y;
	group->points[n].z = z;

	group->numPoints++;

	formatInstanceData->gNumPointsInCurrentGroup++;
	
	return(kQ3Success);
}


/****************** READ OBJ VERTEX NORMAL DATA ************************/

static TQ3Status ReadOBJVertexNormalData(TE3FFormat_OBJ_Data* formatInstanceData)
{
float	x,y,z;
OBJGroupType	*group;
int		n;

	group =formatInstanceData->gGroups[formatInstanceData->gCurrentGroup];						// get ptr to group


		/* CHECK IF OVERFLOW */
		
	if (group->numNormals >= MAX_OBJ_VERTS)
	{
		//DoAlert("\pReadOBJVertexNormalData: array overflow");
		return(kQ3Failure);
	}


		/* TRY TO READ 3 FLOATS */
		
	if (ReadFloat(formatInstanceData,&x) != kQ3Success)
		return(kQ3Failure);
	if (ReadFloat(formatInstanceData,&y) != kQ3Success)
		return(kQ3Failure);
	if (ReadFloat(formatInstanceData,&z) != kQ3Success)
		return(kQ3Failure);


		/* STORE FLOATS */
		
	n = group->numNormals;
		
	group->normals[n].x = x;
	group->normals[n].y = y;
	group->normals[n].z = z;

	group->numNormals++;

	formatInstanceData->gNumNormalsInCurrentGroup++;

	return(kQ3Success);
}



/****************** READ OBJ UV DATA ************************/

static TQ3Status ReadOBJUVData(TE3FFormat_OBJ_Data* formatInstanceData)
{
float	u,v;
OBJGroupType	*group;
int		n;

	group =formatInstanceData-> gGroups[formatInstanceData->gCurrentGroup];						// get ptr to group


		/* CHECK IF OVERFLOW */
		
	if (group->numUVs >= MAX_OBJ_VERTS)
	{
		//DoAlert("\pReadOBJUVData: array overflow");
		return(kQ3Failure);
	}


		/* TRY TO READ 2 FLOATS */
		
	if (ReadFloat(formatInstanceData,&u) != kQ3Success)
		return(kQ3Failure);
	if (ReadFloat(formatInstanceData,&v) != kQ3Success)
		return(kQ3Failure);


		/* STORE FLOATS */
		
	n = group->numUVs;
		
	group->uvs[n].u = u;
	group->uvs[n].v = v;

	group->numUVs++;		

	formatInstanceData->gNumUVsInCurrentGroup++;

	return(kQ3Success);
}




/*********************** READ OBJ FACE ****************************/
//
// This reads in a face and tesselates to triangles, and adds to triangle list
//

static TQ3Status ReadOBJFace(TE3FFormat_OBJ_Data* formatInstanceData)
{
long		iVertex[MAX_POLY_SIZE], iUV[MAX_POLY_SIZE], iNormal[MAX_POLY_SIZE];
short		i,numTriangles,t;
OBJGroupType	*group;


	formatInstanceData->gNextVertIsNewGroup = kQ3True;							// we're reading faces, so next vertex will indicate new group

	group = formatInstanceData->gGroups[formatInstanceData->gCurrentGroup];						// get ptr to group

	
			/*********************/
			/* READ ALL VERTICES */
			/*********************/

	i = 0;
	do
	{
		if (i >= MAX_POLY_SIZE)
			return kQ3Failure;//DoFatalAlert("\pReadOBJFace: # verts on face exceeded max allowed");

			/* READ VERTEX INDEX */
			
		ReadInt(formatInstanceData,&iVertex[i]);
		if (iVertex[i] == 0)									// see if didnt read it
			return kQ3Failure;//DoFatalAlert("\pReadOBJFace: blank vertex index");
						
		if (iVertex[i] < 0)										// see if negative index
			iVertex[i] = -iVertex[i] - 1;						// convert to 0..n
		else
			iVertex[i] -= formatInstanceData->gPointIndexBase;						// convert to 0..n

		if (formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == '/')						// see if has another element
		{
			formatInstanceData->gFileIndex++;
			
				/* READ UV INDEX */
				
			ReadInt(formatInstanceData,&iUV[i]);
			if (iUV[i] < 0)										// see if negative index
				iUV[i] = -iUV[i] - 1;
			else
				iUV[i] -= formatInstanceData->gUVIndexBase;						

			if (formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == '/')					// see if has another element
			{
				formatInstanceData->gFileIndex++;
				
					/* READ NORMAL INDEX */
					
				ReadInt(formatInstanceData,&iNormal[i]);
				if (iNormal[i] < 0)								// see if negative index
					iNormal[i] = -iNormal[i] - 1;
				else
					iNormal[i] -= formatInstanceData->gNormalIndexBase;
			}
		}
				
		i++;
					
	}while(((formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] >= '0') &&
			(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] <= '9') ||
			formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == '-'));


		/************************/
		/*		TESSELATE	    */
		/************************/

	numTriangles = i - 2;
		
	for (t = 0; t < numTriangles; t++)
	{			
		int	a = 2, b = 1, c = 0;
		int	n;

		n = group->numTriangles;							// get # triangles in group

	
			/* SET TRIANGLE POINTS */
			
		group->trianglePointIndecies[n][0] = iVertex[0];
		group->trianglePointIndecies[n][1] = iVertex[t+1];
		group->trianglePointIndecies[n][2] = iVertex[t+2];
			
		
			/* SET TRIANGLE UVS */
			
		group->triangleUVIndecies[n][0] = iUV[0];
		group->triangleUVIndecies[n][1] = iUV[t+1];
		group->triangleUVIndecies[n][2] = iUV[t+2];
				
	
			/* SET VERTEX NORMALS */
			
		group->triangleNormalIndecies[n][0] = iNormal[0];
		group->triangleNormalIndecies[n][1] = iNormal[t+1];
		group->triangleNormalIndecies[n][2] = iNormal[t+2];
			
		group->numTriangles++;		
	}
	
	return(kQ3Success);
}



//============================================================================================
//============================================================================================
//============================================================================================

#pragma mark ========= TEXT PARSING ===========


/***************** READ WORD *******************/
//
// Read current text until cr, lf, sp, or /
//

static TQ3Status ReadWord(TE3FFormat_OBJ_Data* formatInstanceData)
{
short	i;

			/**********************/
			/* READ IN THE STRING */
			/**********************/

	i = 0;
	
	while((formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] != ' ') &&				// copy until <sp>, CR, or LF or /
			(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] != 0x0A) &&
			(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] != 0x0D) &&	
			(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] != '/'))
	{
		formatInstanceData->gWord[i] = formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex];					// copy char
		i++;

		formatInstanceData->gWord[i] = 0;											// init string
		if (++formatInstanceData->gFileIndex >= formatInstanceData->gFileSize)
			return(kQ3Failure);
	}
	
	
	SkipSpaces(formatInstanceData);										// after reading token, skip any spaces

	return(kQ3Success);
}



/****************** READ FLOAT *******************/

static TQ3Status ReadFloat(TE3FFormat_OBJ_Data* formatInstanceData,float *value)
{
	if (ReadWord(formatInstanceData) != kQ3Success)
		return(kQ3Failure);
		
	*value = atof(formatInstanceData->gWord);
	
	return(kQ3Success);
}

/****************** READ INT *******************/

static TQ3Status ReadInt(TE3FFormat_OBJ_Data* formatInstanceData,long *value)
{
	if (ReadWord(formatInstanceData) != kQ3Success)
		return(kQ3Failure);
		
	if (formatInstanceData->gWord[0] == 0)			// see if empty
	{
		*value = 0;
		return(kQ3Success);
	}
	
	*value = atoi(formatInstanceData->gWord);
	
	return(kQ3Success);
}


/************************ GET TOKEN ************************/
//
// Assumes the current file pointer is pointing to the start of the next token
//

static int GetToken(TE3FFormat_OBJ_Data* formatInstanceData)
{

			/**********************/
			/* READ IN THE STRING */
			/**********************/

	if (ReadWord(formatInstanceData) != kQ3Success)
		return(TOKEN_END);

	
		/************************/
		/* INTERPRET THE STRING */
		/************************/
			
	if (strcmp(formatInstanceData->gWord, "g") == 0)
		return(TOKEN_GROUP);

	if (strcmp(formatInstanceData->gWord, "o") == 0)
		return(TOKEN_OBJECT);

	if (strcmp(formatInstanceData->gWord, "v") == 0)
		return(TOKEN_VERTEX);

	if (strcmp(formatInstanceData->gWord, "vn") == 0)
		return(TOKEN_VERTEX_NORMAL);
				
	if (strcmp(formatInstanceData->gWord, "vt") == 0)
		return(TOKEN_UV);
	
	if (strcmp(formatInstanceData->gWord, "usemtl") == 0)
		return(TOKEN_USEMTL);

	if (strcmp(formatInstanceData->gWord, "mtllib") == 0)
		return(TOKEN_MTLLIB);

	if (strcmp(formatInstanceData->gWord, "f") == 0)
		return(TOKEN_FACE);

	if (strcmp(formatInstanceData->gWord, "s") == 0)
		return(TOKEN_SHADE);

	//DoAlert("\pUnrecognized Token");
	//DoAlert(gWord);
	
	return(TOKEN_UNKNOWN);
}



/********************* NEXT LINE ********************/
//
// Skips to next qualified line and skips comment lines and blank lines
//
// Returns true if end of file
//

static TQ3Status NextLine(TE3FFormat_OBJ_Data* formatInstanceData)
{
	do
	{
				/* FIND END OF LINE */
				
		while((formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] != 0x0D) &&
				(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] != 0x0A))
		{
			if (++formatInstanceData->gFileIndex >= formatInstanceData->gFileSize)
				return(kQ3Failure);
		}

				/* FIND START OF NEXT LINE */
				
		while((formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == 0x0D) ||
				(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == 0x0A))
		{
			if (++formatInstanceData->gFileIndex >= formatInstanceData->gFileSize)
				return(kQ3Failure);
		}

				/* POINT TO TOKEN */
				
		if (SkipUnwanted(formatInstanceData) != kQ3Success)
			return(kQ3Failure);
			
			
			/* IF TOKEN IS A COMMENT, THEN SKIP THIS LINE */			
	
	}while(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == '#');


	return(kQ3Success);
}


/********************** SKIP SPACES ***********************/
//
// skips spaces CR, and LF.
//

static TQ3Status SkipSpaces(TE3FFormat_OBJ_Data* formatInstanceData)
{
	while(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == ' ')
	{
		if (++formatInstanceData->gFileIndex >= formatInstanceData->gFileSize)
			return(kQ3Failure);
	}
	
	return(kQ3Success);
}

/********************** SKIP UNWANTED ***********************/
//
// skips spaces CR, and LF.
//

static TQ3Status SkipUnwanted(TE3FFormat_OBJ_Data* formatInstanceData)
{
	while((formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == ' ') ||
		(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == 0x0A) || 	// also skip CR / LF's
		(formatInstanceData->gTheOBJData[formatInstanceData->gFileIndex] == 0x0D))
	{
		if (++formatInstanceData->gFileIndex >= formatInstanceData->gFileSize)
			return(kQ3Failure);
	}
	
	return(kQ3Success);
}


#pragma mark -

/******************** NEW GROUP **************************/

static void NewGroup(TE3FFormat_OBJ_Data* formatInstanceData)
{
OBJGroupType	*group;

	
	formatInstanceData->gPointIndexBase 	+= formatInstanceData->gNumPointsInCurrentGroup;
	formatInstanceData->gUVIndexBase 		+= formatInstanceData->gNumUVsInCurrentGroup;
	formatInstanceData->gNormalIndexBase 	+= formatInstanceData->gNumNormalsInCurrentGroup;
	
	formatInstanceData->gGroups[formatInstanceData->gNumGroups] = (OBJGroupType *)Q3Memory_Allocate(sizeof(OBJGroupType));
	if (formatInstanceData->gGroups[formatInstanceData->gNumGroups] == nil)
		return;//DoFatalAlert("\pNewGroup: AllocPtr failed");


	group = formatInstanceData->gGroups[formatInstanceData->gNumGroups];
	
	group->materialNum	= -1;
	group->numPoints	= 0;
	group->numUVs		= 0;
	group->numNormals	= 0;
	group->numTriangles	= 0;

	formatInstanceData->gCurrentGroup = formatInstanceData->gNumGroups;
	formatInstanceData->gNumGroups++;
	
	
	formatInstanceData->gNumPointsInCurrentGroup = 
	formatInstanceData->gNumUVsInCurrentGroup = 
	formatInstanceData->gNumNormalsInCurrentGroup = 0;
}


#pragma mark -

#if 1

TQ3TriMeshTriangleData	tm_triangles[MAX_TRIANGLES];
TQ3Point3D				tm_points[MAX_TRIANGLES * 3];
TQ3Param2D				tm_uvs[MAX_TRIANGLES * 3];
TQ3TriMeshAttributeData	tm_vattrib;

/******************** BUILD QD3D MESH ***********************/

static void BuildQD3DMesh(TE3FFormat_OBJ_Data* formatInstanceData)
{
int				pointCount;
int				i,v,t;
TQ3SurfaceShaderObject	texture;
TQ3AttributeSet		attrib = nil;
TQ3TriMeshData		data;
TQ3Object       newMesh;
	

	for (i = 0; i < formatInstanceData->gNumGroups; i++)
	{
			/* GET TEXTURE ATTRIBUTE */
		
		texture = nil;//gOBJMaterials[gCurrentActiveMTLMaterial].texture;			// get texture reference
		if (texture)
		{
			attrib = Q3AttributeSet_New();
			Q3AttributeSet_Add(attrib, kQ3AttributeTypeSurfaceShader, &texture);
		}
		else
			attrib = nil;
	
	
			/* INIT TRIMESH */
							
		data.triMeshAttributeSet		=	attrib;
		data.numTriangles				=	formatInstanceData->gGroups[i]->numTriangles;
		data.triangles					=	&tm_triangles[0];
		data.numTriangleAttributeTypes	=	0;
		data.triangleAttributeTypes		=	nil;
		data.numEdges					=	0;
		data.edges						=	nil;
		data.numEdgeAttributeTypes		=	0;
		data.edgeAttributeTypes			=	nil;
		data.numPoints					=	formatInstanceData->gGroups[i]->numTriangles * 3;		// no vertex removal - unoptimized
		data.points						=	&tm_points[0];
		data.numVertexAttributeTypes	=	0;
		data.vertexAttributeTypes		=	nil;
		data.bBox.isEmpty				=	kQ3True;

		//if (texture)										// if textured then add vertex uv attributes
		{
			data.numVertexAttributeTypes	=	1;
			data.vertexAttributeTypes		=	&tm_vattrib;
		
			tm_vattrib.attributeType = kQ3AttributeTypeShadingUV;
			tm_vattrib.data = &tm_uvs[0];
			tm_vattrib.attributeUseArray = nil;
		}

			/*****************************/
			/* ADD ALL TRIANGLES TO MESH */
			/*****************************/

		pointCount = 0;
			
		for (t = 0; t < formatInstanceData->gGroups[i]->numTriangles; t++)
		{
			int				pointIndex;
			int				uvIndex;
		
		
					/*************************************/
					/* BUILD 3 VERTICES FOR THE TRIANGLE */
					/*************************************/
					//
					// This generates really sloppy 3DMF data - no shared vertices.
					// But 3DMF Optimizer will clean it up for us.
					//
					
			for (v = 0; v < 3; v++)
			{
				pointIndex = formatInstanceData->gGroups[i]->trianglePointIndecies[t][v];
				uvIndex = formatInstanceData->gGroups[i]->triangleUVIndecies[t][v];
				
								
						/* SET COORD */
						
				tm_points[pointCount] = formatInstanceData->gGroups[i]->points[pointIndex];
				

						/* SET UVS */
						
				tm_uvs[pointCount] = formatInstanceData->gGroups[i]->uvs[uvIndex];
				

						/* SET TRIANGLE INDECIES */
						
				tm_triangles[t].pointIndices[v] = pointCount;

				pointCount++;
			}		
		}


				/***********************/
				/* MAKE TRIMESH OBJECT */
				/***********************/

		newMesh = Q3TriMesh_New(&data);
		if(formatInstanceData->model)
       	 	Q3Group_AddObject(formatInstanceData->model, newMesh);
        Q3Object_Dispose(newMesh);


		if (attrib)
			Q3Object_Dispose(attrib);
		attrib = nil;
	}
}


#else

/******************** BUILD QD3D MESH ***********************/

static void BuildQD3DMesh(void)
{
int				i,v,t;
TQ3Vertex3D		vertex;
TQ3SurfaceShaderObject	texture;
TQ3AttributeSet		attrib;

	for (i = 0; i < gNumGroups; i++)
	{
		/*********************************************/
		/* MAKE NEW MESH TO STORE OBJ GROUP GEOMETRY */
		/*********************************************/
				
		gMeshes[i] = Q3Mesh_New();
		if (gMeshes[i]== nil)
			DoFatalAlert("\pBuildQD3DMesh: Q3Mesh_New failed!");

		Q3Mesh_DelayUpdates(gMeshes[i]);


			/* ASSIGN TEXTURE TO IT */
		
		texture = gOBJMaterials[gCurrentActiveMTLMaterial].texture;			// get texture reference
		
		attrib = Q3AttributeSet_New();
		Q3AttributeSet_Add(attrib, kQ3AttributeTypeSurfaceShader, &texture);
		Q3Geometry_SetAttributeSet(gMeshes[i], attrib);
		Q3Object_Dispose(attrib);


			/*****************************/
			/* ADD ALL TRIANGLES TO MESH */
			/*****************************/
			
		for (t = 0; t < gGroups[i]->numTriangles; t++)
		{
			TQ3MeshVertex	faceVerts[3];
			int				pointIndex;
			int				uvIndex;
		
		
					/*************************************/
					/* BUILD 3 VERTICES FOR THE TRIANGLE */
					/*************************************/
					//
					// This generates really sloppy 3DMF data - no shared vertices.
					// But 3DMF Optimizer will clean it up for us.
					//
					
			for (v = 0; v < 3; v++)
			{
				pointIndex = gGroups[i]->trianglePointIndecies[t][v];
				uvIndex = gGroups[i]->triangleUVIndecies[t][v];
				
			
						/* BUILD VERTEX ATTRIB */
						
				if (gGroups[i]->numUVs > 0)
				{
					vertex.attributeSet = Q3AttributeSet_New();
					Q3AttributeSet_Add(vertex.attributeSet, kQ3AttributeTypeShadingUV, &gGroups[i]->uvs[uvIndex]);
				}
				else
					vertex.attributeSet = nil;
				
						/* SET COORD */
						
				vertex.point = gGroups[i]->points[pointIndex];
				
				
							/* MAKE VERTEX */
							
				faceVerts[v] = Q3Mesh_VertexNew(gMeshes[i], &vertex);
				if (faceVerts[v] == nil)
					DoFatalAlert("\pBuildQD3DMesh: Q3Mesh_VertexNew failed!");
			
				if (vertex.attributeSet)
					Q3Object_Dispose(vertex.attributeSet);
			}		
		
					
			if (Q3Mesh_FaceNew(gMeshes[i], 3, faceVerts, nil) == nil)
				DoFatalAlert("\pBuildQD3DMesh: Q3Mesh_FaceNew failed!");			
		}


		
		Q3Mesh_ResumeUpdates(gMeshes[i]);
	}
}
#endif
#if 0
/***************** DRAW OBJ MODEL ************************/

void DrawOBJModel(QD3DSetupOutputType *setupInfo)
{
int	i;

		/* SUBMIT THE MATERIALS FIRST - FOR 3DMF */
			
	for (i = 0; i < gNumOBJMaterials; i++)
	{
		if (gOBJMaterials[i].texture)
			Q3Object_Submit(gOBJMaterials[i].texture, setupInfo->viewObject);
	}


	for (i = 0; i < gNumGroups; i++)
	{
		Q3Geometry_Submit(gMeshes[i], setupInfo->viewObject);
	}

}


#endif
















