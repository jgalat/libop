OS ?= unix

#compiler and linker
CC 	:= gcc
CPPC := g++
ifeq ($(OS), win32)
CC := x86_64-w64-mingw32-gcc
CPPC := x86_64-w64-mingw32-g++
endif

MAKE := make
RMS := rm -rf
MKDIR := mkdir
CP := cp

TARGET := libop.so
ifeq ($(OS), win32)
TARGET := libop.dll
endif

#Directories
SRCDIR   := ../src
PMDIR		 := $(SRCDIR)/pricing_methods
PMEUR  	 := $(PMDIR)/european
PMAM     := $(PMDIR)/american
IVMDIR   := $(SRCDIR)/impl_vol_methods

QSSDIR   := ../qss-solver
QSSENGINEDIR := $(QSSDIR)/engine
QSSCOMMONDIR := $(QSSENGINEDIR)/common
QSSCLASSICDIR := $(QSSENGINEDIR)/classic
QSSDOPRIDIR := $(QSSENGINEDIR)/classic/dopri5

BUILDDIR := obj
EXPORTDIR := ../export

#Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2
ifeq ($(OS), unix)
CFLAGS += -fPIC
endif
CPPFLAGS  := -std=gnu++11
LDFLAGS  	:= -lstdc++
ifeq ($(OS), win32)
LDFLAGS   := -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread
endif
INC       := -I$(SRCDIR) -I$(QSSDIR)/..
QSSINC    := -I$(QSSCOMMONDIR) -I$(QSSENGINEDIR)
.SUFFIXES: .c

#Source files
COMMONSRC := $(SRCDIR)/parameter.c $(SRCDIR)/risk_free_rate.c $(SRCDIR)/volatility.c \
	$(SRCDIR)/dividend.c $(SRCDIR)/time_period.c $(SRCDIR)/option_data.c \
	$(SRCDIR)/pricing_data.c $(SRCDIR)/result.c $(SRCDIR)/pm_settings.c \
	$(SRCDIR)/pricing_method.c $(SRCDIR)/option.c

PMSRC := $(PMEUR)/european_analytic.c \
	$(PMAM)/finite_difference/american_finite_difference_uniform_grid.c

BSMSRC := $(QSSDIR)/BSMUniformGrid.cpp $(QSSDIR)/BSMNonUniformGrid.cpp

IVMSRC := $(IVMDIR)/impl_vol_mf.c $(IVMDIR)/secant_method.c

EXPORTH := $(SRCDIR)/libop.h $(SRCDIR)/option.h $(SRCDIR)/pricing_method.h \
	$(SRCDIR)/option_data.h $(SRCDIR)/pm_settings.h \
	$(SRCDIR)/result.h $(SRCDIR)/time_period.h \
	$(SRCDIR)/dividend.h $(SRCDIR)/volatility.h $(SRCDIR)/risk_free_rate.h

QSSCOMMONSRC = $(QSSCOMMONDIR)/data.c $(QSSCOMMONDIR)/utils.c $(QSSCOMMONDIR)/settings.c \
	$(QSSCOMMONDIR)/simulator.c $(QSSCOMMONDIR)/integrator.c $(QSSCOMMONDIR)/engine.c

QSSCLASSICSRC = $(QSSCLASSICDIR)/classic_data.c $(QSSCLASSICDIR)/classic_integrator.c \
	$(QSSCLASSICDIR)/classic_dopri_integrator.c $(QSSCLASSICDIR)/classic_simulator.c

QSSDOPRISRC = $(QSSDOPRIDIR)/dopri5.cpp

#Objects
COMMONOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))
PMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(PMSRC:.c=.o)))
IVMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(IVMSRC:.c=.o)))

QSSCOMMONOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(QSSCOMMONSRC:.c=.o)))
QSSCLASSICOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(QSSCLASSICSRC:.c=.o)))
QSSDOPRIOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(QSSDOPRISRC:.cpp=.o)))
QSSOBJ = $(QSSCOMMONOBJ) $(QSSCLASSICOBJ) $(QSSDOPRIOBJ)

BSMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(BSMSRC:.cpp=.o)))

default: $(TARGET)

$(BUILDDIR)/%.o : $(QSSCOMMONDIR)/%.c
	$(CPPC) -c $< -o $(@) $(QSSINC) $(CFLAGS) $(CPPFLAGS)

$(BUILDDIR)/%.o : $(QSSCLASSICDIR)/%.c
	$(CPPC) -c $< -o $(@) $(QSSINC) $(CFLAGS) $(CPPFLAGS)

$(BUILDDIR)/%.o : $(QSSDOPRIDIR)/%.cpp
	$(CPPC) -c $< -o $(@) $(QSSINC) $(CFLAGS) $(CPPFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $(@) $(INC) $(CFLAGS)

$(BUILDDIR)/%.o: $(PMEUR)/%.c
	$(CC) -c $< -o $(@) $(INC) $(CFLAGS)

$(BUILDDIR)/%.o: $(QSSDIR)/%.cpp
	$(CPPC) -c $< -o $(@) $(INC) $(CFLAGS) $(CPPFLAGS)

$(BUILDDIR)/%.o: $(PMAM)/finite_difference/%.c
	$(CC) -c $< -o $(@) $(INC) $(CFLAGS)

$(BUILDDIR)/%.o: $(IVMDIR)/%.c
	$(CC) -c $< -o $(@) $(INC) $(CFLAGS)

$(TARGET): $(QSSOBJ) $(BSMOBJ) $(COMMONOBJ) $(PMOBJ) $(IVMOBJ)
	$(CC) $(CFLAGS) -shared $(QSSOBJ) $(BSMOBJ) $(COMMONOBJ) $(PMOBJ) $(IVMOBJ) -o $(@) $(LDFLAGS)

$(QSSOBJ): | $(BUILDDIR)

$(COMMONOBJ): | $(BUILDDIR)

$(PMOBJ): | $(BUILDDIR)

$(BUILDDIR):
	$(MKDIR) -p $(BUILDDIR)

export_all: $(TARGET)
	$(MKDIR) -p $(EXPORTDIR) $(EXPORTDIR)/include $(EXPORTDIR)/lib
	$(CP) $(TARGET) $(EXPORTDIR)/lib
	$(CP) $(EXPORTH) $(EXPORTDIR)/include

.PHONY : clean

clean:
	$(RMS) $(EXPORTDIR) $(BUILDDIR) $(TARGET)
