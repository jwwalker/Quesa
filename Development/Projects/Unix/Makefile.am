# Directories
SRC      = $(srcdir)/Source
OBJ      = ./obj

GLUE     = /Core/Glue
SYSTEM   = /Core/System
SUPPORT  = /Core/Support
GEOMETRY = /Core/Geometry
RENDERER = /Renderers
FFORMAT  = /FileFormats
FFORMATR = /FileFormats/Readers
FFORMATW = /FileFormats/Writers
PLATFORM = /Unix
QUESAAPI = $(srcdir)/APIincludes

QUESAINCLUDES= -I${QUESAAPI}		                \
          -I${SRC}${GLUE}                          \
          -I${SRC}${SYSTEM}                        \
          -I${SRC}${SUPPORT}                       \
          -I${SRC}${GEOMETRY}                      \
          -I${SRC}${FFORMAT}                       \
          -I${SRC}${FFORMATR}/3DMF                 \
          -I${SRC}${FFORMATW}/3DMF                 \
          -I${SRC}${RENDERER}/Common               \
          -I${SRC}${RENDERER}/Generic              \
          -I${SRC}${RENDERER}/Wireframe            \
          -I${SRC}${RENDERER}/Interactive          \
          -I${SRC}${PLATFORM}  


lib_LTLIBRARIES= libquesa.la

# Flags
WARN= -Wall -Wpointer-arith -Wno-cast-qual -Waggregate-return         \
       -Wstrict-prototypes -Wmissing-prototypes \
       -Wredundant-decls -Wno-multichar -Wno-conversion                \
       -Wno-unknown-pragmas -Wno-unused
					 

pkginclude_HEADERS= ${QUESAAPI}/Quesa.h				\
				${QUESAAPI}/CQ3ObjectRef.h	\
				${QUESAAPI}/QuesaCamera.h		\
				${QUESAAPI}/QuesaController.h		\
				${QUESAAPI}/QuesaCustomElements.h	\
				${QUESAAPI}/QuesaDrawContext.h		\
				${QUESAAPI}/QuesaErrors.h		\
				${QUESAAPI}/QuesaExtension.h		\
				${QUESAAPI}/QuesaGeometry.h		\
				${QUESAAPI}/QuesaGroup.h		\
				${QUESAAPI}/QuesaIO.h			\
				${QUESAAPI}/QuesaLight.h		\
				${QUESAAPI}/QuesaMath.h			\
				${QUESAAPI}/QuesaMemory.h		\
				${QUESAAPI}/QuesaPick.h			\
				${QUESAAPI}/QuesaRenderer.h		\
				${QUESAAPI}/QuesaSet.h			\
				${QUESAAPI}/QuesaShader.h		\
				${QUESAAPI}/QuesaStorage.h		\
				${QUESAAPI}/QuesaString.h		\
				${QUESAAPI}/QuesaStyle.h		\
				${QUESAAPI}/QuesaTransform.h		\
				${QUESAAPI}/QuesaView.h			\
				${QUESAAPI}/QuesaViewer.h

