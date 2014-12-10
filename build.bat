@ECHO OFF && PUSHD "%~dp0" && SETLOCAL

set interactive=1
echo %cmdcmdline% | find /i "%~0" >nul
if not errorlevel 1 set interactive=0


REM ====================================================================
REM   Build: Clean
REM ====================================================================
IF "%~1"=="clean" (
    RMDIR build /S /Q && (
      REM Do nothing
    ) || (
      if _%interactive%_==_0_ pause
      if _%interactive%_==_0_ exit
    )
    shift
) ELSE IF _%interactive%_==_0_ (
    RMDIR build /S /Q && (
      REM Do nothing
    ) || (
      if _%interactive%_==_0_ pause
      if _%interactive%_==_0_ exit
    )
)



REM ====================================================================
REM   Create: Build folder
REM ====================================================================
IF NOT EXIST "build" MKDIR build && (
      REM Do nothing
    ) || (
      if _%interactive%_==_0_ pause
      if _%interactive%_==_0_ exit
    )

REM ====================================================================
REM   CMake: Clean any top-level cmake buid artifacts
REM ====================================================================
IF EXIST "cmake" RMDIR cmake /S /Q && (
      REM Do nothing
    ) || (
      if _%interactive%_==_0_ pause
      if _%interactive%_==_0_ exit
    )
IF EXIST "CMakeFiles" RMDIR CMakeFiles /S /Q && (
      REM Do nothing
    ) || (
      if _%interactive%_==_0_ pause
      if _%interactive%_==_0_ exit
    )
IF EXIST "CmakeCache.txt" DEL CmakeCache.txt && (
      REM Do nothing
    ) || (
      if _%interactive%_==_0_ pause
      if _%interactive%_==_0_ exit
    )


REM ====================================================================
REM   CMake: Begin cmake build process
REM ====================================================================
cd build && (
  REM Do nothing
) || (
  if _%interactive%_==_0_ pause
  if _%interactive%_==_0_ exit
)

REM ====================================================================
REM   CMake: Create build files
REM ====================================================================


cmake ../ && (
  REM Do nothing
) || (
  if _%interactive%_==_0_ pause
  if _%interactive%_==_0_ exit
)
IF "%~1"=="rebuild" (
    make clean && (
      echo Running make clean
    ) || (
      if _%interactive%_==_0_ pause
      if _%interactive%_==_0_ exit
    )
    shift
)
REM ====================================================================
REM   CMake: Run Make
REM ====================================================================
make %* && (
  echo Running make %*
) || (
  if _%interactive%_==_0_ pause
  if _%interactive%_==_0_ exit
)