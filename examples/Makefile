OS ?= unix
SOURCE ?=


#compiler
CC := gcc
ifeq ($(OS), win32)
CC := x86_64-w64-mingw32-gcc
endif
MAKE := make
RMS := rm -rf

SRCDIR := ../src
BUILDDIR := ../build
BINDIR := bin

CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2
LDFLAGS   := -lop
LIB				:= -L$(BUILDDIR)
INC				:= -I$(SRCDIR)

TARGET 		:= $(BINDIR)/$(patsubst %.c,%,$(SOURCE))

ifeq ($(SOURCE), )
default: help
else
default: $(TARGET)
endif

$(TARGET):
	$(CC) $(CFLAGS) $(INC) $(LIB) $(SOURCE) -o $(@) $(LDFLAGS)

ifeq ($(SOURCE), )
run : help
else
run: $(TARGET)
	export LD_LIBRARY_PATH=$(shell pwd)/$(BUILDDIR):$(LD_LIBRARY_PATH) && $(TARGET)
endif

.PHONY: clean help

clean:
	rm $(BINDIR)/*

help:
	@echo "Usage: make SOURCE=source.c"
	@echo "make run SOURCE=source.c"