myIncludes= ${pkginclude_HEADERS} \
             ${SRC}${SYSTEM}/E3Camera.h                   \
             ${SRC}${SYSTEM}/E3Controller.h               \
             ${SRC}${SYSTEM}/E3CustomElements.h           \
             ${SRC}${SYSTEM}/E3DrawContext.h              \
             ${SRC}${SYSTEM}/E3Errors.h                   \
             ${SRC}${SYSTEM}/E3Extension.h                \
             ${SRC}${SYSTEM}/E3Group.h                    \
             ${SRC}${SYSTEM}/E3IO.h                       \
             ${SRC}${SYSTEM}/E3IOData.h                   \
             ${SRC}${SYSTEM}/E3Light.h                    \
             ${SRC}${SYSTEM}/E3Main.h                     \
             ${SRC}${SYSTEM}/E3Math.h                     \
             ${SRC}${SYSTEM}/E3Memory.h                   \
             ${SRC}${SYSTEM}/E3Pick.h                     \
             ${SRC}${SYSTEM}/E3Renderer.h                 \
             ${SRC}${SYSTEM}/E3Set.h                      \
             ${SRC}${SYSTEM}/E3Shader.h                   \
             ${SRC}${SYSTEM}/E3Storage.h                  \
             ${SRC}${SYSTEM}/E3String.h                   \
             ${SRC}${SYSTEM}/E3Style.h                    \
             ${SRC}${SYSTEM}/E3Texture.h                  \
             ${SRC}${SYSTEM}/E3Transform.h                \
             ${SRC}${SYSTEM}/E3View.h                     \
             ${SRC}${SUPPORT}/E3ArrayOrList.h             \
             ${SRC}${SUPPORT}/E3ClassTree.h               \
             ${SRC}${SUPPORT}/E3Compatibility.h           \
             ${SRC}${SUPPORT}/E3ErrorManager.h            \
             ${SRC}${SUPPORT}/E3Globals.h                 \
             ${SRC}${SUPPORT}/E3HashTable.h               \
             ${SRC}${SUPPORT}/E3Pool.h                    \
             ${SRC}${SUPPORT}/E3System.h                  \
             ${SRC}${SUPPORT}/E3Tessellate.h              \
             ${SRC}${SUPPORT}/E3Utils.h                   \
             ${SRC}${SUPPORT}/E3Prefix.h                  \
             ${SRC}${SUPPORT}/E3Debug.h                   \
             ${SRC}${SUPPORT}/E3Version.h                 \
             ${SRC}${SUPPORT}/E3StackCrawl.h              \
             ${SRC}${GEOMETRY}/E3Geometry.h               \
             ${SRC}${GEOMETRY}/E3GeometryBox.h            \
             ${SRC}${GEOMETRY}/E3GeometryCone.h           \
             ${SRC}${GEOMETRY}/E3GeometryCylinder.h       \
             ${SRC}${GEOMETRY}/E3GeometryDisk.h           \
             ${SRC}${GEOMETRY}/E3GeometryEllipse.h        \
             ${SRC}${GEOMETRY}/E3GeometryEllipsoid.h      \
             ${SRC}${GEOMETRY}/E3GeometryGeneralPolygon.h \
             ${SRC}${GEOMETRY}/E3GeometryLine.h           \
             ${SRC}${GEOMETRY}/E3GeometryMarker.h         \
             ${SRC}${GEOMETRY}/E3GeometryMesh.h           \
             ${SRC}${GEOMETRY}/E3GeometryNURBCurve.h      \
             ${SRC}${GEOMETRY}/E3GeometryNURBPatch.h      \
             ${SRC}${GEOMETRY}/E3GeometryPixmapMarker.h   \
             ${SRC}${GEOMETRY}/E3GeometryPoint.h          \
             ${SRC}${GEOMETRY}/E3GeometryPolygon.h        \
             ${SRC}${GEOMETRY}/E3GeometryPolyhedron.h     \
             ${SRC}${GEOMETRY}/E3GeometryPolyLine.h       \
             ${SRC}${GEOMETRY}/E3GeometryTriangle.h       \
             ${SRC}${GEOMETRY}/E3GeometryTriGrid.h        \
             ${SRC}${GEOMETRY}/E3GeometryTriMesh.h        \
             ${SRC}${GEOMETRY}/E3GeometryTorus.h          \
             ${SRC}${FFORMAT}/E3IOFileFormat.h            \
             ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF.h          \
             ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Bin.h      \
             ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Text.h     \
             ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Geometry.h \
             ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Geometry.h \
             ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Register.h \
             ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Writer.h \
             ${SRC}${RENDERER}/Common/GLCamera.h          \
             ${SRC}${RENDERER}/Common/GLPrefix.h           \
             ${SRC}${RENDERER}/Common/GLUtils.h           \
             ${SRC}${RENDERER}/Common/GLDrawContext.h     \
             ${SRC}${RENDERER}/Common/GLTextureManager.h     \
             ${SRC}${RENDERER}/Generic/GNPrefix.h       \
             ${SRC}${RENDERER}/Generic/GNGeometry.h       \
             ${SRC}${RENDERER}/Generic/GNRegister.h       \
             ${SRC}${RENDERER}/Generic/GNRenderer.h       \
             ${SRC}${RENDERER}/Wireframe/WFPrefix.h     \
             ${SRC}${RENDERER}/Wireframe/WFGeometry.h     \
             ${SRC}${RENDERER}/Wireframe/WFRegister.h     \
             ${SRC}${RENDERER}/Wireframe/WFRenderer.h     \
             ${SRC}${RENDERER}/Wireframe/WFUpdate.h       \
             ${SRC}${RENDERER}/Interactive/IRPrefix.h   \
             ${SRC}${RENDERER}/Interactive/IRGeometry.h   \
             ${SRC}${RENDERER}/Interactive/IRGeometryTriMesh.h \
             ${SRC}${RENDERER}/Interactive/IRRegister.h   \
             ${SRC}${RENDERER}/Interactive/IRRenderer.h   \
             ${SRC}${RENDERER}/Interactive/IRTexture.h    \
             ${SRC}${RENDERER}/Interactive/IRTransparent.h  \
             ${SRC}${RENDERER}/Interactive/IRTriBuffer.h  \
             ${SRC}${RENDERER}/Interactive/IRLights.h     \
             ${SRC}${RENDERER}/Interactive/IRUpdate.h     \
             ${SRC}${PLATFORM}/E3UnixPrefix.h       

          

