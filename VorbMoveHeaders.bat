@ECHO OFF

SET EXEC_DIR=%~dp0
SET EXEC_DIR=%EXEC_DIR:~0,-1%
ECHO %EXEC_DIR%

IF %1.==. GOTO NO_ARGS

SET OUTPUT=%1
SHIFT

ROBOCOPY /E /XO "%EXEC_DIR%\include" %OUTPUT% *.h *.inl *.hpp /XD "html" "examples"
GOTO END_PROGRAM

:NO_ARGS
ECHO No arguments specified

:END_PROGRAM
