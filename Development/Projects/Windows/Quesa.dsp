# Microsoft Developer Studio Project File - Name="Quesa" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Quesa - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Quesa.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Quesa.mak" CFG="Quesa - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Quesa - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Quesa - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Quesa - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32_EXPORTING" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QUESA_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../Source/Core/Geometry" /I "../../Source/Core/Glue" /I "../../Source/Core/Support" /I "../../Source/Core/System" /I "../../Source/Platform/Windows" /I "../../Source/Renderers/Common" /I "../../Source/Renderers/Generic" /I "../../Source/Renderers/Interactive" /I "../../Source/Renderers/Wireframe" /I "../../Source/FileFormats" /I "../../Source/FileFormats/Readers/3dmf" /I "../../Source/FileFormats/Writers/3dmf" /I "../../../SDK/Includes/Quesa" /D "QUESA_OS_WIN32" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WIN32_EXPORTING" /YX"E3Prefix.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../SDK/Libraries/Windows/Release/Quesa.dll" /implib:"../../../SDK/Libraries/Windows/Stub/Quesa.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Quesa - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32_EXPORTING" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QUESA_EXPORTS" /D "Q3_DEBUG" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Source/Core/Geometry" /I "../../Source/Core/Glue" /I "../../Source/Core/Support" /I "../../Source/Core/System" /I "../../Source/Platform/Windows" /I "../../Source/Renderers/Common" /I "../../Source/Renderers/Generic" /I "../../Source/Renderers/Interactive" /I "../../Source/Renderers/Wireframe" /I "../../Source/FileFormats" /I "../../Source/FileFormats/Readers/3dmf" /I "../../Source/FileFormats/Writers/3dmf" /I "../../../SDK/Includes/Apple" /I "../../../SDK/Includes/Quesa" /D "QUESA_OS_WIN32" /D "Q3_DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WIN32_EXPORTING" /FR /YX"E3Prefix.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../SDK/Libraries/Windows/Debug/Quesa.dll" /implib:"../../../SDK/Libraries/Windows/Stub/Quesa.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Quesa - Win32 Release"
# Name "Quesa - Win32 Debug"
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Group "Geometry"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3Geometry.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryBox.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryCone.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryCylinder.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryDisk.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryEllipse.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryEllipsoid.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryGeneralPolygon.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryLine.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryMarker.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryMesh.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryNURBCurve.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryNURBPatch.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryPixmapMarker.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryPoint.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryPolygon.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryPolyhedron.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryPolyLine.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryTorus.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryTriangle.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryTriGrid.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Geometry\E3GeometryTriMesh.c
# End Source File
# End Group
# Begin Group "Glue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DCamera.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DController.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DCustomElements.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DDrawContext.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DErrors.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DExtension.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DGeometry.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DGroup.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DIO.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DLight.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DMain.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DMath.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DMemory.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DPick.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DRenderer.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DSet.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DShader.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DStorage.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DString.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DStyle.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DTransform.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Glue\QD3DView.c
# End Source File
# End Group
# Begin Group "Support"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3ArrayOrList.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3ClassTree.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3Compatibility.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3ErrorManager.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3Globals.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3HashTable.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3Pool.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3System.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\Support\E3Utils.c
# End Source File
# End Group
# Begin Group "System"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Camera.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Controller.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3CustomElements.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3DrawContext.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Errors.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Extension.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Group.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3IO.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3IOData.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Light.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Main.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Math.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Memory.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Pick.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Renderer.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Set.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Shader.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Storage.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3String.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Style.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Texture.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3Transform.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Core\System\E3View.c
# End Source File
# End Group
# End Group
# Begin Group "Platform"

# PROP Default_Filter ""
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Platform\Windows\E3WindowsDebug.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Platform\Windows\E3WindowsDrawContext.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Platform\Windows\E3WindowsStorage.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Platform\Windows\E3WindowsSystem.c
# End Source File
# End Group
# End Group
# Begin Group "Renderers"

# PROP Default_Filter ""
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Renderers\Common\GLCamera.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Common\GLDrawContext.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Common\GLUtils.c
# End Source File
# End Group
# Begin Group "Generic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Renderers\Generic\GNGeometry.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Generic\GNRegister.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Generic\GNRenderer.c
# End Source File
# End Group
# Begin Group "Interactive"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Renderers\Interactive\IRGeometry.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Interactive\IRLights.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Interactive\IRRegister.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Interactive\IRRenderer.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Interactive\IRUpdate.c
# End Source File
# End Group
# Begin Group "Wireframe"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Renderers\Wireframe\WFGeometry.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Wireframe\WFRegister.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Wireframe\WFRenderer.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Renderers\Wireframe\WFUpdate.c
# End Source File
# End Group
# End Group
# Begin Group "FileFormats"

# PROP Default_Filter ""
# Begin Group "Readers"

# PROP Default_Filter ""
# Begin Group "3dmf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\FileFormats\Readers\3DMF\E3FFR_3DMF.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\FileFormats\Readers\3DMF\E3FFR_3DMF_Bin.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\FileFormats\Readers\3DMF\E3FFR_3DMF_Geometry.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\FileFormats\Readers\3DMF\E3FFR_3DMF_Text.c
# End Source File
# End Group
# End Group
# Begin Group "Writers"

# PROP Default_Filter ""
# Begin Group "3dm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\FileFormats\Writers\3DMF\E3FFW_3DMFBin_Geometry.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\FileFormats\Writers\3DMF\E3FFW_3DMFBin_Register.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\FileFormats\Writers\3DMF\E3FFW_3DMFBin_Writer.c
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\..\Source\FileFormats\E3IOFileFormat.c
# End Source File
# End Group
# End Group
# End Target
# End Project
