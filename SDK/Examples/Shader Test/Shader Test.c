/*  NAME:
        Shaders Test.c
        
    DESCRIPTION:
        Shaders test.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include "Qut.h"
#include "QutTexture.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Menu item constants
#define kMenuItemShaderNULL									1
#define kMenuItemShaderLambert								2
#define kMenuItemShaderPhong								3
#define kMenuItemDivider1									4

#define kMenuItemPixmapTexture								5
#define kMenuItemMipmapTexture								6
#define kMenuItemCompressedTexture							7
#define kMenuItemTextureNone								8





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
TQ3GeometryObject		gSceneGeometry    	= NULL;
TQ3ShaderObject			gSceneIllumination  = NULL;
TQ3ShaderObject			gSceneTexture	  	= NULL;
TQ3ShaderObject			gPixmapShader	  	= NULL;
TQ3ShaderObject			gMipmapShader	  	= NULL;
TQ3ShaderObject			gCompressedShader 	= NULL;
TQ3Matrix4x4			gMatrixCurrent;
TQ3Matrix4x4			gMatrixRotation;




//=============================================================================
//      createGeomTriangle : Create a textured triangle object.
//-----------------------------------------------------------------------------
static TQ3GeometryObject 
createGeomTriangle(void)
{
	TQ3Point3D			vertUVs[3]		= { {0.0f, 0.0f},
											{0.0f, 1.0f},
										 	{1.0f, 1.0f} };
	TQ3Point3D			vertPoints[3]	= { {-1.5f, -1.5f, 0.0f},
											{ 0.0f,  1.5f, 0.0f},
										 	{ 1.5f, -1.5f, 0.0f} };
	TQ3ColorRGB			vertColours[3]	= { {1.0f, 0.0f, 0.0f},
											{0.0f, 1.0f, 0.0f},
											{0.0f, 0.0f, 1.0f} };
	TQ3GeometryObject	myTriangle;
	TQ3TriangleData		myTriData;
	TQ3Uns32			n;



	// Create the geometry
	myTriData.triangleAttributeSet = NULL;

	for (n = 0; n < 3; n++)
	{
		myTriData.vertices[n].point        = vertPoints[n];
		myTriData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (myTriData.vertices[n].attributeSet != NULL)
			{
			Q3AttributeSet_Add(myTriData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);

			Q3AttributeSet_Add(myTriData.vertices[n].attributeSet,
								kQ3AttributeTypeShadingUV,
								&vertUVs[n]);
			}
	}



	// create the triangle
	myTriangle = Q3Triangle_New(&myTriData);
	
	
	
	// cleanup vertex and triangle attribute sets
	for (n = 0; n < 3; n++)
	{
		if (myTriData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(myTriData.vertices[n].attributeSet);
	}

	if (myTriData.triangleAttributeSet != NULL)
		Q3Object_Dispose(myTriData.triangleAttributeSet);
	

	
	return(myTriangle);
}





//=============================================================================
//      appMenuSelect : Handle menu selections.
//-----------------------------------------------------------------------------
static void
appMenuSelect(TQ3ViewObject theView, TQ3Uns32 menuItem)
{
#pragma unused(theView)



	// Adjust our shader state
	switch (menuItem) {
		case kMenuItemShaderNULL:
			if( gSceneIllumination != NULL )
			{
				TQ3ShaderObject	newShader = NULL ;
				newShader = Q3NULLIllumination_New() ;
				
				if( newShader != NULL )
				{
					Q3Object_Dispose( gSceneIllumination ) ;
					gSceneIllumination = newShader ;
				}
			}			
			break;


		case kMenuItemShaderLambert:
			if( gSceneIllumination != NULL )
			{
				TQ3ShaderObject	newShader = NULL ;
				newShader = Q3LambertIllumination_New() ;
				
				if( newShader != NULL )
				{
					Q3Object_Dispose( gSceneIllumination ) ;
					gSceneIllumination = newShader ;
				}
			}		
			break;


		case kMenuItemShaderPhong:
			if( gSceneIllumination != NULL )
			{
				TQ3ShaderObject	newShader = NULL ;
				newShader = Q3PhongIllumination_New() ;
				
				if( newShader != NULL )
				{
					Q3Object_Dispose( gSceneIllumination ) ;
					gSceneIllumination = newShader ;
				}
			}		
			break;


		case kMenuItemPixmapTexture:
			gSceneTexture = gPixmapShader ;
			break;


		case kMenuItemMipmapTexture:
			gSceneTexture = gMipmapShader ;
			break;


		case kMenuItemCompressedTexture:
			gSceneTexture = gCompressedShader ;
			break;


		case kMenuItemTextureNone:
			gSceneTexture = NULL ;
			break;
		}
}





//=============================================================================
//      appRender : Render another frame.
//-----------------------------------------------------------------------------
static void
appRender(TQ3ViewObject theView)
{


	// submit the illumination shader if it isn't NULL
	if( gSceneIllumination != NULL )
	{
		Q3Shader_Submit(gSceneIllumination,			theView);
	}



	// submit the texture shader if it isn't NULL
	if( gSceneTexture != NULL )
	{
		Q3Shader_Submit(gSceneTexture,				theView);
	}



	// Submit the scene
	Q3MatrixTransform_Submit(&gMatrixCurrent,		theView);
	Q3Geometry_Submit(gSceneGeometry,         		theView);



	// Update the rotation matrix
	Q3Matrix4x4_Multiply(&gMatrixCurrent, &gMatrixRotation, &gMatrixCurrent);
}





//=============================================================================
//      App_Initialise : Initialise the app.
//-----------------------------------------------------------------------------
void
App_Initialise(void)
{	TQ3Status		qd3dStatus;
	FSSpec			theFSSpec;



	// Initialise Qut
	Qut_CreateWindow("Shader Test", 300, 300, kQ3True);
	Qut_CreateView(NULL);
	Qut_SetRenderFunc(appRender);



	// Select a picture file
	qd3dStatus = Qut_SelectPictureFile(&theFSSpec, sizeof(FSSpec));
	if (qd3dStatus == kQ3Success)
	{

		//create our textures (surface shaders)
		gPixmapShader	= QutTexture_CreateTextureFromFile(	&theFSSpec,
															kQ3PixelTypeRGB32,
															kQ3True );
															
		gMipmapShader	= QutTexture_CreateTextureFromFile(	&theFSSpec,
															kQ3PixelTypeRGB32,
															kQ3False ) ;
		
		gCompressedShader = QutTexture_CreateCompressedTextureFromFile( &theFSSpec,
																		kQ3PixelTypeRGB32,
																		kQ3False ) ;
														
		// set the current texture to the pixmap texture
		gSceneTexture = gPixmapShader ;
	}	



	// initialize the scene illumination shader to a NULLIlluminationShader
	gSceneIllumination = Q3NULLIllumination_New() ;



	// Create the geometry
	gSceneGeometry = createGeomTriangle();



	// Initialise the matrices
	Q3Matrix4x4_SetIdentity(&gMatrixCurrent);
	Q3Matrix4x4_SetRotate_XYZ(&gMatrixRotation, 0.0f, 0.05f, 0.0f);
	


	// Set up the menu bar
	Qut_CreateMenu(appMenuSelect);
			
	Qut_CreateMenuItem(kMenuItemLast, "NULL Shader");
	Qut_CreateMenuItem(kMenuItemLast, "Lambert Shader");
	Qut_CreateMenuItem(kMenuItemLast, "Phong Shader");
	Qut_CreateMenuItem(kMenuItemLast,  kMenuItemDivider);
	Qut_CreateMenuItem(kMenuItemLast, "Pixmap Texture");
	Qut_CreateMenuItem(kMenuItemLast, "Mipmap Texture");
	Qut_CreateMenuItem(kMenuItemLast, "No Texture");
}





//=============================================================================
//      App_Terminate : Terminate the app.
//-----------------------------------------------------------------------------
void
App_Terminate(void)
{


	// Clean up
	if (gSceneGeometry != NULL)
		Q3Object_Dispose(gSceneGeometry);
		
	if( gPixmapShader != NULL )
		Q3Object_Dispose( gPixmapShader ) ;
		
	if( gMipmapShader != NULL )
		Q3Object_Dispose( gMipmapShader ) ;
	
	if( gCompressedShader != NULL )
		Q3Object_Dispose( gCompressedShader ) ;
		
	if( gSceneIllumination != NULL )
		Q3Object_Dispose( gSceneIllumination ) ;
}
