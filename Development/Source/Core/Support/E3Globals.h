/*  NAME:
        E3Globals.h

    DESCRIPTION:
        Header file for E3Globals.c.

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
#ifndef E3GLOBALS_HDR
#define E3GLOBALS_HDR
//=============================================================================
//		Include files
//-----------------------------------------------------------------------------
#include "E3ClassTree.h"
#include "E3HashTable.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Class names
//
// Note - classes which exist in QD3D should have the same class name in Quesa,
// as determined by the ObjHierarchy application.
//
// Classes which do not exist in QD3D should take the "Quesa:type:name" format,
// to ensure they do not clash with any existing types (all class names in the
// class tree must be unique).
//
// The QD3D convention for third party class names is "Company:Type:Name".
#define kQ3ClassNameAttribute						"Attribute"
#define kQ3ClassNameAttributeAmbientCoefficient		"AmbientCoefficient"
#define kQ3ClassNameAttributeCSGID					"InteractiveCSGId"
#define kQ3ClassNameAttributeDiffuseColor			"DiffuseColor"
#define kQ3ClassNameAttributeHighlightState			"HighlightState"
#define kQ3ClassNameAttributeNormal					"Normal"
#define kQ3ClassNameAttributeShadingUV				"ShadingUV"
#define kQ3ClassNameAttributeSpecularColor			"SpecularColor"
#define kQ3ClassNameAttributeSpecularControl		"SpecularControl"
#define kQ3ClassNameAttributeSurfaceShader			"SurfaceShaderAttribute"
#define kQ3ClassNameAttributeSurfaceTangent			"SurfaceTangent"
#define kQ3ClassNameAttributeSurfaceUV				"SurfaceUV"	
#define kQ3ClassNameAttributeTransparencyColor		"TransparencyColor"
#define kQ3ClassNameCamera							"Camera"
#define kQ3ClassNameCameraOrthographic				"OrthographicCamera"
#define kQ3ClassNameCameraViewAngle					"ViewAngleCamera"
#define kQ3ClassNameCameraViewPlane					"ViewPlaneCamera"
#define kQ3ClassNameCustomElementName				"Apple Computer, Inc.:NameElement"
#define kQ3ClassNameCustomElementUrl				"Apple Computer, Inc.:URLElement"
#define kQ3ClassNameCustomElementWire				"Apple Computer, Inc.:WireElement"
#define kQ3ClassNameCustomElementProperties			"Quesa:PropertyElement"
#define kQ3ClassNameDrawContext						"DrawContext"
#define kQ3ClassNameDrawContextDDSurface			"DDSurfaceDrawContext"
#define kQ3ClassNameDrawContextMac					"MacDrawContext"
#define kQ3ClassNameDrawContextCocoa				"CocoaDrawContext"
#define kQ3ClassNameDrawContextPixmap				"PixmapDrawContext"
#define kQ3ClassNameDrawContextWin32DC				"Win32DCDrawContext"
#define kQ3ClassNameDrawContextX11					"X11DrawContext"
#define kQ3ClassNameElement							"Element"
#define kQ3ClassNameSetElement						"SetElement"
#define kQ3ClassNameStorageFSSpec					"MacintoshFSSpec"
#define kQ3ClassNameFile							"File"
#define kQ3ClassNameFileFormat						"Quesa:FileFormat"
#define kQ3ClassNameFileFormatReader				"Quesa:FileFormat:Reader"
#define kQ3ClassNameFileFormatR_3DMF_Bin			"Quesa:FileFormat:Reader:3DMF Binary"
#define kQ3ClassNameFileFormatR_3DMF_BinSwap		"Quesa:FileFormat:Reader:3DMF Binary Swapped"
#define kQ3ClassNameFileFormatR_3DMF_Text			"Quesa:FileFormat:Reader:3DMF Text"
#define kQ3ClassNameFileFormatWriter				"Quesa:FileFormat:Writer"
#define kQ3ClassNameFileFormatW_3DMF_S_Bin			"Quesa:FileFormat:Writer:3DMF Stream Binary"
#define kQ3ClassNameFileFormatW_3DMF_N_Bin			"Quesa:FileFormat:Writer:3DMF Normal Binary"
#define kQ3ClassNameFileFormatW_3DMF_D_Bin			"Quesa:FileFormat:Writer:3DMF Database Binary"
#define kQ3ClassNameFileFormatW_3DMF_DS_Bin			"Quesa:FileFormat:Writer:3DMF Database Stream Binary"
#define kQ3ClassNameFileFormatW_3DMF_SW_Bin			"Quesa:FileFormat:Writer:3DMF Stream Binary Swapped"
#define kQ3ClassNameFileFormatW_3DMF_NW_Bin			"Quesa:FileFormat:Writer:3DMF Normal Binary Swapped"
#define kQ3ClassNameFileFormatW_3DMF_DW_Bin			"Quesa:FileFormat:Writer:3DMF Database Binary Swapped"
#define kQ3ClassNameFileFormatW_3DMF_DSW_Bin		"Quesa:FileFormat:Writer:3DMF Database Stream Binary Swapped"
#define kQ3ClassNameGeometry						"Geometry"
#define kQ3ClassNameGeometryBox						"Box"
#define kQ3ClassNameGeometryBundle					"GeometryBundle"
#define kQ3ClassNameGeometryCone					"Cone"
#define kQ3ClassNameGeometryCylinder				"Cylinder"
#define kQ3ClassNameGeometryDisk					"Disk"
#define kQ3ClassNameGeometryEllipse					"Ellipse"
#define kQ3ClassNameGeometryEllipsoid				"Ellipsoid"
#define kQ3ClassNameGeometryGeneralPolygon			"GeneralPolygon"
#define kQ3ClassNameGeometryLine					"Line"
#define kQ3ClassNameGeometryMarker					"Marker"
#define kQ3ClassNameGeometryMesh					"Mesh"
#define kQ3ClassNameGeometryNURBCurve				"NURBCurve"
#define kQ3ClassNameGeometryNURBPatch				"NURBPatch"
#define kQ3ClassNameGeometryPixmapMarker			"PixmapMarker"
#define kQ3ClassNameGeometryPoint					"Point"
#define kQ3ClassNameGeometryPolyLine				"PolyLine"
#define kQ3ClassNameGeometryPolygon					"Polygon"
#define kQ3ClassNameGeometryPolyhedron				"Polyhedron"
#define kQ3ClassNameGeometryTorus					"Torus"
#define kQ3ClassNameGeometryTriGrid					"TriGrid"
#define kQ3ClassNameGeometryTriMesh					"TriMesh"
#define kQ3ClassNameGeometryTriangle				"Triangle"
#define kQ3ClassNameGroup							"Group"
#define kQ3ClassNameGroupDisplay					"DisplayGroup"
#define kQ3ClassNameGroupDisplayIOProxy				"IOProxyDisplayGroup"
#define kQ3ClassNameGroupDisplayOrdered				"OrderedDisplayGroup"
#define kQ3ClassNameGroupInfo						"InfoGroup"
#define kQ3ClassNameGroupLight						"LightGroup"
#define kQ3ClassNameEndGroup						"EndGroup"
#define kQ3ClassNameStorageHandle					"Handle"
#define kQ3ClassNameIlluminationLambert				"LambertIllumination"
#define kQ3ClassNameIlluminationNULL				"NULLIllumination"
#define kQ3ClassNameIlluminationPhong				"PhongIllumination"
#define kQ3ClassNameLight							"Light"
#define kQ3ClassNameLightAmbient					"AmbientLight"
#define kQ3ClassNameLightDirectional				"DirectionalLight"
#define kQ3ClassNameLightPoint						"PointLight"
#define kQ3ClassNameLightSpot						"SpotLight"
#define kQ3ClassNameStorageMacintosh				"MacintoshStorage"
#define kQ3ClassNameStorageMemory					"Memory"
#define kQ3ClassNameMeshEdgePart					"MeshEdgePart"
#define kQ3ClassNameMeshFacePart					"MeshFacePart"
#define kQ3ClassNameMeshVertexPart					"MeshVertexPart"
#define kQ3ClassNameMeshShapePart					"MeshShapePart"
#define kQ3ClassNameMeshCorners						"MeshCorners"
#define kQ3ClassNameMeshEdges						"MeshEdges"
#define kQ3ClassNamePick							"Pick"
#define kQ3ClassNamePickWindowPoint					"WindowPointPick"
#define kQ3ClassNamePickWindowRect					"WindowRectPick"
#define kQ3ClassNamePickWorldRay					"WorldRayPick"
#define kQ3ClassNameRenderer						"Renderer"
#define kQ3ClassNameRendererGeneric					"GenericRenderer"
#define kQ3ClassNameRendererInteractive				"InteractiveRenderer"
#define kQ3ClassNameRendererWireframe				"WireFrame"
#define kQ3ClassNameRoot							"Quesa:Root"
#define kQ3ClassNameSet								"Set"
#define kQ3ClassNameSetAttribute					"AttributeSet"
#define kQ3ClassNameShader							"Shader"
#define kQ3ClassNameShaderIllumination				"IlluminationShader"
#define kQ3ClassNameShaderSurface					"SurfaceShader"
#define kQ3ClassNameShape							"Shape"
#define kQ3ClassNameShapePart						"ShapePart"
#define kQ3ClassNameShared							"SharedObject"
#define kQ3ClassNameSharedUnknown					"Unknown"
#define kQ3ClassNameStateOperator					"StateOperator"
#define kQ3ClassNameStateOperatorPop				"Pop"
#define kQ3ClassNameStateOperatorPush				"Push"
#define kQ3ClassNameStorage							"Storage"
#define kQ3ClassNameString							"String"
#define kQ3ClassNameStringTypeC						"CString"
#define kQ3ClassNameStyle							"Style"
#define kQ3ClassNameStyleAntiAlias					"AntiAliasStyle"
#define kQ3ClassNameStyleBackfacing					"BackfacingStyle"
#define kQ3ClassNameStyleFill						"FillStyle"
#define kQ3ClassNameStyleFog						"FogStyle"
#define kQ3ClassNameStyleHighlight					"HighlightStyle"
#define kQ3ClassNameStyleInterpolation				"InterpolationStyle"
#define kQ3ClassNameStyleOrientation				"OrientationStyle"
#define kQ3ClassNameStylePickID						"PickIDStyle"
#define kQ3ClassNameStylePickParts					"PickPartsStyle"
#define kQ3ClassNameStyleCastShadows				"CastShadowsStyle"
#define kQ3ClassNameStyleReceiveShadows				"ReceiveShadowsStyle"
#define kQ3ClassNameStyleSubdivision				"SubdivisionStyle"
#define kQ3ClassNameSurfaceTexture					"TextureShader"
#define kQ3ClassNameTexture							"Texture"
#define kQ3ClassNameTextureCompressed				"CompressedPixmapTexture"
#define kQ3ClassNameTextureMipmap					"MipmapTexture"
#define kQ3ClassNameTexturePixmap					"PixmapTexture"
#define kQ3ClassNameTransform						"Transform"
#define kQ3ClassNameTransformMatrix					"Matrix"
#define kQ3ClassNameTransformQuaternion				"Quaternion"
#define kQ3ClassNameTransformReset					"Reset"
#define kQ3ClassNameTransformRotate					"Rotate"
#define kQ3ClassNameTransformRotateAboutAxis		"RotateAboutAxis"
#define kQ3ClassNameTransformRotateAboutPoint		"RotateAboutPoint"
#define kQ3ClassNameTransformScale					"Scale"
#define kQ3ClassNameTransformTranslate				"Translate"
#define kQ3ClassNameTransformCamera					"Quesa:Transform:Camera"
#define kQ3ClassNameTransformCameraRasterize		"Quesa:Transform:Camera:Rasterize"
#define kQ3ClassNameStorageUnixPath					"UnixPath"
#define kQ3ClassNameStorageUnix						"UnixStorage"
#define kQ3ClassNameUnknown							"Unknown"
#define kQ3ClassNameUnknownBinary					"UnknownBinary"
#define kQ3ClassNameUnknownText						"UnknownText"
#define kQ3ClassNameView							"View"
#define kQ3ClassNameViewer							"Quesa:Viewer"
#define kQ3ClassNameSlab							"Quesa:Slab"
#define kQ3ClassNameStorageWin32					"Win32Storage"
#define kQ3ClassNameAttributeSetList				"AttributeSetList"
#define kQ3ClassNameAttributeArray					"AttributeArray"
#define kQ3ClassNameAttributeSetListGeometry		"GeometryAttributeSetList"
#define kQ3ClassNameAttributeSetListFace			"FaceAttributeSetList"
#define kQ3ClassNameAttributeSetListVertex			"VertexAttributeSetList"
#define kQ3ClassNameGeneralPolygonHint				"GeneralPolygonHint"
#define kQ3ClassNameDisplayGroupState				"DisplayGroupState"
#define kQ3ClassNameCaps							"Caps"
#define kQ3ClassNameTopCapAttributeSet				"TopCapAttributeSet"
#define kQ3ClassNameBottomCapAttributeSet			"BottomCapAttributeSet"
#define kQ3ClassNameFaceCapAttributeSet				"FaceCapAttributeSet"
#define kQ3ClassNameInteriorCapAttributeSet			"InteriorCapAttributeSet"
#define kQ3ClassNameShaderUVTransform				"ShaderUVTransform"
#define kQ3ClassNameStoragePath						"Quesa:Storage:Path"
#define kQ3ClassNameStorageBe						"Quesa:Storage:Be"
#define kQ3ClassNameDrawContextBe					"Quesa:DrawContext:Be"
#define kQ3ClassName3DMF							"Metafile"
#define kQ3ClassNameTOC								"TableOfContents"
#define kQ3ClassNameReference						"Reference"
#define kQ3ClassNameType							"Type"
#define kQ3ClassNameViewHint						"ViewHint"


// Class types
#define kQ3ObjectTypeQuesa							Q3_OBJECT_TYPE(0xDE, 0xAD, 0xD0, 0xD0)
#define kQ3ObjectTypeRoot							Q3_OBJECT_TYPE('Q', 'r', 'o', 'o')
#define kQ3ObjectTypeQuesaInteractive				Q3_OBJECT_TYPE('Q', 'r', 'n', 'i')
#define kQ3ObjectTypeAttributeSurfaceUV				Q3_OBJECT_TYPE('s', 'r', 'u', 'v')
#define kQ3ObjectTypeAttributeShadingUV				Q3_OBJECT_TYPE('s', 'h', 'u', 'v')
#define kQ3ObjectTypeAttributeNormal				Q3_OBJECT_TYPE('n', 'r', 'm', 'l')
#define kQ3ObjectTypeAttributeAmbientCoefficient	Q3_OBJECT_TYPE('c', 'a', 'm', 'b')
#define kQ3ObjectTypeAttributeDiffuseColor			Q3_OBJECT_TYPE('k', 'd', 'i', 'f')
#define kQ3ObjectTypeAttributeSpecularColor			Q3_OBJECT_TYPE('k', 's', 'p', 'c')
#define kQ3ObjectTypeAttributeSpecularControl		Q3_OBJECT_TYPE('c', 's', 'p', 'c')
#define kQ3ObjectTypeAttributeTransparencyColor		Q3_OBJECT_TYPE('k', 'x', 'p', 'r')
#define kQ3ObjectTypeAttributeSurfaceTangent		Q3_OBJECT_TYPE('s', 'r', 't', 'n')
#define kQ3ObjectTypeAttributeHighlightState		Q3_OBJECT_TYPE('h', 'l', 's', 't')
#define kQ3ObjectTypeAttributeSurfaceShader			Q3_OBJECT_TYPE('s', 's', 'a', 't')
#define kQ3ObjectTypeCustomElementName				Q3_OBJECT_TYPE(0xF0, 'e', 'n', 'a')
#define kQ3ObjectTypeCustomElementUrl				Q3_OBJECT_TYPE(0xF0, 'e', 'u', 'r')
#define kQ3ObjectTypeCustomElementWire				Q3_OBJECT_TYPE(0xF0, 'e', 'w', 'i')
#define kQ3ObjectTypeCustomElementProperties		Q3_OBJECT_TYPE(0xF0, 'e', 'p', 'r')
#define kQ3ObjectTypeSetElement						Q3_OBJECT_TYPE('s', 'e', 't', 'e')
#define kQ3StorageTypePath							Q3_OBJECT_TYPE('Q', 's', 't', 'p')
#define kQ3ObjectType3DMF							Q3_OBJECT_TYPE('3', 'D', 'M', 'F')


// Class methods
#define kQ3XMethodTypeObjectDispose					Q3_METHOD_TYPE('Q', 'd', 's', 'p')
#define kQ3XMethodTypeObjectSubmitRender			Q3_METHOD_TYPE('Q', 's', 'u', 'r')
#define kQ3XMethodTypeObjectSubmitPick				Q3_METHOD_TYPE('Q', 's', 'u', 'p')
#define kQ3XMethodTypeObjectSubmitWrite				Q3_METHOD_TYPE('Q', 's', 'u', 'w')
#define kQ3XMethodTypeObjectSubmitBounds			Q3_METHOD_TYPE('Q', 's', 'u', 'b')
#define kQ3XMethodTypeDrawContextUpdate				Q3_METHOD_TYPE('Q', 'u', 'p', 'd')
#define kQ3XMethodTypeDrawContextGetDimensions		Q3_METHOD_TYPE('Q', 'g', 'd', 'i')
#define kQ3XMethodTypeTransformMatrix				Q3_METHOD_TYPE('Q', 't', 'm', 'x')
#define kQ3XMethodTypeCameraFrustumMatrix			Q3_METHOD_TYPE('Q', 'c', 'm', 'x')
#define kQ3XMethodTypeRendererMethodsCached			Q3_METHOD_TYPE('Q', 'r', 'm', 'c')
#define kQ3XMethodTypeTextureDimensions				Q3_METHOD_TYPE('Q', 't', 'd', 'm')
#define kQ3XMethodTypeGeomGetAttribute				Q3_METHOD_TYPE('Q', 'g', 'g', 'a')
#define kQ3XMethodTypeGeomGetPublicData				Q3_METHOD_TYPE('Q', 'g', 'p', 'u')
#define kQ3XMethodTypeGeomUsesSubdivision			Q3_METHOD_TYPE('Q', 'g', 'u', 's')
#define kQ3XMethodTypeGeomUsesOrientation			Q3_METHOD_TYPE('Q', 'g', 'u', 'o')
#define kQ3XMethodTypeGeomCacheNew					Q3_METHOD_TYPE('Q', 'g', 'c', 'n')
#define kQ3XMethodTypeGeomCacheIsValid				Q3_METHOD_TYPE('Q', 'g', 'c', 'v')
#define kQ3XMethodTypeGeomCacheUpdate				Q3_METHOD_TYPE('Q', 'g', 'c', 'u')
#define kQ3XMethodTypeStorageReadData				Q3_METHOD_TYPE('Q', 'r', 'e', 'a')
#define kQ3XMethodTypeStorageWriteData				Q3_METHOD_TYPE('Q', 'w', 'r', 'i')
#define kQ3XMethodTypeStorageGetSize				Q3_METHOD_TYPE('Q', 'G', 's', 'z')
#define kQ3XMethodTypeStorageSetSize				Q3_METHOD_TYPE('Q', 'S', 's', 'z')
#define kQ3XMethodTypeStorageOpen					Q3_METHOD_TYPE('Q', 'O', 'p', 'n')
#define kQ3XMethodTypeStorageClose					Q3_METHOD_TYPE('Q', 'C', 'l', 's')


// 3DMF object types
#define kQ3AttributeSetTypeBottomCap				Q3_OBJECT_TYPE('b', 'c', 'a', 's')
#define kQ3AttributeSetTypeFaceCap					Q3_OBJECT_TYPE('f', 'c', 'a', 's')
#define kQ3AttributeSetTypeInteriorCap				Q3_OBJECT_TYPE('i', 'c', 'a', 's')
#define kQ3AttributeSetTypeTopCap					Q3_OBJECT_TYPE('t', 'c', 'a', 's')
#define kQ3ObjectTypeAttributeArray					Q3_OBJECT_TYPE('a', 't', 'a', 'r')
#define kQ3ObjectTypeAttributeSetList				Q3_OBJECT_TYPE('r', 'a', 's', 'l')
#define kQ3ObjectTypeAttributeSetListFace			Q3_OBJECT_TYPE('f', 'a', 's', 'l')
#define kQ3ObjectTypeAttributeSetListGeometry		Q3_OBJECT_TYPE('g', 'a', 's', 'l')
#define kQ3ObjectTypeAttributeSetListVertex			Q3_OBJECT_TYPE('v', 'a', 's', 'l')
#define kQ3ObjectTypeDisplayGroupState				Q3_OBJECT_TYPE('d', 'g', 's', 't')
#define kQ3ObjectTypeGeneralPolygonHint				Q3_OBJECT_TYPE('g', 'p', 'l', 'h')
#define kQ3ObjectTypeGeometryCaps					Q3_OBJECT_TYPE('c', 'a', 'p', 's')
#define kQ3ObjectTypeMeshCorners					Q3_OBJECT_TYPE('c', 'r', 'n', 'r')
#define kQ3ObjectTypeMeshEdges						Q3_OBJECT_TYPE('e', 'd', 'g', 'e')
#define kQ3ObjectTypeShaderTransform				Q3_OBJECT_TYPE('s', 'd', 'x', 'f')
#define kQ3ObjectTypeShaderUVTransform				Q3_OBJECT_TYPE('s', 'd', 'u', 'v')
#define kQ3ObjectTypeTOC							Q3_OBJECT_TYPE('t', 'o', 'c', ' ')
#define kQ3ObjectTypeType							Q3_OBJECT_TYPE('t', 'y', 'p', 'e')


// Internal object types
#define kQ3ObjectTypeLeaf							Q3_OBJECT_TYPE('l', 'e', 'a', 'f')





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Object methods
typedef Q3_CALLBACK_API_C(void,   		TQ3XObjectDisposeMethod)(TQ3Object theObject);
typedef Q3_CALLBACK_API_C(TQ3Status,	TQ3XObjectSubmitMethod)(TQ3ViewObject		theView,
																TQ3ObjectType	objectType,
																TQ3Object		theObject,
																const void		*objectData);


// Misc methods
typedef Q3_CALLBACK_API_C(TQ3Status, 	TQ3XDrawContextUpdateMethod)(TQ3DrawContextObject theDrawContext);
typedef Q3_CALLBACK_API_C(void, 		TQ3XDrawContextGetDimensionsMethod)(TQ3DrawContextObject theDrawContext, TQ3Area *thePane);
typedef Q3_CALLBACK_API_C(void,      	TQ3XTransformMatrixMethod)(const void *transformData, TQ3Matrix4x4 *theMatrix);
typedef Q3_CALLBACK_API_C(void,		TQ3XCameraFrustumMatrixMethod)(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix);
typedef Q3_CALLBACK_API_C(void,		TQ3XTextureDimensionsMethod)(TQ3TextureObject theTexture, TQ3Point2D *theDimensions);


// Geometry methods
typedef Q3_CALLBACK_API_C(TQ3AttributeSet *,	TQ3XGeomGetAttributeMethod)(TQ3GeometryObject theObject);
typedef Q3_CALLBACK_API_C(const void *,			TQ3XGeomGetPublicDataMethod)(TQ3GeometryObject theObject);
typedef Q3_CALLBACK_API_C(TQ3Object,			TQ3XGeomCacheNewMethod)(TQ3ViewObject		theView,
																	TQ3GeometryObject	theGeom,
																	const void			*geomData);
typedef Q3_CALLBACK_API_C(TQ3Boolean,			TQ3XGeomCacheIsValidMethod)(TQ3ViewObject		theView,
																		TQ3ObjectType		objectType,
																		TQ3GeometryObject	theGeom,
																		const void			*geomData,
																		TQ3Object			cachedGeom);
typedef Q3_CALLBACK_API_C(void,				TQ3XGeomCacheUpdateMethod)(TQ3ViewObject		theView,
																		TQ3ObjectType		objectType,
																		TQ3GeometryObject	theGeom,
																		const void			*geomData,
																		TQ3Object			*cachedGeom);


// Storage methods
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XStorageReadDataMethod)(TQ3StorageObject storage,
																TQ3Uns32		offset,
																TQ3Uns32		dataSize,
																TQ3Uns8			*data,
																TQ3Uns32		*sizeRead);
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XStorageWriteDataMethod)(TQ3StorageObject storage,
																TQ3Uns32		offset,
																TQ3Uns32		dataSize,
																const TQ3Uns8	*data,
																TQ3Uns32		*sizeWritten);
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XStorageGetSizeMethod)(TQ3StorageObject storage, TQ3Uns32 *size);
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XStorageSetSizeMethod)(TQ3StorageObject storage, TQ3Uns32 size);
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XStorageOpenMethod)(TQ3StorageObject storage, TQ3Boolean forWriting);
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XStorageCloseMethod)(TQ3StorageObject storage);


// Definition of TQ3Object
#if !QUESA_OBJECTS_ARE_OPAQUE

typedef struct OpaqueTQ3Object {
	TQ3ObjectType		quesaTag;
	E3ClassInfoPtr		theClass;
	void				*instanceData;
	TQ3Object			parentObject;
	
#if Q3_DEBUG
	TQ3Object			childObject;
#endif
} OpaqueTQ3Object;

#endif // !QUESA_OBJECTS_ARE_OPAQUE


// Global state for each instance of Quesa.
//
// With a few exceptions, data in this structure should be considered opaque
// to the internals of Quesa - there are internal APIs to get and set almost
// every field in this structure, which minimises the amount of code which
// depends on the content of the global state.
//
// Global state introduces penalties when running under a preemptively
// threaded environment (which Quesa probably will, at some stage). Please
// only use the global state as a last resort.
typedef struct E3Globals {
	// System
	TQ3Boolean				systemInitialised;
	TQ3Boolean				systemDoBottleneck;
	TQ3Uns32				systemRefCount;


	// Class tree
	E3HashTablePtr			classTree;
	E3ClassInfoPtr			classTreeRoot;
	TQ3ObjectType			classNextType;
	TQ3Uns32				classStats1;
	TQ3Uns32				classStats2;
	TQ3Uns32				classStats3;
	TQ3Uns32				classStats4;
	TQ3Uns32				classStats5;
	

	// Shared libraries
	TQ3Uns32				sharedLibraryCount;
	TQ3XSharedLibraryInfo	*sharedLibraryInfo;


	// Error Manager
	TQ3Boolean				errMgrClearError;
	TQ3Boolean				errMgrClearWarning;
	TQ3Boolean				errMgrClearNotice;
	TQ3Boolean				errMgrClearPlatform;
	TQ3Boolean				errMgrIsFatalError;
	TQ3Error				errMgrOldestError;
	TQ3Warning				errMgrOldestWarning;
	TQ3Notice				errMgrOldestNotice;
	TQ3Uns32				errMgrOldestPlatform;
	TQ3Error				errMgrLatestError;
	TQ3Warning				errMgrLatestWarning;
	TQ3Notice				errMgrLatestNotice;
	TQ3Uns32				errMgrLatestPlatform;
	TQ3ErrorMethod			errMgrHandlerFuncError;
	TQ3WarningMethod		errMgrHandlerFuncWarning;
	TQ3NoticeMethod			errMgrHandlerFuncNotice;
	TQ3ErrorMethod 			errMgrHandlerFuncPlatform;
	TQ3Uns32				errMgrHandlerDataError;
	TQ3Uns32				errMgrHandlerDataWarning;
	TQ3Uns32				errMgrHandlerDataNotice;
	TQ3Uns32		 		errMgrHandlerDataPlatform;


	// Platform specific (just Mac for now)
#if QUESA_OS_MACINTOSH
	DMNotificationUPP		dmNotifyUPP;
	TQ3Boolean				dmNotifiedChanges;
#endif


	// Debugging
#if Q3_DEBUG
	TQ3Object				listHead;
	TQ3Boolean				isLeakChecking;
#endif

} E3Globals, *E3GlobalsPtr;





//=============================================================================
//      Globals
//-----------------------------------------------------------------------------
// Global Quesa state
//
// Note, all code should access these globals through  E3Globals_Get accessor
// rather than accessing them directly. The one exception to this is in
// E3System.c, where the bottleneck reads them directly for performance.
extern E3Globals gE3Globals;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Get access to the Quesa global state
E3GlobalsPtr	E3Globals_Get(void);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

