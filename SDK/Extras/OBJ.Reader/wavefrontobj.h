/*  NAME:
        WAVEFRONTOBJ.h

    DESCRIPTION:
        header of the implementation WaveFront .OBJ Importer Plug-In for Quesa.
        
    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
