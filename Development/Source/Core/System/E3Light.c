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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Light.h"





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
	*instanceData = *lightData;

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
	*instanceData = *lightData;

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
	*instanceData = *lightData;

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
	*instanceData = *lightData;

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


#pragma mark -

//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Light_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the Light classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
											kQ3ShapeTypeLight,
											kQ3ClassNameLight,
											NULL,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeLight,
												kQ3LightTypeAmbient,
												kQ3ClassNameLightAmbient,
												e3light_ambient_metahandler,
												sizeof(TQ3LightData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeLight,
												kQ3LightTypeDirectional,
												kQ3ClassNameLightDirectional,
												e3light_directional_metahandler,
												sizeof(TQ3DirectionalLightData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeLight,
												kQ3LightTypePoint,
												kQ3ClassNameLightPoint,
												e3light_point_metahandler,
												sizeof(TQ3PointLightData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeLight,
												kQ3LightTypeSpot,
												kQ3ClassNameLightSpot,
												e3light_spot_metahandler,
												sizeof(TQ3SpotLightData));

	return(qd3dStatus);
}





//=============================================================================
//      E3Light_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class in reverse order
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3LightTypeAmbient,		kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3LightTypeDirectional,	kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3LightTypePoint,			kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3LightTypeSpot,			kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeLight,			kQ3True);

	return(qd3dStatus);
}



#pragma mark -

//-----------------------------------------------------------------------------
//      E3Light_GetType : Gets the light's type.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Light_GetType(TQ3LightObject light)
{


	// Return the type
	return(E3ClassTree_GetObjectType(light, kQ3ShapeTypeLight));
}





//=============================================================================
//      E3Light_GetState : Gets whether the light is on.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_GetState(TQ3LightObject light, TQ3Boolean *isOn)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Get the field
	*isOn = instanceData->isOn;
	return(kQ3Success);
}





//=============================================================================
//      E3Light_GetBrightness : Gets the light's brightness.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_GetBrightness(TQ3LightObject light, float *brightness)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Get the field
	*brightness = instanceData->brightness;
	return(kQ3Success);
}





//=============================================================================
//      E3Light_GetColor : Gets the light's color.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_GetColor(TQ3LightObject light, TQ3ColorRGB *color)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Get the field
	*color = instanceData->color;
	return(kQ3Success);
}





//=============================================================================
//      E3Light_GetData : Gets the light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_GetData(TQ3LightObject light, TQ3LightData *lightData)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Get the field
	*lightData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3Light_SetState : Sets whether the light is on.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_SetState(TQ3LightObject light, TQ3Boolean isOn)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Set the field
	instanceData->isOn = isOn;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3Light_SetBrightness : Sets the light's brightness.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_SetBrightness(TQ3LightObject light, float brightness)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Set the field
	//
	// We do not enforce any limits on the light brightness, and renderers which do
	// not support over-saturated lights should post kQ3NoticeBrightnessGreaterThanOne
	// if they detect brightness values which are out of range.
	instanceData->brightness = brightness;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3Light_SetColor : Sets the light's color.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_SetColor(TQ3LightObject light, const TQ3ColorRGB *color)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Set the field
	instanceData->color = *color;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3Light_SetData : Sets the light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3Light_SetData(TQ3LightObject light, const TQ3LightData *lightData)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Set the field
	*instanceData = *lightData;
	Q3Shared_Edited(light);

	return(kQ3Success);
}


#pragma mark -



//=============================================================================
//      E3AmbientLight_New : Creates a new ambient light.
//-----------------------------------------------------------------------------
TQ3LightObject
E3AmbientLight_New(const TQ3LightData *lightData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3LightTypeAmbient, kQ3False, lightData);
	return(theObject);
}





//=============================================================================
//      E3AmbientLight_GetData : Gets the ambient light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3AmbientLight_GetData(TQ3LightObject light, TQ3LightData *lightData)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Get the field
	*lightData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3AmbientLight_SetData : Sets the ambient light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3AmbientLight_SetData(TQ3LightObject light, const TQ3LightData *lightData)
{	TQ3LightData		*instanceData = (TQ3LightData *) light->instanceData;



	// Set the field
	*instanceData = *lightData;
	Q3Shared_Edited(light);

	return(kQ3Success);
}



