//	InfoWindow.cpp
//
//	This class represents the Info window, which gives the user details
//	about a model.
//

#include "InfoWindow.h"

const Rect kInfoWindowRect = {100, 100, 100+300, 100+256};

struct TypeMapEntry
{
	TQ3ObjectType type;
	Str31 name;
};

static TypeMapEntry sTypeMap[] = {
	{ kQ3ObjectTypeInvalid, "\pQ3ObjectTypeInvalid"},
	{ kQ3ObjectTypeView, "\pQ3ObjectTypeView"},
	{ kQ3ObjectTypeElement, "\pQ3ObjectTypeElement"},
	{ kQ3ElementTypeAttribute, "\pQ3ElementTypeAttribute"},
	{ kQ3ObjectTypePick, "\pQ3ObjectTypePick"},
	{ kQ3PickTypeWindowPoint, "\pQ3PickTypeWindowPoint"},
	{ kQ3PickTypeWindowRect, "\pQ3PickTypeWindowRect"},
	{ kQ3PickTypeWorldRay, "\pQ3PickTypeWorldRay"},
	{ kQ3ObjectTypeShared, "\pQ3ObjectTypeShared"},
	{ kQ3SharedTypeRenderer, "\pQ3SharedTypeRenderer"},
	{ kQ3RendererTypeWireFrame, "\pQ3RendererTypeWireFrame"},
	{ kQ3RendererTypeGeneric, "\pQ3RendererTypeGeneric"},
	{ kQ3RendererTypeInteractive, "\pQ3RendererTypeInteractive"},
	{ kQ3SharedTypeShape, "\pQ3SharedTypeShape"},
	{ kQ3ShapeTypeGeometry, "\pQ3ShapeTypeGeometry"},
	{ kQ3GeometryTypeBox, "\pQ3GeometryTypeBox"},
	{ kQ3GeometryTypeGeneralPolygon, "\pQ3GeometryTypeGeneralPolygon"},
	{ kQ3GeometryTypeLine, "\pQ3GeometryTypeLine"},
	{ kQ3GeometryTypeMarker, "\pQ3GeometryTypeMarker"},
	{ kQ3GeometryTypePixmapMarker, "\pQ3GeometryTypePixmapMarker"},
	{ kQ3GeometryTypeMesh, "\pQ3GeometryTypeMesh"},
	{ kQ3GeometryTypeNURBCurve, "\pQ3GeometryTypeNURBCurve"},
	{ kQ3GeometryTypeNURBPatch, "\pQ3GeometryTypeNURBPatch"},
	{ kQ3GeometryTypePoint, "\pQ3GeometryTypePoint"},
	{ kQ3GeometryTypePolygon, "\pQ3GeometryTypePolygon"},
	{ kQ3GeometryTypePolyLine, "\pQ3GeometryTypePolyLine"},
	{ kQ3GeometryTypeTriangle, "\pQ3GeometryTypeTriangle"},
	{ kQ3GeometryTypeTriGrid, "\pQ3GeometryTypeTriGrid"},
	{ kQ3GeometryTypeCone, "\pQ3GeometryTypeCone"},
	{ kQ3GeometryTypeCylinder, "\pQ3GeometryTypeCylinder"},
	{ kQ3GeometryTypeDisk, "\pQ3GeometryTypeDisk"},
	{ kQ3GeometryTypeEllipse, "\pQ3GeometryTypeEllipse"},
	{ kQ3GeometryTypeEllipsoid, "\pQ3GeometryTypeEllipsoid"},
	{ kQ3GeometryTypePolyhedron, "\pQ3GeometryTypePolyhedron"},
	{ kQ3GeometryTypeTorus, "\pQ3GeometryTypeTorus"},
	{ kQ3GeometryTypeTriMesh, "\pQ3GeometryTypeTriMesh"},
	{ kQ3ShapeTypeShader, "\pQ3ShapeTypeShader"},
	{ kQ3ShaderTypeSurface, "\pQ3ShaderTypeSurface"},
	{ kQ3SurfaceShaderTypeTexture, "\pQ3SurfaceShaderTypeTexture"},
	{ kQ3ShaderTypeIllumination, "\pQ3ShaderTypeIllumination"},
	{ kQ3IlluminationTypePhong, "\pQ3IlluminationTypePhong"},
	{ kQ3IlluminationTypeLambert, "\pQ3IlluminationTypeLambert"},
	{ kQ3IlluminationTypeNULL, "\pQ3IlluminationTypeNULL"},
	{ kQ3ShapeTypeStyle, "\pQ3ShapeTypeStyle"},
	{ kQ3StyleTypeBackfacing, "\pQ3StyleTypeBackfacing"},
	{ kQ3StyleTypeInterpolation, "\pQ3StyleTypeInterpolation"},
	{ kQ3StyleTypeFill, "\pQ3StyleTypeFill"},
	{ kQ3StyleTypePickID, "\pQ3StyleTypePickID"},
	{ kQ3StyleTypeCastShadows, "\pQ3StyleTypeCastShadows"},
	{ kQ3StyleTypeReceiveShadows, "\pQ3StyleTypeReceiveShadows"},
	{ kQ3StyleTypeHighlight, "\pQ3StyleTypeHighlight"},
	{ kQ3StyleTypeSubdivision, "\pQ3StyleTypeSubdivision"},
	{ kQ3StyleTypeOrientation, "\pQ3StyleTypeOrientation"},
	{ kQ3StyleTypePickParts, "\pQ3StyleTypePickParts"},
	{ kQ3StyleTypeAntiAlias, "\pQ3StyleTypeAntiAlias"},
	{ kQ3StyleTypeFog, "\pQ3StyleTypeFog"},
	{ kQ3ShapeTypeTransform, "\pQ3ShapeTypeTransform"},
	{ kQ3TransformTypeMatrix, "\pQ3TransformTypeMatrix"},
	{ kQ3TransformTypeScale, "\pQ3TransformTypeScale"},
	{ kQ3TransformTypeTranslate, "\pQ3TransformTypeTranslate"},
	{ kQ3TransformTypeRotate, "\pQ3TransformTypeRotate"},
	{ kQ3TransformTypeRotateAboutPoint, "\pQ3TransformTypeRotateAboutPoint"},
	{ kQ3TransformTypeRotateAboutAxis, "\pQ3TransformTypeRotateAboutAxis"},
	{ kQ3TransformTypeQuaternion, "\pQ3TransformTypeQuaternion"},
	{ kQ3TransformTypeReset, "\pQ3TransformTypeReset"},
	{ kQ3ShapeTypeLight, "\pQ3ShapeTypeLight"},
	{ kQ3LightTypeAmbient, "\pQ3LightTypeAmbient"},
	{ kQ3LightTypeDirectional, "\pQ3LightTypeDirectional"},
	{ kQ3LightTypePoint, "\pQ3LightTypePoint"},
	{ kQ3LightTypeSpot, "\pQ3LightTypeSpot"},
	{ kQ3ShapeTypeCamera, "\pQ3ShapeTypeCamera"},
	{ kQ3CameraTypeOrthographic, "\pQ3CameraTypeOrthographic"},
	{ kQ3CameraTypeViewPlane, "\pQ3CameraTypeViewPlane"},
	{ kQ3CameraTypeViewAngleAspect, "\pQ3CameraTypeViewAngleAspect"},
	{ kQ3ShapeTypeStateOperator, "\pQ3ShapeTypeStateOperator"},
	{ kQ3StateOperatorTypePush, "\pQ3StateOperatorTypePush"},
	{ kQ3StateOperatorTypePop, "\pQ3StateOperatorTypePop"},
	{ kQ3ShapeTypeGroup, "\pQ3ShapeTypeGroup"},
	{ kQ3GroupTypeDisplay, "\pQ3GroupTypeDisplay"},
	{ kQ3DisplayGroupTypeOrdered, "\pQ3DisplayGroupTypeOrdered"},
	{ kQ3DisplayGroupTypeIOProxy, "\pQ3DisplayGroupTypeIOProxy"},
	{ kQ3GroupTypeLight, "\pQ3GroupTypeLight"},
	{ kQ3GroupTypeInfo, "\pQ3GroupTypeInfo"},
	{ kQ3ShapeTypeUnknown, "\pQ3ShapeTypeUnknown"},
	{ kQ3UnknownTypeText, "\pQ3UnknownTypeText"},
	{ kQ3UnknownTypeBinary, "\pQ3UnknownTypeBinary"},
	{ kQ3ShapeTypeReference, "\pQ3ShapeTypeReference"},
	{ kQ3ReferenceTypeExternal, "\pQ3ReferenceTypeExternal"},
	{ kQ3SharedTypeSet, "\pQ3SharedTypeSet"},
	{ kQ3SetTypeAttribute, "\pQ3SetTypeAttribute"},
	{ kQ3SharedTypeDrawContext, "\pQ3SharedTypeDrawContext"},
	{ kQ3DrawContextTypePixmap, "\pQ3DrawContextTypePixmap"},
	{ kQ3DrawContextTypeMacintosh, "\pQ3DrawContextTypeMacintosh"},
	{ kQ3DrawContextTypeCocoa, "\pQ3DrawContextTypeCocoa"},
	{ kQ3DrawContextTypeWin32DC, "\pQ3DrawContextTypeWin32DC"},
	{ kQ3DrawContextTypeDDSurface, "\pQ3DrawContextTypeDDSurface"},
	{ kQ3DrawContextTypeX11, "\pQ3DrawContextTypeX11"},
	{ kQ3SharedTypeTexture, "\pQ3SharedTypeTexture"},
	{ kQ3TextureTypePixmap, "\pQ3TextureTypePixmap"},
	{ kQ3TextureTypeMipmap, "\pQ3TextureTypeMipmap"},
	{ kQ3TextureTypeCompressedPixmap, "\pQ3TextureTypeCompressedPixmap"},
	{ kQ3SharedTypeFile, "\pQ3SharedTypeFile"},
	{ kQ3SharedTypeStorage, "\pQ3SharedTypeStorage"},
	{ kQ3StorageTypeMemory, "\pQ3StorageTypeMemory"},
	{ kQ3MemoryStorageTypeHandle, "\pQ3MemoryStorageTypeHandle"},
	{ kQ3StorageTypeUnix, "\pQ3StorageTypeUnix"},
	{ kQ3UnixStorageTypePath, "\pQ3UnixStorageTypePath"},
	{ kQ3StorageTypeMacintosh, "\pQ3StorageTypeMacintosh"},
	{ kQ3MacintoshStorageTypeFSSpec, "\pQ3MacintoshStorageTypeFSSpec"},
	{ kQ3StorageTypeWin32, "\pQ3StorageTypeWin32"},
	{ kQ3SharedTypeString, "\pQ3SharedTypeString"},
	{ kQ3StringTypeCString, "\pQ3StringTypeCString"},
	{ kQ3SharedTypeShapePart, "\pQ3SharedTypeShapePart"},
	{ kQ3ShapePartTypeMeshPart, "\pQ3ShapePartTypeMeshPart"},
	{ kQ3MeshPartTypeMeshFacePart, "\pQ3MeshPartTypeMeshFacePart"},
	{ kQ3MeshPartTypeMeshEdgePart, "\pQ3MeshPartTypeMeshEdgePart"},
	{ kQ3MeshPartTypeMeshVertexPart, "\pQ3MeshPartTypeMeshVertexPart"},
	{ kQ3SharedTypeControllerState, "\pQ3SharedTypeControllerState"},
	{ kQ3SharedTypeTracker, "\pQ3SharedTypeTracker"},
	{ kQ3SharedTypeViewHints, "\pQ3SharedTypeViewHints"},
	{ kQ3SharedTypeEndGroup, "\pQ3SharedTypeEndGroup"}
};

