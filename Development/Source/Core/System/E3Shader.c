/*  NAME:
        E3Shader.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Shader.h"
#include "E3View.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Shader state data
typedef struct TQ3ShaderData {
	TQ3ShaderUVBoundary		uBoundary;
	TQ3ShaderUVBoundary		vBoundary;
	TQ3Matrix3x3			uvTransform;
} TQ3ShaderData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3shader_new : Shader new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shader_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3ShaderData *			instanceData	= (TQ3ShaderData *) privateData ;
#pragma unused( theObject )
#pragma unused( paramData )



	//set the uv boundary types
	instanceData->uBoundary = kQ3ShaderUVBoundaryWrap ;
	instanceData->vBoundary = kQ3ShaderUVBoundaryWrap ;
	
	//set the uv transform to the idenity transform
	Q3Matrix3x3_SetIdentity( &instanceData->uvTransform ) ;

	return(kQ3Success) ;
}





//=============================================================================
//      e3shader_duplicate : Shader duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shader_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
					TQ3Object toObject,   void       *toPrivateData)
{	
	const TQ3ShaderData *		fromInstanceData = (const TQ3ShaderData *) fromPrivateData;
	TQ3ShaderData *				toInstanceData   = (TQ3ShaderData *)       toPrivateData;
#pragma unused(fromObject)
#pragma unused(toObject)


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	//duplicate the uv boundary methods
	toInstanceData->uBoundary	= fromInstanceData->uBoundary ;
	toInstanceData->vBoundary	= fromInstanceData->vBoundary ;
	
	//duplicate the uv transfomrm matrix
	Q3Matrix3x3_Copy( &fromInstanceData->uvTransform, &toInstanceData->uvTransform ) ;
	
	return(kQ3Success) ;
}





//=============================================================================
//      e3shader_metahandler : Shader metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shader_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3shader_new;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3shader_duplicate;
			break;

		case kQ3XMethodTypeObjectIsDrawable:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3shader_illumination_submit : Illumination submit method.
//-----------------------------------------------------------------------------
//		Note :	Shaders are never submitted in immediate mode form, and so we
//				pass the shader object rather than the objectData to the view.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3shader_illumination_submit(	TQ3ViewObject theView, TQ3ObjectType objectType, 
					TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(objectData)



	// Submit the shader
	E3View_State_SetShaderIllumination(theView, (TQ3IlluminationShaderObject ) theObject);

	return(kQ3Success);
}





//=============================================================================
//      e3shader_illumination_metahandler :	Illumination shader metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shader_illumination_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3shader_illumination_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3shader_surface_submit : Surface shader submit method.
//-----------------------------------------------------------------------------
//		Note :	Shaders are never submitted in immediate mode form, and so we
//				pass the shader object rather than the objectData to the view.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3shader_surface_submit(TQ3ViewObject theView, TQ3ObjectType objectType, 
					    TQ3Object theObject,   const void *objectData)
{
#pragma unused(objectType)
#pragma unused(objectData)



	// Submit the shader
	E3View_State_SetShaderSurface(theView, (TQ3SurfaceShaderObject) theObject);
	
	return(kQ3Success);
}





//=============================================================================
//      e3shader_surface_metahandler : Surface shader metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shader_surface_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3shader_surface_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3shader_texture_new : Texture shader new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3shader_texture_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3TextureObject 			*instanceData	= (TQ3TextureObject  *) privateData;
	const TQ3TextureObject 		*textureData	= (const TQ3TextureObject *) paramData;
#pragma unused( theObject )



	// get the texture
	E3Shared_Acquire(instanceData, *textureData);
	
	return(kQ3Success) ;
}





//=============================================================================
//      e3shader_texture_duplicate : Texture shader duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shader_texture_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
							TQ3Object toObject,   void       *toPrivateData)
{	
	const TQ3TextureObject *		fromInstanceData = (const TQ3TextureObject *) fromPrivateData;
	TQ3TextureObject *				toInstanceData   = (TQ3TextureObject *)       toPrivateData;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	//duplicate the texture
	E3Shared_Acquire(toInstanceData, *fromInstanceData);
		
	return(kQ3Success) ;
}





//=============================================================================
//      e3shader_texture_delete : Texture shader delete method.
//-----------------------------------------------------------------------------
static void
e3shader_texture_delete(TQ3Object theObject, void *privateData)
{	TQ3TextureObject*	instanceData = (TQ3TextureObject *) privateData;
#pragma unused(theObject)



	//delete the storage for the texture
	Q3Object_Dispose( *instanceData ) ;
}





//=============================================================================
//      e3shader_texture_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shader_texture_copyreplace (TQ3SharedObject *source, TQ3SharedObject *dest)
{
	TQ3SharedObject	temp;
	
	temp = Q3Shared_GetReference(*source);
	if (temp == NULL) 
		return kQ3Failure;

	if (*dest)
		Q3Object_Dispose(*dest);
	
	*dest = temp;

	return kQ3Success;
}





//=============================================================================
//      e3shader_texture_metahandler :	Texture shader metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shader_texture_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3shader_texture_new;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3shader_texture_duplicate;
			break;
			
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3shader_texture_delete;
			break;
			
		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3shader_texture_copyreplace;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3ColorARGB_Set : Initialise a TQ3ColorARGB.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ColorARGB *
E3ColorARGB_Set(TQ3ColorARGB *color, float a, float r, float g, float b)
{
	Q3FastColorARGB_Set(color, a, r, g, b);
	return(color);
}





//=============================================================================
//      E3ColorARGB_Add : Add two TQ3ColorARGBs.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
E3ColorARGB_Add(const TQ3ColorARGB *c1, const TQ3ColorARGB *c2, TQ3ColorARGB *result)
{
	Q3FastColorARGB_Add(c1, c2, result);
	return(result);
}





//=============================================================================
//      E3ColorARGB_Subtract : Subtract two TQ3ColorARGBs.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
E3ColorARGB_Subtract(const TQ3ColorARGB *c1, const TQ3ColorARGB *c2, TQ3ColorARGB *result)
{
	Q3FastColorARGB_Subtract(c1, c2, result);
	return(result);
}





//=============================================================================
//      E3ColorARGB_Scale : Scale the components of a TQ3ColorARGB.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
E3ColorARGB_Scale(const TQ3ColorARGB *color, float scale, TQ3ColorARGB *result)
{
	Q3FastColorARGB_Scale(color, scale, result);
	return(result);
}





//=============================================================================
//      E3ColorARGB_Clamp : Clamp a TQ3ColorARGB.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
E3ColorARGB_Clamp(const TQ3ColorARGB *color, TQ3ColorARGB *result)
{
	Q3FastColorARGB_Clamp(color, result);
	return(result);
}





//=============================================================================
//      E3ColorARGB_Lerp : Perform a linear interpolation between two colours.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
E3ColorARGB_Lerp(const TQ3ColorARGB *first, const TQ3ColorARGB *last, float alpha, TQ3ColorARGB *result)
{
	Q3FastColorARGB_Lerp(first, last, alpha, result);
	return(result);
}





//=============================================================================
//      E3ColorARGB_Accumulate : Accumulate src into result.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
E3ColorARGB_Accumulate(const TQ3ColorARGB *src, TQ3ColorARGB *result)
{
	Q3FastColorARGB_Accumulate(src, result);
	return(result);
}





//=============================================================================
//      E3ColorRGB_Set : Initialise a TQ3ColorRGB.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
E3ColorRGB_Set(TQ3ColorRGB *color, float r, float g, float b)
{
	Q3FastColorRGB_Set(color, r, g, b);
	return(color);
}





//=============================================================================
//      E3ColorRGB_Add : Add two TQ3ColorRGBs.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
E3ColorRGB_Add(const TQ3ColorRGB *c1, const TQ3ColorRGB *c2, TQ3ColorRGB *result)
{
	Q3FastColorRGB_Add(c1, c2, result);
	return(result);
}





//=============================================================================
//      E3ColorRGB_Subtract : Subtract two TQ3ColorRGBs.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
E3ColorRGB_Subtract(const TQ3ColorRGB *c1, const TQ3ColorRGB *c2, TQ3ColorRGB *result)
{
	Q3FastColorRGB_Subtract(c1, c2, result);
	return(result);
}





//=============================================================================
//      E3ColorRGB_Scale : Scale the components of a TQ3ColorRGB.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
E3ColorRGB_Scale(const TQ3ColorRGB *color, float scale, TQ3ColorRGB *result)
{
	Q3FastColorRGB_Scale(color, scale, result);
	return(result);
}





//=============================================================================
//      E3ColorRGB_Clamp : Clamp a TQ3ColorRGB.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
E3ColorRGB_Clamp(const TQ3ColorRGB *color, TQ3ColorRGB *result)
{
	Q3FastColorRGB_Clamp(color, result);
	return(result);
}





//=============================================================================
//      E3ColorRGB_Lerp : Perform a linear interpolation between two colours.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
E3ColorRGB_Lerp(const TQ3ColorRGB *first, const TQ3ColorRGB *last, float alpha, TQ3ColorRGB *result)
{
	Q3FastColorRGB_Lerp(first, last, alpha, result);
	return(result);
}





//=============================================================================
//      E3ColorRGB_Accumulate : Accumulate src into result.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
E3ColorRGB_Accumulate(const TQ3ColorRGB *src, TQ3ColorRGB *result)
{
	Q3FastColorRGB_Accumulate(src, result);
	return(result);
}





//=============================================================================
//      E3ColorRGB_Luminance : Compute the luminance of a colour.
//-----------------------------------------------------------------------------
//		Note : We use the formula described in the QD3D docs.
//-----------------------------------------------------------------------------
float *
E3ColorRGB_Luminance(const TQ3ColorRGB *color, float *luminance)
{
	Q3FastColorRGB_Luminance(color, luminance);
	return(luminance);
}





//=============================================================================
//      E3Shader_RegisterClass : Register the shader classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Shader_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the shader base class
	qd3dStatus = E3ClassTree_RegisterClass(	kQ3SharedTypeShape,
											kQ3ShapeTypeShader,
											kQ3ClassNameShader,
											e3shader_metahandler,
											sizeof(TQ3ShaderData)) ;


	//register illumination shader bases class
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(	kQ3ShapeTypeShader,
												kQ3ShaderTypeIllumination,
												kQ3ClassNameShaderIllumination,
												e3shader_illumination_metahandler,
												0) ;


	//register illumination shaders
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(	kQ3ShaderTypeIllumination,
												kQ3IlluminationTypeNULL,
												kQ3ClassNameIlluminationNULL,
												NULL,
												0);
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(	kQ3ShaderTypeIllumination,
												kQ3IlluminationTypeLambert,
												kQ3ClassNameIlluminationLambert,
												NULL,
												0);
	
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(	kQ3ShaderTypeIllumination,
												kQ3IlluminationTypePhong,
												kQ3ClassNameIlluminationPhong,
												NULL,
												0);	


	//register surface shader base class
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass( kQ3ShapeTypeShader,
												kQ3ShaderTypeSurface,
												kQ3ClassNameShaderSurface,
												e3shader_surface_metahandler,
												0) ;


	//register surface shaders
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass( kQ3ShaderTypeSurface,
												kQ3SurfaceShaderTypeTexture,
												kQ3ClassNameSurfaceTexture,
												e3shader_texture_metahandler,
												sizeof(TQ3TextureObject)) ;


	return(qd3dStatus) ;
}





//=============================================================================
//      E3Shader_UnregisterClass : Unregister the shader classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes	
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SurfaceShaderTypeTexture, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShaderTypeSurface,        kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3IlluminationTypePhong,    kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3IlluminationTypeLambert,  kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3IlluminationTypeNULL,     kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShaderTypeIllumination,   kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeShader,          kQ3True);
		
	return(qd3dStatus);
}





//=============================================================================
//      E3Shader_GetType : Get the type of a shader.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3Shader_GetType(TQ3ShaderObject shader)
{


	// Return the type
	return(E3ClassTree_GetObjectType(shader, kQ3ShapeTypeShader));
}





//=============================================================================
//      E3Shader_Submit : Submit the shader.
//-----------------------------------------------------------------------------
//		Note :	Shaders are never submitted in immediate mode form, and so we
//				submit the object as a retained submit.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_Submit(TQ3ShaderObject shader, TQ3ViewObject view)
{	TQ3Status		qd3dStatus;



	// Submit the shader to the view
	qd3dStatus = E3View_SubmitRetained(view, shader);

	return(qd3dStatus);
}





//=============================================================================
//      E3Shader_SetUVTransform : Set the UV transform of a shader.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_SetUVTransform(TQ3ShaderObject shader, const TQ3Matrix3x3 *uvTransform)
{	TQ3ShaderData		*instanceData = (TQ3ShaderData*) E3ClassTree_FindInstanceData(shader, kQ3ShapeTypeShader);



	// Set the transform
	Q3Matrix3x3_Copy( uvTransform, &instanceData->uvTransform ) ;
	Q3Shared_Edited(shader);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Shader_GetUVTransform : Get the UV transform of a shader.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_GetUVTransform(TQ3ShaderObject shader, TQ3Matrix3x3 *uvTransform)
{	TQ3ShaderData		*instanceData = (TQ3ShaderData*) E3ClassTree_FindInstanceData(shader, kQ3ShapeTypeShader);



	// Get the transform
	Q3Matrix3x3_Copy( &instanceData->uvTransform, uvTransform ) ;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Shader_SetUBoundary :	Set the U parameterization boundary type.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_SetUBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary uBoundary)
{	TQ3ShaderData		*instanceData = (TQ3ShaderData*) E3ClassTree_FindInstanceData(shader, kQ3ShapeTypeShader);



	// Set the boundary
	instanceData->uBoundary = uBoundary ;
	Q3Shared_Edited(shader);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Shader_SetVBoundary :	Set the V parameterization boundary type.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_SetVBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary vBoundary)
{	TQ3ShaderData		*instanceData = (TQ3ShaderData*) E3ClassTree_FindInstanceData(shader, kQ3ShapeTypeShader);



	// Set the boundary
	instanceData->vBoundary	= vBoundary ;
	Q3Shared_Edited(shader);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Shader_GetUBoundary :	Get the U parametrization boundary type.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_GetUBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary *uBoundary)
{	TQ3ShaderData		*instanceData = (TQ3ShaderData*) E3ClassTree_FindInstanceData(shader, kQ3ShapeTypeShader);



	// Get the U boundary type	
	*uBoundary	= instanceData->uBoundary;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Shader_GetVBoundary :	Get the V parametrization boundary type.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_GetVBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary *vBoundary)
{	TQ3ShaderData		*instanceData = (TQ3ShaderData*) E3ClassTree_FindInstanceData(shader, kQ3ShapeTypeShader);



	// Get the V boundary type
	*vBoundary	= instanceData->vBoundary;
	
	return(kQ3Success);
}





//=============================================================================
//      E3IlluminationShader_GetType : Get the type of an illumination shader.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3IlluminationShader_GetType(TQ3ShaderObject shader)
{


	// Return the type
	return(E3ClassTree_GetObjectType(shader, kQ3ShaderTypeIllumination));
}





//=============================================================================
//      E3PhongIllumination_New : Create a new Phong shader.
//-----------------------------------------------------------------------------
TQ3ShaderObject
E3PhongIllumination_New(void)
{	TQ3Object	phongShader ;
	
	
	
	// Create the object
	phongShader = E3ClassTree_CreateInstance(kQ3IlluminationTypePhong, kQ3False, NULL) ;

	return(phongShader);
}





//=============================================================================
//      E3LambertIllumination_New :	Create a new Lambert shader.
//-----------------------------------------------------------------------------
TQ3ShaderObject
E3LambertIllumination_New(void)
{	TQ3Object	lambertShader ;
	
	
	
	// Create the object
	lambertShader = E3ClassTree_CreateInstance(kQ3IlluminationTypeLambert, kQ3False, NULL) ;

	return(lambertShader);
}





//=============================================================================
//      E3NULLIllumination_New : Create a new NULL illumination shader.
//-----------------------------------------------------------------------------
TQ3ShaderObject
E3NULLIllumination_New(void)
{	TQ3Object	nullShader ;



	// Create the object	
	nullShader = E3ClassTree_CreateInstance(kQ3IlluminationTypeNULL, kQ3False, NULL) ;

	return(nullShader);
}





//=============================================================================
//      E3SurfaceShader_GetType : Get the type of a surface shader.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3SurfaceShader_GetType(TQ3SurfaceShaderObject shader)
{


	// Return the type
	return(E3ClassTree_GetObjectType(shader, kQ3ShaderTypeSurface));
}





//=============================================================================
//      E3TextureShader_New : Create a new texture shader.
//-----------------------------------------------------------------------------
TQ3ShaderObject
E3TextureShader_New(TQ3TextureObject texture)
{	TQ3Object	textureShader ;
	
	
	
	// Create the object
	textureShader = E3ClassTree_CreateInstance(	kQ3SurfaceShaderTypeTexture, kQ3False, &texture);
	
	return(textureShader) ;
}





//=============================================================================
//      E3TextureShader_GetTexture : Get the texture shader's texture object.
//-----------------------------------------------------------------------------
TQ3Status
E3TextureShader_GetTexture(TQ3ShaderObject shader, TQ3TextureObject *texture)
{	TQ3TextureObject			*instanceData = (TQ3TextureObject *) shader->instanceData;



	// Create a new reference to our renderer
	E3Shared_Acquire(texture, *instanceData);

	return(kQ3Success);
}





//=============================================================================
//      E3TextureShader_SetTexture : Set a texture shader's texture.
//-----------------------------------------------------------------------------
TQ3Status
E3TextureShader_SetTexture(TQ3ShaderObject shader, TQ3TextureObject texture)
{	TQ3TextureObject			*instanceData = (TQ3TextureObject *) shader->instanceData;



	// Replace the existing texture reference
	E3Shared_Replace(instanceData, texture);
	Q3Shared_Edited(shader);

	return(kQ3Success);
}
