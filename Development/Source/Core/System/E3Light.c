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
        Copyright (c) 1999-2009, Quesa Developers. All rights reserved.

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

#include <cstring>
using namespace std;


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


struct E3DirectionalLightData
{
    TQ3Boolean				castsShadows ;
    TQ3Vector3D				direction ;
};


class E3DirectionalLight : public E3Light // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3LightTypeDirectional, E3DirectionalLight, E3Light )
public :
	E3DirectionalLightData	instanceData;
	} ;



struct E3PointLightData
{
    TQ3Boolean				castsShadows ;
    TQ3AttenuationType		attenuation ;
    TQ3Point3D				location ;
    TQ3Float32				radius ;
};
	


class E3PointLight : public E3Light // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3LightTypePoint, E3PointLight, E3Light )
public :
	E3PointLightData	instanceData;
	} ;



struct E3SpotLightData
{
    TQ3Boolean				castsShadows ;
    TQ3AttenuationType		attenuation ;
    TQ3Point3D				location ;
    TQ3Vector3D				direction ;
    float					hotAngle ;
    float					outerAngle ;
    TQ3FallOffType			fallOff ;
};



class E3SpotLight : public E3Light // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3LightTypeSpot, E3SpotLight, E3Light )
public :
	E3SpotLightData			instanceData;
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
//      e3light_ambient_read : read an ambient light.
//-----------------------------------------------------------------------------
static TQ3Object
e3light_ambient_read ( TQ3FileObject theFile )
	{
	TQ3LightData lightData ;
	
	// Initialise the light data
	Q3Memory_Clear ( &lightData, sizeof ( lightData ) ) ;

	// Read in the attributes
	while ( Q3File_IsEndOfContainer ( theFile, NULL ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != NULL )
			{
			switch ( childObject->GetLeafType () )
				{
				case kQ3LightData :
					{
					memcpy( &lightData, childObject->FindLeafInstanceData(), sizeof ( TQ3LightData ) );
					break ;
					}
				case kQ3SharedTypeSet :
					{
					// Set must be at end so we know we've finished
					
					TQ3LightObject result = Q3AmbientLight_New ( &lightData ) ;
					result->SetSet ( childObject ) ;
					Q3Object_Dispose ( childObject ) ;
					return result ;
					}
				}
				
			Q3Object_Dispose ( childObject ) ;
			}
		}

	// Create the camera
	return Q3AmbientLight_New ( &lightData ) ;
	}


//=============================================================================
//      e3light_ambient_traverse : traverse an ambient light.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_ambient_traverse ( TQ3SharedObject theObject, void *data, TQ3ViewObject theView )
	{
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, 0 , theObject, NULL ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3LightData ), sizeof ( TQ3LightData ), theObject, NULL ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3light_ambient_metahandler : Ambient light metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3light_ambient_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectNew :
			return (TQ3XFunctionPointer) e3light_ambient_new ;
			
		case kQ3XMethodTypeObjectRead :
			return (TQ3XFunctionPointer) e3light_ambient_read ;
			
		case kQ3XMethodTypeObjectTraverse :
			return (TQ3XFunctionPointer) e3light_ambient_traverse ;
		}
	
	return NULL ;
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
//      e3light_directional_read : read a directional light.
//-----------------------------------------------------------------------------
static TQ3Object
e3light_directional_read ( TQ3FileObject theFile )
	{
	TQ3DirectionalLightData lightData ;
	
	// Initialise the light data
	Q3Memory_Clear ( &lightData, sizeof ( lightData ) ) ;

	if ( Q3Vector3D_Read ( &lightData.direction, theFile ) == kQ3Failure ) 
		lightData.direction.y = -1.0f ; // What is a sensible default?
		
	Q3Uns32_Read ( (TQ3Uns32*) &lightData.castsShadows, theFile ) ; 

	// Read in the attributes
	while ( Q3File_IsEndOfContainer ( theFile, NULL ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != NULL )
			{
			switch ( childObject->GetLeafType () )
				{
				case kQ3LightData :
					{
					memcpy( &lightData.lightData, childObject->FindLeafInstanceData(), sizeof ( TQ3LightData ) );
					break ;
					}
				case kQ3SharedTypeSet :
					{
					// Set must be at end so we know we've finished
					
					TQ3LightObject result = Q3DirectionalLight_New ( &lightData ) ;
					result->SetSet ( childObject ) ;
					Q3Object_Dispose ( childObject ) ;
					return result ;
					}
				}
				
			Q3Object_Dispose ( childObject ) ;
			}
		}

	// Create the camera
	return Q3DirectionalLight_New ( &lightData ) ;
	}


