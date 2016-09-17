# Preprocessor directives
VORB_PREPROCESSOR = WIN32 _DEBUG _CONSOLE _UNICODE UNICODE

# Convert into compiler-friendly flags
CPP_FLAGS_PREPROCESSOR = $(VORB_PREPROCESSOR:%=/D "%")

# Print info
$(info $(CPP_FLAGS_PREPROCESSOR))
