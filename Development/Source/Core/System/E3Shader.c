/*  NAME:
        E3Shader.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
#include "E3Shader.h"
#include "E3View.h"
#include "E3Main.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Shader state data
typedef struct TQ3ShaderData {
	TQ3ShaderUVBoundary		uBoundary;
	TQ3ShaderUVBoundary		vBoundary;
	TQ3Matrix3x3			uvTransform;
} TQ3ShaderData;





class E3Shader : public E3ShapeData // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
public :

	TQ3ShaderData		shaderData ;
	} ;
	


class E3IlluminationShader : public E3Shader // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
public :

	// There is no extra data for this class
	} ;
	


class E3NULLIllumination : public E3IlluminationShader // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	// There is no extra data for this class
	} ;
	


class E3LambertIllumination : public E3IlluminationShader // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	// There is no extra data for this class
	} ;
	


class E3PhongIllumination : public E3IlluminationShader // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	// There is no extra data for this class
	} ;
	


class E3SurfaceShader : public E3Shader // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
public :

	// There is no extra data for this class
	} ;
	


class E3TextureShader : public E3SurfaceShader // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TQ3TextureObject		texture ;
	} ;
	


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
	*toInstanceData = Q3Object_Duplicate( *fromInstanceData );
		
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
	qd3dStatus = E3ClassTree::RegisterClass(kQ3SharedTypeShape,
											kQ3ShapeTypeShader,
											kQ3ClassNameShader,
											e3shader_metahandler,
											~sizeof(E3Shader)) ;


	//register illumination shader bases class
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3ShapeTypeShader,
												kQ3ShaderTypeIllumination,
												kQ3ClassNameShaderIllumination,
												e3shader_illumination_metahandler,
												~sizeof(E3IlluminationShader)) ;


	//register illumination shaders
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3ShaderTypeIllumination,
												kQ3IlluminationTypeNULL,
												kQ3ClassNameIlluminationNULL,
												NULL,
												~sizeof(E3NULLIllumination));
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3ShaderTypeIllumination,
												kQ3IlluminationTypeLambert,
												kQ3ClassNameIlluminationLambert,
												NULL,
												~sizeof(E3LambertIllumination));
	
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3ShaderTypeIllumination,
												kQ3IlluminationTypePhong,
												kQ3ClassNameIlluminationPhong,
												NULL,
												~sizeof(E3PhongIllumination));	


	//register surface shader base class
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3ShapeTypeShader,
												kQ3ShaderTypeSurface,
												kQ3ClassNameShaderSurface,
												e3shader_surface_metahandler,
												~sizeof(E3SurfaceShader)) ;


	//register surface shaders
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass(kQ3ShaderTypeSurface,
												kQ3SurfaceShaderTypeTexture,
												kQ3ClassNameSurfaceTexture,
												e3shader_texture_metahandler,
												~sizeof(E3TextureShader)) ;


	return(qd3dStatus) ;
}





//=============================================================================
//      E3Shader_UnregisterClass : Unregister the shader classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes	
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SurfaceShaderTypeTexture, kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ShaderTypeSurface,        kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3IlluminationTypePhong,    kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3IlluminationTypeLambert,  kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3IlluminationTypeNULL,     kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ShaderTypeIllumination,   kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ShapeTypeShader,          kQ3True);
		
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
E3Shader_SetUVTransform(TQ3ShaderObject theShader, const TQ3Matrix3x3 *uvTransform)
	{
	// Set the transform
	Q3Matrix3x3_Copy ( uvTransform, & ( (E3Shader*) theShader )->shaderData.uvTransform ) ;
	Q3Shared_Edited ( theShader ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Shader_GetUVTransform : Get the UV transform of a shader.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_GetUVTransform(TQ3ShaderObject theShader, TQ3Matrix3x3 *uvTransform)
	{
	// Get the transform
	Q3Matrix3x3_Copy ( & ( (E3Shader*) theShader )->shaderData.uvTransform, uvTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Shader_SetUBoundary :	Set the U parameterization boundary type.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_SetUBoundary(TQ3ShaderObject theShader, TQ3ShaderUVBoundary uBoundary)
	{
	// Set the boundary
	( (E3Shader*) theShader )->shaderData.uBoundary = uBoundary ;
	Q3Shared_Edited ( theShader ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Shader_SetVBoundary :	Set the V parameterization boundary type.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_SetVBoundary(TQ3ShaderObject theShader, TQ3ShaderUVBoundary vBoundary)
	{
	// Set the boundary
	( (E3Shader*) theShader )->shaderData.vBoundary	= vBoundary ;
	Q3Shared_Edited ( theShader ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Shader_GetUBoundary :	Get the U parametrization boundary type.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_GetUBoundary(TQ3ShaderObject theShader, TQ3ShaderUVBoundary *uBoundary)
	{
	// Get the U boundary type	
	*uBoundary	= ( (E3Shader*) theShader )->shaderData.uBoundary ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Shader_GetVBoundary :	Get the V parametrization boundary type.
//-----------------------------------------------------------------------------
TQ3Status
E3Shader_GetVBoundary(TQ3ShaderObject theShader, TQ3ShaderUVBoundary *vBoundary)
	{
	// Get the V boundary type
	*vBoundary	= ( (E3Shader*) theShader )->shaderData.vBoundary;
	
	return kQ3Success ;
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
E3TextureShader_GetTexture(TQ3ShaderObject theShader, TQ3TextureObject *texture)
	{
	// Create a new reference to our renderer
	E3Shared_Acquire ( texture , ( (E3TextureShader*) theShader )->texture ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3TextureShader_SetTexture : Set a texture shader's texture.
//-----------------------------------------------------------------------------
TQ3Status
E3TextureShader_SetTexture(TQ3ShaderObject theShader, TQ3TextureObject texture)
	{
	// Replace the existing texture reference
	E3Shared_Replace ( & ( (E3TextureShader*) theShader )->texture, texture ) ;
	Q3Shared_Edited ( theShader ) ;

	return kQ3Success ;
	}
