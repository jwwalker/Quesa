#***************************************************************************
# Makefile for Unix builds
# dair@webthing.net
#
# NB - Real makefile hackers required: should we be using autoconf for this?
#***************************************************************************



# Directories
SRC      = ../../Source
SDK      = ../../../SDK
OBJ      = ./obj
LIB      = ${SDK}/Libraries/Unix/Debug

GLUE     = /Core/Glue
SYSTEM   = /Core/System
SUPPORT  = /Core/Support
GEOMETRY = /Core/Geometry
RENDERER = /Renderers
FFORMAT  = /FileFormats
FFORMATR = /FileFormats/Readers
FFORMATW = /FileFormats/Writers
PLATFORM = /Platform/Unix



# Flags
WARN = -Wall -Wpointer-arith -Wcast-qual -Waggregate-return            \
       -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
       -Wredundant-decls -Wno-multichar -Wno-conversion

INCLUDE = -I ${SDK}"/Includes/Apple"                \
          -I ${SDK}"/Includes/Quesa"                \
          -I ${SRC}${GLUE}                          \
          -I ${SRC}${SYSTEM}                        \
          -I ${SRC}${SUPPORT}                       \
          -I ${SRC}${GEOMETRY}                      \
          -I ${SRC}${FFORMAT}                       \
          -I ${SRC}${FFORMATR}/3DMF                 \
          -I ${SRC}${FFORMATW}/3DMF                 \
          -I ${SRC}${RENDERER}/Common               \
          -I ${SRC}${RENDERER}/Generic              \
          -I ${SRC}${RENDERER}/Wireframe            \
          -I ${SRC}${RENDERER}/Interactive          \
          -I ${SRC}${PLATFORM}                      \

CFLAGS = $(WARN) $(INCLUDE) -c -DQUESA_OS_UNIX=1

LDFLAGS = -export-dynamic -lc -lm -lX11 -lGL -lGLU -L/usr/X11/lib



# Platform specific (must be a better way to do this?)
# dair, to do
MACHINE = $(shell uname -m)
ifneq ($(MACHINE), ppc)
	CFLAGS += -DQUESA_HOST_IS_BIG_ENDIAN=0
endif



# Tools
CC      = gcc
LD      = ar cqs
COMPILE = $(CC) $(CFLAGS) $^ -o $@
TARGETS = ${LIB}/libquesa.a





#***************************************************************************
# File lists
#***************************************************************************
QUESA_DIRS = ${LIB}/                          \
             ${OBJ}${GLUE}                    \
             ${OBJ}${SYSTEM}                  \
             ${OBJ}${SUPPORT}                 \
             ${OBJ}${GEOMETRY}                \
             ${OBJ}${FFORMAT}                 \
             ${OBJ}${FFORMATR}                \
             ${OBJ}${FFORMATW}                \
             ${OBJ}${FFORMATR}/3DMF           \
             ${OBJ}${FFORMATW}/3DMF           \
             ${OBJ}${RENDERER}                \
             ${OBJ}${RENDERER}/Common         \
             ${OBJ}${RENDERER}/Generic        \
             ${OBJ}${RENDERER}/Wireframe      \
             ${OBJ}${RENDERER}/Interactive    \
             ${OBJ}${PLATFORM}                \