static void DrawItemLine(TQ3Object item, short x, short y)
{
	// look up the type name in our map
	TQ3ObjectType type = Q3Object_GetLeafType(item);
	Str31 *name = NULL;
	int qtyTypes = sizeof(sTypeMap) / sizeof(TypeMapEntry);
	for (int i=0; i<qtyTypes; i++) {
		if (sTypeMap[i].type == type) {
			name = &sTypeMap[i].name;
			break;
		}
	}

	// if not found, construct a generic one
	Str31 s = "\p????";
	if (not name) {
		*(TQ3ObjectType*)(&s[1]) = type;
		name = &s;
	}
	
	// now, draw this label
	MoveTo(x,y);
	DrawString(*name);
}

static void DrawItemInfo(TQ3Object item, short *x, short *y, short dx, short dy)
{
	DrawItemLine(item, *x, *y);	
	*y += dy;
	if (Q3Object_IsType(item, kQ3ShapeTypeGroup)) {
		TQ3GroupObject group = (TQ3GroupObject)item;
		TQ3GroupPosition position;
		if (kQ3Success != Q3Group_GetFirstPosition(group, &position)) return;
		do {
			TQ3Object subobj;
			if (kQ3Success == Q3Group_GetPositionObject(group, position, &subobj)) {
				*x += dx;
				DrawItemInfo(subobj, x, y, dx, dy);
				*x -= dx;
			}
		} while (kQ3Success == Q3Group_GetNextPosition(group, &position));
	}
}

#pragma mark -

InfoWindow::InfoWindow(TQ3GroupObject quesaData)
: inherited(kInfoWindowRect, "\pInfo"), mQuesaData(quesaData)
{
}

InfoWindow::~InfoWindow()
{
	if (mQuesaData) Q3Object_Dispose(mQuesaData);
}

void InfoWindow::Draw()
{
	TextFont(kFontIDGeneva);
	TextSize(9);
	
	short dy = 12;
	short y = dy;
	short x = 4;
	short dx = 12;
	
	if (not mQuesaData) {
		MoveTo(x,y);
		DrawString("\pno data");
	}
	else {
		DrawItemInfo(mQuesaData, &x, &y, dx, dy);
	}

	DrawGrowBox(false);
}
