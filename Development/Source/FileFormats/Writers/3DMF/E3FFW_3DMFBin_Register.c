/*  NAME:
        E3FFW_3DMFBin_Register.c

    DESCRIPTION:
        Quesa 3DMFBin Writer registration.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3FFR_3DMF.h"
#include "E3FFW_3DMFBin_Register.h"
#include "E3FFW_3DMFBin_Writer.h"
#include "E3FFW_3DMFBin_Geometry.h"






//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------

#define kTextNormalNickName							"3DMF Text"

#define kBinNormalNickName							"3DMF Binary"
#define kBinStreamNickName							"3DMF Binary Stream"
#define kBinDatabaseNickName						"3DMF Binary Database"
#define kBinDatabaseStreamNickName					"3DMF Binary Database Stream"

#define kBinNormalSwapNickName						"3DMF Binary Byte swapped"
#define kBinStreamSwapNickName						"3DMF Binary Stream Byte swapped"
#define kBinDatabaseSwapNickName					"3DMF Binary Database Byte swapped"
#define kBinDatabaseStreamSwapNickName				"3DMF Binary Database Stream Byte swapped"



//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------



class E3Binary3DMFWriter_S : public E3FileFormatWriter  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TE3FFormatW3DMF_Data				instanceData ;
	} ;
	


class E3Binary3DMFWriter_N : public E3FileFormatWriter  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TE3FFormatW3DMF_Data				instanceData ;
	} ;
	


class E3Binary3DMFWriter_D : public E3FileFormatWriter  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TE3FFormatW3DMF_Data				instanceData ;
	} ;
	


class E3Binary3DMFWriter_DS : public E3FileFormatWriter  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TE3FFormatW3DMF_Data				instanceData ;
	} ;
	


class E3Binary3DMFWriter_SW : public E3FileFormatWriter  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TE3FFormatW3DMF_Data				instanceData ;
	} ;
	


class E3Binary3DMFWriter_NW : public E3FileFormatWriter  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TE3FFormatW3DMF_Data				instanceData ;
	} ;
	


class E3Binary3DMFWriter_DW : public E3FileFormatWriter  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TE3FFormatW3DMF_Data				instanceData ;
	} ;
	


class E3Binary3DMFWriter_DSW : public E3FileFormatWriter  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TE3FFormatW3DMF_Data				instanceData ;
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3ffw_3dmf_geom : Writer geometry metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmf_geom(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		// Required
		case kQ3GeometryTypeTriangle:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Triangle;
			break;

		case kQ3GeometryTypeLine:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Line;
			break;

		case kQ3GeometryTypePoint:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Point;
			break;

		case kQ3GeometryTypePolyLine:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_PolyLine;
			break;

		case kQ3GeometryTypeEllipse:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Ellipse;
			break;

		case kQ3GeometryTypePolygon:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Polygon;
			break;

		case kQ3GeometryTypeEllipsoid:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Ellipsoid;
			break;

		case kQ3GeometryTypeTriMesh:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_TriMesh;
			break;

		case kQ3GeometryTypeCone:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Cone;
			break;

		case kQ3GeometryTypeTorus:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Torus;
			break;

		case kQ3GeometryTypeCylinder:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Cylinder;
			break;

		case kQ3GeometryTypeDisk:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Disk;
			break;

		case kQ3GeometryTypeBox:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Box;
			break;

		case kQ3GeometryTypeTriGrid:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_TriGrid;
			break;

		case kQ3GeometryTypeGeneralPolygon:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_GeneralPolygon;
			break;

		case kQ3GeometryTypeMesh:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Mesh;
			break;

		case kQ3GeometryTypeNURBPatch:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_NURBPatch;
			break;


		case kQ3GeometryTypeMarker:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Marker;
			break;

		case kQ3GeometryTypePixmapMarker:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_PixmapMarker;
			break;


/* 
	by now let's convert everything to triangles
		case kQ3GeometryTypeNURBCurve:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_NURBCurve;
			break;

		case kQ3GeometryTypePolyhedron:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Polyhedron;
			break;

*/
		}
	
	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_new : Initialize the data.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_new(TQ3Object theObject, void *privateData, const void *paramData)
{
#pragma unused(theObject)
	((TE3FFormatW3DMF_Data*)privateData)->fileMode = Q3Object_GetLeafType(theObject);
	
    return(kQ3Success);
}





