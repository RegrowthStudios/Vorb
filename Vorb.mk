# Compiler tools
!IF "$(COMPILER)"=="MSVC"
CC   = cl.exe
CPP  = cl.exe
LINK = link.exe
!ELSEIF "$(COMPILER)"=="CLANG"
!ERROR TODO(Cristian): Clang tools not supported yet
!ELSEIF "$(COMPILER)"=="GNU"
!ERROR TODO(Cristian): GNU tools not supported yet
!ELSE
!ERROR No good compiler specified - must be one of [MSVC]
!ENDIF


# Display the toolset to the user
!MESSAGE Toolset:
!MESSAGE CC   = $(CC)
!MESSAGE CPP  = $(CPP)
!MESSAGE LINK = $(LINK)


# Determine the file system under which we are operating
VORB_SRC_DIR = src
VORB_INT_DIR = obj

# Preprocessor directives
VORB_PREPROCESSOR = \
WIN32\
_DEBUG\
_CONSOLE\

!MESSAGE $(VORB_PREPROCESSOR)

# Add all include directories here
VORB_INCLUDE_DIRS = \


# Add all source files here
SOURCE_FILES = \
vorb.cpp\

!MESSAGE $$(SOURCE_FILES):
!MESSAGE $(SOURCE_FILES)

dummy:
	ECHO Dummy target

all: vorb.lib

# Common CPP inference rule
{$(VORB_SRC_DIR)}.cpp{$(VORB_INT_DIR)}.obj:
	$(CPP) /c $< /Fo$@

# How to build Vorb
vorb.lib: $(VORB_INT_DIR)\vorb.obj
	!MESSAGE FOR THE LIB: $<
	$(LINK) /OUT:vorb.lib $<
	
clean:
	del main.obj
	del vorb.lib



# /Yu"stdafx.h"
# /GS
# /analyze-
# /W3
# /Zc:wchar_t
# /ZI
# /Gm
# /Od
# /sdl
# /Fd"Debug\vc140.pdb"
# /Zc:inline
# /fp:precise
# /D "WIN32"
# /D "_DEBUG"
# /D "_CONSOLE"
# /D "_UNICODE"
# /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /Oy- /MDd /Fa"Debug\" /EHsc /nologo /Fo"Debug\" /Fp"Debug\ConsoleApplication1.pch"