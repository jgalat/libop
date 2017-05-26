#Target OS variables
OS ?= unix
DEBUG ?= False

#Compiler and Linker
CC          := g++
FC 			:= gfortran

#The Target Binary Program
TARGET      := libqss.a
LIBTIMESTEP := libtimestep.a
ifeq ($(DEBUG),True)
TARGET      := libqssd.a
LIBTIMESTEP := libtimestepd.a
endif

#The Directories, Source, Includes, Objects, Binary
SRCDIR      := ../../qss-solver
ENGINEDIR 	:= ../engine
CLASSICDIR 	:= $(ENGINEDIR)/classic
DASSLDIR 	:= $(ENGINEDIR)/classic/dassl
DOPRIDIR 	:= $(ENGINEDIR)/classic/dopri5
COMMONDIR 	:= $(ENGINEDIR)/common
OBJDIR      := obj
BUILDDIR    := $(ENGINEDIR)/$(OBJDIR)/release
ifeq ($(DEBUG),True)
BUILDDIR    := $(ENGINEDIR)/$(OBJDIR)/debug
endif
#GRAPHDIR    := $(SRCDIR)/graph
TARGETDIR   := ./
SRCEXT      := c
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O0 -std=gnu++11 -g
ifeq ($(DEBUG),True)
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -g -DDEBUG
endif
LIB         := -lm
INC         := -I$(COMMONDIR) -I$(ENGINEDIR)
ifeq ($(OS),win32)
INC         += -I/GnuWin32/include
endif
RMS 		:= rm -rf
vpath %.c $(ENGINEDIR)
.SUFFIXES: .c

# Source files.
COMMONSRC = $(COMMONDIR)/data.c $(COMMONDIR)/utils.c $(COMMONDIR)/settings.c $(COMMONDIR)/simulator.c $(COMMONDIR)/integrator.c $(COMMONDIR)/engine.c

CLASSICSRC = $(CLASSICDIR)/classic_data.c $(CLASSICDIR)/classic_integrator.c $(CLASSICDIR)/classic_dopri_integrator.c $(CLASSICDIR)/classic_dassl_integrator.c $(CLASSICDIR)/classic_simulator.c

DASSLSRC = $(DASSLDIR)/daux.f $(DASSLDIR)/ddaskr.f $(DASSLDIR)/dlinpk.f

DOPRISRC = $(DOPRIDIR)/dopri5.cpp

# Objects
COMMONOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))

CLASSICOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(CLASSICSRC:.c=.o)))

DASSLOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(DASSLSRC:.f=.o)))

DOPRIOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(DOPRISRC:.cpp=.o)))

# Make dependencies
DEPS = $(COMMONOBJ:.o=.d) $(CLASSICOBJ:.o=.d) $(DOPRIOBJ:.o=.d)

default: $(TARGET) $(LIBTIMESTEP)

$(BUILDDIR)/%.o : $(COMMONDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)


$(BUILDDIR)/%.o : $(CLASSICDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o : $(DOPRIDIR)/%.cpp
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o : $(DASSLDIR)/%.f
	$(FC) $(INC) -c $< -o $@ $(CFLAGS)

$(LIBTIMESTEP): $(DASSLOBJ) $(DOPRIOBJ)
	@ar rvs $(@) $(DASSLOBJ) $(DOPRIOBJ)

$(TARGET): $(COMMONOBJ) $(CLASSICOBJ)
	@ar rsc $@ $(COMMONOBJ) $(CLASSICOBJ)

$(COMMONOBJ): | $(BUILDDIR)

$(CLASSICOBJ): | $(BUILDDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

-include $(DEPS)

.PHONY: clean

clean:
	$(RMS) $(DEPS) $(TARGET) $(LIBTIMESTEP) $(COMMONOBJ) $(CLASSICOBJ) $(DASSLOBJ) $(DOPRIOBJ)

help:
	@echo "make DEBUG=<True|False> OS=<unix|win32|mac>"
	@echo "Default value:"
	@echo ""
	@echo "DEBUG=False"
	@echo "OS=unix"
