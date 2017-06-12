# Microsoft Developer Studio Project File - Name="AutoFP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AutoFP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AutoFP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AutoFP.mak" CFG="AutoFP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AutoFP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AutoFP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AutoFP - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"C:\S100\bin\AutoFP.exe"

!ELSEIF  "$(CFG)" == "AutoFP - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "1240" /I "MotionInclude\\" /I "MotionInclude\public\\" /I "inc" /I "inc\include" /I "inc\include\halconcpp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ADVMOT.lib Pxx_ATE.lib halconcpp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"c:\S100\bin\AutoFP_Advantech_Stepper_160913.exe" /pdbtype:sept /libpath:"lib"

!ENDIF 

# Begin Target

# Name "AutoFP - Win32 Release"
# Name "AutoFP - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoFP.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoFP.rc
# End Source File
# Begin Source File

SOURCE=.\AutoFPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\BurningPort.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionInclude\Cmmsdk.cpp
# End Source File
# Begin Source File

SOURCE=.\DataPos.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReject.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSupply.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTapeIn.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTapeOut.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTray.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionAct.cpp
# End Source File
# Begin Source File

SOURCE=.\P800IspDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundButton2.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundButtonStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketComm.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XPGroupBox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoFP.h
# End Source File
# Begin Source File

SOURCE=.\AutoFPDlg.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\BurningPort.h
# End Source File
# Begin Source File

SOURCE=.\MotionInclude\Cmmsdk.h
# End Source File
# Begin Source File

SOURCE=.\MotionInclude\CmmsdkDef.h
# End Source File
# Begin Source File

SOURCE=.\DataPos.h
# End Source File
# Begin Source File

SOURCE=.\DlgPosition.h
# End Source File
# Begin Source File

SOURCE=.\DlgReject.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgSocket.h
# End Source File
# Begin Source File

SOURCE=.\DlgSupply.h
# End Source File
# Begin Source File

SOURCE=.\DlgTapeIn.h
# End Source File
# Begin Source File

SOURCE=.\DlgTapeOut.h
# End Source File
# Begin Source File

SOURCE=.\DlgTray.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\MotionAct.h
# End Source File
# Begin Source File

SOURCE=.\P800IspDlg.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Pxx_ATEInterface.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Pxx_LastErrCode.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoundButton2.h
# End Source File
# Begin Source File

SOURCE=.\RoundButtonStyle.h
# End Source File
# Begin Source File

SOURCE=.\sendef.h
# End Source File
# Begin Source File

SOURCE=.\SerialCtl.h
# End Source File
# Begin Source File

SOURCE=.\SocketComm.h
# End Source File
# Begin Source File

SOURCE=.\SocketManager.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XPGroupBox.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\1.bin
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\ARROWdown.ico
# End Source File
# Begin Source File

SOURCE=.\res\ARROWdown.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\ARROWdown.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\ARROWleft.ico
# End Source File
# Begin Source File

SOURCE=.\res\ARROWleft.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\ARROWleft.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\ARROWright.ico
# End Source File
# Begin Source File

SOURCE=.\res\ARROWright.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\ARROWright.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\ARROWup.ico
# End Source File
# Begin Source File

SOURCE=.\res\ARROWup.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\ARROWup.ico
# End Source File
# Begin Source File

SOURCE=.\res\AutoFP.ico
# End Source File
# Begin Source File

SOURCE=.\res\AutoFP.rc2
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\b1.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\b1.ico
# End Source File
# Begin Source File

SOURCE=.\res\BITMAP10.BMP
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\EXIT.ICO
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\gray.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\gray.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\Handler.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\Handler.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\HandlerDoc.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\HandlerDoc.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\Home.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\Home2.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\no1.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\no1.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\pause.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\r1.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\r1.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\Run.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\save.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\save.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\SCREEN.ICO
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\SETUP.ICO
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\Stop1.ico
# End Source File
# Begin Source File

SOURCE=..\SpHnAuto\res\stop2.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\2014_09_04\HnAutoSix_nozzle_Fix_origin\res\yellow.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\Global.pp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Lib\HVDAILT.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\HVUtil.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\Raw2Rgb.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\GxIAPI.lib
# End Source File
# End Target
# End Project
