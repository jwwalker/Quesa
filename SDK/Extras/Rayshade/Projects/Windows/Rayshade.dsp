# Microsoft Developer Studio Project File - Name="Rayshade" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Rayshade - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Rayshade.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Rayshade.mak" CFG="Rayshade - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Rayshade - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Rayshade - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Rayshade"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Rayshade - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RAYSHADE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "..\..\Sources\QD3DPlugIn" /I "..\..\Sources\Rayshade" /I "..\..\Sources\RTInterface" /I "..\..\Sources\QD3DPlugin" /I "..\..\..\..\Includes\Quesa" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RAYSHADE_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40e /d "NDEBUG"
# ADD RSC /l 0x40e /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\..\..\..\Libraries\Windows\Stub\Quesa.lib /nologo /dll /machine:I386 /out:"../../Bin/Release/Rayshade.q3x"

!ELSEIF  "$(CFG)" == "Rayshade - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RAYSHADE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GR /GX /ZI /Od /I "..\..\Sources\QD3DPlugin" /I "..\..\Sources\Rayshade" /I "..\..\Sources\RTInterface" /I "..\..\..\..\Includes\Quesa" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RAYSHADE_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40e /d "_DEBUG"
# ADD RSC /l 0x40e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\..\..\..\Libraries\Windows\Stub\Quesa.lib /nologo /dll /debug /machine:I386 /out:"../../Bin/Debug/Rayshade.q3x" /pdbtype:sept
# SUBTRACT LINK32 /verbose /incremental:no /nodefaultlib /force

!ENDIF 

# Begin Target

# Name "Rayshade - Win32 Release"
# Name "Rayshade - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Rayshade"

# PROP Default_Filter ""
# Begin Group "LibCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\color.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\common.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\error.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\expr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\expr.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\memory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\ray.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\rotate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\rotate.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\sampling.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\sampling.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\scale.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\scale.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\transform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\transform.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\translate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\translate.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\vecmath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\vector.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\xform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibCommon\xform.h
# End Source File
# End Group
# Begin Group "LibLight"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\extended.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\extended.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\infinite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\infinite.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\jittered.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\jittered.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\light.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\light.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\point.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\point.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\shadow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\spot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibLight\spot.h
# End Source File
# End Group
# Begin Group "LibText"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\blotch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\blotch.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\bump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\bump.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\checker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\checker.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\CImageTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\CImageTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\CTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\CTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\fbm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\fbm.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\fbmbump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\fbmbump.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\gloss.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\gloss.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\mapping.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\mapping.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\marble.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\marble.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\mount.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\mount.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\noise.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\sky.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\sky.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\stripe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\stripe.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\textaux.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\textureutils.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\windy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\windy.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\wood.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibText\wood.h
# End Source File
# End Group
# Begin Group "LibSurf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\atmosphere.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\atmosphere.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\fog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\fog.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\fogdeck.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\fogdeck.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\mist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\mist.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\surface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\surface.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibSurf\surfshade.cpp
# End Source File
# End Group
# Begin Group "LibObj"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\blob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\blob.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\bounds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\bounds.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\box.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\box.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\cone.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\cone.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\csg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\csg.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\cylinder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\cylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\disc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\disc.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\geom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\geom.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\grid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\grid.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\hf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\hf.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\instance.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\instance.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\intersect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\list.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\list.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\plane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\plane.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\poly.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\poly.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\roots.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\sphere.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\sphere.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\torus.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\torus.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\triangle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibObj\triangle.h
# End Source File
# End Group
# Begin Group "LibShade"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibShade\builtin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibShade\lightdef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibShade\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibShade\objdef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibShade\shade.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibShade\stats.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibShade\surfdef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibShade\viewing.cpp
# End Source File
# End Group
# Begin Group "LibImage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\Rayshade\LibImage\image.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Sources\Rayshade\config.h
# End Source File
# End Group
# Begin Group "RT Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_DrawContext.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Geometry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Geometry.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Light.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Light.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Raytrace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Surface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Surface.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Texture.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\RTInterface\RT_Tracer.h
# End Source File
# End Group
# Begin Group "QD3D Plugin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\CurrentVersion.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_Attributes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_Attributes.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_Geometry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_Rasterize.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_Rasterize.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_ScanLine_Rasterize.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_Texture.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RS_Update.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RSPlugin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RSPlugin.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RSPrefix.h
# End Source File
# Begin Source File

SOURCE=..\..\Sources\QD3DPlugin\RSRegister.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
