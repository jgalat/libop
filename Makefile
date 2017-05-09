#compiler
CC	:= gcc
MAKE := make
RMS := rm -rf

TARGET := example

SRCDIR := src
BUILDDIR := build

CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2
LDFLAGS   := -lop -lm
LIB				:= -L$(BUILDDIR)
INC				:= -I$(SRCDIR)

default: $(TARGET)

SRC := example.c

libop:
	cd $(BUILDDIR) && $(MAKE)

$(TARGET): libop
	$(CC) $(INC) $(CFLAGS) $(SRC) -o $@ $(LIB) $(LDFLAGS)


clean:
	cd $(BUILDDIR) && $(MAKE) clean
	$(RMS) $(TARGET)