//=============================================================================
//      e3ffw_3dmf_formatname : Get the format name.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmf_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize, const char * nickName)
{


	// Return the amount of space we need
    *actualDataSize = strlen(nickName) + 1;



	// If we have a buffer, return the nick name
	if (dataBuffer != NULL)
		{
		// Clamp the buffer size
		if (bufferSize < *actualDataSize)
			*actualDataSize = bufferSize;



		// Return the string
		Q3Memory_Copy(nickName, dataBuffer, (*actualDataSize)-1);
        dataBuffer[(*actualDataSize)-1] = 0x00;
        }

    return(kQ3Success);
}




//=============================================================================
//      e3ffw_3dmftxt_N_formatname : Get the format name for normal files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmftxt_N_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kTextNormalNickName);
}




//=============================================================================
//      e3ffw_3dmfbin_N_formatname : Get the format name for normal files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_N_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kBinNormalNickName);
}




//=============================================================================
//      e3ffw_3dmfbin_S_formatname : Get the format name for stream files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_S_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kBinStreamNickName);
}




//=============================================================================
//      e3ffw_3dmfbin_D_formatname : Get the format name for database files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_D_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kBinDatabaseNickName);
}




//=============================================================================
//      e3ffw_3dmfbin_DS_formatname : Get the format name for database stream files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_DS_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kBinDatabaseStreamNickName);
}




//=============================================================================
//      e3ffw_3dmfbin_NW_formatname : Get the format name for normal swapped files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_NW_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kBinNormalSwapNickName);
}




//=============================================================================
//      e3ffw_3dmfbin_SW_formatname : Get the format name for stream swapped files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_SW_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kBinStreamSwapNickName);
}




//=============================================================================
//      e3ffw_3dmfbin_DW_formatname : Get the format name for database swapped files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_DW_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kBinDatabaseSwapNickName);
}




//=============================================================================
//      e3ffw_3dmfbin_DSW_formatname : Get the format name for database stream swapped files.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_DSW_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	return e3ffw_3dmf_formatname(dataBuffer, bufferSize, actualDataSize, kBinDatabaseStreamSwapNickName);
}




//=============================================================================
//      e3ffw_3dmf_metahandler : Writer metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmf_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_new;
			break;

		case kQ3XMethodTypeRendererStartFrame:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_StartFile;
			break;

		case kQ3XMethodTypeRendererEndPass:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_EndPass;
			break;

		case kQ3XMethodTypeRendererCancel:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Cancel;
			break;

		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Close;
			break;

		// object submit
		case kQ3XMethodTypeFFormatSubmitObject:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_TraverseObject;
			break;

		case kQ3XMethodTypeFFormatSubmitGroup:
			// needs to be special to allow contents submit
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Group;
			break;
		
		default: // get the geometry
			// needs to be special to allow geometry decomposition
			theMethod = e3ffw_3dmf_geom (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_metahandler : Writer metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeFFormatFloat32Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_64;
			break;

		case kQ3XMethodTypeFFormatInt8Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_8;
			break;

		case kQ3XMethodTypeFFormatInt16Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_16;
			break;

		case kQ3XMethodTypeFFormatInt32Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_32;
			break;

		case kQ3XMethodTypeFFormatInt64Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_64;
			break;

		case kQ3XMethodTypeFFormatStringWrite:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_String;
			break;

		case kQ3XMethodTypeFFormatRawWrite:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_Raw;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmf_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_N_metahandler : Writer metahandler for normal files.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_N_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_N_formatname;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_S_metahandler : Writer metahandler for stream files.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_S_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_S_formatname;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_D_metahandler : Writer metahandler for database files.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_D_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_D_formatname;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_DS_metahandler : Writer metahandler for database stream files.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_DS_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_DS_formatname;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}