QUESA_OBJS = ${OBJ}${GLUE}/QD3DCamera.o                   \
             ${OBJ}${GLUE}/QD3DController.o               \
             ${OBJ}${GLUE}/QD3DCustomElements.o           \
             ${OBJ}${GLUE}/QD3DDrawContext.o              \
             ${OBJ}${GLUE}/QD3DErrors.o                   \
             ${OBJ}${GLUE}/QD3DExtension.o                \
             ${OBJ}${GLUE}/QD3DGeometry.o                 \
             ${OBJ}${GLUE}/QD3DGroup.o                    \
             ${OBJ}${GLUE}/QD3DIO.o                       \
             ${OBJ}${GLUE}/QD3DLight.o                    \
             ${OBJ}${GLUE}/QD3DMain.o                     \
             ${OBJ}${GLUE}/QD3DMath.o                     \
             ${OBJ}${GLUE}/QD3DPick.o                     \
             ${OBJ}${GLUE}/QD3DRenderer.o                 \
             ${OBJ}${GLUE}/QD3DSet.o                      \
             ${OBJ}${GLUE}/QD3DShader.o                   \
             ${OBJ}${GLUE}/QD3DStorage.o                  \
             ${OBJ}${GLUE}/QD3DString.o                   \
             ${OBJ}${GLUE}/QD3DStyle.o                    \
             ${OBJ}${GLUE}/QD3DTransform.o                \
             ${OBJ}${GLUE}/QD3DView.o                     \
             ${OBJ}${SYSTEM}/E3Camera.o                   \
             ${OBJ}${SYSTEM}/E3Controller.o               \
             ${OBJ}${SYSTEM}/E3CustomElements.o           \
             ${OBJ}${SYSTEM}/E3DrawContext.o              \
             ${OBJ}${SYSTEM}/E3Errors.o                   \
             ${OBJ}${SYSTEM}/E3Extension.o                \
             ${OBJ}${SYSTEM}/E3Group.o                    \
             ${OBJ}${SYSTEM}/E3IO.o                       \
             ${OBJ}${SYSTEM}/E3IOData.o                   \
             ${OBJ}${SYSTEM}/E3Light.o                    \
             ${OBJ}${SYSTEM}/E3Main.o                     \
             ${OBJ}${SYSTEM}/E3Math.o                     \
             ${OBJ}${SYSTEM}/E3Pick.o                     \
             ${OBJ}${SYSTEM}/E3Renderer.o                 \
             ${OBJ}${SYSTEM}/E3Set.o                      \
             ${OBJ}${SYSTEM}/E3Shader.o                   \
             ${OBJ}${SYSTEM}/E3Storage.o                  \
             ${OBJ}${SYSTEM}/E3String.o                   \
             ${OBJ}${SYSTEM}/E3Style.o                    \
             ${OBJ}${SYSTEM}/E3Texture.o                  \
             ${OBJ}${SYSTEM}/E3Transform.o                \
             ${OBJ}${SYSTEM}/E3View.o                     \
             ${OBJ}${SUPPORT}/E3ArrayOrList.o             \
             ${OBJ}${SUPPORT}/E3ClassTree.o               \
             ${OBJ}${SUPPORT}/E3Compatibility.o           \
             ${OBJ}${SUPPORT}/E3ErrorManager.o            \
             ${OBJ}${SUPPORT}/E3Globals.o                 \
             ${OBJ}${SUPPORT}/E3HashTable.o               \
             ${OBJ}${SUPPORT}/E3Memory.o                  \
             ${OBJ}${SUPPORT}/E3Pool.o                    \
             ${OBJ}${SUPPORT}/E3System.o                  \
             ${OBJ}${SUPPORT}/E3Utils.o                   \
             ${OBJ}${GEOMETRY}/E3Geometry.o               \
             ${OBJ}${GEOMETRY}/E3GeometryBox.o            \
             ${OBJ}${GEOMETRY}/E3GeometryCone.o           \
             ${OBJ}${GEOMETRY}/E3GeometryCylinder.o       \
             ${OBJ}${GEOMETRY}/E3GeometryDisk.o           \
             ${OBJ}${GEOMETRY}/E3GeometryEllipse.o        \
             ${OBJ}${GEOMETRY}/E3GeometryEllipsoid.o      \
             ${OBJ}${GEOMETRY}/E3GeometryGeneralPolygon.o \
             ${OBJ}${GEOMETRY}/E3GeometryLine.o           \
             ${OBJ}${GEOMETRY}/E3GeometryMarker.o         \
             ${OBJ}${GEOMETRY}/E3GeometryMesh.o           \
             ${OBJ}${GEOMETRY}/E3GeometryNURBCurve.o      \
             ${OBJ}${GEOMETRY}/E3GeometryNURBPatch.o      \
             ${OBJ}${GEOMETRY}/E3GeometryPixmapMarker.o   \
             ${OBJ}${GEOMETRY}/E3GeometryPoint.o          \
             ${OBJ}${GEOMETRY}/E3GeometryPolygon.o        \
             ${OBJ}${GEOMETRY}/E3GeometryPolyhedron.o     \
             ${OBJ}${GEOMETRY}/E3GeometryPolyLine.o       \
             ${OBJ}${GEOMETRY}/E3GeometryTriangle.o       \
             ${OBJ}${GEOMETRY}/E3GeometryTriGrid.o        \
             ${OBJ}${GEOMETRY}/E3GeometryTriMesh.o        \
             ${OBJ}${GEOMETRY}/E3GeometryTorus.o          \
             ${OBJ}${FFORMAT}/E3IOFileFormat.o            \
             ${OBJ}${FFORMATR}/3DMF/E3FFR_3DMF.o          \
             ${OBJ}${FFORMATR}/3DMF/E3FFR_3DMF_Bin.o      \
             ${OBJ}${FFORMATR}/3DMF/E3FFR_3DMF_Text.o     \
             ${OBJ}${FFORMATR}/3DMF/E3FFR_3DMF_Geometry.o \
             ${OBJ}${FFORMATW}/3DMF/E3FFW_3DMFBin_Geometry.o \
             ${OBJ}${FFORMATW}/3DMF/E3FFW_3DMFBin_Register.o \
             ${OBJ}${FFORMATW}/3DMF/E3FFW_3DMFBin_Writer.o \
             ${OBJ}${RENDERER}/Common/GLCamera.o          \
             ${OBJ}${RENDERER}/Common/GLUtils.o           \
             ${OBJ}${RENDERER}/Common/GLDrawContext.o     \
             ${OBJ}${RENDERER}/Generic/GNGeometry.o       \
             ${OBJ}${RENDERER}/Generic/GNRegister.o       \
             ${OBJ}${RENDERER}/Generic/GNRenderer.o       \
             ${OBJ}${RENDERER}/Wireframe/WFGeometry.o     \
             ${OBJ}${RENDERER}/Wireframe/WFRegister.o     \
             ${OBJ}${RENDERER}/Wireframe/WFRenderer.o     \
             ${OBJ}${RENDERER}/Wireframe/WFUpdate.o       \
             ${OBJ}${RENDERER}/Interactive/IRGeometry.o   \
             ${OBJ}${RENDERER}/Interactive/IRRegister.o   \
             ${OBJ}${RENDERER}/Interactive/IRRenderer.o   \
             ${OBJ}${RENDERER}/Interactive/IRLights.o     \
             ${OBJ}${RENDERER}/Interactive/IRUpdate.o     \
             ${OBJ}${PLATFORM}/E3UnixDebug.o              \
             ${OBJ}${PLATFORM}/E3UnixStorage.o            \
             ${OBJ}${PLATFORM}/E3UnixSystem.o             \
             ${OBJ}${PLATFORM}/E3UnixDrawContext.o        \