libquesa_la_SOURCES=	${myIncludes}            \
			${SRC}${GLUE}/QD3DCamera.c                    \
             ${SRC}${GLUE}/QD3DController.c               \
             ${SRC}${GLUE}/QD3DCustomElements.c           \
             ${SRC}${GLUE}/QD3DDrawContext.c              \
             ${SRC}${GLUE}/QD3DErrors.c                   \
             ${SRC}${GLUE}/QD3DExtension.c                \
             ${SRC}${GLUE}/QD3DGeometry.c                 \
             ${SRC}${GLUE}/QD3DGroup.c                    \
             ${SRC}${GLUE}/QD3DIO.c                       \
             ${SRC}${GLUE}/QD3DLight.c                    \
             ${SRC}${GLUE}/QD3DMain.c                     \
             ${SRC}${GLUE}/QD3DMath.c                     \
             ${SRC}${GLUE}/QD3DMemory.c                   \
             ${SRC}${GLUE}/QD3DPick.c                     \
             ${SRC}${GLUE}/QD3DRenderer.c                 \
             ${SRC}${GLUE}/QD3DSet.c                      \
             ${SRC}${GLUE}/QD3DShader.c                   \
             ${SRC}${GLUE}/QD3DStorage.c                  \
             ${SRC}${GLUE}/QD3DString.c                   \
             ${SRC}${GLUE}/QD3DStyle.c                    \
             ${SRC}${GLUE}/QD3DTransform.c                \
             ${SRC}${GLUE}/QD3DView.c                     \
             ${SRC}${SYSTEM}/E3Camera.c                   \
             ${SRC}${SYSTEM}/E3Controller.c               \
             ${SRC}${SYSTEM}/E3CustomElements.c           \
             ${SRC}${SYSTEM}/E3DrawContext.c              \
             ${SRC}${SYSTEM}/E3Errors.c                   \
             ${SRC}${SYSTEM}/E3Extension.c                \
             ${SRC}${SYSTEM}/E3Group.c                    \
             ${SRC}${SYSTEM}/E3IO.c                       \
             ${SRC}${SYSTEM}/E3IOData.c                   \
             ${SRC}${SYSTEM}/E3Light.c                    \
             ${SRC}${SYSTEM}/E3Main.c                     \
             ${SRC}${SYSTEM}/E3Math.c                     \
             ${SRC}${SYSTEM}/E3Memory.c                   \
             ${SRC}${SYSTEM}/E3Pick.c                     \
             ${SRC}${SYSTEM}/E3Renderer.c                 \
             ${SRC}${SYSTEM}/E3Set.c                      \
             ${SRC}${SYSTEM}/E3Shader.c                   \
             ${SRC}${SYSTEM}/E3Storage.c                  \
             ${SRC}${SYSTEM}/E3String.c                   \
             ${SRC}${SYSTEM}/E3Style.c                    \
             ${SRC}${SYSTEM}/E3Texture.c                  \
             ${SRC}${SYSTEM}/E3Transform.c                \
             ${SRC}${SYSTEM}/E3View.c                     \
             ${SRC}${SUPPORT}/E3ArrayOrList.c             \
             ${SRC}${SUPPORT}/E3ClassTree.c               \
             ${SRC}${SUPPORT}/E3Compatibility.c           \
             ${SRC}${SUPPORT}/E3ErrorManager.c            \
             ${SRC}${SUPPORT}/E3Globals.c                 \
             ${SRC}${SUPPORT}/E3HashTable.c               \
             ${SRC}${SUPPORT}/E3Pool.c                    \
             ${SRC}${SUPPORT}/E3System.c                  \
             ${SRC}${SUPPORT}/E3Tessellate.c              \
             ${SRC}${SUPPORT}/E3Utils.c                   \
             ${SRC}${GEOMETRY}/E3Geometry.c               \
             ${SRC}${GEOMETRY}/E3GeometryBox.c            \
             ${SRC}${GEOMETRY}/E3GeometryCone.c           \
             ${SRC}${GEOMETRY}/E3GeometryCylinder.c       \
             ${SRC}${GEOMETRY}/E3GeometryDisk.c           \
             ${SRC}${GEOMETRY}/E3GeometryEllipse.c        \
             ${SRC}${GEOMETRY}/E3GeometryEllipsoid.c      \
             ${SRC}${GEOMETRY}/E3GeometryGeneralPolygon.c \
             ${SRC}${GEOMETRY}/E3GeometryLine.c           \
             ${SRC}${GEOMETRY}/E3GeometryMarker.c         \
             ${SRC}${GEOMETRY}/E3GeometryMesh.c           \
             ${SRC}${GEOMETRY}/E3GeometryNURBCurve.c      \
             ${SRC}${GEOMETRY}/E3GeometryNURBPatch.c      \
             ${SRC}${GEOMETRY}/E3GeometryPixmapMarker.c   \
             ${SRC}${GEOMETRY}/E3GeometryPoint.c          \
             ${SRC}${GEOMETRY}/E3GeometryPolygon.c        \
             ${SRC}${GEOMETRY}/E3GeometryPolyhedron.c     \
             ${SRC}${GEOMETRY}/E3GeometryPolyLine.c       \
             ${SRC}${GEOMETRY}/E3GeometryTriangle.c       \
             ${SRC}${GEOMETRY}/E3GeometryTriGrid.c        \
             ${SRC}${GEOMETRY}/E3GeometryTriMesh.c        \
             ${SRC}${GEOMETRY}/E3GeometryTorus.c          \
             ${SRC}${FFORMAT}/E3IOFileFormat.c            \
             ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF.c          \
             ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Bin.c      \
             ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Text.c     \
             ${SRC}${FFORMATR}/3DMF/E3FFR_3DMF_Geometry.c \
             ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Geometry.c \
             ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Register.c \
             ${SRC}${FFORMATW}/3DMF/E3FFW_3DMFBin_Writer.c \
             ${SRC}${RENDERER}/Common/GLCamera.c          \
             ${SRC}${RENDERER}/Common/GLUtils.c           \
             ${SRC}${RENDERER}/Common/GLDrawContext.c     \
             ${SRC}${RENDERER}/Common/GLTextureManager.c     \
             ${SRC}${RENDERER}/Generic/GNGeometry.c       \
             ${SRC}${RENDERER}/Generic/GNRegister.c       \
             ${SRC}${RENDERER}/Generic/GNRenderer.c       \
             ${SRC}${RENDERER}/Wireframe/WFGeometry.c     \
             ${SRC}${RENDERER}/Wireframe/WFRegister.c     \
             ${SRC}${RENDERER}/Wireframe/WFRenderer.c     \
             ${SRC}${RENDERER}/Wireframe/WFUpdate.c       \
             ${SRC}${RENDERER}/Interactive/IRGeometry.c   \
             ${SRC}${RENDERER}/Interactive/IRGeometryTriMesh.c \
             ${SRC}${RENDERER}/Interactive/IRRegister.c   \
             ${SRC}${RENDERER}/Interactive/IRRenderer.c   \
             ${SRC}${RENDERER}/Interactive/IRTexture.c    \
             ${SRC}${RENDERER}/Interactive/IRTransparent.c  \
             ${SRC}${RENDERER}/Interactive/IRTriBuffer.c  \
             ${SRC}${RENDERER}/Interactive/IRLights.c     \
             ${SRC}${RENDERER}/Interactive/IRUpdate.c     \
             ${SRC}${PLATFORM}/E3UnixDebug.c              \
             ${SRC}${PLATFORM}/E3UnixStorage.c            \
             ${SRC}${PLATFORM}/E3UnixSystem.c             \
             ${SRC}${PLATFORM}/E3UnixDrawContext.c       


libquesa_la_CFLAGS= -x c++ -DQUESA_OS_UNIX=1 $(WARN) $(QUESAINCLUDES)
libquesa_la_LIBADD= -lm -lc -lX11 -lGL -lGLU
