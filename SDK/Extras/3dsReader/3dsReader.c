/*  NAME:
        3dsReader.c

    DESCRIPTION:
        Sample file to create an Importer Plug-In for Quesa.
        
    COPYRIGHT:
        Quesa Copyright © 1999, Quesa Developers.
        
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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include <QD3D.h>
#include <QD3DExtension.h>
#include <QD3DGeometry.h>
#include <QD3DMath.h>
#include <QD3DGroup.h>
#include <QD3DIO.h>
#include <QD3DStorage.h>

#include "QuesaIO.h"

#include "3dsReader.h"

#include <stdlib.h>
#include <string.h>

#if OS_WIN32
#endif
//=============================================================================
//      Internal Types
//-----------------------------------------------------------------------------

enum T3dsChunks {
	k3dsChunkMain						=	0x4d4d,
		kChunkMainVersion				=		0x0002,
			kChunkByteRGB				=			0x0011,
			kChunkIntData				=			0x0030,
			kChunkFloatData				=			0x0031,
		kChunkModelData					=		0x3d3d,
			kChunkModelVersion			=			0x3d3e,
			kChunkMaterial				=			0xafff,
				kChunkMaterialName		=				0xa000,
				kChunkAmbient			=				0xa010,
				kChunkDiffuse			=				0xa020,
				kChunkSpecular			=				0xa030,
				kChunkShininess			=				0xa040,
				kChunkShineStrength		=				0xa041,
				kChunkTransparency		=				0xa050,
				kChunkTextureMap		=				0xa200,
					kChunkTextureFile	=					0xa300,
			kChunkObjBlock				=			0x4000,
				kChunkObjHidden			=				0x4010,
				kChunkTransformMatrix	=				0x4160,
			kChunkTriMesh = 0x4100,
			kChunkVertList = 0x4110,
			kChunkFaceList = 0x4120,
			kChunkFaceMaterial = 0x4130,
			kChunkMapList = 0x4140,
			kChunkSmoothList = 0x4150,
			kChunkDirectionalLight		=			0x4600,
			kChunkAmbientLight			=			0x4680,
			kChunkCamera				=			0x4700,
		kChunkKeyData					=		0xb000,
			kChunkKeyObject				=			0xb002,
				kChunkKeyNodeData		=			0xb010,
				kChunkKeyNodeLevel		=			0xb030,
			kChunkKeyRange				=			0xb008,
			kChunkKeyCurrentTime		=			0xb009,
			kChunkKeyVersion			=			0xb00a,
			kChunkKeyPivot				=			0xb013,
			kChunkKeyTranslate			=			0xb020,
			kChunkKeyRotate				=			0xb021,
			kChunkKeyScale				=			0xb022
};

typedef enum TobjectManagerCmd {
	kParseBegin,				// Called when parsing is about to begin
		kNewMaterial,			// A new material is ready.  cmdData contains TmaterialData
		kNewPointsPolygon,		// A new polygon is ready.  cmdData contains TpointsPolygonData
		kNewLight,				// A new light is ready.  cmdData contains TpointsPolygonData
		kNewCamera,				// A new camera is ready.  cmdData contains TpointsPolygonData
		kNewNode,				// A new node is ready.  cmdData contains TnodeData
		kIsSuppressed,			// Should this object be parsed ?  cmdData contains char *
	kParseDone					// Called when parsing is done
} TobjectManagerCmd;

typedef CALLBACK_API_C(TQ3Boolean, ObjectManagerProcPtr)(TobjectManagerCmd cmd, void *cmdData, void *userData);

typedef struct TmaterialData {
	char *			name;
	TQ3ColorRGB		specularcolor;
	TQ3ColorRGB		Cs;
	TQ3ColorRGB		Os;
	char *			texturename;
	char *			id;
} TmaterialData;

typedef struct TmaterialGroupData {
	char *		material;
	TQ3Int16		nfaces;
	TQ3Int16 *	faces;
	struct TmaterialGroupData *	next;
} TmaterialGroupData;

typedef struct TpointsPolygonData {
	char *		name;
	void *		hanger;
	TQ3Boolean	hidden;
	TQ3Matrix4x4	matrix;
	TQ3Point3D		pivot;
	char*			Ptoken;
	TQ3Point3D*		Ppointer;
	TQ3Int32		npolys;
	TQ3Int32 *		nverts;
	TQ3Int32 *		verts;
	TQ3Int32		numMaterials;
	TmaterialGroupData	materialGroup;
} TpointsPolygonData;

typedef struct TcameraData {
	TQ3Point3D		eyePt;
	TQ3Point3D		viewPt;
	TQ3Float32		viewAngle;
	TQ3Float32		twist;
} TcameraData;


typedef struct TparamData {
	ObjectManagerProcPtr cmd;
	void *userData;
	TQ3Boolean parseAnimation;

	TpointsPolygonData *pointsPolygons;
	TmaterialData *material;

	TQ3ColorRGB colorData;
	TQ3Float32 floatData;
} TparamData;



typedef struct {
	TQ3FFormatBaseData baseData;

	TQ3Object model;
} TE3FFormat_3ds_Data;


//=============================================================================
//      Debug Macros
//-----------------------------------------------------------------------------

// #define DEBUG_3DS

#ifdef DEBUG_3DS
static FILE *sLogFile = NULL;
#define REPORT(a) fprintf a
#else
#define REPORT(a)
#endif

//=============================================================================
//      Global Variables
//-----------------------------------------------------------------------------


static TQ3ObjectType sRegisteredType;	// global variable to hold the
									// signature assigned by Quesa at register time
static TQ3XObjectClass sRegisteredClass;

#define kCameraHint  -2
#define kLightHint  -1


//=============================================================================
//      A Quasi std string.h function
//-----------------------------------------------------------------------------
//      strdup : duplicaters a string
//-----------------------------------------------------------------------------

static char *
strdup (const char *rhs)
{
	char *str = NULL;
	char *c;

	if (rhs == NULL) {
		return (str);
	}
	str = (char *)malloc (strlen (rhs) + 1);
	for (c = str; *rhs != 0x00; c++, rhs++) *c = *rhs;
	*c = 0x00;
	return (str);
}


//=============================================================================
//      Internal functions (Parser Specific)
//-----------------------------------------------------------------------------
//      newMaterialData : Creates and initializes a new TmaterialData struct
//-----------------------------------------------------------------------------

static TmaterialData *
newMaterialData ()
{
	TmaterialData *material = (TmaterialData *)malloc (sizeof(TmaterialData));

	if (material == NULL) {
		return (NULL);
	}

	material->name = NULL;
	material->specularcolor.r = 1.0;
	material->specularcolor.g = 1.0;
	material->specularcolor.b = 1.0;
	material->Cs.r = 1.0;
	material->Cs.g = 1.0;
	material->Cs.b = 1.0;
	material->Os.r = 1.0;
	material->Os.g = 1.0;
	material->Os.b = 1.0;
	material->texturename = NULL;
	material->id = NULL;

	return (material);
}

//=============================================================================
//      deleteMaterialData : Frees a TmaterialData struct
//-----------------------------------------------------------------------------
static void
deleteMaterialData (TmaterialData *material)
{
	if (material == NULL) {
		return;
	}
	if (material->name != NULL) free (material->name);
	if (material->texturename != NULL) free (material->texturename);
	if (material->id != NULL) free (material->id);
	free (material);
}

//=============================================================================
//      newPointsPolygonData : Creates and initializes a new TpointsPolygonData struct
//-----------------------------------------------------------------------------
static TpointsPolygonData *
newPointsPolygonData ()
{
	TpointsPolygonData *pointsPolygons = (TpointsPolygonData *)malloc (sizeof(TpointsPolygonData));

	if (pointsPolygons == NULL) {
		return (NULL);
	}

	pointsPolygons->name = NULL;
	pointsPolygons->hanger = NULL;
	pointsPolygons->hidden = kQ3False;
	pointsPolygons->Ptoken = NULL;
	pointsPolygons->Ppointer = NULL;
	pointsPolygons->npolys = 0;
	pointsPolygons->nverts = NULL;
	pointsPolygons->verts = NULL;
	pointsPolygons->numMaterials = 0;
	pointsPolygons->materialGroup.next = NULL;
	pointsPolygons->pivot.x = 0.0;
	pointsPolygons->pivot.y = 0.0;
	pointsPolygons->pivot.z = 0.0;

	return (pointsPolygons);
}

//=============================================================================
//      deletePointsPolygonData : Frees a TpointsPolygonData struct
//-----------------------------------------------------------------------------
static void
deletePointsPolygonData (TpointsPolygonData *pointsPolygons)
{
	if (pointsPolygons == NULL) {
		return;
	}
	if (pointsPolygons->name != NULL) free (pointsPolygons->name);
	if (pointsPolygons->Ppointer != NULL) free (pointsPolygons->Ppointer);
	if (pointsPolygons->nverts != NULL) free (pointsPolygons->nverts);
	if (pointsPolygons->verts != NULL) free (pointsPolygons->verts);
	free (pointsPolygons);
}



//=============================================================================
//      parse3DSChunk : recursively parses the file.
//-----------------------------------------------------------------------------
static TQ3Boolean
parse3DSChunk (TQ3FileObject theFile, TQ3Uns32 endPos, TparamData *paramData)
{
	char buffer[128];

	TQ3Uns32 curPos;

	TQ3Int16 chunkID;
	TQ3Int32 chunkLength;
	TQ3Boolean continueParsing = kQ3True;
	TQ3FileFormatObject format 		= Q3File_GetFileFormat (theFile);
	TQ3XObjectClass theFormatClass = Q3XObject_GetClass (format);
	TE3FFormat_3ds_Data		*instanceData = (TE3FFormat_3ds_Data *) Q3XObjectClass_GetPrivate(theFormatClass, format);
	
	while (continueParsing && ((curPos = instanceData->baseData.currentStoragePosition) < endPos)) {
		if (Q3Int16_Read (&chunkID, theFile) != kQ3Success) {
			return (kQ3False);
		}
		if (Q3Int32_Read (&chunkLength, theFile) != kQ3Success) {
			return (kQ3False);
		}
		switch (chunkID) {
		case kChunkModelData:
			REPORT ((sLogFile, "kChunkModelData\n"));
			continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
			break;
#ifdef DEBUG_3DS
		case kChunkMainVersion:
			{
				TQ3Int32 mainVersion;
				continueParsing = (TQ3Boolean)(Q3Int32_Read (&mainVersion, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkMainVersion %ld\n", mainVersion));
				instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			}
			break;
#endif
#ifdef DEBUG_3DS
		case kChunkModelVersion:
			{
				TQ3Int32 modelVersion;
				continueParsing = (TQ3Boolean)(Q3Int32_Read (&modelVersion, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkModelVersion %ld\n", modelVersion));
				instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			}
			break;
#endif
#ifdef LATER
		// Materials
		//
		case kChunkMaterial:
			REPORT ((sLogFile, "kChunkMaterial\n"));
			// Create a new material to hold onto the parsed information
			//
			paramData->material = newMaterialData ();

			continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
			if (continueParsing) {
				(*(paramData->cmd))(kNewMaterial, paramData->material, paramData->userData);
			}
			deleteMaterialData (paramData->material);
			paramData->material = NULL;
			break;
		case kChunkMaterialName:
			if (paramData->material != NULL) {
				unsigned long length;
				continueParsing = (TQ3Boolean)(Q3String_Read (buffer, &length, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkMaterialName %s\n", buffer));
				if (continueParsing) {
					if (paramData->material->id != NULL)
						free (paramData->material->id);
					paramData->material->id = strdup(buffer);	
				}
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
		case kChunkByteRGB:
			{
				TQ3Int8 colorComponent;

				continueParsing = (TQ3Boolean)(Q3Int8_Read (&colorComponent, theFile) == kQ3Success);
				if(continueParsing)
					paramData->colorData.r = (colorComponent / 255.0);
				continueParsing = (TQ3Boolean)(Q3Int8_Read (&colorComponent, theFile) == kQ3Success);
				if(continueParsing)
					paramData->colorData.g = (colorComponent / 255.0);
				continueParsing = (TQ3Boolean)(Q3Int8_Read (&colorComponent, theFile) == kQ3Success);
				if(continueParsing)
					paramData->colorData.b = (colorComponent / 255.0);

				REPORT ((sLogFile, "kChunkByteRGB (%g, %g, %g)\n",
											 paramData->colorData.r,
											 paramData->colorData.g,
											 paramData->colorData.b));
				instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			}
			break;
		case kChunkIntData:
			{
				TQ3Int16 intData;
				continueParsing = (TQ3Boolean)(Q3Int16_Read (&intData, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkIntData %d\n", intData));
				if (continueParsing) {
					paramData->floatData = intData;
				}
				instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			}
			break;
		case kChunkFloatData:
			{
				TQ3Float32 floatData;
				continueParsing = (TQ3Boolean)(Q3Float32_Read (&floatData, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkFloatData %g\n", floatData));
				if (continueParsing) {
					paramData->floatData = floatData;
				}
				instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			}
			break;
		case kChunkAmbient:
			REPORT ((sLogFile, "kChunkAmbient\n"));
			continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
			break;
		case kChunkDiffuse:
			REPORT ((sLogFile, "kChunkDiffuse\n"));
			// Parse the diffuse parameters
			//
			continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
			if (continueParsing && (paramData->material != NULL)) {
				paramData->material->Cs = paramData->colorData;
			}
			break;
		case kChunkSpecular:
			REPORT ((sLogFile, "kChunkSpecular\n"));
			// Parse the specular parameters
			//
			continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
			if (continueParsing && (paramData->material != NULL)) {
				paramData->material->specularcolor = paramData->colorData;
			}
			break;
#ifdef DEBUG_3DS
		case kChunkShininess:
			REPORT ((sLogFile, "Unknown chunk: kChunkShininess\n"));
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
#endif
#ifdef DEBUG_3DS
		case kChunkShineStrength:
			REPORT ((sLogFile, "Unknown chunk: kChunkShineStrength\n"));
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
#endif
		case kChunkTransparency:
			// Parse the opacity parameters
			//
			continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
			if (continueParsing && (paramData->material != NULL)) {
				TQ3Float32 opacity = 1.0 - (paramData->floatData / 100.0);
				paramData->material->Os.r = opacity;
				paramData->material->Os.g = opacity;
				paramData->material->Os.b = opacity;
				REPORT ((sLogFile, "kChunkTransparency: %g\n", paramData->floatData));
			}
			break;
		case kChunkTextureMap:
			REPORT ((sLogFile, "kChunkTextureMap\n"));
			continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
			break;
		case kChunkTextureFile:
			if (paramData->material != NULL) {
				unsigned long length;
				continueParsing = (TQ3Boolean)(Q3String_Read (buffer, &length, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkTextureFile %s\n", buffer));
				if (continueParsing) {
					if (paramData->material->texturename != NULL)
						free (paramData->material->texturename);
					paramData->material->texturename = strdup (buffer);
				}
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
#endif
		// Meshes
		//
		case kChunkObjBlock:
			{
				unsigned long length;
				continueParsing = (TQ3Boolean)(Q3String_Read (buffer, &length, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkObjBlock %s\n", buffer));
				paramData->pointsPolygons = newPointsPolygonData ();
				paramData->pointsPolygons->name = strdup (buffer);
				continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
				if (continueParsing && (paramData->pointsPolygons->npolys > 0)) {
					(*(paramData->cmd))(kNewPointsPolygon, paramData->pointsPolygons, paramData->userData);
				}
				else if (paramData->pointsPolygons->npolys < 0) {
					switch (paramData->pointsPolygons->npolys) {
					case kLightHint:
						(*(paramData->cmd))(kNewLight, paramData->pointsPolygons, paramData->userData);
						break;
					case kCameraHint:
						(*(paramData->cmd))(kNewCamera, paramData->pointsPolygons, paramData->userData);
						if (paramData->pointsPolygons->hanger != NULL) {
							free (paramData->pointsPolygons->hanger);
							paramData->pointsPolygons->hanger = NULL;
						}
						break;
					}
				}
				deletePointsPolygonData (paramData->pointsPolygons);
				paramData->pointsPolygons = NULL;
			}
			break;
		case kChunkObjHidden:
			REPORT ((sLogFile, "kChunkObjHidden\n"));
			if (paramData->pointsPolygons != NULL) {
				paramData->pointsPolygons->hidden = kQ3True;
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
		case kChunkTriMesh:
			REPORT ((sLogFile, "kChunkTriMesh\n"));
			continueParsing = parse3DSChunk (theFile, curPos + chunkLength, paramData);
			break;
		case kChunkVertList:
			if (paramData->pointsPolygons != NULL) {
				TQ3Int16 n;
				TQ3Point3D *pointList = NULL;
				TQ3Uns16 numVerts;
				continueParsing = (TQ3Boolean)(Q3Uns16_Read(&numVerts, theFile) == kQ3Success);
				if (numVerts > 0) {
					if (paramData->pointsPolygons->Ppointer != NULL)
						free (paramData->pointsPolygons->Ppointer);
					paramData->pointsPolygons->Ppointer = (TQ3Point3D *)malloc (sizeof(TQ3Point3D) * numVerts);
				}
				REPORT ((sLogFile, "kChunkVertList %d\n", numVerts));
				pointList = (TQ3Point3D *)(paramData->pointsPolygons->Ppointer);
				for (n = 0; (n < numVerts) && continueParsing; n++) {
					TQ3Point3D vertex;
					continueParsing = (TQ3Boolean)(Q3Point3D_Read (&vertex, theFile) == kQ3Success);
					pointList[n].x = -vertex.x;
					pointList[n].y = vertex.z;
					pointList[n].z = -vertex.y;
					REPORT ((sLogFile, "  (%g, %g, %g)\n", vertex.x, vertex.y, vertex.z));
				}
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
		case kChunkMapList:
			REPORT ((sLogFile, "Unknown chunk: kChunkMapList\n"));
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
		case kChunkFaceList:
			if (paramData->pointsPolygons != NULL) {
				TQ3Int32 vert = 0;
				TQ3Int16 n = 0;
				TQ3Int16 numFaces;
				continueParsing = (TQ3Boolean)(Q3Int16_Read(&numFaces, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkFaceList %d\n", numFaces));
				paramData->pointsPolygons->npolys = numFaces;
				if (numFaces > 0) {
					if (paramData->pointsPolygons->nverts != NULL)
						free (paramData->pointsPolygons->nverts);
					paramData->pointsPolygons->nverts = (TQ3Int32 *)malloc (sizeof(TQ3Int32) * numFaces);
					if (paramData->pointsPolygons->verts != NULL)
						free (paramData->pointsPolygons->verts);
					paramData->pointsPolygons->verts = (TQ3Int32 *)malloc (sizeof(TQ3Int32) * numFaces * 3);
				}
				for (n = 0; (n < numFaces) && continueParsing; n++) {
					TQ3Int16 p0, p1, p2, flags;
					continueParsing = (TQ3Boolean)(Q3Int16_Read (&p0, theFile) == kQ3Success);
					if (!continueParsing) break;
					continueParsing = (TQ3Boolean)(Q3Int16_Read (&p1, theFile) == kQ3Success);
					if (!continueParsing) break;
					continueParsing = (TQ3Boolean)(Q3Int16_Read (&p2, theFile) == kQ3Success);
					if (!continueParsing) break;
					continueParsing = (TQ3Boolean)(Q3Int16_Read (&flags, theFile) == kQ3Success);
					if (!continueParsing) break;
					paramData->pointsPolygons->nverts[n] = 3;
					paramData->pointsPolygons->verts[vert++] = p0;
					paramData->pointsPolygons->verts[vert++] = (flags == 7 ? p2 : p1);
					paramData->pointsPolygons->verts[vert++] = (flags == 7 ? p1 : p2);
//					REPORT ((sLogFile, " (" << p0 << ", " << p1 << ", " << p2 << ") " << (void *)(int)flags << "\n";
				}
			}
			else {
				instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			}
			break;
#ifdef LATER
		case kChunkFaceMaterial:
			if (paramData->pointsPolygons != NULL) {
				TmaterialGroupData *materialGroup;
				unsigned long length;
				continueParsing = (TQ3Boolean)(Q3String_Read (buffer, &length, theFile) == kQ3Success);
				REPORT ((sLogFile, "kChunkFaceMaterial %s\n", buffer));
				materialGroup = &(paramData->pointsPolygons->materialGroup);
				if (paramData->pointsPolygons->numMaterials != 0) {
					TmaterialGroupData *last = materialGroup;
					while (last->next != NULL) {
						last = last->next;
					}
					materialGroup = (TmaterialGroupData *)malloc (sizeof(TmaterialGroupData));
					materialGroup->faces = NULL;
					materialGroup->next = NULL;
					last->next = materialGroup;
				}
				materialGroup->material = strdup (buffer);
				paramData->pointsPolygons->numMaterials++;
				continueParsing = (TQ3Boolean)(Q3Int16_Read  (&(materialGroup->nfaces), theFile) == kQ3Success);
				if (continueParsing && (materialGroup->nfaces != 0)) {
					TQ3Int16 n;
					materialGroup->faces = (TQ3Int16 *)malloc (sizeof(TQ3Int16) * materialGroup->nfaces);
					for (n = 0; (n < materialGroup->nfaces) && continueParsing; n++) {
						continueParsing = (TQ3Boolean)(Q3Int16_Read  (&(materialGroup->faces[n]), theFile) == kQ3Success);
					}
				}
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
		case kChunkSmoothList:
			REPORT ((sLogFile, "kChunkSmoothList\n"));
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
#endif
		case kChunkTransformMatrix:
			if (paramData->pointsPolygons != NULL) {
				TQ3Int16 row, col;
				TQ3Matrix4x4 matrix;
				for (row = 0; (row < 4) && continueParsing; row++) {
					for (col = 0; (col < 3) && continueParsing; col++) {
						TQ3Float32 element;
						continueParsing = (TQ3Boolean)(Q3Float32_Read (&element, theFile) == kQ3Success);
						if (continueParsing) {
							matrix.value[row][col] = element;
						}
					}
					matrix.value[row][3] = 0.0;
				}
				matrix.value[3][3] = 1.0;
#ifdef DEBUG_3DS
				REPORT ((sLogFile, "kChunkTransformMatrix\n"));
				REPORT ((sLogFile, "Transform ["));
				for (row = 0; row < 4; row++) {
					for (col = 0; col < 4; col++) {
						if ((row > 0) || (col > 0)) {
							REPORT ((sLogFile, " "));
						}
						REPORT ((sLogFile, "%g", matrix.value[row][col]));
					}
				}
				REPORT ((sLogFile, "]\n"));
#endif
				if (continueParsing) {
					paramData->pointsPolygons->pivot.x = -matrix.value[3][0];
					paramData->pointsPolygons->pivot.y = matrix.value[3][2];
					paramData->pointsPolygons->pivot.z = -matrix.value[3][1];
					matrix.value[3][0] = 0.0;
					matrix.value[3][1] = 0.0;
					matrix.value[3][2] = 0.0;
					for (col = 0; col < 3; col++) {
						TQ3Float32 swap = matrix.value[1][col];
						matrix.value[1][col] = matrix.value[2][col];
						matrix.value[2][col] = swap;
					}
					for (row = 0; row < 4; row++) {
						TQ3Float32 swap = matrix.value[row][1];
						matrix.value[row][1] = matrix.value[row][2];
						matrix.value[row][2] = swap;
					}
					Q3Matrix4x4_Invert (&matrix, &paramData->pointsPolygons->matrix);
#ifdef DEBUG_3DS
					REPORT ((sLogFile, "  pivot (%g, %g, %g)\n", paramData->pointsPolygons.pivot[0], paramData->pointsPolygons.pivot[1], paramData->pointsPolygons.pivot[2]));
					REPORT ((sLogFile, "Transform ["));
					for (row = 0; row < 4; row++) {
						for (col = 0; col < 4; col++) {
							if ((row > 0) || (col > 0)) {
								REPORT ((sLogFile, " "));
							}
							REPORT ((sLogFile, "%g", paramData->pointsPolygons.matrix.value[row][col]));
						}
					}
					REPORT ((sLogFile, "]\n"));
#endif
				}
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
#ifdef LATER
		case kChunkDirectionalLight:
			REPORT ((sLogFile, "kChunkDirectionalLight\n"));
			// Tell kChunkObjBlock we found a light
			//
			if (paramData->pointsPolygons != NULL) {
				paramData->pointsPolygons->npolys = kLightHint;
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
		case kChunkAmbientLight:
			REPORT ((sLogFile, "kChunkAmbientLight\n"));
			// Tell kChunkObjBlock we found a light
			//
			if (paramData->pointsPolygons != NULL) {
				paramData->pointsPolygons->npolys = kLightHint;
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
		case kChunkCamera:
			REPORT ((sLogFile, "kChunkCamera\n"));
			if (paramData->pointsPolygons != NULL) {
				TcameraData *cameraData = (TcameraData *)malloc (sizeof(TcameraData));
				TQ3Point3D fltPoint;
				TQ3Float32 twist;
				TQ3Float32 angle;

				// Tell kChunkObjBlock we found a camera
				//
				paramData->pointsPolygons->npolys = kCameraHint;

				continueParsing = (TQ3Boolean)(Q3Point3D_Read (&fltPoint, theFile) == kQ3Success);
				cameraData->eyePt.x = -fltPoint.x;
				cameraData->eyePt.y = fltPoint.y;
				cameraData->eyePt.z = -fltPoint.z;

				continueParsing = (TQ3Boolean)(Q3Point3D_Read (&fltPoint, theFile) == kQ3Success);
				cameraData->viewPt.x = -fltPoint.x;
				cameraData->viewPt.y = fltPoint.z;
				cameraData->viewPt.z = -fltPoint.y;

				continueParsing = (TQ3Boolean)(Q3Float32_Read (&twist, theFile) == kQ3Success);
				cameraData->twist = twist;

				continueParsing = (TQ3Boolean)(Q3Float32_Read (&angle, theFile) == kQ3Success);
				cameraData->viewAngle = angle;

				paramData->pointsPolygons->hanger = cameraData;
			}
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
#endif
		default:
			REPORT ((sLogFile, "Unknown chunk: 0x%x\n", (void *)(int)chunkID));
			instanceData->baseData.currentStoragePosition = curPos + chunkLength;
			break;
		};
	};
	instanceData->baseData.currentStoragePosition = endPos;

	return (TQ3Boolean)((endPos <=instanceData->baseData.logicalEOF) && continueParsing);
}


//=============================================================================
//      parse3DS : Ignites the parsing.
//-----------------------------------------------------------------------------
static TQ3Boolean
parse3DS (TQ3FileObject theFile, ObjectManagerProcPtr cmd, void *userData)
{
	TQ3Int32 chunkLength;
	TQ3Int16 label;
	TQ3Boolean status;
	TparamData paramData;

	if ((theFile == NULL) || (cmd == NULL)) {
		return (kQ3False);
	}
	// Check to make sure this is indeed a 3D Studio scene
	//

	
	if (Q3Int16_Read (&label, theFile) != kQ3Success) {
		return (kQ3False);
	}

	// Get the main chunk length
	//
	if (Q3Int32_Read (&chunkLength, theFile) != kQ3Success) {
		return (kQ3False);
	}

	// Fill in paramData
	//
	paramData.cmd = cmd;
	paramData.userData = userData;
	paramData.parseAnimation = kQ3False;

	paramData.pointsPolygons = NULL;
	paramData.material = NULL;

	// Parse the scene
	//
#ifdef DEBUG_3DS
	sLogFile = fopen ("parse3DStudio.log", "w+");
#endif
	(*cmd)(kParseBegin, NULL, userData);
	status = parse3DSChunk (theFile, chunkLength, &paramData);
	(*cmd)(kParseDone, NULL, userData);
#ifdef DEBUG_3DS
	fclose (sLogFile);
#endif

	// Clean up on the way out
	//
	deletePointsPolygonData (paramData.pointsPolygons);
	deleteMaterialData (paramData.material);

	return (status);
}

//=============================================================================
//      parse3DStudioCallback : TBD.
//-----------------------------------------------------------------------------

static TQ3Boolean
parse3DStudioCallback (TobjectManagerCmd cmd, void *cmdData, void *userData)
{
	TE3FFormat_3ds_Data *instanceData = (TE3FFormat_3ds_Data *)userData;

	if (instanceData == NULL) {
		return (kQ3False);
	}


	switch (cmd) {
	case kParseBegin:
		instanceData->model = Q3DisplayGroup_New ();
		break;
#ifdef LATER
	case kNewMaterial: {
			TmaterialData *materialData = (TmaterialData *)cmdData;
			RtMaterial *material = new RtMaterial;
			material->name = materialData->name;
			materialData->name = NULL;
			material->specularcolor = materialData->specularcolor;
			material->Cs = materialData->Cs;
			material->Os = materialData->Os;
			material->texturename = materialData->texturename;
			materialData->texturename = NULL;
			material->id = materialData->id;
			materialData->id = NULL;
			material->next = TparseScene::fsMaterialList->next;
			TparseScene::fsMaterialList->next = material;

		}
		break;
#endif
	case kNewPointsPolygon: {
			unsigned int t;
			TpointsPolygonData *polygonData = (TpointsPolygonData *)cmdData;
			TQ3TriMeshData triMeshData;
			TQ3GeometryObject triMesh;

			// Set up the data
			triMeshData.triMeshAttributeSet       = NULL;
			triMeshData.numTriangles              = polygonData->npolys;
			triMeshData.triangles                 = (TQ3TriMeshTriangleData *)(polygonData->verts);
			triMeshData.numPoints                 = 0;
			for (t = 0; t < triMeshData.numTriangles; t++) {
				TQ3TriMeshTriangleData *triangleData = triMeshData.triangles + t;
				if (triangleData->pointIndices[0] > triMeshData.numPoints) triMeshData.numPoints = triangleData->pointIndices[0];
				if (triangleData->pointIndices[1] > triMeshData.numPoints) triMeshData.numPoints = triangleData->pointIndices[1];
				if (triangleData->pointIndices[2] > triMeshData.numPoints) triMeshData.numPoints = triangleData->pointIndices[2];
			}
			triMeshData.numPoints++;
			triMeshData.points                    = (TQ3Point3D *)(polygonData->Ppointer);
			triMeshData.numTriangleAttributeTypes = 0;
			triMeshData.triangleAttributeTypes    = NULL;
			triMeshData.numEdges                  = 0;
			triMeshData.edges                     = NULL;
			triMeshData.numEdgeAttributeTypes     = 0;
			triMeshData.edgeAttributeTypes        = NULL;
			triMeshData.numVertexAttributeTypes   = 0;
			triMeshData.vertexAttributeTypes      = NULL;

			Q3BoundingBox_SetFromPoints3D (&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));

			triMesh = Q3TriMesh_New (&triMeshData);
			Q3Group_AddObject (instanceData->model, triMesh);
			Q3Object_Dispose (triMesh);
		}
		break;
#ifdef LATER
	case kNewLight: {
			TpointsPolygonData *polygonData = (TpointsPolygonData *)cmdData;
			RtNodeData *node = new RtNodeData;
			node->name = polygonData->name;
			polygonData->name = NULL;
			node->nodeType = RtNodeData::kLightNode;
			TparseScene::fsWorld->addObject (node);
		}
		break;
	case kNewCamera: {
			TpointsPolygonData *polygonData = (TpointsPolygonData *)cmdData;
			RtNodeData *node = new RtNodeData;
			node->name = polygonData->name;
			polygonData->name = NULL;
			node->nodeType = RtNodeData::kCameraNode;

			TcameraData *cameraData = (TcameraData *)(polygonData->hanger);
			if (cameraData != NULL) {
				RtCameraData *camera = new RtCameraData;

				camera->setEye (cameraData->eyePt);
				camera->setView (cameraData->viewPt);
				camera->setTwist (cameraData->twist);

				if ((me->fCamera != NULL) && !me->fCameraSet) {
					*me->fCamera = *camera;
					me->fCameraSet = RI_TRUE;
				}
				node->hanger = camera;
			}

			TparseScene::fsWorld->addObject (node);
		}
		break;
#endif
	case kParseDone:
//		RtMaterial::destroy (TparseScene::fsMaterialList);
//		TparseScene::fsMaterialList = NULL;
		break;
	}
	return (kQ3True);
}

//=============================================================================
//      Internal functions (Quesa FileFormat Glue)
//-----------------------------------------------------------------------------
//      e3fformat_3ds_canread : Determines the if we can read this data.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_3ds_canread(TQ3StorageObject storage, TQ3ObjectType* theFileFormatFound)
{
	TQ3Int16 label;
	TQ3Uns32 sizeRead;

	if (theFileFormatFound == NULL) {
		return (kQ3False);
	}

	*theFileFormatFound = kQ3ObjectTypeInvalid;
	

	// Check the signature
	//
		// read 2 bytes, search for 0x4d4d
		//
		// unfortunately we cant check here for endianness
		// since 0x4D4D is symetric
		
	if(Q3Storage_GetData (storage,0, 2,(unsigned char*)&label, &sizeRead) == kQ3Success){

		if (sizeRead != 2)
			return kQ3False;
		if (label == k3dsChunkMain)
			*theFileFormatFound = sRegisteredType;
			return kQ3True;
		}
	return kQ3False;
}




//-----------------------------------------------------------------------------
//      e3fformat_3ds_read_header : Initialize the reader.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_3ds_read_header(TQ3FileObject theFile)
{
	TQ3Boolean result;

	TQ3FileFormatObject format = Q3File_GetFileFormat (theFile);
	TQ3XObjectClass theFormatClass = Q3XObject_GetClass (format);
	TE3FFormat_3ds_Data		*instanceData = (TE3FFormat_3ds_Data *) Q3XObjectClass_GetPrivate(theFormatClass, format);

	instanceData->model = NULL;
	
	// a 3DS file is allways littlendian
	instanceData->baseData.byteOrder = kQ3EndianLittle;

	result = parse3DS (theFile, &parse3DStudioCallback, instanceData);


	return (result);
}


//=============================================================================
//      e3fformat_3ds_skipobject : Skips the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3ds_skipobject(TQ3FileObject theFile)
{
	/*
	
	in this method you have to discard the next object in file (or in the cached list
	created in the readHeader method)
	
	pay attention at the readInGroup Flag of TE3FFormatBaseData to know if you have to
	discard just one element or the entire group (the meaning of element or group is up to you)
	
	*/


	return (kQ3Failure);
}