#***************************************************************************
# Top level targets
#***************************************************************************
all:: $(TARGETS)

clean::
	@rm -rf ${OBJ} $(TARGETS)

${QUESA_DIRS}:
	mkdir -p $@

${LIB}/libquesa.a: ${QUESA_DIRS} ${OBJ} $(QUESA_OBJS)
	$(LD) $@ $(QUESA_OBJS)





#***************************************************************************
# Individual rules
#***************************************************************************
${OBJ}${GLUE}/QD3DCamera.o: ${SRC}${GLUE}/QD3DCamera.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DController.o: ${SRC}${GLUE}/QD3DController.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DCustomElements.o: ${SRC}${GLUE}/QD3DCustomElements.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DDrawContext.o: ${SRC}${GLUE}/QD3DDrawContext.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DErrors.o: ${SRC}${GLUE}/QD3DErrors.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DExtension.o: ${SRC}${GLUE}/QD3DExtension.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DGeometry.o: ${SRC}${GLUE}/QD3DGeometry.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DGroup.o: ${SRC}${GLUE}/QD3DGroup.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DIO.o: ${SRC}${GLUE}/QD3DIO.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DLight.o: ${SRC}${GLUE}/QD3DLight.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DMain.o: ${SRC}${GLUE}/QD3DMain.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DMath.o: ${SRC}${GLUE}/QD3DMath.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DPick.o: ${SRC}${GLUE}/QD3DPick.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DRenderer.o: ${SRC}${GLUE}/QD3DRenderer.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DSet.o: ${SRC}${GLUE}/QD3DSet.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DShader.o: ${SRC}${GLUE}/QD3DShader.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DStorage.o: ${SRC}${GLUE}/QD3DStorage.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DString.o: ${SRC}${GLUE}/QD3DString.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DStyle.o: ${SRC}${GLUE}/QD3DStyle.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DTransform.o: ${SRC}${GLUE}/QD3DTransform.c
	$(COMPILE)

