# Microsoft Developer Studio Project File - Name="DB_MAKER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=DB_MAKER - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DB_MAKER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DB_MAKER.mak" CFG="DB_MAKER - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DB_MAKER - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "DB_MAKER - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DB_MAKER - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "DB_MAKER - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /Zm900 /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DB_MAKER - Win32 Release"
# Name "DB_MAKER - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DB_MAKER.cpp
# End Source File
# Begin Source File

SOURCE=.\DB_MAKER.rc
# End Source File
# Begin Source File

SOURCE=.\decrypt.c
# ADD CPP /D "NODECRYPTMAIN"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Commercial"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Commercial\AIM.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\Config.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\CREDITCARD.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\CustomData.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\Customer.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\InventoryRec.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\MainTrans.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\OBD_TABLES.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\ProductConfig.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\ReportConfig.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\Security_Password.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\Site.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\TankConfig.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\TransAllZero.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\UnitConfig.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\User.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\Utility.h
# End Source File
# Begin Source File

SOURCE=.\Commercial\Vehicle.h
# End Source File
# End Group
# Begin Group "FMLedger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\binary.asm

!IF  "$(CFG)" == "DB_MAKER - Win32 Release"

# Begin Custom Build
IntDir=.\Release
InputPath=.\binary.asm
InputName=binary

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -t -f  win32 -o$(IntDir)\$(InputName).obj -Xvc $(InputName).asm

# End Custom Build

!ELSEIF  "$(CFG)" == "DB_MAKER - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
IntDir=.\Debug
InputPath=.\binary.asm
InputName=binary

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -t -f  win32 -o$(IntDir)\$(InputName).obj -Xvc $(InputName).asm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_Additive.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_Config.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_credit.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_custom.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_DODAAC.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_Link_DoDAAC_NSN.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_messages.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_secu.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_site.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_tank.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_tim_trans.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_tmu.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_transactions.h
# End Source File
# Begin Source File

SOURCE=.\FMLedger\fmledger_veh.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DB_MAKER.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project