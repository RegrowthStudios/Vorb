IDIR =include
CC=clang
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =deps/lib

LIBS=-lm

_DEPS = include/color.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = src/color.cpp.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.cpp.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Vorb.lib: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.cpp.o *~ core $(INCDIR)/*~ 
