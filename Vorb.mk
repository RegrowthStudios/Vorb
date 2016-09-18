# Give help information
help:
	@type MakeHelp.txt

# Predefine configuration defaults
VORB_TARGET ?= DEBUG
VORB_OS     ?= WINDOWS
VORB_ARCH   ?= INTEL_X86
COMPILER    ?= MSVC

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

# Convert CPP to OBJ targets
VORB_OBJ_FILES = $(VORB_SRC_FILES:$(VORB_SRC_DIR)%.cpp=$(VORB_INT_DIR)%.obj)

# Everything is built
all: vorb.lib


# Common CPP inference rules
%.depend: %.cpp
	@set -e; rm -f $@; \
	 ($(CC) -M $(CPP_FLAGS) $<) > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

# How to build Vorb
vorb.lib: $(VORB_OBJ_FILES)
	# Display the toolset to the user
	@echo Toolset:
	@echo CC   = $(CC)
	@echo CPP  = $(CPP)
	@echo LINK = $(LINK)

	@echo FOR THE LIB: $<
	$(LINK) /OUT:vorb.lib $<
	
.PHONY : clean
clean:
	del $(VORB_OBJ_FILES)
	del vorb.lib

# # Auto-dependency generation
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