//=============================================================================
//      e3light_directional_traverse : traverse a directional light.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_directional_traverse ( TQ3SharedObject theObject, void *data, TQ3ViewObject theView )
	{
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3DirectionalLightData ) - sizeof ( TQ3LightData ), theObject, NULL ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3LightData ), sizeof ( TQ3LightData ), theObject, NULL ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3light_directional_write : write a directional light.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_directional_write ( TQ3LightObject theObject, TQ3FileObject theFile )
	{
	TQ3DirectionalLightData lightData ;
	Q3DirectionalLight_GetData ( theObject , &lightData ) ;
	
	TQ3Uns32 result = kQ3Success ;

	result &= Q3Vector3D_Write ( &lightData.direction, theFile ) ;
	result &= Q3Uns32_Write ( lightData.castsShadows, theFile ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3light_directional_metahandler : Directional light metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3light_directional_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectNew :
			return (TQ3XFunctionPointer) e3light_directional_new ;

		case kQ3XMethodTypeObjectRead :
			return (TQ3XFunctionPointer) e3light_directional_read ;

		case kQ3XMethodTypeObjectTraverse :
			return (TQ3XFunctionPointer) e3light_directional_traverse ;
			
		case kQ3XMethodTypeObjectWrite :
			return (TQ3XFunctionPointer) e3light_directional_write ;
		}
	
	return NULL ;
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
//      e3light_point_read : read a point light.
//-----------------------------------------------------------------------------
static TQ3Object
e3light_point_read ( TQ3FileObject theFile )
	{
	TQ3PointLightData lightData ;
	
	// Initialise the light data
	Q3Memory_Clear ( &lightData, sizeof ( lightData ) ) ;

	Q3Point3D_Read ( &lightData.location, theFile ) ; // If fails, 0,0,0 is a reasonable default
			
	Q3Uns32_Read ( (TQ3Uns32*) &lightData.attenuation, theFile ) ; 

	Q3Uns32_Read ( (TQ3Uns32*) &lightData.castsShadows, theFile ) ; 

	// Read in the attributes
	while ( Q3File_IsEndOfContainer ( theFile, NULL ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != NULL )
			{
			switch ( childObject->GetLeafType () )
				{
				case kQ3LightData :
					{
					memcpy( &lightData.lightData, childObject->FindLeafInstanceData(), sizeof ( TQ3LightData ) );
					break ;
					}
				case kQ3SharedTypeSet :
					{
					// Set must be at end so we know we've finished
					
					TQ3LightObject result = Q3PointLight_New ( &lightData ) ;
					result->SetSet ( childObject ) ;
					Q3Object_Dispose ( childObject ) ;
					return result ;
					}
				}
				
			Q3Object_Dispose ( childObject ) ;
			}
		}

	// Create the camera
	return Q3PointLight_New ( &lightData ) ;
	}


//=============================================================================
//      e3light_point_traverse : traverse a point light.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_point_traverse ( TQ3SharedObject theObject, void *data, TQ3ViewObject theView )
	{
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3PointLightData ) - sizeof ( TQ3LightData ), theObject, NULL ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3LightData ), sizeof ( TQ3LightData ), theObject, NULL ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3light_point_write : write a point light.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_point_write ( TQ3LightObject theObject, TQ3FileObject theFile )
	{
	TQ3PointLightData lightData ;
	Q3PointLight_GetData ( theObject , &lightData ) ;
	
	TQ3Uns32 result = kQ3Success ;

	result &= Q3Point3D_Write ( &lightData.location, theFile ) ;
	result &= Q3Uns32_Write ( lightData.castsShadows, theFile ) ;
	result &= Q3Uns32_Write ( lightData.attenuation, theFile ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3light_point_metahandler : Point light metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3light_point_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectNew :
			return (TQ3XFunctionPointer) e3light_point_new ;

		case kQ3XMethodTypeObjectRead :
			return (TQ3XFunctionPointer) e3light_point_read ;

		case kQ3XMethodTypeObjectTraverse :
			return (TQ3XFunctionPointer) e3light_point_traverse ;
			
		case kQ3XMethodTypeObjectWrite :
			return (TQ3XFunctionPointer) e3light_point_write ;
		}
	
	return NULL ;
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
//      e3light_spot_read : read a point light.
//-----------------------------------------------------------------------------
static TQ3Object
e3light_spot_read ( TQ3FileObject theFile )
	{
	TQ3SpotLightData lightData ;
	
	// Initialise the light data
	Q3Memory_Clear ( &lightData, sizeof ( lightData ) ) ;

	Q3Point3D_Read ( &lightData.location, theFile ) ; // If fails, 0,0,0 is a reasonable default
	
	if ( Q3Vector3D_Read ( &lightData.direction, theFile ) == kQ3Failure ) 
		lightData.direction.y = -1.0f ; // What is a sensible default?
		
	Q3Uns32_Read ( (TQ3Uns32*) &lightData.castsShadows, theFile ) ; 

	Q3Uns32_Read ( (TQ3Uns32*) &lightData.attenuation, theFile ) ; 

	Q3Float32_Read ( &lightData.hotAngle, theFile ) ; 

	Q3Float32_Read ( &lightData.outerAngle, theFile ) ; 

	Q3Uns32_Read ( (TQ3Uns32*) &lightData.fallOff, theFile ) ; 

	// Read in the attributes
	while ( Q3File_IsEndOfContainer ( theFile, NULL ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != NULL )
			{
			switch ( childObject->GetLeafType () )
				{
				case kQ3LightData :
					{
					memcpy( &lightData.lightData, childObject->FindLeafInstanceData(), sizeof ( TQ3LightData ) );
					break ;
					}
				case kQ3SharedTypeSet :
					{
					// Set must be at end so we know we've finished
					
					TQ3LightObject result = Q3SpotLight_New ( &lightData ) ;
					result->SetSet ( childObject ) ;
					Q3Object_Dispose ( childObject ) ;
					return result ;
					}
				}
				
			Q3Object_Dispose ( childObject ) ;
			}
		}

	// Create the camera
	return Q3SpotLight_New ( &lightData ) ;
	}




//=============================================================================
//      e3light_spot_traverse : traverse a spot light.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_spot_traverse ( TQ3SharedObject theObject, void *data, TQ3ViewObject theView )
	{
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3SpotLightData ) - sizeof ( TQ3LightData ), theObject, NULL ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3LightData ), sizeof ( TQ3LightData ), theObject, NULL ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3light_spot_write : write a spot light.
//-----------------------------------------------------------------------------
static TQ3Status
e3light_spot_write ( TQ3LightObject theObject, TQ3FileObject theFile )
	{
	TQ3SpotLightData lightData ;
	Q3SpotLight_GetData ( theObject , &lightData ) ;
	
	TQ3Uns32 result = kQ3Success ;

	result &= Q3Point3D_Write ( &lightData.location, theFile ) ;
	result &= Q3Vector3D_Write ( &lightData.direction, theFile ) ;
	result &= Q3Uns32_Write ( lightData.castsShadows, theFile ) ;
	result &= Q3Uns32_Write ( lightData.attenuation, theFile ) ;
	result &= Q3Float32_Write ( lightData.hotAngle, theFile ) ;
	result &= Q3Float32_Write ( lightData.outerAngle, theFile ) ;
	result &= Q3Uns32_Write ( lightData.fallOff, theFile ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3light_spot_metahandler : Spot light metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3light_spot_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectNew :
			return (TQ3XFunctionPointer) e3light_spot_new ;

		case kQ3XMethodTypeObjectRead :
			return (TQ3XFunctionPointer) e3light_spot_read ;

		case kQ3XMethodTypeObjectTraverse :
			return (TQ3XFunctionPointer) e3light_spot_traverse ;
			
		case kQ3XMethodTypeObjectWrite :
			return (TQ3XFunctionPointer) e3light_spot_write ;
		}
	
	return NULL ;
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
	qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameLight,
										NULL,
										E3Light,
										sizeof(TQ3LightData) ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameLightAmbient,
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
	*castsShadows = ( (E3DirectionalLight*) theLight )->instanceData.castsShadows ;

	return kQ3Success ;
	}





