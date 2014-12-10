@ECHO OFF && PUSHD "%~dp0" && SETLOCAL

set interactive=1
echo %cmdcmdline% | find /i "%~0" >nul
if not errorlevel 1 set interactive=0

if _%1_==__ (
    echo usage: %~nx0 filename

)



REM ====================================================================
REM   Build: Clean
REM ====================================================================
IF "%~1"=="clean" (
    RMDIR build /S /Q
    shift
)

REM ====================================================================
REM   Create: Build folder
REM ====================================================================
IF NOT EXIST "build" MKDIR build

REM ====================================================================
REM   CMake: Clean any top-level cmake buid artifacts
REM ====================================================================
IF EXIST "cmake" RMDIR cmake /S /Q
IF EXIST "CMakeFiles" RMDIR CMakeFiles /S /Q
IF EXIST "CmakeCache.txt" DEL CmakeCache.txt


REM ====================================================================
REM   CMake: Begin cmake build process
REM ====================================================================
cd build

REM ====================================================================
REM   CMake: Create build files
REM ====================================================================


cmake ../ && (
  echo Running cmake
) || (
  if _%interactive%_==_0_ pause
)
IF "%~1"=="rebuild" (
    make clean && (
      echo Running make clean
    ) || (
      if _%interactive%_==_0_ pause
    )
    shift
)
REM ====================================================================
REM   CMake: Run Make
REM ====================================================================
make %*  && (
  echo Running make %*
) || (
  if _%interactive%_==_0_ pause
)