//=============================================================================
//      e3fformat_3ds_readobject : Reads the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3ds_readobject (TQ3FileObject theFile)
{
	TQ3FileFormatObject format = Q3File_GetFileFormat (theFile);
	TQ3XObjectClass theFormatClass = Q3XObject_GetClass (format);
	TE3FFormat_3ds_Data		*instanceData = (TE3FFormat_3ds_Data *) Q3XObjectClass_GetPrivate(theFormatClass, format);
	
	if(instanceData){
		instanceData->baseData.noMoreObjects = kQ3True;

		return (instanceData->model);
		}
	return NULL;
}





//=============================================================================
//      e3fformat_3ds_get_nexttype : Returns the type of the next object.
//-----------------------------------------------------------------------------
// 		Note : Doesn't move the storage position.
//-----------------------------------------------------------------------------
static TQ3ObjectType
e3fformat_3ds_get_nexttype(TQ3FileObject theFile)
{
 
 /*
 	
	in this method you have to return the Quesa type of the object that will be returned
	by a next call to readObject
	
	pay attention at the readInGroup Flag of TE3FFormatBaseData to know if you have to
	discard just one element or the entire group (the meaning of element or group is up to you)
	
 */

	return (kQ3ObjectTypeInvalid);
}





//=============================================================================
//      e3fformat_3ds_close : frees memory.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3ds_close(TQ3FileFormatObject format, TQ3Boolean abort)
{

/*

	this method is the counterpart of the readHeader one,
	free here the memory allocated by the calls above
	
	this method is the last method called prior deallocating the reader

*/
	return (kQ3Success);

}