#pragma mark -


//=============================================================================
//      E3DirectionalLight_New : Creates a new directional light.
//-----------------------------------------------------------------------------
TQ3LightObject
E3DirectionalLight_New(const TQ3DirectionalLightData *directionalLightData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3LightTypeDirectional, kQ3False, directionalLightData);
	return(theObject);
}





//=============================================================================
//      E3DirectionalLight_GetCastShadowsState
//-----------------------------------------------------------------------------
//		Gets whether the directional light casts shadows.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows)
{	TQ3DirectionalLightData		*instanceData = (TQ3DirectionalLightData *) light->instanceData;



	// Get the field
	*castsShadows = instanceData->castsShadows;
	return(kQ3Success);
}





//=============================================================================
//      E3DirectionalLight_GetDirection : Gets the directional light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_GetDirection(TQ3LightObject light, TQ3Vector3D *direction)
{	TQ3DirectionalLightData		*instanceData = (TQ3DirectionalLightData *) light->instanceData;



	// Get the field
	*direction = instanceData->direction;
	return(kQ3Success);
}





//=============================================================================
//      E3DirectionalLight_GetData : Gets the directional light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_GetData(TQ3LightObject light, TQ3DirectionalLightData *directionalLightData)
{	TQ3DirectionalLightData		*instanceData = (TQ3DirectionalLightData *) light->instanceData;



	// Get the field
	*directionalLightData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3DirectionalLight_SetCastShadowsState
//-----------------------------------------------------------------------------
//		Sets whether the directional light casts shadows
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows)
{	TQ3DirectionalLightData		*instanceData = (TQ3DirectionalLightData *) light->instanceData;



	// Set the field
	instanceData->castsShadows = castsShadows;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3DirectionalLight_SetDirection : Sets the directional light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_SetDirection(TQ3LightObject light, const TQ3Vector3D *direction)
{	TQ3DirectionalLightData		*instanceData = (TQ3DirectionalLightData *) light->instanceData;



	// Set the field
	instanceData->direction = *direction;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3DirectionalLight_SetData : Sets the directional light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_SetData(TQ3LightObject light, const TQ3DirectionalLightData *directionalLightData)
{	TQ3DirectionalLightData		*instanceData = (TQ3DirectionalLightData *) light->instanceData;



	// Set the field
	*instanceData = *directionalLightData;
	Q3Shared_Edited(light);

	return(kQ3Success);
}




#pragma mark -

//=============================================================================
//      E3PointLight_New : Creates a new point light.
//-----------------------------------------------------------------------------
TQ3LightObject
E3PointLight_New(const TQ3PointLightData *pointLightData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3LightTypePoint, kQ3False, pointLightData);
	return(theObject);
}





//=============================================================================
//      E3PointLight_GetCastShadowsState
//-----------------------------------------------------------------------------
//		Gets whether the point light casts shadows.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows)
{	TQ3PointLightData		*instanceData = (TQ3PointLightData *) light->instanceData;



	// Get the field
	*castsShadows = instanceData->castsShadows;
	return(kQ3Success);
}





//=============================================================================
//      E3PointLight_GetAttenuation : Gets the point light's attenuation.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetAttenuation(TQ3LightObject light, TQ3AttenuationType *attenuation)
{	TQ3PointLightData		*instanceData = (TQ3PointLightData *) light->instanceData;



	// Get the field
	*attenuation = instanceData->attenuation;
	return(kQ3Success);
}





//=============================================================================
//      E3PointLight_GetLocation : Gets the point light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetLocation(TQ3LightObject light, TQ3Point3D *location)
{	TQ3PointLightData		*instanceData = (TQ3PointLightData *) light->instanceData;



	// Get the field
	*location = instanceData->location;
	return(kQ3Success);
}





//=============================================================================
//      E3PointLight_GetData : Gets the point light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetData(TQ3LightObject light, TQ3PointLightData *pointLightData)
{	TQ3PointLightData		*instanceData = (TQ3PointLightData *) light->instanceData;



	// Get the field
	*pointLightData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3PointLight_SetCastShadowsState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows)
{	TQ3PointLightData		*instanceData = (TQ3PointLightData *) light->instanceData;



	// Set the field
	instanceData->castsShadows = castsShadows;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3PointLight_SetAttenuation : Sets the point light's attenuation
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetAttenuation(TQ3LightObject light, TQ3AttenuationType attenuation)
{	TQ3PointLightData		*instanceData = (TQ3PointLightData *) light->instanceData;



	// Set the field
	instanceData->attenuation = attenuation;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3PointLight_SetLocation : Sets the point light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetLocation(TQ3LightObject light, const TQ3Point3D *location)
{	TQ3PointLightData		*instanceData = (TQ3PointLightData *) light->instanceData;



	// Set the field
	instanceData->location = *location;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3PointLight_SetData : Sets the point light's data
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetData(TQ3LightObject light, const TQ3PointLightData *pointLightData)
{	TQ3PointLightData		*instanceData = (TQ3PointLightData *) light->instanceData;



	// Set the field
	*instanceData = *pointLightData;
	Q3Shared_Edited(light);

	return(kQ3Success);
}



#pragma mark -


//=============================================================================
//      E3SpotLight_New : Creates a new spot light.
//-----------------------------------------------------------------------------
TQ3LightObject
E3SpotLight_New(const TQ3SpotLightData *spotLightData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3LightTypeSpot, kQ3False, spotLightData);
	return(theObject);
}





//=============================================================================
//      E3SpotLight_GetCastShadowsState : Gets whether the spot light casts shadows.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Get the field
	*castsShadows = instanceData->castsShadows;
	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_GetAttenuation : Gets the spot light's attenuation.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetAttenuation(TQ3LightObject light, TQ3AttenuationType *attenuation)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Get the field
	*attenuation = instanceData->attenuation;
	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_GetLocation : Gets the spot light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetLocation(TQ3LightObject light, TQ3Point3D *location)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Get the field
	*location = instanceData->location;
	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_GetDirection : Gets the spot light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetDirection(TQ3LightObject light, TQ3Vector3D *direction)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Get the field
	*direction = instanceData->direction;
	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_GetHotAngle : Gets the spot light's hot angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetHotAngle(TQ3LightObject light, float *hotAngle)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Get the field
	*hotAngle = instanceData->hotAngle;
	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_GetOuterAngle : Gets the spot light's outer angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetOuterAngle(TQ3LightObject light, float *outerAngle)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Get the field
	*outerAngle = instanceData->outerAngle;
	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_GetFallOff : Gets the spot light's fall off type.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetFallOff(TQ3LightObject light, TQ3FallOffType *fallOff)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Get the field
	*fallOff = instanceData->fallOff;
	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_GetData : Gets the spot light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetData(TQ3LightObject light, TQ3SpotLightData *spotLightData)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Get the field
	*spotLightData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_SetCastShadowsState : Sets whether the spot light casts shadows
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Set the field
	instanceData->castsShadows = castsShadows;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_SetAttenuation : Sets the spot light's attenuation
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetAttenuation(TQ3LightObject light, TQ3AttenuationType attenuation)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Set the field
	instanceData->attenuation = attenuation;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_SetLocation : Sets the spot light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetLocation(TQ3LightObject light, const TQ3Point3D *location)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Set the field
	instanceData->location = *location;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_SetDirection : Sets the spot light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetDirection(TQ3LightObject light, const TQ3Vector3D *direction)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Set the field
	instanceData->direction = *direction;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_SetHotAngle : Sets the spot light's hot angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetHotAngle(TQ3LightObject light, float hotAngle)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Set the field
	instanceData->hotAngle = hotAngle;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_SetOuterAngle : Sets the spot light's outer angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetOuterAngle(TQ3LightObject light, float outerAngle)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Set the field
	instanceData->outerAngle = outerAngle;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_SetFallOff : Sets the spot light's fall off type.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetFallOff(TQ3LightObject light, TQ3FallOffType fallOff)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Set the field
	instanceData->fallOff = fallOff;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





//=============================================================================
//      E3SpotLight_SetData : Sets the spot light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetData(TQ3LightObject light, const TQ3SpotLightData *spotLightData)
{	TQ3SpotLightData		*instanceData = (TQ3SpotLightData *) light->instanceData;



	// Set the field
	*instanceData = *spotLightData;
	Q3Shared_Edited(light);

	return(kQ3Success);
}