${OBJ}${GLUE}/QD3DView.o: ${SRC}${GLUE}/QD3DView.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Camera.o: ${SRC}${SYSTEM}/E3Camera.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Controller.o: ${SRC}${SYSTEM}/E3Controller.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3CustomElements.o: ${SRC}${SYSTEM}/E3CustomElements.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3DrawContext.o: ${SRC}${SYSTEM}/E3DrawContext.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Errors.o: ${SRC}${SYSTEM}/E3Errors.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Extension.o: ${SRC}${SYSTEM}/E3Extension.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Group.o: ${SRC}${SYSTEM}/E3Group.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3IO.o: ${SRC}${SYSTEM}/E3IO.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3IOData.o: ${SRC}${SYSTEM}/E3IOData.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Light.o: ${SRC}${SYSTEM}/E3Light.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Main.o: ${SRC}${SYSTEM}/E3Main.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Math.o: ${SRC}${SYSTEM}/E3Math.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Pick.o: ${SRC}${SYSTEM}/E3Pick.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Renderer.o: ${SRC}${SYSTEM}/E3Renderer.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Set.o: ${SRC}${SYSTEM}/E3Set.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Shader.o: ${SRC}${SYSTEM}/E3Shader.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Storage.o: ${SRC}${SYSTEM}/E3Storage.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3String.o: ${SRC}${SYSTEM}/E3String.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Style.o: ${SRC}${SYSTEM}/E3Style.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Texture.o: ${SRC}${SYSTEM}/E3Texture.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3Transform.o: ${SRC}${SYSTEM}/E3Transform.c
	$(COMPILE)

${OBJ}${SYSTEM}/E3View.o: ${SRC}${SYSTEM}/E3View.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3ArrayOrList.o: ${SRC}${SUPPORT}/E3ArrayOrList.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3ClassTree.o: ${SRC}${SUPPORT}/E3ClassTree.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3Compatibility.o: ${SRC}${SUPPORT}/E3Compatibility.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3ErrorManager.o: ${SRC}${SUPPORT}/E3ErrorManager.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3Globals.o: ${SRC}${SUPPORT}/E3Globals.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3HashTable.o: ${SRC}${SUPPORT}/E3HashTable.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3Memory.o: ${SRC}${SUPPORT}/E3Memory.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3Pool.o: ${SRC}${SUPPORT}/E3Pool.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3System.o: ${SRC}${SUPPORT}/E3System.c
	$(COMPILE)

${OBJ}${SUPPORT}/E3Utils.o: ${SRC}${SUPPORT}/E3Utils.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3Geometry.o: ${SRC}${GEOMETRY}/E3Geometry.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryBox.o: ${SRC}${GEOMETRY}/E3GeometryBox.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryCone.o: ${SRC}${GEOMETRY}/E3GeometryCone.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryCylinder.o: ${SRC}${GEOMETRY}/E3GeometryCylinder.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryDisk.o: ${SRC}${GEOMETRY}/E3GeometryDisk.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryEllipse.o: ${SRC}${GEOMETRY}/E3GeometryEllipse.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryEllipsoid.o: ${SRC}${GEOMETRY}/E3GeometryEllipsoid.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryGeneralPolygon.o: ${SRC}${GEOMETRY}/E3GeometryGeneralPolygon.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryLine.o: ${SRC}${GEOMETRY}/E3GeometryLine.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryMarker.o: ${SRC}${GEOMETRY}/E3GeometryMarker.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryMesh.o: ${SRC}${GEOMETRY}/E3GeometryMesh.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryNURBCurve.o: ${SRC}${GEOMETRY}/E3GeometryNURBCurve.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryNURBPatch.o: ${SRC}${GEOMETRY}/E3GeometryNURBPatch.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryPixmapMarker.o: ${SRC}${GEOMETRY}/E3GeometryPixmapMarker.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryPoint.o: ${SRC}${GEOMETRY}/E3GeometryPoint.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryPolygon.o: ${SRC}${GEOMETRY}/E3GeometryPolygon.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryPolyhedron.o: ${SRC}${GEOMETRY}/E3GeometryPolyhedron.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryPolyLine.o: ${SRC}${GEOMETRY}/E3GeometryPolyLine.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryTriangle.o: ${SRC}${GEOMETRY}/E3GeometryTriangle.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryTriGrid.o: ${SRC}${GEOMETRY}/E3GeometryTriGrid.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryTriMesh.o: ${SRC}${GEOMETRY}/E3GeometryTriMesh.c
	$(COMPILE)

