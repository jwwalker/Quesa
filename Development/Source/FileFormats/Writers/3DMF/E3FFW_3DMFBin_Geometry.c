/*  NAME:
        E3FFW_3DMFBin_Geometry.c

    DESCRIPTION:
        Quesa 3DMFBin writer geometry methods.

    COPYRIGHT:
        Quesa Copyright � 1999-2003, Quesa Developers.
        
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
#include "E3FFW_3DMFBin_Geometry.h"
#include "E3FFW_3DMFBin_Writer.h"
#include "E3Set.h"



//=============================================================================
//      Private functions
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_storage_write : utility method to write storages (for pixmaps ecc).
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_storage_write(TQ3StorageObject theStorage,TQ3Uns32 expectedSize,TQ3FileObject theFile)
{
	TQ3Status			qd3dStatus;
	TQ3Uns8				*basePtr = NULL;
	TQ3Boolean			wasCopied = kQ3False;
	TQ3Uns32			validSize, bufferSize;
	TQ3ObjectType		theType;
#if QUESA_OS_MACINTOSH
	Handle				theHnd = NULL;
#endif			

	// Get a pointer to the data for the image, ideally without copying

	theType    = Q3Storage_GetType(theStorage);
	switch (theType) {
		case kQ3StorageTypeMemory:
			qd3dStatus = Q3MemoryStorage_GetBuffer(theStorage, &basePtr, &validSize, &bufferSize);
			break;

#if QUESA_OS_MACINTOSH
		case kQ3MemoryStorageTypeHandle:
			{
			
			qd3dStatus = Q3HandleStorage_Get(theStorage, &theHnd, &validSize);
			if (qd3dStatus == kQ3Success && theHnd != NULL)
				{
			    MoveHHi(theHnd);
    			HLock(theHnd);
    			basePtr = (TQ3Uns8 *) *theHnd;
    			}
    		}
    		break;
#endif

		default:
			qd3dStatus = Q3Storage_GetSize(theStorage, &bufferSize);
			if (qd3dStatus == kQ3Success)
				basePtr = (TQ3Uns8 *) Q3Memory_Allocate(bufferSize);
			
			if (basePtr != NULL)
				{
				qd3dStatus = Q3Storage_GetData(theStorage, 0, bufferSize, basePtr, &validSize);
				wasCopied = (TQ3Boolean) (qd3dStatus == kQ3Success);
				
				if (qd3dStatus != kQ3Success)
					Q3Memory_Free(&basePtr);
				}
			break;
		}

	
	
	if(basePtr == NULL)
		return (kQ3Failure);
	
	validSize = Q3Size_Pad(validSize);
	
	Q3_ASSERT(validSize>=expectedSize);
	

	qd3dStatus = Q3RawData_Write (basePtr, expectedSize, theFile);
	
#if QUESA_OS_MACINTOSH
	// If this is a Mac handle object, unlock the handle
	if(theHnd != NULL)
		HUnlock(theHnd);

#endif

	// If the data was copied, dispose of it
	if (wasCopied)
		Q3Memory_Free(&basePtr);
	
	return(qd3dStatus);

}





//=============================================================================
//      Transforms
//-----------------------------------------------------------------------------
//      e3ffw_3DMF_transform_vector_traverse : Traverse method for scale and translate transforms.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_vector_traverse(TQ3Object object,
					 TQ3Vector3D *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 12, (void*)data, NULL);
	
	return qd3dstatus;
}





//=============================================================================
//      e3ffw_3DMF_transform_vector_write :  Write method for scale and translate transforms.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_vector_write(const TQ3Vector3D *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Vector3D_Write(object,theFile);
		
	return(writeStatus);
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_transform_matrix_traverse : Transform Matrix traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_matrix_traverse(TQ3Object object,
					 TQ3Matrix4x4 *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 64, (void*)data, NULL);
	
	return qd3dstatus;
}





//=============================================================================
//      e3ffw_3DMF_transform_matrix_write : Transform Matrix write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_matrix_write(const TQ3Matrix4x4 *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Matrix4x4_Write(object,theFile);
	
	
	return(writeStatus);
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_transform_rotate_traverse : Transform Rotate traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_rotate_traverse(TQ3Object object,
					 TQ3RotateTransformData *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 8, (void*)data, NULL);
	
	return qd3dstatus;
}





//=============================================================================
//      e3ffw_3DMF_transform_rotate_write : Transform Rotate write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_rotate_write(const TQ3RotateTransformData *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Uns32_Write(object->axis,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->radians,theFile);
	
	
	return(writeStatus);
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_transform_rotatepoint_traverse : Transform RotateAboutPoint traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_rotatepoint_traverse(TQ3Object object,
					 TQ3RotateAboutPointTransformData *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 20, (void*)data, NULL);
	
	return qd3dstatus;
}





//=============================================================================
//      e3ffw_3DMF_transform_rotatepoint_write : Transform RotateAboutPoint write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_rotatepoint_write(const TQ3RotateAboutPointTransformData *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Uns32_Write(object->axis,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->radians,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write(&object->about,theFile);
	
	
	return(writeStatus);
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_transform_rotateaxis_traverse : Transform RotateAboutAxis traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_rotateaxis_traverse(TQ3Object object,
					 TQ3RotateAboutAxisTransformData *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 28, (void*)data, NULL);
	
	return qd3dstatus;
}





//=============================================================================
//      e3ffw_3DMF_transform_rotateaxis_write : Transform RotateAboutAxis write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_rotateaxis_write(const TQ3RotateAboutAxisTransformData *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Point3D_Write(&object->origin,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write(&object->orientation,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->radians,theFile);
	
	
	return(writeStatus);
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_transform_quaternion_traverse : Transform Quaternion traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_quaternion_traverse(TQ3Object object,
					 TQ3Quaternion *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 16, (void*)data, NULL);
	
	return qd3dstatus;
}




//=============================================================================
//      e3ffw_3DMF_transform_quaternion_write : Transform Quaternion write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_transform_quaternion_write(const TQ3Quaternion *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3RationalPoint4D_Write((TQ3RationalPoint4D*)object,theFile);
	
	
	return(writeStatus);
}





//=============================================================================
//      e3ffw_3DMF_set_traverse : Set traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_set_traverse(TQ3Object theSet,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(data)
	return E3Set_SubmitElements( theSet, view );
}





//=============================================================================
//      Attributes
//-----------------------------------------------------------------------------
//      e3ffw_3DMF_attributeset_traverse : AttributeSet traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attributeset_traverse(TQ3Object theSet,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(data)
	TQ3Object				subObject;
	void*					subObjectData;
	TQ3XObjectClass			attributeClass;

	TQ3ElementType theType = kQ3ElementTypeNone;
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 0, NULL, NULL);
	
	if(qd3dstatus != kQ3Success)
		return qd3dstatus;
		
	do{
		qd3dstatus = Q3Set_GetNextElementType (theSet, &theType);
		if(qd3dstatus == kQ3Success){
			switch (theType) {
			
				case kQ3ElementTypeNone:
					break;
				case kQ3ObjectTypeAttributeSurfaceUV:
				case kQ3ObjectTypeAttributeShadingUV:
					subObjectData = Q3Memory_Allocate(sizeof(TQ3Param2D));
					qd3dstatus = Q3Set_Get (theSet, theType, subObjectData);
					if(qd3dstatus == kQ3Success){
						attributeClass = Q3XObjectHierarchy_FindClassByType (theType);
					
						if(attributeClass != NULL)
							qd3dstatus = Q3XView_SubmitSubObjectData (view, attributeClass, sizeof(TQ3Param2D),
																subObjectData, E3FFW_3DMF_Default_Delete);
						}
					break;
					
				case kQ3ObjectTypeAttributeNormal:
					subObjectData = Q3Memory_Allocate(sizeof(TQ3Vector3D));
					qd3dstatus = Q3Set_Get (theSet, theType, subObjectData);
					if(qd3dstatus == kQ3Success){
						attributeClass = Q3XObjectHierarchy_FindClassByType (theType);
					
						if(attributeClass != NULL)
							qd3dstatus = Q3XView_SubmitSubObjectData (view, attributeClass, sizeof(TQ3Vector3D),
																subObjectData, E3FFW_3DMF_Default_Delete);
						}
					break;
					
				case kQ3ObjectTypeAttributeDiffuseColor:
				case kQ3ObjectTypeAttributeSpecularColor:
				case kQ3ObjectTypeAttributeTransparencyColor:
					subObjectData = Q3Memory_Allocate(sizeof(TQ3ColorRGB));
					qd3dstatus = Q3Set_Get (theSet, theType, subObjectData);
					if(qd3dstatus == kQ3Success){
						attributeClass = Q3XObjectHierarchy_FindClassByType (theType);
					
						if(attributeClass != NULL)
							qd3dstatus = Q3XView_SubmitSubObjectData (view, attributeClass, sizeof(TQ3ColorRGB),
																subObjectData, E3FFW_3DMF_Default_Delete);
						}
					break;
					
				case kQ3ObjectTypeAttributeAmbientCoefficient:
				case kQ3ObjectTypeAttributeSpecularControl:
					subObjectData = Q3Memory_Allocate(sizeof(TQ3Float32));
					qd3dstatus = Q3Set_Get (theSet, theType, subObjectData);
					if(qd3dstatus == kQ3Success){
						attributeClass = Q3XObjectHierarchy_FindClassByType (theType);
					
						if(attributeClass != NULL)
							qd3dstatus = Q3XView_SubmitSubObjectData (view, attributeClass, sizeof(TQ3Float32),
																subObjectData, E3FFW_3DMF_Default_Delete);
						}
					break;
					
				case kQ3ObjectTypeAttributeSurfaceTangent:
					subObjectData = Q3Memory_Allocate(sizeof(TQ3Tangent2D));
					qd3dstatus = Q3Set_Get (theSet, theType, subObjectData);
					if(qd3dstatus == kQ3Success){
						attributeClass = Q3XObjectHierarchy_FindClassByType (theType);
					
						if(attributeClass != NULL)
							qd3dstatus = Q3XView_SubmitSubObjectData (view, attributeClass, sizeof(TQ3Tangent2D),
																subObjectData, E3FFW_3DMF_Default_Delete);
						}
					break;
					
				case kQ3ObjectTypeAttributeHighlightState:
					subObjectData = Q3Memory_Allocate(sizeof(TQ3Switch));
					qd3dstatus = Q3Set_Get (theSet, theType, subObjectData);
					if(qd3dstatus == kQ3Success){
						attributeClass = Q3XObjectHierarchy_FindClassByType (theType);
					
						if(attributeClass != NULL)
							qd3dstatus = Q3XView_SubmitSubObjectData (view, attributeClass, sizeof(TQ3Switch),
																subObjectData, E3FFW_3DMF_Default_Delete);
						}
					break;
					
				case kQ3ObjectTypeAttributeSurfaceShader:
				//default:	// this was dangerous
					qd3dstatus = Q3Set_Get (theSet, theType, &subObject);
					if(qd3dstatus == kQ3Success)
						{
						qd3dstatus = Q3Object_Submit (subObject, view);
						Q3Object_Dispose( subObject );
						}
					break;
			}	// end switch
		
			}	// end if
		
	}while((qd3dstatus == kQ3Success) && (theType != kQ3ElementTypeNone));
	
	return qd3dstatus;
}




//=============================================================================
//      e3ffw_3DMF_attribute_surfaceuv_write : Surface UV attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_surfaceuv_write(const TQ3Param2D *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Float32_Write(object->u,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->v,theFile);
	
	
	return(writeStatus);
}





//=============================================================================
//      e3ffw_3DMF_attribute_shadinguv_write : Shading UV attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_shadinguv_write(const TQ3Param2D *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Float32_Write(object->u,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->v,theFile);
	
	
	return(writeStatus);
}





//=============================================================================
//      e3ffw_3DMF_attribute_normal_write : Normal attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_normal_write(const TQ3Vector3D *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Vector3D_Write(object,theFile);
	
	
	return(writeStatus);
}





//=============================================================================
//      e3ffw_3DMF_attribute_ambientcoefficient_write : Ambient coefficient attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_ambientcoefficient_write(const TQ3Float32 *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Float32_Write(*object,theFile);
	
	
	return(writeStatus);
}





//=============================================================================
//      e3ffw_3DMF_attribute_diffusecolor_write : Diffuse color attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_diffusecolor_write(const TQ3ColorRGB *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Float32_Write(object->r,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->g,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->b,theFile);
	
	
	return(writeStatus);
}




//=============================================================================
//      e3ffw_3DMF_attribute_specularcolor_write : Specular color attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_specularcolor_write(const TQ3ColorRGB *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Float32_Write(object->r,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->g,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->b,theFile);
	
	
	return(writeStatus);
}




//=============================================================================
//      e3ffw_3DMF_attribute_specularcontrol_write : Specular control attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_specularcontrol_write(const TQ3Float32 *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Float32_Write(*object,theFile);
	
	
	return(writeStatus);
}




//=============================================================================
//      e3ffw_3DMF_attribute_transparencycolor_write : Transparency color attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_transparencycolor_write(const TQ3ColorRGB *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Float32_Write(object->r,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->g,theFile);
	if(writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write(object->b,theFile);
	
	
	return(writeStatus);
}





//=============================================================================
//      e3ffw_3DMF_attribute_surfacetangent_write : Surface tangent attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_surfacetangent_write(const TQ3Tangent2D *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Tangent2D_Write(object,theFile);
	
	
	return(writeStatus);
}




//=============================================================================
//      e3ffw_3DMF_attribute_highlightstate_write : Highlight state attribute write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_attribute_highlightstate_write(const TQ3Uns32 *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Uns32_Write(*object,theFile);
	
	
	return(writeStatus);
}





//=============================================================================
//      Shaders
//-----------------------------------------------------------------------------
//      e3ffw_3DMF_shader_traverse : Shader traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_shader_traverse(TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(data)
	TQ3ShaderUVBoundary		uBoundary;
	TQ3ShaderUVBoundary		vBoundary;
	TQ3Uns32*				boundaries;
	TQ3Matrix3x3*			uvTransform;
	const TQ3Matrix3x3	identityMatrix = { { { 1.0f, 0.0f, 0.0f},
											{ 0.0f, 1.0f, 0.0f},
											{ 0.0f,0.0f, 1.0f} } };



	TQ3Status qd3dstatus;
	
	// Write uv Boundaries
	
	TQ3XObjectClass	theClass = Q3XObjectHierarchy_FindClassByType (kQ3ShapeTypeShader);
	
	if(theClass == NULL){
		E3ErrorManager_PostWarning(kQ3WarningTypeHasNotBeenRegistered);
		}
	else{
		qd3dstatus = Q3Shader_GetUBoundary (object, &uBoundary);
		if(qd3dstatus != kQ3Success)
			return qd3dstatus;
			
		qd3dstatus = Q3Shader_GetVBoundary (object, &vBoundary);
		if(qd3dstatus != kQ3Success)
			return qd3dstatus;
			
		if((uBoundary != kQ3ShaderUVBoundaryWrap) || (vBoundary != kQ3ShaderUVBoundaryWrap)){
		
			boundaries = (TQ3Uns32*)Q3Memory_Allocate (8);
			if(boundaries == NULL)
				return kQ3Failure;
				
			boundaries[0] = (TQ3Uns32)uBoundary;
			boundaries[1] = (TQ3Uns32)vBoundary;
			
			theClass = Q3XObjectHierarchy_FindClassByType (kQ3ObjectTypeDisplayGroupState);
	
			qd3dstatus = Q3XView_SubmitSubObjectData (view, theClass, 8, boundaries, E3FFW_3DMF_Default_Delete);
			if(qd3dstatus != kQ3Success){
				Q3Memory_Free(&boundaries);
				return qd3dstatus;
				}
			}
		}
	// Write uvTransform


	theClass = Q3XObjectHierarchy_FindClassByType (kQ3ObjectTypeShaderUVTransform);
	
	if(theClass == NULL){
		E3ErrorManager_PostWarning(kQ3WarningTypeHasNotBeenRegistered);
		}
	else{
		uvTransform = (TQ3Matrix3x3*)Q3Memory_Allocate (sizeof(TQ3Matrix3x3));
		if(uvTransform == NULL)
			return kQ3Failure;
			
		qd3dstatus = Q3Shader_GetUVTransform (object, uvTransform);
		if(qd3dstatus != kQ3Success)
			return qd3dstatus;
		if(memcmp(&identityMatrix, uvTransform, sizeof(TQ3Matrix3x3)) != 0){
			qd3dstatus = Q3XView_SubmitSubObjectData (view, theClass, sizeof(TQ3Matrix3x3), uvTransform, E3FFW_3DMF_Default_Delete);
			if(qd3dstatus != kQ3Success){
				Q3Memory_Free(&uvTransform);
				return qd3dstatus;
				}
			}
		else
			Q3Memory_Free(&uvTransform);
		}
	
	return qd3dstatus;
}




//=============================================================================
//      e3ffw_3DMF_illumination_traverse : Illumination Shader traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_illumination_traverse(TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(object, data)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 0, NULL, NULL);
	
	return qd3dstatus;
}





//=============================================================================
//      e3ffw_3DMF_shader_texture_traverse : Texture Shader traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_shader_texture_traverse(TQ3Object object,
					 TQ3TextureObject *data,
					 TQ3ViewObject view)
{	
	TQ3Status qd3dstatus = kQ3Success;
	
	if(data == NULL || *data == NULL){
		E3ErrorManager_PostWarning(kQ3WarningInvalidSubObjectForObject);
		return qd3dstatus;
		}
	
	qd3dstatus = Q3XView_SubmitWriteData (view, 0, NULL, NULL);
	if(qd3dstatus != kQ3Success)
		return qd3dstatus;
	
	qd3dstatus = e3ffw_3DMF_shader_traverse (object, NULL, view);
	if(qd3dstatus != kQ3Success)
		return qd3dstatus;
	
	qd3dstatus = Q3Object_Submit(*data, view);
	
	return qd3dstatus;
}




//=============================================================================
//      e3ffw_3DMF_shader_write : ShaderData write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_shader_write(TQ3Uns32 *object,TQ3FileObject theFile)
{
	TQ3Status						result = kQ3Success;

		result = Q3Uns32_Write(object[0],theFile);
		if(result != kQ3Success)
			result = Q3Uns32_Write(object[1],theFile);

	return(result);
}





//=============================================================================
//      e3ffw_3DMF_pixmap_traverse : PixMap traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_pixmap_traverse(TQ3Object object,
					 TQ3StoragePixmap *data,
					 TQ3ViewObject view)
{
	TQ3Status       qd3dstatus = kQ3Success;

	TQ3Size size;
	
	if(data == NULL || data->image == NULL){
		E3ErrorManager_PostWarning(kQ3WarningInvalidSubObjectForObject);
		return qd3dstatus;
		}
	
	size = 28 + (data->rowBytes * data->height);
	
	size = Q3Size_Pad(size);
	
	qd3dstatus = Q3XView_SubmitWriteData (view, size, data, NULL);
	
	
	return(qd3dstatus);
}





//=============================================================================
//      e3ffw_3DMF_pixmap_write : PixMap write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_pixmap_write(TQ3StoragePixmap *object,TQ3FileObject theFile)
{

	TQ3Status			qd3dStatus;
	TQ3Uns32			imageSize;


	// Write pixmap parameters
	qd3dStatus = Q3Uns32_Write(object->width,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->height,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->rowBytes,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->pixelSize,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	
	qd3dStatus = Q3Uns32_Write(object->pixelType,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	
	qd3dStatus = Q3Uns32_Write(object->bitOrder,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	
	qd3dStatus = Q3Uns32_Write(object->byteOrder,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	
	imageSize = object->height * object->rowBytes;
	imageSize = Q3Size_Pad(imageSize);

	qd3dStatus = e3ffw_3DMF_storage_write (object->image, imageSize, theFile);
	
	return(qd3dStatus);

}





//=============================================================================
//      e3ffw_3DMF_mipmap_traverse : MipMap traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_mipmap_traverse(TQ3Object object,
					 TQ3Mipmap *data,
					 TQ3ViewObject view)
{
	TQ3Status       qd3dstatus = kQ3Success;

	TQ3Size size;
	
	if(data == NULL || data->image == NULL){
		E3ErrorManager_PostWarning(kQ3WarningInvalidSubObjectForObject);
		return qd3dstatus;
		}
	
	if(data->useMipmapping == kQ3True){
		E3ErrorManager_PostError(kQ3ErrorUnimplemented,kQ3False);
		return kQ3Failure;
		}
	
	size = 32 + (data->mipmaps[0].rowBytes * data->mipmaps[0].height);
	
	size = Q3Size_Pad(size);
	
	qd3dstatus = Q3XView_SubmitWriteData (view, size, data, NULL);
	
	
	return (qd3dstatus);
}





//=============================================================================
//      e3ffw_3DMF_mipmap_write : MipMap write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_mipmap_write(TQ3Mipmap *object,TQ3FileObject theFile)
{

	TQ3Status			qd3dStatus;
	TQ3Uns32			imageSize;

	// Write pixmap parameters
	qd3dStatus = Q3Uns32_Write(object->useMipmapping,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->pixelType,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->bitOrder,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->byteOrder,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->mipmaps[0].width,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->mipmaps[0].height,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->mipmaps[0].rowBytes,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Write(object->mipmaps[0].offset,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	
	imageSize = object->mipmaps[0].height * object->mipmaps[0].rowBytes;
	imageSize = Q3Size_Pad(imageSize);

	qd3dStatus = e3ffw_3DMF_storage_write (object->image, imageSize, theFile);
	
	return(qd3dStatus);

}





//=============================================================================
//      Style
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//      e3ffw_3DMF_subdivision_traverse : Subdivision style traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_subdivision_traverse( TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	TQ3Status	status;
	TQ3SubdivisionStyleData		styleData;
	
	status = Q3SubdivisionStyle_GetData( object, &styleData );
	
	if (status == kQ3Success)
		{
		if (styleData.method == kQ3SubdivisionMethodConstant)
			status = Q3XView_SubmitWriteData( view, 12, data, NULL );
		else
			status = Q3XView_SubmitWriteData( view, 8, data, NULL );
		}
	
	return status;
}





//=============================================================================
//      e3ffw_3DMF_subdivision_write : Subdivision style write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_subdivision_write(const TQ3SubdivisionStyleData *objectData,
				TQ3FileObject theFile)
{
	TQ3Status	status = Q3Uns32_Write( objectData->method, theFile );
	
	if (status == kQ3Success)
		{
		if (objectData->method == kQ3SubdivisionMethodConstant)
			{
				status = Q3Uns32_Write( (TQ3Uns32) objectData->c1, theFile );
				
				if (status == kQ3Success)
					status = Q3Uns32_Write( (TQ3Uns32) objectData->c2, theFile );
			}
			else
			{
				status = Q3Float32_Write( objectData->c1, theFile );
			}
		}
	
	return status;
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_antialias_traverse : Antialias style traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_antialias_traverse( TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	TQ3Status	status;

	status = Q3XView_SubmitWriteData( view, 12, data, NULL );
	
	return status;
}





//=============================================================================
//      e3ffw_3DMF_antialias_write : Antialias style write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_antialias_write(const TQ3AntiAliasStyleData *objectData,
				TQ3FileObject theFile)
{
	TQ3Status	status = Q3Uns32_Write( objectData->state, theFile );
	
	if (status == kQ3Success)
		status = Q3Uns32_Write( objectData->mode, theFile );
	
	if (status == kQ3Success)
		status = Q3Float32_Write( objectData->quality, theFile );
	
	return status;
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_fog_traverse : Fog style traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_fog_traverse( TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	TQ3Status	status;

	status = Q3XView_SubmitWriteData( view, 36, data, NULL );
	
	return status;
}





//=============================================================================
//      e3ffw_3DMF_fog_write : Fog style write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_fog_write(const TQ3FogStyleData *objectData,
				TQ3FileObject theFile)
{
	TQ3Status	status = Q3Uns32_Write( objectData->state, theFile );
	
	if (status == kQ3Success)
		status = Q3Uns32_Write( objectData->mode, theFile );
	
	if (status == kQ3Success)
		status = Q3Float32_Write( objectData->fogStart, theFile );
	
	if (status == kQ3Success)
		status = Q3Float32_Write( objectData->fogEnd, theFile );
	
	if (status == kQ3Success)
		status = Q3Float32_Write( objectData->density, theFile );

	if (status == kQ3Success)
		status = Q3Float32_Write( objectData->color.a, theFile );

	if (status == kQ3Success)
		status = Q3Float32_Write( objectData->color.r, theFile );

	if (status == kQ3Success)
		status = Q3Float32_Write( objectData->color.g, theFile );

	if (status == kQ3Success)
		status = Q3Float32_Write( objectData->color.b, theFile );
	
	return status;
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_simplestyle_traverse : Traverse method for a 4-byte unsigned style.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_simplestyle_traverse( TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	TQ3Status	status;
	
	status = Q3XView_SubmitWriteData( view, 4, data, NULL );
	
	return status;
}





//=============================================================================
//      e3ffw_3DMF_simplestyle_write : Write method for a 4-byte unsigned style.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_simplestyle_write(const TQ3Uns32 *objectData,
				TQ3FileObject theFile)
{
	TQ3Status	status = Q3Uns32_Write( *objectData, theFile );
	
	return status;
}





//-----------------------------------------------------------------------------
//      e3ffw_3DMF_highlight_traverse : Traverse method for a Highlight style.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_highlight_traverse( TQ3Object object,
					 TQ3AttributeSet *data,
					 TQ3ViewObject view)
{
	TQ3Status	status;
	
	status = Q3XView_SubmitWriteData( view, 0, data, NULL );
	
	if ( (status == kQ3Success) && (*data != NULL) )
		status = Q3Object_Submit( *data, view );
	
	return status;
}





//=============================================================================
//      Geometry
//-----------------------------------------------------------------------------
//      e3ffw_3DMF_triangle_traverse : Triangle traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_triangle_traverse(TQ3Object object,
					 TQ3TriangleData *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Object attributeList = NULL;
	TQ3Uns32 i;
	TQ3Status qd3dstatus;
	
	qd3dstatus = Q3XView_SubmitWriteData (view, 36, (void*)data, NULL);
	
	for(i=0;i < 3;i++){
		if(data->vertices[i].attributeSet != NULL){
			attributeList = E3FFormat_3DMF_VertexAttributeSetList_New (3);
			break;
			}
		}
	
	if(attributeList){
		for(i=0;i < 3 && qd3dstatus == kQ3Success;i++){
			if(data->vertices[i].attributeSet != NULL){
				qd3dstatus = E3FFormat_3DMF_AttributeSetList_Set (attributeList, i, data->vertices[i].attributeSet);
				}
			}
		if(qd3dstatus == kQ3Success)
			qd3dstatus = Q3Object_Submit (attributeList, view);
		Q3Object_Dispose(attributeList);
		}
	
	if(data->triangleAttributeSet != NULL && qd3dstatus == kQ3Success)
		Q3Object_Submit (data->triangleAttributeSet, view);
	
	return qd3dstatus;
}





//=============================================================================
//      e3ffw_3DMF_triangle_write : Triangle write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_triangle_write(const TQ3TriangleData *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Point3D_Write(&object->vertices[0].point,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write(&object->vertices[1].point,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write(&object->vertices[2].point,theFile);
	
	
	return(writeStatus);
}

//=============================================================================
//      e3ffw_3DMF_line_traverse : Line traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_line_write : Line write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_point_traverse : Point traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_point_write : Point write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_marker_traverse : Marker traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_marker_write : Marker write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_pixmapmarker_traverse : Pixmap Marker traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_pixmapmarker_write : Pixmap Marker write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_box_traverse : Box traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_box_traverse( TQ3Object object,
					 TQ3BoxData *data,
					 TQ3ViewObject view )
{
	TQ3Status	status;
	TQ3Uns32 i;
	TQ3Object attributeList = NULL;
	
	status = Q3XView_SubmitWriteData( view, 48, (void*)data, NULL );
	
	// Optional bottom caps flag
	if ( (status == kQ3Success) && (data->faceAttributeSet != NULL) )
		{
		
		attributeList = E3FFormat_3DMF_FaceAttributeSetList_New (6);
		
		if(attributeList){
		
			for(i=0;i < 6 && status == kQ3Success;i++){
				if(data->faceAttributeSet[i] != NULL){
					status = E3FFormat_3DMF_AttributeSetList_Set (attributeList, i, data->faceAttributeSet[i]);
					}
				}
				
			if(status == kQ3Success)
				status = Q3Object_Submit (attributeList, view);
			Q3Object_Dispose(attributeList);
			}
		else
			{
			status = kQ3Failure;
			}
		
		}
	
	
	// Overall attribute set
	if ( (status == kQ3Success) && (data->boxAttributeSet != NULL) )
		status = Q3Object_Submit( data->boxAttributeSet, view );


	return status;
}



//=============================================================================
//      e3ffw_3DMF_box_write : Box write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_box_write( const TQ3BoxData *data,
				TQ3FileObject theFile )
{
	TQ3Status	writeStatus = kQ3Failure;
	
	writeStatus = Q3Vector3D_Write( &data->orientation, theFile );

	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->majorAxis, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->minorAxis, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write( &data->origin, theFile );
		
	return writeStatus;
}



//=============================================================================
//      e3ffw_3DMF_generalpolygon_traverse : General Polygon traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_generalpolygon_write : General Polygon write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_mesh_traverse : Mesh traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_mesh_write : Mesh write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_NURBcurve_traverse : NURB Curve traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_NURBcurve_write : NURB Curve write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_NURBpatch_traverse : NURB Patch traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_NURBpatch_write : NURB Patch write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polygon_traverse : Polygon traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polygon_write : Polygon write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polyline_traverse : Polyline traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polyline_write : Polyline write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_trigrid_traverse : TriGrid traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_trigrid_write : TriGrid write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_cone_traverse : Cone traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_cone_traverse( TQ3Object object,
					 TQ3ConeData *data,
					 TQ3ViewObject view )
{
	TQ3Status	status;
	TQ3XObjectClass	theClass;
	TQ3Uns32*	capData;
	
	status = Q3XView_SubmitWriteData( view, 64, (void*)data, NULL );
	
	// Optional bottom caps flag
	if ( (status == kQ3Success) && ((data->caps & kQ3EndCapMaskBottom) != 0) )
	{
		capData = (TQ3Uns32*)Q3Memory_Allocate( sizeof(TQ3Uns32) );
		Q3_REQUIRE_OR_RESULT( capData != NULL, kQ3Failure );
		*capData = data->caps;
		theClass = Q3XObjectHierarchy_FindClassByType( kQ3ObjectTypeGeometryCaps );
		Q3_REQUIRE_OR_RESULT( theClass != NULL, kQ3Failure );
		
		status = Q3XView_SubmitSubObjectData( view, theClass, 4, capData,
			E3FFW_3DMF_Default_Delete );
	}
	
	// optional face cap attribute set
	if ( (status == kQ3Success) && (data->faceAttributeSet != NULL) )
	{
		status = E3FileFormat_Method_SubmitObject( view, NULL,
			kQ3AttributeSetTypeFaceCap, data->faceAttributeSet );
	}
	
	// optional bottom cap attribute set
	if ( (status == kQ3Success) && (data->bottomAttributeSet != NULL) &&
		((data->caps & kQ3EndCapMaskBottom) != 0) )
	{
		status = E3FileFormat_Method_SubmitObject( view, NULL,
			kQ3AttributeSetTypeBottomCap, data->bottomAttributeSet );
	}
	
	// Overall attribute set
	if ( (status == kQ3Success) && (data->coneAttributeSet != NULL) )
		status = Q3Object_Submit( data->coneAttributeSet, view );


	return status;
}




//=============================================================================
//      e3ffw_3DMF_cone_write : Cone write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_cone_write( const TQ3ConeData *data,
				TQ3FileObject theFile )
{
	TQ3Status	writeStatus = kQ3Failure;
	
	writeStatus = Q3Vector3D_Write( &data->orientation, theFile );

	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->majorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->minorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write( &data->origin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMax, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMax, theFile );
	
	return writeStatus;
}




//=============================================================================
//      e3ffw_3DMF_cylinder_traverse : Cylinder traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_cylinder_traverse( TQ3Object object,
					 TQ3CylinderData *data,
					 TQ3ViewObject view )
{
	TQ3Status	status;
	TQ3XObjectClass	theClass;
	TQ3Uns32*	capData;
	
	status = Q3XView_SubmitWriteData( view, 64, (void*)data, NULL );
	
	// Optional bottom caps flag
	if ( (status == kQ3Success) && ((data->caps & kQ3EndCapMaskBottom) != 0) )
	{
		capData = (TQ3Uns32*)Q3Memory_Allocate( sizeof(TQ3Uns32) );
		Q3_REQUIRE_OR_RESULT( capData != NULL, kQ3Failure );
		*capData = data->caps;
		theClass = Q3XObjectHierarchy_FindClassByType( kQ3ObjectTypeGeometryCaps );
		Q3_REQUIRE_OR_RESULT( theClass != NULL, kQ3Failure );
		
		status = Q3XView_SubmitSubObjectData( view, theClass, 4, capData,
			E3FFW_3DMF_Default_Delete );
	}
	
	// optional face cap attribute set
	if ( (status == kQ3Success) && (data->faceAttributeSet != NULL) )
	{
		status = E3FileFormat_Method_SubmitObject( view, NULL,
			kQ3AttributeSetTypeFaceCap, data->faceAttributeSet );
	}
	
	// optional bottom cap attribute set
	if ( (status == kQ3Success) && (data->bottomAttributeSet != NULL) &&
		((data->caps & kQ3EndCapMaskBottom) != 0) )
	{
		status = E3FileFormat_Method_SubmitObject( view, NULL,
			kQ3AttributeSetTypeBottomCap, data->bottomAttributeSet );
	}
	
	// optional top cap attribute set
	if ( (status == kQ3Success) && (data->topAttributeSet != NULL) &&
		((data->caps & kQ3EndCapMaskTop) != 0) )
	{
		status = E3FileFormat_Method_SubmitObject( view, NULL,
			kQ3AttributeSetTypeTopCap, data->topAttributeSet );
	}
	
	// Overall attribute set
	if ( (status == kQ3Success) && (data->cylinderAttributeSet != NULL) )
		status = Q3Object_Submit( data->cylinderAttributeSet, view );


	return status;
}



//=============================================================================
//      e3ffw_3DMF_cylinder_write : Cylinder write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_cylinder_write( const TQ3CylinderData *data,
				TQ3FileObject theFile )
{
	TQ3Status	writeStatus = kQ3Failure;
	
	writeStatus = Q3Vector3D_Write( &data->orientation, theFile );

	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->majorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->minorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write( &data->origin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMax, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMax, theFile );
	
	return writeStatus;
}



//=============================================================================
//      e3ffw_3DMF_disk_traverse : Disk traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_disk_traverse( TQ3Object object,
					 TQ3DiskData *data,
					 TQ3ViewObject view )
{
	TQ3Status	status;
	
	status = Q3XView_SubmitWriteData( view, 52, (void*)data, NULL );


	// Overall attribute set
	if ( (status == kQ3Success) && (data->diskAttributeSet != NULL) )
		status = Q3Object_Submit( data->diskAttributeSet, view );


	return status;
}



//=============================================================================
//      e3ffw_3DMF_disk_write : Disk write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_disk_write( const TQ3DiskData *data,
				TQ3FileObject theFile )
{
	TQ3Status	writeStatus = kQ3Success;
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->majorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->minorRadius, theFile );
		
	if (writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write( &data->origin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMax, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMax, theFile );
	
	return writeStatus;
}


//=============================================================================
//      e3ffw_3DMF_ellipse_traverse : Ellipse traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_ellipse_write : Ellipse write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_ellipsoid_traverse : Ellipsoid traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_ellipsoid_traverse( TQ3Object object,
					 TQ3EllipsoidData *data,
					 TQ3ViewObject view )
{
	TQ3Status	status;
	
	status = Q3XView_SubmitWriteData( view, 64, (void*)data, NULL );
	

	// Overall attribute set
	if ( (data->ellipsoidAttributeSet != NULL) && (status == kQ3Success) )
		status = Q3Object_Submit( data->ellipsoidAttributeSet, view );


	return status;
}


//=============================================================================
//      e3ffw_3DMF_ellipsoid_write : Ellipsoid write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_ellipsoid_write( const TQ3EllipsoidData *data,
				TQ3FileObject theFile )
{
	TQ3Status	writeStatus = kQ3Failure;
	
	writeStatus = Q3Vector3D_Write( &data->orientation, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->majorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->minorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write( &data->origin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMax, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMax, theFile );
	
	return writeStatus;
}


//=============================================================================
//      e3ffw_3DMF_polyedron_traverse : Polyedron traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polyedron_write : Polyedron write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_torus_traverse : Torus traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_torus_traverse( TQ3Object object,
					 TQ3TorusData *data,
					 TQ3ViewObject view )
{
	TQ3Status	status;
	
	status = Q3XView_SubmitWriteData( view, 68, (void*)data, NULL );
	

	// Overall attribute set
	if ( (data->torusAttributeSet != NULL) && (status == kQ3Success) )
		status = Q3Object_Submit( data->torusAttributeSet, view );


	return status;
}



//=============================================================================
//      e3ffw_3DMF_torus_write : Torus write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_torus_write( const TQ3TorusData *data,
				TQ3FileObject theFile )
{
	TQ3Status	writeStatus = kQ3Failure;
	
	writeStatus = Q3Vector3D_Write( &data->orientation, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->majorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Vector3D_Write( &data->minorRadius, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write( &data->origin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->ratio, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->uMax, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMin, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Float32_Write( data->vMax, theFile );
	
	return writeStatus;
}


//=============================================================================
//      e3ffw_3DMF_num_index_bytes : Helper for TriMesh array compression.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3ffw_3DMF_num_index_bytes( TQ3Uns32 numItems )
{
	TQ3Uns32	numBytes = 4;
	
	if (numItems <= 0x0FF)	// maximum index <= 0xFE
	{
		numBytes = 1;
	}
	else if (numItems <= 0x0FFFFUL)	// maximum index <= 0xFFFE
	{
		numBytes = 2;
	}
	return numBytes;
}


//=============================================================================
//      e3ffw_3DMF_write_comp_num : Helper for TriMesh array compression.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_write_comp_num( TQ3Uns32 toWrite, TQ3Uns32 numBytes,
	TQ3FileObject file )
{
	TQ3Status	status;
	switch (numBytes)
	{
		case 1:
			status = Q3Uns8_Write( (TQ3Uns8) toWrite, file );
			break;
		
		case 2:
			status = Q3Uns16_Write( (TQ3Uns16) toWrite, file );
			break;
		
		default:
			status = Q3Uns32_Write( toWrite, file );
			break;
	}
	return status;
}

//=============================================================================
//      e3ffw_3DMF_submit_tm_attarray : Helper for e3ffw_3DMF_trimesh_traverse.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_submit_tm_attarray( TQ3ViewObject view,
								TQ3TriMeshData *data,
								TQ3Uns32 inWhichArray,
								TQ3Uns32 inWhichAttr )
{
	TQ3Status	status = kQ3Success;
	TQ3Uns32	size;
	TQ3TriMeshAttributeData*	theArray;
	TQ3TriMeshAttributeData*	theAttData;
	TQ3Uns32	arraySize;
	TQ3Uns32	attrSize;
	TQ3XObjectClass		attrArrayClass;
	TE3FFormat3DMF_AttributeArray_Data*	objectData;
	TQ3Uns32	i;
	TQ3ObjectType		attClassType;
	E3ClassInfoPtr 		theClass = NULL;
	TQ3Uns8*	attRawData;
	
	switch (inWhichArray)
	{
		case 0:
			theArray = data->triangleAttributeTypes;
			arraySize = data->numTriangles;
			break;
			
		case 1:
			theArray = data->edgeAttributeTypes;
			arraySize = data->numEdges;
			break;
		
		case 2:
			theArray = data->vertexAttributeTypes;
			arraySize = data->numPoints;
			break;
	}
	theAttData = &theArray[ inWhichAttr ];
	
	attClassType = E3Attribute_AttributeToClassType( theAttData->attributeType );
	Q3XElementType_GetElementSize( attClassType, &attrSize );
	
	
	// Compute the size of the AttributeArray object.
	size = 5 * sizeof(TQ3Uns32);	// header
	
	if ( (theAttData->attributeType > 0) &&
		(theAttData->attributeType != kQ3AttributeTypeSurfaceShader) )
	{
		size += arraySize * attrSize;
	}
	else if ( (theAttData->attributeType < 0) &&
		(theAttData->attributeUseArray != NULL) )
	{
		size += arraySize;
	}
	
	attrArrayClass = Q3XObjectHierarchy_FindClassByType( kQ3ObjectTypeAttributeArray );
	Q3_REQUIRE_OR_RESULT( attrArrayClass != NULL, kQ3Failure );
	
	objectData = (TE3FFormat3DMF_AttributeArray_Data*)
		Q3Memory_Allocate( sizeof(TE3FFormat3DMF_AttributeArray_Data) );
	Q3_REQUIRE_OR_RESULT( objectData != NULL, kQ3Failure );

	objectData->attributeData = theAttData;
	objectData->whichArray = inWhichArray;
	objectData->whichAttr = inWhichAttr;
	objectData->arraySize = arraySize;
	objectData->attributeSize = attrSize;
	
	status = Q3XView_SubmitSubObjectData( view, attrArrayClass, size, objectData,
		E3FFW_3DMF_Default_Delete );
	
	// Custom attributes and surface shaders must be submitted as separate objects
	// 
	// This part of the code is untested.
	if ( (theAttData->attributeType < 0) ||
		(theAttData->attributeType == kQ3AttributeTypeSurfaceShader) )
	{
		theClass = E3ClassTree_GetClassByType(attClassType);
		Q3_REQUIRE_OR_RESULT( theClass != NULL, kQ3Failure );
		attRawData = (TQ3Uns8*) theAttData->data;
		
		for (i = 0; (status == kQ3Success) && (i < arraySize); ++i)
		{
			if ( (theAttData->attributeUseArray == NULL) ||
				(theAttData->attributeUseArray[i] != 0) )
			{
				status = Q3XView_SubmitSubObjectData( view, (TQ3XObjectClass)theClass,
					attrSize, attRawData, NULL );
				
				attRawData += attrSize;
			}
		}
	}
	
	return status;
}

//=============================================================================
//      e3ffw_3DMF_trimesh_traverse : TriMesh traverse method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_trimesh_traverse(TQ3Object object,
					 TQ3TriMeshData *data,
					 TQ3ViewObject view)
{
	TQ3Status qd3dstatus;
	TQ3AttributeType	attrType = kQ3AttributeTypeNone;
	TQ3Uns32	size, pointIndexBytes, triIndexBytes, i;
	
	// Compute size of data
	size = 6 * sizeof(TQ3Uns32);
		// array of triangles
	pointIndexBytes = e3ffw_3DMF_num_index_bytes( data->numPoints );
	size += data->numTriangles * pointIndexBytes * 3;
		// array of edges
	triIndexBytes = e3ffw_3DMF_num_index_bytes( data->numTriangles );
	size += data->numEdges * 2 * (pointIndexBytes + triIndexBytes);
		// array of points
	size += data->numPoints * sizeof(TQ3Point3D);
		// bounding box
	size += Q3Size_Pad( sizeof(TQ3BoundingBox) );
	
	qd3dstatus = Q3XView_SubmitWriteData (view, size, (void*)data, NULL);
	
	// Attribute array subobjects
	
	// Triangle attributes
	for (i = 0; (qd3dstatus == kQ3Success) &&
		(i < data->numTriangleAttributeTypes); ++i)
	{
		qd3dstatus = e3ffw_3DMF_submit_tm_attarray( view, data, 0, i );
	}
	
	// Edge attributes
	for (i = 0; (qd3dstatus == kQ3Success) &&
		(i < data->numEdgeAttributeTypes); ++i)
	{
		qd3dstatus = e3ffw_3DMF_submit_tm_attarray( view, data, 1, i );
	}
	
	// Vertex attributes
	for (i = 0; (qd3dstatus == kQ3Success) &&
		(i < data->numVertexAttributeTypes); ++i)
	{
		qd3dstatus = e3ffw_3DMF_submit_tm_attarray( view, data, 2, i );
	}
	
	// Overall attribute set (don't write it unless it's nonempty)
	if ( (data->triMeshAttributeSet != NULL) && (qd3dstatus == kQ3Success) &&
		(kQ3Success == Q3AttributeSet_GetNextAttributeType( data->triMeshAttributeSet,
			&attrType )) &&
		(attrType != kQ3AttributeTypeNone) )
	{
		qd3dstatus = Q3Object_Submit (data->triMeshAttributeSet, view);
	}

	
	return qd3dstatus;
}


//=============================================================================
//      e3ffw_3DMF_trimesh_write : TriMesh write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3DMF_trimesh_write(const TQ3TriMeshData *object,
				TQ3FileObject theFile)
{
	TQ3Status writeStatus;
	TQ3Uns32	i, pointIndexBytes, triIndexBytes;
	
	writeStatus = Q3Uns32_Write( object->numTriangles, theFile );
	
	if (writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write( object->numTriangleAttributeTypes, theFile );

	if (writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write( object->numEdges, theFile );

	if (writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write( object->numEdgeAttributeTypes, theFile );

	if (writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write( object->numPoints, theFile );

	if (writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write( object->numVertexAttributeTypes, theFile );
	
	// Array of triangles
	pointIndexBytes = e3ffw_3DMF_num_index_bytes( object->numPoints );
	for (i = 0; (i < object->numTriangles) && (writeStatus == kQ3Success); ++i)
	{
		writeStatus = e3ffw_3DMF_write_comp_num( object->triangles[i].pointIndices[0],
			pointIndexBytes, theFile );
		if (writeStatus == kQ3Failure)
			break;
		writeStatus = e3ffw_3DMF_write_comp_num( object->triangles[i].pointIndices[1],
			pointIndexBytes, theFile );
		if (writeStatus == kQ3Failure)
			break;
		writeStatus = e3ffw_3DMF_write_comp_num( object->triangles[i].pointIndices[2],
			pointIndexBytes, theFile );
	}
	
	// Array of edges
	triIndexBytes = e3ffw_3DMF_num_index_bytes( object->numTriangles );
	for (i = 0; (i < object->numEdges) && (writeStatus == kQ3Success); ++i)
	{
		writeStatus = e3ffw_3DMF_write_comp_num( object->edges[i].pointIndices[0],
			pointIndexBytes, theFile );
		if (writeStatus == kQ3Failure)
			break;
		writeStatus = e3ffw_3DMF_write_comp_num( object->edges[i].pointIndices[1],
			pointIndexBytes, theFile );
		if (writeStatus == kQ3Failure)
			break;
		
		writeStatus = e3ffw_3DMF_write_comp_num( object->edges[i].triangleIndices[0],
			triIndexBytes, theFile );
		if (writeStatus == kQ3Failure)
			break;
		writeStatus = e3ffw_3DMF_write_comp_num( object->edges[i].triangleIndices[1],
			triIndexBytes, theFile );
	}
	
	// Array of points
	for (i = 0; (i < object->numPoints) && (writeStatus == kQ3Success); ++i)
	{
		writeStatus = Q3Point3D_Write( &object->points[i], theFile );
	}
	
	// Bounding box
	if (writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write( &object->bBox.min, theFile );
	if (writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write( &object->bBox.max, theFile );
	if (writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write( object->bBox.isEmpty, theFile );

	return writeStatus;
}


//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFW_3DMF_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Triangle(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriangleData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeTriangle, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Line : Line handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Line(TQ3ViewObject			theView,
				TE3FFormatW3DMF_Data	*instanceData,
				TQ3GeometryObject		theGeom,
				TQ3LineData				*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeLine, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Point : Point handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Point(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PointData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePoint, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Marker : Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Marker(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3MarkerData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeMarker, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_PixmapMarker : Pixmap Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_PixmapMarker(TQ3ViewObject			theView,
						TE3FFormatW3DMF_Data	*instanceData,
						TQ3GeometryObject		theGeom,
						TQ3PixmapMarkerData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePixmapMarker, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Box : Box handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Box(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3BoxData				*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeBox, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_GeneralPolygon : General Polygon handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_GeneralPolygon(TQ3ViewObject				theView,
							TE3FFormatW3DMF_Data	*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3GeneralPolygonData	*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeGeneralPolygon, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Mesh : Mesh handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Mesh(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					void					*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeMesh, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_NURBCurve : NURB Curve handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_NURBCurve(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3NURBCurveData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeNURBCurve, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_NURBPatch : NURB Patch handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_NURBPatch(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3NURBPatchData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeNURBPatch, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Polygon : Polygon handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Polygon(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolygonData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePolygon, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_PolyLine : PolyLine handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_PolyLine(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyLineData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePolyLine, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_TriGrid : TridGrid handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_TriGrid(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriGridData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeTriGrid, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Cone : Cone handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Cone(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3ConeData				*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeCone, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Cylinder : Cylinder handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Cylinder(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3CylinderData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeCylinder, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Disk : Disk handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Disk(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3DiskData				*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeDisk, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Ellipse : Ellipse handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Ellipse(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3EllipseData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeEllipse, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Ellipsoid : Ellipsoid handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Ellipsoid(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3EllipsoidData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeEllipsoid, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Polyhedron : Polyhedron handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Polyhedron(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyhedronData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePolyhedron, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Torus : Torus handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Torus(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TorusData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeTorus, (void*)geomData);
}


//=============================================================================
//      E3FFW_3DMF_TriMesh : TriMesh handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_TriMesh(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriMeshData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeTriMesh, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_RegisterGeom : Register the geometry traverse & write methods.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_RegisterGeom(void)
{
	// the Transform write Methods

	E3ClassTree_AddMethodByType(kQ3TransformTypeMatrix,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_transform_matrix_traverse);
	E3ClassTree_AddMethodByType(kQ3TransformTypeMatrix,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_transform_matrix_write);
	E3ClassTree_AddMethodByType(kQ3TransformTypeScale,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_transform_vector_traverse);
	E3ClassTree_AddMethodByType(kQ3TransformTypeScale,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_transform_vector_write);
	E3ClassTree_AddMethodByType(kQ3TransformTypeTranslate,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_transform_vector_traverse);
	E3ClassTree_AddMethodByType(kQ3TransformTypeTranslate,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_transform_vector_write);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotate,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_transform_rotate_traverse);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotate,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_transform_rotate_write);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotateAboutPoint,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_transform_rotatepoint_traverse);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotateAboutPoint,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_transform_rotatepoint_write);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotateAboutAxis,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_transform_rotateaxis_traverse);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotateAboutAxis,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_transform_rotateaxis_write);
	E3ClassTree_AddMethodByType(kQ3TransformTypeQuaternion,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_transform_quaternion_traverse);
	E3ClassTree_AddMethodByType(kQ3TransformTypeQuaternion,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_transform_quaternion_write);
	E3ClassTree_AddMethodByType(kQ3TransformTypeReset,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Void_Traverse);

	// the Attribute write Methods

	E3ClassTree_AddMethodByType(kQ3SharedTypeSet,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_set_traverse);
	E3ClassTree_AddMethodByType(kQ3SetTypeAttribute,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_attributeset_traverse);


	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeSurfaceUV,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_surfaceuv_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeShadingUV,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_shadinguv_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeNormal,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_normal_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeAmbientCoefficient,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_ambientcoefficient_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeDiffuseColor,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_diffusecolor_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeSpecularColor,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_specularcolor_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeSpecularControl,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_specularcontrol_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeTransparencyColor,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_transparencycolor_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeSurfaceTangent,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_surfacetangent_write);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeHighlightState,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_attribute_highlightstate_write);

	E3ClassTree_AddMethodByType(kQ3TextureTypePixmap,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_pixmap_traverse);
	E3ClassTree_AddMethodByType(kQ3TextureTypePixmap,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_pixmap_write);
	E3ClassTree_AddMethodByType(kQ3TextureTypeMipmap,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_mipmap_traverse);
	E3ClassTree_AddMethodByType(kQ3TextureTypeMipmap,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_mipmap_write);
	//E3ClassTree_AddMethodByType(kQ3TextureTypeCompressedPixmap,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)void);
	E3ClassTree_AddMethodByType(kQ3ShapeTypeShader,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_shader_write);
	E3ClassTree_AddMethodByType(kQ3IlluminationTypeNULL,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_illumination_traverse);
	E3ClassTree_AddMethodByType(kQ3IlluminationTypePhong,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_illumination_traverse);
	E3ClassTree_AddMethodByType(kQ3IlluminationTypeLambert,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_illumination_traverse);
	E3ClassTree_AddMethodByType(kQ3SurfaceShaderTypeTexture,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_shader_texture_traverse);

	// Style
	E3ClassTree_AddMethodByType(kQ3StyleTypeBackfacing,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypeBackfacing,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypeSubdivision,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_subdivision_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypeSubdivision,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_subdivision_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypeOrientation,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypeOrientation,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypeInterpolation,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypeInterpolation,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypeFill,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypeFill,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypePickID,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypePickID,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypeReceiveShadows,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypeReceiveShadows,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypeHighlight,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_highlight_traverse);
	// Highlight doesn't need a write method since it has 0 bytes of data.
	E3ClassTree_AddMethodByType(kQ3StyleTypePickParts,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypePickParts,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_simplestyle_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypeAntiAlias,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_antialias_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypeAntiAlias,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_antialias_write);
	E3ClassTree_AddMethodByType(kQ3StyleTypeFog,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_fog_traverse);
	E3ClassTree_AddMethodByType(kQ3StyleTypeFog,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_fog_write);


	// Geometries
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriangle,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_triangle_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriangle,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_triangle_write);
	
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriMesh,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_trimesh_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriMesh,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_trimesh_write);

	E3ClassTree_AddMethodByType(kQ3GeometryTypeEllipsoid,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_ellipsoid_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeEllipsoid,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_ellipsoid_write);

	E3ClassTree_AddMethodByType(kQ3GeometryTypeCone,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_cone_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeCone,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_cone_write);
	
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTorus,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_torus_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTorus,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_torus_write);
	
	E3ClassTree_AddMethodByType(kQ3GeometryTypeCylinder,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_cylinder_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeCylinder,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_cylinder_write);
	
	E3ClassTree_AddMethodByType(kQ3GeometryTypeDisk,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_disk_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeDisk,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_disk_write);
	
	E3ClassTree_AddMethodByType(kQ3GeometryTypeBox,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_box_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeBox,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_box_write);
	
	return kQ3Success;
}




//=============================================================================
//      E3FFW_3DMF_UnregisterGeom : Unregister the geometry traverse & write methods
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_UnregisterGeom(void)
{
#if 0
	// Triangle
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypeTriangle,     kQ3XMethodTypeObjectTraverse);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypeTriangle,      kQ3XMethodTypeObjectWrite);

#endif
	return kQ3Success;
}


