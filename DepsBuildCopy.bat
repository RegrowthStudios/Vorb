@ECHO OFF

SET LIBNAME=libyaml-cppmd.lib
SET LIBNAME_DEBUG=libyaml-cppmdd.lib
SET LIBFOLDER=YAML
ECHO Copying YAML
COPY  /Y "deps\src\bin\Win32\Release\%LIBFOLDER%\%LIBNAME% " "deps\lib\Win32\%LIBNAME%"
COPY  /Y "deps\src\bin\Win32\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "   "deps\lib\Win32\%LIBNAME_DEBUG%"
COPY  /Y "deps\src\bin\x64\Release\%LIBFOLDER%\%LIBNAME% "   "deps\lib\x64\%LIBNAME%"
COPY  /Y "deps\src\bin\x64\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "     "deps\lib\x64\%LIBNAME_DEBUG%"

SET LIBNAME=OOLua.lib
SET LIBNAME_DEBUG=OOLua-d.lib
SET LIBFOLDER=OOLua
ECHO Copying Lua
COPY  /Y "deps\src\bin\Win32\Release\%LIBFOLDER%\%LIBNAME% " "deps\lib\Win32\%LIBNAME%"
COPY  /Y "deps\src\bin\Win32\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "   "deps\lib\Win32\%LIBNAME_DEBUG%"
COPY  /Y "deps\src\bin\x64\Release\%LIBFOLDER%\%LIBNAME% "   "deps\lib\x64\%LIBNAME%"
COPY  /Y "deps\src\bin\x64\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "     "deps\lib\x64\%LIBNAME_DEBUG%"

SET LIBNAME=ZLIB.lib
SET LIBNAME_DEBUG=ZLIB-d.lib
SET LIBFOLDER=ZLIB
ECHO Copying ZLIB
COPY  /Y "deps\src\bin\Win32\Release\%LIBFOLDER%\%LIBNAME% " "deps\lib\Win32\%LIBNAME%"
COPY  /Y "deps\src\bin\Win32\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "   "deps\lib\Win32\%LIBNAME_DEBUG%"
COPY  /Y "deps\src\bin\x64\Release\%LIBFOLDER%\%LIBNAME% "   "deps\lib\x64\%LIBNAME%"
COPY  /Y "deps\src\bin\x64\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "     "deps\lib\x64\%LIBNAME_DEBUG%"

SET LIBNAME=LinearMath.lib
SET LIBNAME_DEBUG=LinearMath_Debug.lib
SET LIBFOLDER=Bullet
ECHO Copying Bullet Math
COPY  /Y "deps\src\bin\Win32\Release\%LIBFOLDER%\%LIBNAME% " "deps\lib\Win32\%LIBNAME%"
COPY  /Y "deps\src\bin\Win32\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "   "deps\lib\Win32\%LIBNAME_DEBUG%"
COPY  /Y "deps\src\bin\x64\Release\%LIBFOLDER%\%LIBNAME% "   "deps\lib\x64\%LIBNAME%"
COPY  /Y "deps\src\bin\x64\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "     "deps\lib\x64\%LIBNAME_DEBUG%"

SET LIBNAME=BulletCollision.lib
SET LIBNAME_DEBUG=BulletCollision_Debug.lib
SET LIBFOLDER=Bullet
ECHO Copying Bullet Collision
COPY  /Y "deps\src\bin\Win32\Release\%LIBFOLDER%\%LIBNAME% " "deps\lib\Win32\%LIBNAME%"
COPY  /Y "deps\src\bin\Win32\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "   "deps\lib\Win32\%LIBNAME_DEBUG%"
COPY  /Y "deps\src\bin\x64\Release\%LIBFOLDER%\%LIBNAME% "   "deps\lib\x64\%LIBNAME%"
COPY  /Y "deps\src\bin\x64\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "     "deps\lib\x64\%LIBNAME_DEBUG%"

SET LIBNAME=BulletDynamics.lib
SET LIBNAME_DEBUG=BulletDynamics_Debug.lib
SET LIBFOLDER=Bullet
ECHO Copying Bullet Dynamics
COPY  /Y "deps\src\bin\Win32\Release\%LIBFOLDER%\%LIBNAME% " "deps\lib\Win32\%LIBNAME%"
COPY  /Y "deps\src\bin\Win32\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "   "deps\lib\Win32\%LIBNAME_DEBUG%"
COPY  /Y "deps\src\bin\x64\Release\%LIBFOLDER%\%LIBNAME% "   "deps\lib\x64\%LIBNAME%"
COPY  /Y "deps\src\bin\x64\Debug\%LIBFOLDER%\%LIBNAME_DEBUG% "     "deps\lib\x64\%LIBNAME_DEBUG%"
