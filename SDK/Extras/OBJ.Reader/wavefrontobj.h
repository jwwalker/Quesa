/*  NAME:
        WAVEFRONTOBJ.h

    DESCRIPTION:
        header of the implementation WaveFront .OBJ Importer Plug-In for Quesa.
        
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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/

//adapted with permission from

/****************************/
/*   	WAVEFRONTOBJ.h 		*/
/* (c)2000 Pangea Software  */
/* By Brian Greenstone      */
/****************************/

#include "QuesaIO.h"

#define	MAX_OBJ_VERTS	5000
#define	MAX_TRIANGLES	5000

#define	MAX_POLY_SIZE	256
#define	MAX_GROUPS		50



typedef struct OBJMaterialType {
	unsigned char           name[33];
	TQ3ColorRGB				diffuseColor;
	TQ3ColorRGB				ambientColor;
	TQ3ColorRGB				specularColor;
	float					diffuseCoeff;
	float					specularCoeff;
	TQ3SurfaceShaderObject	texture;
} OBJMaterialType;


typedef struct OBJGroupType {
	short		materialNum;

	TQ3Int32	numPoints;	
	TQ3Point3D	points[MAX_OBJ_VERTS];
	
	TQ3Int32	numUVs;
	TQ3Param2D	uvs[MAX_OBJ_VERTS];
	
	TQ3Int32	numNormals;
	TQ3Vector3D	normals[MAX_OBJ_VERTS];
	
	TQ3Int32	numTriangles;
	TQ3Uns16		trianglePointIndecies[MAX_TRIANGLES][3];
	TQ3Uns16		triangleUVIndecies[MAX_TRIANGLES][3];
	TQ3Uns16		triangleNormalIndecies[MAX_TRIANGLES][3];
} OBJGroupType;






typedef struct TE3FFormat_OBJ_Data {
	TQ3FFormatBaseData baseData;

	TQ3Object			model;
	TQ3Boolean			gNextVertIsNewGroup;
	TQ3Size				gFileIndex;
	TQ3Size				gFileSize;
	TQ3Uns32			gNumPointsInCurrentGroup;
	TQ3Uns32			gPointIndexBase;
	TQ3Uns32			gNumUVsInCurrentGroup;
	TQ3Uns32			gUVIndexBase;
	TQ3Uns32			gNumNormalsInCurrentGroup;
	TQ3Uns32			gNormalIndexBase;
	TQ3Uns8*			gTheOBJData;
	TQ3Uns32			gNumGroups;
	TQ3Uns32			gCurrentGroup;
	OBJGroupType*		gGroups[MAX_GROUPS];
	char				gWord[32];
} TE3FFormat_OBJ_Data;



extern	void ImportObjFile(TE3FFormat_OBJ_Data* formatInstanceData);
