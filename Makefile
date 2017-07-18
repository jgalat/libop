OS ?= unix

#compiler
CC := gcc
ifeq ($(OS), win32)
CC := x86_64-w64-mingw32-gcc
endif
MAKE := make
RMS := rm -rf

TARGET := example

SRCDIR := src
BUILDDIR := build

CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2
LDFLAGS   := -lop
LIB				:= -L$(BUILDDIR)
INC				:= -I$(SRCDIR)

default: $(TARGET)

SRC := example.c

libop:
	$(MAKE) -C $(BUILDDIR) OS=$(OS)

$(TARGET): libop
	$(CC) $(CFLAGS) $(INC) $(LIB) $(SRC) -o $(@) $(LDFLAGS)

export_all: $(TARGET)
	$(MAKE) -C $(BUILDDIR) $(@)

run: $(TARGET)
	export LD_LIBRARY_PATH=$(shell pwd)/$(BUILDDIR):$(LD_LIBRARY_PATH) &&	./$(TARGET)

gdb: $(TARGET)
	export LD_LIBRARY_PATH=$(shell pwd)/$(BUILDDIR):$(LD_LIBRARY_PATH) &&	gdb ./$(TARGET)

valgrind: $(TARGET)
	export LD_LIBRARY_PATH=$(shell pwd)/$(BUILDDIR):$(LD_LIBRARY_PATH) &&	valgrind --track-origins=yes ./$(TARGET)

.PHONY: clean

clean:
	$(MAKE) -C $(BUILDDIR) clean
	$(RMS) $(TARGET) *.dat *.log
