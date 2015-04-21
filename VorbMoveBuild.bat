@ECHO OFF

SET EXEC_DIR=%~dp0
SET EXEC_DIR=%EXEC_DIR:~0,-1%
ECHO %EXEC_DIR%

IF %1.==. GOTO NO_ARGS

SET OUTPUT=%1
SHIFT

ROBOCOPY /E /XO "%EXEC_DIR%\bin\Win32\Debug"   %OUTPUT%\Win32 Vorb-d.lib
ROBOCOPY /E /XO "%EXEC_DIR%\bin\Win32\Release" %OUTPUT%\Win32 Vorb.lib
ROBOCOPY /E /XO "%EXEC_DIR%\bin\x64\Debug"     %OUTPUT%\x64   Vorb-d.lib
ROBOCOPY /E /XO "%EXEC_DIR%\bin\x64\Release"   %OUTPUT%\x64   Vorb.lib
GOTO END_PROGRAM

:NO_ARGS
ECHO No arguments specified

:END_PROGRAM
