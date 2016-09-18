# Compiler tools
ifeq ($(COMPILER),MSVC)
  CC   = cl.exe
  CPP  = cl.exe
  LINK = link.exe
else ifeq ($(COMPILER),CLANG)
  $(error TODO(Cristian): Clang tools not supported yet)
else ifeq ($(COMPILER),GNU)
  $(error TODO(Cristian): GNU tools not supported yet)
else
  $(error No good compiler specified - must be one of [MSVC])
endif
