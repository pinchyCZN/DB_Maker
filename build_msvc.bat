set PATH=.\msvc
set INCLUDE=.\msvc\include\;.\msvc\include\MFC\
set LIB=.\msvc\lib\;.\msvc\lib\MFC\
rem set INCLUDE=C:\DEV\Microsoft Visual Studio\VC98\Include\;C:\DEV\Microsoft Visual Studio\VC98\MFC\Include\
rem set LIB=C:\DEV\Microsoft Visual Studio\VC98\Lib\;C:\DEV\Microsoft Visual Studio\VC98\MFC\Lib\
del /Q .\release\*.*
del /Q .\debug\*.*
del DB_MAKER.dep
@echo off
setlocal

rem The format of %TIME% is HH:MM:SS,CS for example 23:59:59,99
set STARTTIME=%TIME%

rem here begins the command you want to measure

nmake /f DB_MAKER.mak CFG="DB_MAKER - Win32 Release"
rem nmake /f DB_MAKER.mak CFG="DB_MAKER - Win32 Debug"

rem here ends the command you want to measure

set ENDTIME=%TIME%

rem output as time
echo STARTTIME: %STARTTIME%
echo ENDTIME: %ENDTIME%

rem convert STARTTIME and ENDTIME to centiseconds
set /A STARTTIME=(1%STARTTIME:~0,2%-100)*360000 + (1%STARTTIME:~3,2%-100)*6000 + (1%STARTTIME:~6,2%-100)*100 + (1%STARTTIME:~9,2%-100)
set /A ENDTIME=(1%ENDTIME:~0,2%-100)*360000 + (1%ENDTIME:~3,2%-100)*6000 + (1%ENDTIME:~6,2%-100)*100 + (1%ENDTIME:~9,2%-100)

rem calculating the duratyion is easy
set /A DURATION=%ENDTIME%-%STARTTIME%

rem we might have measured the time inbetween days
if %ENDTIME% LSS %STARTTIME% set set /A DURATION=%STARTTIME%-%ENDTIME%

rem now break the centiseconds down to hors, minutes, seconds and the remaining centiseconds
set /A DURATIONH=%DURATION% / 360000
set /A DURATIONM=(%DURATION% - %DURATIONH%*360000) / 6000
set /A DURATIONS=(%DURATION% - %DURATIONH%*360000 - %DURATIONM%*6000) / 100
set /A DURATIONHS=(%DURATION% - %DURATIONH%*360000 - %DURATIONM%*6000 - %DURATIONS%*100)

rem some formatting
if %DURATIONH% LSS 10 set DURATIONH=0%DURATIONH%
if %DURATIONM% LSS 10 set DURATIONM=0%DURATIONM%
if %DURATIONS% LSS 10 set DURATIONS=0%DURATIONS%
if %DURATIONHS% LSS 10 set DURATIONHS=0%DURATIONHS%

rem outputing
echo STARTTIME: %STARTTIME% centiseconds
echo ENDTIME: %ENDTIME% centiseconds
echo DURATION: %DURATION% in centiseconds
echo %DURATIONH%:%DURATIONM%:%DURATIONS%,%DURATIONHS%

endlocal

