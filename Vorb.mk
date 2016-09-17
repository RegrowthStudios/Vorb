# Predefine build type
VORB_BUILD_TARGET ?= DEBUG
VORB_BUILD_OS     ?= WIN32

# Include utility files
include tools.mk
include preprocessors.mk

# Determine the file system under which we are operating
VORB_SRC_DIR = src/
VORB_INT_DIR = obj/

# Add all include directories here
VORB_INCLUDE_DIRS = deps/include/

# Add and omit all source files here
VORB_SRC_FILES = $(wildcard $(VORB_SRC_DIR)*.cpp)
$(info $(VORB_SRC_FILES))

# Convert CPP to OBJ targets
VORB_OBJ_FILES = $(VORB_SRC_FILES:$(VORB_SRC_DIR)%.cpp=$(VORB_INT_DIR)%.obj)
$(info $(VORB_OBJ_FILES))

dummy:
	@echo Dummy target

all: vorb.lib

# Common CPP inference rule
%.depend: %.cpp
	@set -e; rm -f $@; \
	 ($(CC) -M $(CPP_FLAGS) $<) > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

# How to build Vorb
vorb.lib: $(VORB_OBJ_FILES)
	@echo FOR THE LIB: $<
	$(LINK) /OUT:vorb.lib $<
	
.PHONY : clean
clean:
	del $(VORB_OBJ_FILES)
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










# DEPDIR = .depend
# $(shell mkdir -p $(DEPDIR) >/dev/null)
# DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
# 
# COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
# COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
# POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.depend
# 
# %.o : %.c
# %.o : %.c $(DEPDIR)/%.depend
#         $(COMPILE.c) $(OUTPUT_OPTION) $<
#         $(POSTCOMPILE)
# 
# %.o : %.cc
# %.o : %.cc $(DEPDIR)/%.depend
#         $(COMPILE.cc) $(OUTPUT_OPTION) $<
#         $(POSTCOMPILE)
# 
# %.o : %.cxx
# %.o : %.cxx $(DEPDIR)/%.depend
#         $(COMPILE.cc) $(OUTPUT_OPTION) $<
#         $(POSTCOMPILE)
# 
# $(DEPDIR)/%.depend: ;
# .PRECIOUS: $(DEPDIR)/%.depend
# 
# -include $(patsubst %,$(DEPDIR)/%.depend,$(basename $(SRCS)))
