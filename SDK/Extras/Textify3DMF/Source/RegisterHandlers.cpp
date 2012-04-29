/*
 *  RegisterHandlers.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 3/28/12.
 *  Copyright (c) 2012 James W. Walker.
 *
 *  This software is provided 'as-is', without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from the
 *  use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source distribution.
 *
 */

#include "RegisterHandlers.h"

#include "Controller.h"

#include "Caps.h"
#include "Line.h"
#include "Ellipsoid.h"
#include "Torus.h"
#include "Cone.h"
#include "TriMesh.h"
#include "AttributeArray.h"
#include "Point.h"
#include "PolyLine.h"
#include "Triangle.h"
#include "Polygon.h"
#include "GeneralPolygon.h"
#include "GeneralPolygonHint.h"
#include "Box.h"
#include "Marker.h"
#include "PixmapMarker.h"
#include "Ellipse.h"
#include "Cylinder.h"
#include "Disk.h"
#include "TriGrid.h"
#include "Polyhedron.h"
#include "Mesh.h"
#include "NURBCurve.h"
#include "NURBPatch.h"

#include "DiffuseColor.h"
#include "SpecularColor.h"
#include "EmissiveColor.h"
#include "TransparencyColor.h"
#include "SpecularControl.h"
#include "HighlightState.h"
#include "ShadingUV.h"
#include "SurfaceUV.h"
#include "AmbientCoefficient.h"
#include "Normal.h"
#include "SurfaceTangent.h"

#include "TextureShader.h"
#include "PixmapTexture.h"
#include "PhongIllumination.h"
#include "LambertIllumination.h"
#include "NullIllumination.h"

#include "AttributeSet.h"
#include "GenericRenderer.h"
#include "BottomCapAttributeSet.h"
#include "TopCapAttributeSet.h"
#include "FaceCapAttributeSet.h"

#include "ViewHints.h"
#include "ViewAngleAspectCamera.h"
#include "CameraPlacement.h"
#include "CameraRange.h"
#include "CameraViewPort.h"
#include "OrthographicCamera.h"
#include "ViewPlaneCamera.h"

#include "AmbientLight.h"
#include "LightData.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "InterpolationStyle.h"
#include "SubdivisionStyle.h"
#include "HighlightStyle.h"
#include "FillStyle.h"
#include "BackfacingStyle.h"
#include "OrientationStyle.h"
#include "AntialiasStyle.h"
#include "FogStyle.h"
#include "ReceiveShadowsStyle.h"
#include "CastShadowsStyle.h"
#include "PickIDStyle.h"
#include "LineWidthStyle.h"
#include "PickPartsStyle.h"

#include "Matrix.h"
#include "Rotate.h"
#include "ShaderUVTransform.h"
#include "Translate.h"
#include "Scale.h"
#include "RotateAboutPoint.h"
#include "RotateAboutAxis.h"
#include "Quaternion.h"
#include "Reset.h"
#include "Rasterize.h"

#include "Group.h"
#include "DisplayGroup.h"
#include "EndGroup.h"
#include "BeginGroup.h"
#include "DisplayGroupState.h"
#include "OrderedDisplayGroup.h"
#include "LightGroup.h"
#include "InfoGroup.h"
#include "IOProxyDisplayGroup.h"

#include "CString.h"
#include "NameElement.h"
#include "TableOfContents.h"
#include "Reference.h"

#include "VertexAttributeSetList.h"


#define	Register( className ) \
	inController->RegisterHandler( new className )

void	RegisterHandlers( Controller* inController )
{
	// Attributes
	Register( DiffuseColor );
	Register( SpecularColor );
	Register( EmissiveColor );
	Register( TransparencyColor );
	Register( SpecularControl );
	Register( HighlightState );
	Register( ShadingUV );
	Register( SurfaceUV );
	Register( AmbientCoefficient );
	Register( Normal );
	Register( SurfaceTangent );

	// Styles
	Register( InterpolationStyle );
	Register( SubdivisionStyle );
	Register( HighlightStyle );
	Register( FillStyle );
	Register( BackfacingStyle );
	Register( OrientationStyle );
	Register( AntialiasStyle );
	Register( FogStyle );
	Register( ReceiveShadowsStyle );
	Register( CastShadowsStyle );
	Register( PickIDStyle );
	Register( PickPartsStyle );
	Register( LineWidthStyle );

	// Transforms
	Register( Matrix );
	Register( Rotate );
	Register( ShaderUVTransform );
	Register( Translate );
	Register( Scale );
	Register( RotateAboutPoint );
	Register( RotateAboutAxis );
	Register( Quaternion );
	Register( Reset );
	Register( Rasterize );
	
	// Geometries
	Register( Line );
	Register( Caps );
	Register( Ellipsoid );
	Register( Torus );
	Register( Cone );
	Register( TriMesh );
	Register( AttributeArray );
	Register( Point );
	Register( PolyLine );
	Register( Triangle );
	Register( Polygon );
	Register( GeneralPolygon );
	Register( GeneralPolygonHint );
	Register( Box );
	Register( Marker );
	Register( PixmapMarker );
	Register( Ellipse );
	Register( Cylinder );
	Register( Disk );
	Register( TriGrid );
	Register( Polyhedron );
	Register( Mesh );
	Register( NURBCurve );
	Register( NURBPatch );

	// Shaders
	Register( TextureShader );
	Register( PixmapTexture );
	Register( PhongIllumination );
	Register( LambertIllumination );
	Register( NullIllumination );

	// Attribute Set Lists
	Register( VertexAttributeSetList );
	Register( FaceAttributeSetList );
	Register( GeometryAttributeSetList );
	
	// Attribute Sets
	Register( AttributeSet );
	Register( BottomCapAttributeSet );
	Register( TopCapAttributeSet );
	Register( FaceCapAttributeSet );
	
	// Cameras
	Register( ViewAngleAspectCamera );
	Register( CameraPlacement );
	Register( CameraRange );
	Register( CameraViewPort );
	Register( OrthographicCamera );
	Register( ViewPlaneCamera );

	// Lights
	Register( AmbientLight );
	Register( DirectionalLight );
	Register( LightData );
	Register( PointLight );
	Register( SpotLight );

	// Groups
	Register( Group );
	Register( DisplayGroup );
	Register( BeginGroup );
	Register( EndGroup );
	Register( DisplayGroupState );
	Register( OrderedDisplayGroup );
	Register( LightGroup );
	Register( InfoGroup );
	Register( IOProxyDisplayGroup );

	Register( GenericRenderer );
	Register( ViewHints );
	Register( CString );
	Register( NameElement );
	Register( TableOfContents );
	Register( Reference );
}