//=============================================================================
//      e3ffw_3dmfbin_swap_metahandler : Writer metahandler for swapped modes.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_swap_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeFFormatFloat32Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinSwap_64;
			break;

		case kQ3XMethodTypeFFormatInt16Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinSwap_16;
			break;

		case kQ3XMethodTypeFFormatInt32Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatInt64Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinSwap_64;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_NW_metahandler : Writer metahandler for normal swapped files.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_NW_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_NW_formatname;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_swap_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_SW_metahandler : Writer metahandler for stream swapped files.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_SW_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_SW_formatname;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_swap_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_DW_metahandler : Writer metahandler for database swapped files.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_DW_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_DW_formatname;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_swap_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_DSW_metahandler : Writer metahandler for database stream swapped files.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_DSW_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;


	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_DSW_formatname;
			break;

		default: // get the common methods
			theMethod = e3ffw_3dmfbin_swap_metahandler (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFW_3DMFBin_Register : Register the writer.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3FFW_3DMFBin_Register(void)
{	TQ3Status		qd3dStatus;


// the native formats
	qd3dStatus = E3ClassTree::RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFStreamBin,
											kQ3ClassNameFileFormatW_3DMF_S_Bin,
											e3ffw_3dmfbin_S_metahandler,
											~sizeof(E3Binary3DMFWriter_S));

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFNormalBin,
											kQ3ClassNameFileFormatW_3DMF_N_Bin,
											e3ffw_3dmfbin_N_metahandler,
											~sizeof(E3Binary3DMFWriter_N));

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFDatabaseBin,
											kQ3ClassNameFileFormatW_3DMF_D_Bin,
											e3ffw_3dmfbin_D_metahandler,
											~sizeof(E3Binary3DMFWriter_D));

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFDatabaseStreamBin,
											kQ3ClassNameFileFormatW_3DMF_DS_Bin,
											e3ffw_3dmfbin_DS_metahandler,
											~sizeof(E3Binary3DMFWriter_DS));

// the swapped formats

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFStreamBinSwap,
											kQ3ClassNameFileFormatW_3DMF_SW_Bin,
											e3ffw_3dmfbin_SW_metahandler,
											~sizeof(E3Binary3DMFWriter_SW));

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFNormalBinSwap,
											kQ3ClassNameFileFormatW_3DMF_NW_Bin,
											e3ffw_3dmfbin_NW_metahandler,
											~sizeof(E3Binary3DMFWriter_NW));

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFDatabaseBinSwap,
											kQ3ClassNameFileFormatW_3DMF_DW_Bin,
											e3ffw_3dmfbin_DW_metahandler,
											~sizeof(E3Binary3DMFWriter_DW));

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFDatabaseStreamBinSwap,
											kQ3ClassNameFileFormatW_3DMF_DSW_Bin,
											e3ffw_3dmfbin_DSW_metahandler,
											~sizeof(E3Binary3DMFWriter_DSW));

	return(qd3dStatus);
}





//=============================================================================
//      E3FFW_3DMFBin_Unregister : Unregister the writer.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Unregister(void)
{	TQ3Status			qd3dStatus;


	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatWriterType3DMFStreamBin,kQ3True);
	
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatWriterType3DMFNormalBin,kQ3True);
	
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatWriterType3DMFDatabaseBin,kQ3True);
	
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatWriterType3DMFDatabaseStreamBin,kQ3True);
	


	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatWriterType3DMFStreamBinSwap,kQ3True);
	
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatWriterType3DMFNormalBinSwap,kQ3True);
	
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatWriterType3DMFDatabaseBinSwap,kQ3True);
	
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatWriterType3DMFDatabaseStreamBinSwap,kQ3True);

	return(qd3dStatus);
}

