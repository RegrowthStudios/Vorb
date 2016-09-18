# Preprocessor directives
VORB_PREPROCESSOR = WIN32 _DEBUG _CONSOLE _UNICODE UNICODE

# Convert into compiler-friendly flags
CPP_FLAGS_PREPROCESSOR = $(VORB_PREPROCESSOR:%=/D "%")

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
# /errorReport:prompt
# /WX-
# /Zc:forScope
# /RTC1
# /Gd
# /Oy-
# /MDd
# /Fa"Debug\"
# /EHsc
# /nologo
# /Fo"Debug\"
# /Fp"Debug\ConsoleApplication1.pch"
