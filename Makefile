#compiler
CC	:= gcc
MAKE := make
RMS := rm -rf

TARGET := example

SRCDIR := src
BUILDDIR := build

CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O0 -g
LDFLAGS   := -lop -lm -lstdc++ -lgfortran
LIB				:= -L$(BUILDDIR)
INC				:= -I$(SRCDIR)

default: $(TARGET)

SRC := example.c

libop:
	cd $(BUILDDIR) && $(MAKE)

$(TARGET): libop
	$(CC) $(INC) $(SRC) $(CFLAGS) $(LIB) $(LDFLAGS) -o $(@)

run: $(TARGET)
	export LD_LIBRARY_PATH=$(shell pwd)/$(BUILDDIR):$(LD_LIBRARY_PATH) &&	./$(TARGET)

gdb: $(TARGET)
	export LD_LIBRARY_PATH=$(shell pwd)/$(BUILDDIR):$(LD_LIBRARY_PATH) &&	gdb ./$(TARGET)

valgrind: $(TARGET)
	export LD_LIBRARY_PATH=$(shell pwd)/$(BUILDDIR):$(LD_LIBRARY_PATH) &&	valgrind --track-origins=yes ./$(TARGET)

clean:
	cd $(BUILDDIR) && $(MAKE) clean
	$(RMS) $(TARGET) *.a *.dat *.log