//=============================================================================
//      e3fformat_3ds_metahandler : Metahandler for 3DMF Text.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3ds_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeFFormatCanRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3ds_canread;
			break;

		case kQ3XMethodTypeFFormatReadHeader:
			theMethod = (TQ3XFunctionPointer) e3fformat_3ds_read_header;
			break;


		case kQ3XMethodTypeFFormatReadObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_3ds_readobject;
			break;

		case kQ3XMethodTypeFFormatSkipObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_3ds_skipobject;
			break;

		case kQ3XMethodTypeFFormatGetNextType:
			theMethod = (TQ3XFunctionPointer) e3fformat_3ds_get_nexttype;
			break;

		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) e3fformat_3ds_close;
			break;


#if QUESA_HOST_IS_BIG_ENDIAN


		case kQ3XMethodTypeFFormatFloat32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_64;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatInt64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_64;
			break;

#else

		case kQ3XMethodTypeFFormatFloat32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_64;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_32;
			break;

		case kQ3XMethodTypeFFormatInt64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_64;
			break;


#endif
		case kQ3XMethodTypeFFormatInt8Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_8;
			break;

		case kQ3XMethodTypeFFormatStringRead:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_String;
			break;

		case kQ3XMethodTypeFFormatRawRead:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_Raw;
			break;


		}
	
	return(theMethod);
}


