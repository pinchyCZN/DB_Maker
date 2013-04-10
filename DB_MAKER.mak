# Microsoft Developer Studio Generated NMAKE File, Based on DB_MAKER.dsp
!IF "$(CFG)" == ""
CFG=DB_MAKER - Win32 Debug
!MESSAGE No configuration specified. Defaulting to DB_MAKER - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DB_MAKER - Win32 Release" && "$(CFG)" != "DB_MAKER - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "DB_MAKER - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\DB_MAKER.exe" "$(OUTDIR)\DB_MAKER.bsc"


CLEAN :
	-@erase "$(INTDIR)\DB_MAKER.obj"
	-@erase "$(INTDIR)\DB_MAKER.pch"
	-@erase "$(INTDIR)\DB_MAKER.res"
	-@erase "$(INTDIR)\DB_MAKER.sbr"
	-@erase "$(INTDIR)\decrypt.obj"
	-@erase "$(INTDIR)\decrypt.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\DB_MAKER.bsc"
	-@erase "$(OUTDIR)\DB_MAKER.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DB_MAKER.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\DB_MAKER.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DB_MAKER.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DB_MAKER.sbr" \
	"$(INTDIR)\decrypt.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\DB_MAKER.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\DB_MAKER.pdb" /machine:I386 /out:"$(OUTDIR)\DB_MAKER.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DB_MAKER.obj" \
	"$(INTDIR)\decrypt.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DB_MAKER.res" \
	"$(INTDIR)\binary.obj"

"$(OUTDIR)\DB_MAKER.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DB_MAKER - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\DB_MAKER.exe" "$(OUTDIR)\DB_MAKER.bsc"


CLEAN :
	-@erase "$(INTDIR)\DB_MAKER.obj"
	-@erase "$(INTDIR)\DB_MAKER.pch"
	-@erase "$(INTDIR)\DB_MAKER.res"
	-@erase "$(INTDIR)\DB_MAKER.sbr"
	-@erase "$(INTDIR)\decrypt.obj"
	-@erase "$(INTDIR)\decrypt.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\DB_MAKER.bsc"
	-@erase "$(OUTDIR)\DB_MAKER.exe"
	-@erase "$(OUTDIR)\DB_MAKER.ilk"
	-@erase "$(OUTDIR)\DB_MAKER.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DB_MAKER.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Zm900 /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\DB_MAKER.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DB_MAKER.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DB_MAKER.sbr" \
	"$(INTDIR)\decrypt.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\DB_MAKER.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\DB_MAKER.pdb" /debug /machine:I386 /out:"$(OUTDIR)\DB_MAKER.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\DB_MAKER.obj" \
	"$(INTDIR)\decrypt.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DB_MAKER.res" \
	"$(INTDIR)\binary.obj"

"$(OUTDIR)\DB_MAKER.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("DB_MAKER.dep")
!INCLUDE "DB_MAKER.dep"
!ELSE 
!MESSAGE Warning: cannot find "DB_MAKER.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "DB_MAKER - Win32 Release" || "$(CFG)" == "DB_MAKER - Win32 Debug"
SOURCE=.\DB_MAKER.cpp

"$(INTDIR)\DB_MAKER.obj"	"$(INTDIR)\DB_MAKER.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DB_MAKER.pch"


SOURCE=.\DB_MAKER.rc

"$(INTDIR)\DB_MAKER.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\decrypt.c

!IF  "$(CFG)" == "DB_MAKER - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "NODECRYPTMAIN" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\decrypt.obj"	"$(INTDIR)\decrypt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "DB_MAKER - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "NODECRYPTMAIN" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Zm900 /c 

"$(INTDIR)\decrypt.obj"	"$(INTDIR)\decrypt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "DB_MAKER - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DB_MAKER.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\DB_MAKER.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "DB_MAKER - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DB_MAKER.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Zm900 /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\DB_MAKER.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\binary.asm

!IF  "$(CFG)" == "DB_MAKER - Win32 Release"

IntDir=.\Release
InputPath=.\binary.asm
InputName=binary

"$(INTDIR)\binary.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasmw -t -f  win32 -o$(IntDir)\$(InputName).obj -Xvc $(InputName).asm
<< 
	

!ELSEIF  "$(CFG)" == "DB_MAKER - Win32 Debug"

IntDir=.\Debug
InputPath=.\binary.asm
InputName=binary

"$(INTDIR)\binary.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasmw -t -f  win32 -o$(IntDir)\$(InputName).obj -Xvc $(InputName).asm
<< 
	

!ENDIF 


!ENDIF 