//=============================================================================
//      E3DirectionalLight_GetDirection : Gets the directional light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_GetDirection(TQ3LightObject theLight, TQ3Vector3D *direction)
	{
	// Get the field
	*direction = ( (E3DirectionalLight*) theLight )->instanceData.direction;

	return kQ3Success ;
	}





//=============================================================================
//      E3DirectionalLight_GetData : Gets the directional light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3DirectionalLight_GetData(TQ3LightObject theLight, TQ3DirectionalLightData *directionalLightData)
	{
	E3DirectionalLight* instanceData = (E3DirectionalLight*) theLight ;

	// Get the fields
	directionalLightData->lightData = instanceData->lightData;
	directionalLightData->castsShadows = instanceData->instanceData.castsShadows;
	directionalLightData->direction = instanceData->instanceData.direction;

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
	( (E3DirectionalLight*) theLight )->instanceData.castsShadows = castsShadows ;
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
	( (E3DirectionalLight*) theLight )->instanceData.direction = *direction ;
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

	// Set the fields
	instanceData->lightData = directionalLightData->lightData;
	instanceData->instanceData.castsShadows = directionalLightData->castsShadows;
	instanceData->instanceData.direction = directionalLightData->direction;
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
	*castsShadows = ( (E3PointLight*) theLight )->instanceData.castsShadows ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_GetAttenuation : Gets the point light's attenuation.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetAttenuation(TQ3LightObject theLight, TQ3AttenuationType *attenuation)
	{
	// Get the field
	*attenuation = ( (E3PointLight*) theLight )->instanceData.attenuation ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_GetRadius : Gets the point light's radius.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetRadius(TQ3LightObject theLight, TQ3Float32 *radius)
	{
	// Get the field
	*radius = ( (E3PointLight*) theLight )->instanceData.radius ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_GetLocation : Gets the point light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetLocation(TQ3LightObject theLight, TQ3Point3D *location)
	{
	// Get the field
	*location = ( (E3PointLight*) theLight )->instanceData.location ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_GetData : Gets the point light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_GetData(TQ3LightObject theLight, TQ3PointLightData *pointLightData)
	{
	E3PointLight* instanceData = (E3PointLight*) theLight ;

	// Get the fields
	pointLightData->lightData = instanceData->lightData;
	pointLightData->castsShadows = instanceData->instanceData.castsShadows;
	pointLightData->attenuation = instanceData->instanceData.attenuation;
	pointLightData->location = instanceData->instanceData.location;

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
	( (E3PointLight*) theLight )->instanceData.castsShadows = castsShadows;
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
	( (E3PointLight*) theLight )->instanceData.attenuation = attenuation ;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PointLight_SetRadius : Sets the point light's attenuation
//-----------------------------------------------------------------------------
TQ3Status
E3PointLight_SetRadius(TQ3LightObject theLight, TQ3Float32 radius)
	{
	// Set the field
	( (E3PointLight*) theLight )->instanceData.radius = radius ;
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
	( (E3PointLight*) theLight )->instanceData.location = *location ;
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

	// Set the fields
	instanceData->lightData = pointLightData->lightData;
	instanceData->instanceData.castsShadows = pointLightData->castsShadows;
	instanceData->instanceData.attenuation = pointLightData->attenuation;
	instanceData->instanceData.location = pointLightData->location;
	instanceData->instanceData.radius = 0.0f;
	
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
	*castsShadows = ( (E3SpotLight*) theLight )->instanceData.castsShadows ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetAttenuation : Gets the spot light's attenuation.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetAttenuation(TQ3LightObject theLight, TQ3AttenuationType *attenuation)
	{
	// Get the field
	*attenuation = ( (E3SpotLight*) theLight )->instanceData.attenuation ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetLocation : Gets the spot light's location.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetLocation(TQ3LightObject theLight, TQ3Point3D *location)
	{
	// Get the field
	*location = ( (E3SpotLight*) theLight )->instanceData.location ;

	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetDirection : Gets the spot light's direction.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetDirection(TQ3LightObject theLight, TQ3Vector3D *direction)
	{
	// Get the field
	*direction = ( (E3SpotLight*) theLight )->instanceData.direction ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetHotAngle : Gets the spot light's hot angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetHotAngle(TQ3LightObject theLight, float *hotAngle)
	{
	// Get the field
	*hotAngle = ( (E3SpotLight*) theLight )->instanceData.hotAngle ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetOuterAngle : Gets the spot light's outer angle.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetOuterAngle(TQ3LightObject theLight, float *outerAngle)
	{
	// Get the field
	*outerAngle = ( (E3SpotLight*) theLight )->instanceData.outerAngle ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetFallOff : Gets the spot light's fall off type.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetFallOff(TQ3LightObject theLight, TQ3FallOffType *fallOff)
	{
	// Get the field
	*fallOff = ( (E3SpotLight*) theLight )->instanceData.fallOff ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_GetData : Gets the spot light's data.
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_GetData(TQ3LightObject theLight, TQ3SpotLightData *spotLightData)
	{
	E3SpotLight* instanceData = (E3SpotLight*) theLight ;

	// Get the fields
	spotLightData->lightData = instanceData->lightData;
	spotLightData->castsShadows = instanceData->instanceData.castsShadows;
	spotLightData->attenuation = instanceData->instanceData.attenuation;
	spotLightData->location = instanceData->instanceData.location;
	spotLightData->direction = instanceData->instanceData.direction;
	spotLightData->hotAngle = instanceData->instanceData.hotAngle;
	spotLightData->outerAngle = instanceData->instanceData.outerAngle;
	spotLightData->fallOff = instanceData->instanceData.fallOff;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3SpotLight_SetCastShadowsState : Sets whether the spot light casts shadows
//-----------------------------------------------------------------------------
TQ3Status
E3SpotLight_SetCastShadowsState(TQ3LightObject theLight, TQ3Boolean castsShadows)
	{
	// Set the field
	( (E3SpotLight*) theLight )->instanceData.castsShadows = castsShadows ;
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
	( (E3SpotLight*) theLight )->instanceData.attenuation = attenuation ;
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
	( (E3SpotLight*) theLight )->instanceData.location = *location ;
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
	( (E3SpotLight*) theLight )->instanceData.direction = *direction ;
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
	( (E3SpotLight*) theLight )->instanceData.hotAngle = hotAngle ;
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
	( (E3SpotLight*) theLight )->instanceData.outerAngle = outerAngle ;
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
	( (E3SpotLight*) theLight )->instanceData.fallOff = fallOff ;
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

	// Set the fields
	instanceData->lightData = spotLightData->lightData;
	instanceData->instanceData.castsShadows = spotLightData->castsShadows;
	instanceData->instanceData.attenuation = spotLightData->attenuation;
	instanceData->instanceData.location = spotLightData->location;
	instanceData->instanceData.direction = spotLightData->direction;
	instanceData->instanceData.hotAngle = spotLightData->hotAngle;
	instanceData->instanceData.outerAngle = spotLightData->outerAngle;
	instanceData->instanceData.fallOff = spotLightData->fallOff;
	Q3Shared_Edited ( theLight ) ;

	return kQ3Success ;
	}





