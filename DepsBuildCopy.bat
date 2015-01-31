@ECHO OFF
ROBOCOPY /E /XO "deps\bin\Win32\Debug" "deps\lib\Win32" *.lib
ROBOCOPY /E /XO "deps\bin\Win32\Release" "deps\lib\Win32" *.lib
ROBOCOPY /E /XO "deps\bin\x64\Debug" "deps\lib\x64" *.lib
ROBOCOPY /E /XO "deps\bin\x64\Release" "deps\lib\x64" *.lib