//-----------------------------------------------------------------------------
//      e3fformat_3ds_runtime_registerclass : Register the classes.
//-----------------------------------------------------------------------------
static void
e3fformat_3ds_runtime_registerclass(void)
{
	

	sRegisteredClass = Q3XObjectHierarchy_RegisterClass (kQ3FileFormatTypeReader,
										 &sRegisteredType, 
										 kQ3ClassNameFileFormatReader_3DS,
										 e3fformat_3ds_metahandler,
										 NULL,
										 0,
										 sizeof(TE3FFormat_3ds_Data));
										 		

}





//=============================================================================
//      e3fformat_3ds_runtime_unregisterclass : Unregister the classes.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3ds_runtime_unregisterclass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes


	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(sRegisteredClass);

	return(qd3dStatus);
}






//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFormat_Sample_Reader_RegisterClass : Register the classes.
//-----------------------------------------------------------------------------
#if ((defined QUESA_REGISTER_BUILTIN_PLUGINS) && (QUESA_REGISTER_BUILTIN_PLUGINS == 0)) 
TQ3Status
E3FFormat_3ds_Reader_RegisterClass(void)
{	TQ3Status		qd3dStatus;
	

	qd3dStatus = E3ClassTree_RegisterClass(kQ3FileFormatTypeReader,
											kE3FFormatReaderType3DS,
											kQ3ClassNameFileFormatReader_3DS,
											e3fformat_3ds_metahandler,
											sizeof(TE3FFormat_3ds_Data));

	return (qd3dStatus);
}





