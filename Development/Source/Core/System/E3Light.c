/*  NAME:
        E3Light.c

    DESCRIPTION:
        
        The implementation of the E3Lightxxx routines _requires_ that a
        TQ3LightData field be the first field in any light specific
        instance data.
        
        This allows the generic routines to manipulate the generic fields
        in any light object, without having to have accessors for every
        light type.

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
#include "E3Light.h"
#include "E3Main.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Light : public E3Shape // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file. Also the subclasses do want
								// access to the fields as thats the way QD3D was
								// structured as for instance spot light data
								// includes TQ3LightData even though there are
								// methods for Light which also access TQ3LightData.
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeLight, E3Light, E3Shape )
public :

	TQ3LightData			lightData ;

	} ;
	


class E3AmbientLight : public E3Light // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3LightTypeAmbient, E3AmbientLight, E3Light )
public :

	// There is no extra data for an ambient light
	} ;
	


class E3DirectionalLight : public E3Light // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3LightTypeDirectional, E3DirectionalLight, E3Light )
public :

    TQ3Boolean				castsShadows ;
    TQ3Vector3D				direction ;
	} ;
	


class E3PointLight : public E3Light // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3LightTypePoint, E3PointLight, E3Light )
public :

    TQ3Boolean				castsShadows ;
    TQ3AttenuationType		attenuation ;
    TQ3Point3D				location ;
	} ;
	


class E3SpotLight : public E3Light // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3LightTypeSpot, E3SpotLight, E3Light )
public :

    TQ3Boolean				castsShadows ;
    TQ3AttenuationType		attenuation ;
    TQ3Point3D				location ;
    TQ3Vector3D				direction ;
    float					hotAngle ;
    float					outerAngle ;
    TQ3FallOffType			fallOff ;
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3light_ambient_new : Ambient light new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_ambient_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3LightData			*instanceData = (TQ3LightData *)       privateData;
	const TQ3LightData		*lightData    = (const TQ3LightData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	if ( lightData != NULL )
		*instanceData = *lightData ;

	return(kQ3Success);
}





//=============================================================================
//      e3light_ambient_metahandler : Ambient light metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3light_ambient_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3light_ambient_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3light_directional_new : Directional light new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_directional_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DirectionalLightData			*instanceData = (TQ3DirectionalLightData *)       privateData;
	const TQ3DirectionalLightData	*lightData    = (const TQ3DirectionalLightData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	if ( lightData != NULL )
		*instanceData = *lightData ;

	return(kQ3Success);
}





//=============================================================================
//      e3light_directional_metahandler : Directional light metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3light_directional_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3light_directional_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3light_point_new : Point light new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_point_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PointLightData			*instanceData = (TQ3PointLightData *)       privateData;
	const TQ3PointLightData		*lightData    = (const TQ3PointLightData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	if ( lightData != NULL )
		*instanceData = *lightData ;

	return(kQ3Success);
}





//=============================================================================
//      e3light_point_metahandler : Point light metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3light_point_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3light_point_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3light_spot_new : Spot light new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_spot_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3SpotLightData			*instanceData = (TQ3SpotLightData *)       privateData;
	const TQ3SpotLightData		*lightData    = (const TQ3SpotLightData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	if ( lightData != NULL )
		*instanceData = *lightData ;

	return(kQ3Success);
}





//=============================================================================
//      e3light_spot_metahandler : Spot light metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3light_spot_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3light_spot_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Light_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Light_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the Light classes
	qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameLight,
										NULL,
										E3Light ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameLightAmbient,
											e3light_ambient_metahandler,
											E3AmbientLight ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameLightDirectional,
											e3light_directional_metahandler,
											E3DirectionalLight ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameLightPoint,
											e3light_point_metahandler,
											E3PointLight ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameLightSpot,
											e3light_spot_metahandler,
											E3SpotLight ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3Light_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class in reverse order
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3LightTypeAmbient,		kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3LightTypeDirectional,	kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3LightTypePoint,		kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3LightTypeSpot,			kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ShapeTypeLight,		kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Light_IsOfMyClass : Check if object pointer is valid and of type light
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3ShapeTypeLight )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3Light_IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Light ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Light_GetType : Gets the light's type.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3Light_GetType(TQ3LightObject theLight)
	{
	// Return the type
	return theLight->GetObjectType ( kQ3ShapeTypeLight ) ;
	}





//=============================================================================
//      E3Light_GetState : Gets whether the light is on.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_GetState(TQ3LightObject theLight, TQ3Boolean *isOn)
	{
	// Get the field
	*isOn = ( (E3Light*) theLight )->lightData.isOn ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Light_GetBrightness : Gets the light's brightness.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_GetBrightness(TQ3LightObject theLight, float *brightness)
	{
	// Get the field
	*brightness = ( (E3Light*) theLight )->lightData.brightness ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Light_GetColor : Gets the light's color.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_GetColor(TQ3LightObject theLight, TQ3ColorRGB *color)
	{
	// Get the field
	*color = ( (E3Light*) theLight )->lightData.color ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Light_GetData : Gets the light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_GetData(TQ3LightObject theLight, TQ3LightData *lightData)
	{
	// Get the field
	*lightData = ( (E3Light*) theLight )->lightData ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Light_SetState : Sets whether the light is on.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_SetState(TQ3LightObject theLight, TQ3Boolean isOn)
	{
	// Set the field
	( (E3Light*) theLight )->lightData.isOn = isOn ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Light_SetBrightness : Sets the light's brightness.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_SetBrightness(TQ3LightObject theLight, float brightness)
	{
	// Set the field
	//
	// We do not enforce any limits on the light brightness, and renderers which do
	// not support over-saturated lights should post kQ3NoticeBrightnessGreaterThanOne
	// if they detect brightness values which are out of range.
	( (E3Light*) theLight )->lightData.brightness = brightness ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Light_SetColor : Sets the light's color.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_SetColor(TQ3LightObject theLight, const TQ3ColorRGB *color)
	{
	// Set the field
	( (E3Light*) theLight )->lightData.color = *color ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Light_SetData : Sets the light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_SetData(TQ3LightObject theLight, const TQ3LightData *lightData)
	{
	// Set the field
	( (E3Light*) theLight )->lightData = *lightData ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3AmbientLight_New : Creates a new ambient light.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3LightObject
E3AmbientLight_New(const TQ3LightData *lightData)
	{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3LightTypeAmbient, kQ3False, NULL ) ;

	E3AmbientLight_SetData ( theObject , lightData ) ;

	return theObject ;
	}





//=============================================================================
//      E3AmbientLight_GetData : Gets the ambient light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3AmbientLight_GetData(TQ3LightObject theLight, TQ3LightData *lightData)
	{
	// Get the field
	*lightData = ( (E3AmbientLight*) theLight )->lightData ;

	return kQ3Success ;
	}





//=============================================================================
//      E3AmbientLight_SetData : Sets the ambient light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3AmbientLight_SetData(TQ3LightObject theLight, const TQ3LightData *lightData)
	{
	// Set the field
	( (E3AmbientLight*) theLight )->lightData = *lightData ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3DirectionalLight_New : Creates a new directional light.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3LightObject
E3DirectionalLight_New(const TQ3DirectionalLightData *directionalLightData)
	{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3LightTypeDirectional, kQ3False, NULL ) ;
	
	E3DirectionalLight_SetData ( theObject , directionalLightData ) ;
	
	return theObject ;
	}





//=============================================================================
//      E3DirectionalLight_GetCastShadowsState
//-----------------------------------------------------------------------------
//		Gets whether the directional light casts shadows.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_GetCastShadowsState(TQ3LightObject theLight, TQ3Boolean *castsShadows)
	{
	// Get the field
	*castsShadows = ( (E3DirectionalLight*) theLight )->castsShadows ;

	return kQ3Success ;
	}





//=============================================================================
//      E3DirectionalLight_GetDirection : Gets the directional light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_GetDirection(TQ3LightObject theLight, TQ3Vector3D *direction)
	{
	// Get the field
	*direction = ( (E3DirectionalLight*) theLight )->direction;

	return kQ3Success ;
	}





//=============================================================================
//      E3DirectionalLight_GetData : Gets the directional light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_GetData(TQ3LightObject theLight, TQ3DirectionalLightData *directionalLightData)
	{
	E3DirectionalLight* instanceData = (E3DirectionalLight*) theLight ;

	// Get the field
	*directionalLightData = * ( (TQ3DirectionalLightData*) & instanceData->lightData ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3DirectionalLight_SetCastShadowsState
//-----------------------------------------------------------------------------
//		Sets whether the directional light casts shadows
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_SetCastShadowsState(TQ3LightObject theLight, TQ3Boolean castsShadows)
	{
	// Set the field
	( (E3DirectionalLight*) theLight )->castsShadows = castsShadows ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3DirectionalLight_SetDirection : Sets the directional light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_SetDirection(TQ3LightObject theLight, const TQ3Vector3D *direction)
	{
	// Set the field
	( (E3DirectionalLight*) theLight )->direction = *direction ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3DirectionalLight_SetData : Sets the directional light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_SetData(TQ3LightObject theLight, const TQ3DirectionalLightData *directionalLightData)
	{
	E3DirectionalLight* instanceData = (E3DirectionalLight*) theLight ;

	// Set the field
	* ( (TQ3DirectionalLightData*) & instanceData->lightData ) = *directionalLightData ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_New : Creates a new point light.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3LightObject
E3PointLight_New(const TQ3PointLightData *pointLightData)
	{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3LightTypePoint, kQ3False, NULL ) ;
	
	E3PointLight_SetData ( theObject , pointLightData ) ;

	return theObject ;
	}





//=============================================================================
//      E3PointLight_GetCastShadowsState
//-----------------------------------------------------------------------------
//		Gets whether the point light casts shadows.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetCastShadowsState(TQ3LightObject theLight, TQ3Boolean *castsShadows)
	{
	// Get the field
	*castsShadows = ( (E3PointLight*) theLight )->castsShadows ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_GetAttenuation : Gets the point light's attenuation.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetAttenuation(TQ3LightObject theLight, TQ3AttenuationType *attenuation)
	{
	// Get the field
	*attenuation = ( (E3PointLight*) theLight )->attenuation ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_GetLocation : Gets the point light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetLocation(TQ3LightObject theLight, TQ3Point3D *location)
	{
	// Get the field
	*location = ( (E3PointLight*) theLight )->location ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_GetData : Gets the point light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetData(TQ3LightObject theLight, TQ3PointLightData *pointLightData)
	{
	E3PointLight* instanceData = (E3PointLight*) theLight ;

	// Get the field
	*pointLightData = * ( (TQ3PointLightData*) & instanceData->lightData ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_SetCastShadowsState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetCastShadowsState(TQ3LightObject theLight, TQ3Boolean castsShadows)
	{
	// Set the field
	( (E3PointLight*) theLight )->castsShadows = castsShadows;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_SetAttenuation : Sets the point light's attenuation
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetAttenuation(TQ3LightObject theLight, TQ3AttenuationType attenuation)
	{
	// Set the field
	( (E3PointLight*) theLight )->attenuation = attenuation ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_SetLocation : Sets the point light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetLocation(TQ3LightObject theLight, const TQ3Point3D *location)
	{
	// Set the field
	( (E3PointLight*) theLight )->location = *location ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_SetData : Sets the point light's data
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetData(TQ3LightObject theLight, const TQ3PointLightData *pointLightData)
	{
	E3PointLight* instanceData = (E3PointLight*) theLight ;

	// Set the field
	* ( (TQ3PointLightData*) & instanceData->lightData ) = *pointLightData ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_New : Creates a new spot light.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3LightObject
E3SpotLight_New(const TQ3SpotLightData *spotLightData)
	{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3LightTypeSpot, kQ3False,  NULL ) ;

	E3SpotLight_SetData ( theObject , spotLightData ) ;

	return theObject ;
	}





//=============================================================================
//      E3SpotLight_GetCastShadowsState : Gets whether the spot light casts shadows.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetCastShadowsState(TQ3LightObject theLight, TQ3Boolean *castsShadows)
	{
	// Get the field
	*castsShadows = ( (E3SpotLight*) theLight )->castsShadows ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetAttenuation : Gets the spot light's attenuation.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetAttenuation(TQ3LightObject theLight, TQ3AttenuationType *attenuation)
	{
	// Get the field
	*attenuation = ( (E3SpotLight*) theLight )->attenuation ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetLocation : Gets the spot light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetLocation(TQ3LightObject theLight, TQ3Point3D *location)
	{
	// Get the field
	*location = ( (E3SpotLight*) theLight )->location ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetDirection : Gets the spot light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetDirection(TQ3LightObject theLight, TQ3Vector3D *direction)
	{
	// Get the field
	*direction = ( (E3SpotLight*) theLight )->direction ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetHotAngle : Gets the spot light's hot angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetHotAngle(TQ3LightObject theLight, float *hotAngle)
	{
	// Get the field
	*hotAngle = ( (E3SpotLight*) theLight )->hotAngle ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetOuterAngle : Gets the spot light's outer angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetOuterAngle(TQ3LightObject theLight, float *outerAngle)
	{
	// Get the field
	*outerAngle = ( (E3SpotLight*) theLight )->outerAngle ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetFallOff : Gets the spot light's fall off type.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetFallOff(TQ3LightObject theLight, TQ3FallOffType *fallOff)
	{
	// Get the field
	*fallOff = ( (E3SpotLight*) theLight )->fallOff ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetData : Gets the spot light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetData(TQ3LightObject theLight, TQ3SpotLightData *spotLightData)
	{
	E3SpotLight* instanceData = (E3SpotLight*) theLight ;

	// Get the field
	*spotLightData = * ( (TQ3SpotLightData*) & instanceData->lightData ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetCastShadowsState : Sets whether the spot light casts shadows
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetCastShadowsState(TQ3LightObject theLight, TQ3Boolean castsShadows)
	{
	// Set the field
	( (E3SpotLight*) theLight )->castsShadows = castsShadows ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetAttenuation : Sets the spot light's attenuation
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetAttenuation(TQ3LightObject theLight, TQ3AttenuationType attenuation)
	{
	// Set the field
	( (E3SpotLight*) theLight )->attenuation = attenuation ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetLocation : Sets the spot light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetLocation(TQ3LightObject theLight, const TQ3Point3D *location)
	{
	// Set the field
	( (E3SpotLight*) theLight )->location = *location ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetDirection : Sets the spot light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetDirection(TQ3LightObject theLight, const TQ3Vector3D *direction)
	{
	// Set the field
	( (E3SpotLight*) theLight )->direction = *direction ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetHotAngle : Sets the spot light's hot angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetHotAngle(TQ3LightObject theLight, float hotAngle)
	{
	// Set the field
	( (E3SpotLight*) theLight )->hotAngle = hotAngle ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetOuterAngle : Sets the spot light's outer angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetOuterAngle(TQ3LightObject theLight, float outerAngle)
	{
	// Set the field
	( (E3SpotLight*) theLight )->outerAngle = outerAngle ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetFallOff : Sets the spot light's fall off type.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetFallOff(TQ3LightObject theLight, TQ3FallOffType fallOff)
	{
	// Set the field
	( (E3SpotLight*) theLight )->fallOff = fallOff ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetData : Sets the spot light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetData(TQ3LightObject theLight, const TQ3SpotLightData *spotLightData)
	{
	E3SpotLight* instanceData = (E3SpotLight*) theLight ;

	// Set the field
	* ( (TQ3SpotLightData*) & instanceData->lightData ) = *spotLightData ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





