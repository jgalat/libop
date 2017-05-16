#Compiler and Linker
CC    := g++
MAKE  := make

#The Target Binary Program
TARGET    := black_scholes_model.o

#Flags, Libraries and Includes
LDFLAGS    :=-Llibs
LIBS    := -lqss -ltimestep -lm -lgsl -lconfig -lgfortran -lpthread -lrt -lgslcblas
INC    := -I..
CFLAGS    := -Wall -msse2 -mfpmath=sse -O2 $(LDFLAGS) $(LIBS)
RMS    := rm -rf

#Source Files
TARGET_SRC    := black_scholes_model.cpp

default: $(TARGET)

$(TARGET):
	cd libs && $(MAKE)
	$(CC) $(INC) $(TARGET_SRC) $(CFLAGS) -c -o $@

.PHONY: clean

clean:
	cd libs && $(MAKE) clean
	$(RMS) $(TARGET) *.dat *.log