${OBJ}${GEOMETRY}/E3GeometryTorus.o: ${SRC}${GEOMETRY}/E3GeometryTorus.c
	$(COMPILE)

${OBJ}${FFORMAT}/E3IOFileFormat.o: ${SRC}${FFORMAT}/E3IOFileFormat.c
	$(COMPILE)

${OBJ}${FFORMATR}/3DMF/E3FFR_3DMF.o: ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF.c
	$(COMPILE)

${OBJ}${FFORMATR}/3DMF/E3FFR_3DMF_Bin.o: ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Bin.c
	$(COMPILE)

${OBJ}${FFORMATR}/3DMF/E3FFR_3DMF_Text.o: ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Text.c
	$(COMPILE)

${OBJ}${FFORMATR}/3DMF/E3FFR_3DMF_Geometry.o: ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Geometry.c
	$(COMPILE)

${OBJ}${FFORMATW}/3DMF/E3FFW_3DMFBin_Geometry.o: ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Geometry.c
	$(COMPILE)

${OBJ}${FFORMATW}/3DMF/E3FFW_3DMFBin_Register.o: ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Register.c
	$(COMPILE)

${OBJ}${FFORMATW}/3DMF/E3FFW_3DMFBin_Writer.o: ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Writer.c
	$(COMPILE)

${OBJ}${RENDERER}/Common/GLCamera.o: ${SRC}${RENDERER}/Common/GLCamera.c
	$(COMPILE)

${OBJ}${RENDERER}/Common/GLUtils.o: ${SRC}${RENDERER}/Common/GLUtils.c
	$(COMPILE)

${OBJ}${RENDERER}/Common/GLDrawContext.o: ${SRC}${RENDERER}/Common/GLDrawContext.c
	$(COMPILE)

${OBJ}${RENDERER}/Generic/GNGeometry.o: ${SRC}${RENDERER}/Generic/GNGeometry.c
	$(COMPILE)

${OBJ}${RENDERER}/Generic/GNRegister.o: ${SRC}${RENDERER}/Generic/GNRegister.c
	$(COMPILE)

${OBJ}${RENDERER}/Generic/GNRenderer.o: ${SRC}${RENDERER}/Generic/GNRenderer.c
	$(COMPILE)

${OBJ}${RENDERER}/Wireframe/WFGeometry.o: ${SRC}${RENDERER}/Wireframe/WFGeometry.c
	$(COMPILE)

${OBJ}${RENDERER}/Wireframe/WFRegister.o: ${SRC}${RENDERER}/Wireframe/WFRegister.c
	$(COMPILE)

${OBJ}${RENDERER}/Wireframe/WFRenderer.o: ${SRC}${RENDERER}/Wireframe/WFRenderer.c
	$(COMPILE)

${OBJ}${RENDERER}/Wireframe/WFUpdate.o: ${SRC}${RENDERER}/Wireframe/WFUpdate.c
	$(COMPILE)

${OBJ}${RENDERER}/Interactive/IRGeometry.o: ${SRC}${RENDERER}/Interactive/IRGeometry.c
	$(COMPILE)

${OBJ}${RENDERER}/Interactive/IRRegister.o: ${SRC}${RENDERER}/Interactive/IRRegister.c
	$(COMPILE)

${OBJ}${RENDERER}/Interactive/IRRenderer.o: ${SRC}${RENDERER}/Interactive/IRRenderer.c
	$(COMPILE)

${OBJ}${RENDERER}/Interactive/IRLights.o: ${SRC}${RENDERER}/Interactive/IRLights.c
	$(COMPILE)

${OBJ}${RENDERER}/Interactive/IRUpdate.o: ${SRC}${RENDERER}/Interactive/IRUpdate.c
	$(COMPILE)

${OBJ}${PLATFORM}/E3UnixDebug.o: ${SRC}${PLATFORM}/E3UnixDebug.c
	$(COMPILE)

${OBJ}${PLATFORM}/E3UnixStorage.o: ${SRC}${PLATFORM}/E3UnixStorage.c
	$(COMPILE)

${OBJ}${PLATFORM}/E3UnixSystem.o: ${SRC}${PLATFORM}/E3UnixSystem.c
	$(COMPILE)

${OBJ}${PLATFORM}/E3UnixDrawContext.o: ${SRC}${PLATFORM}/E3UnixDrawContext.c
	$(COMPILE)