//=============================================================================
//      E3FFormat_3ds_Reader_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3ds_Reader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes


	qd3dStatus = E3ClassTree_UnregisterClass(kE3FFormatReaderType3DS, kQ3True);


	return(qd3dStatus);
}

#endif

//=============================================================================
//      E3FFormat_3ds_Reader_Entry : CFM entry point for Mac.
//-----------------------------------------------------------------------------
#if OS_MACINTOSH
OSErr
E3FFormat_3ds_Reader_Entry(void)
{	
	e3fformat_3ds_runtime_registerclass();
	
	if(sRegisteredClass == NULL)
	{
		return kQ3Failure;
	}
	
	return noErr;

}
#endif



//=============================================================================
//      E3FFormat_3ds_Reader_Exit : CFM exit point for Mac.
//-----------------------------------------------------------------------------
#if OS_MACINTOSH
long
E3FFormat_3ds_Reader_Exit(void)
{

	e3fformat_3ds_runtime_unregisterclass();

	return 0;
}

#endif

//=============================================================================
//      DllMain : DLL entry point for Windows.
//-----------------------------------------------------------------------------
#if OS_WIN32
BOOL APIENTRY
DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			e3fformat_3ds_runtime_registerclass();
			
			if(sRegisteredClass == NULL)
			{
				return FALSE;
			}
			break;
		case DLL_PROCESS_DETACH:
			e3fformat_3ds_runtime_unregisterclass();
			break;
    }
    return TRUE;
}

#endif
