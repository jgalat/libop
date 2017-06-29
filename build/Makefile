#compiler and linker
CC 	:= gcc
CPPC := g++
MAKE := make
RMS := rm -rf
MKDIR := mkdir
CP := cp

TARGET := libop.so

#Directories
SRCDIR   := ../src
PMDIR		 := $(SRCDIR)/pricing_methods
PMEUR  	 := $(PMDIR)/european
PMAM     := $(PMDIR)/american
IVMDIR   := $(SRCDIR)/impl_vol_methods
QSSDIR   := ../qss-solver
QSSLIBSDIR := $(QSSDIR)/libs
BUILDDIR := $(SRCDIR)/obj
EXPORTDIR := ../export


#Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2 -fPIC
CPPFLAGS  := -std=gnu++11
LDFLAGS  	:= -lstdc++ -lgfortran
INC       := -I$(SRCDIR) -I$(QSSDIR)/..
.SUFFIXES: .c

#Source files
COMMONSRC := $(SRCDIR)/option_data.c $(SRCDIR)/pricing_data.c $(SRCDIR)/result.c \
	$(SRCDIR)/pm_settings.c $(SRCDIR)/pricing_method.c $(SRCDIR)/option.c \
	$(SRCDIR)/dividend.c $(SRCDIR)/time_period.c

PMSRC := $(PMEUR)/european_analytic.c \
	$(PMAM)/finite_difference/american_finite_difference.c

CPPSRC := $(QSSDIR)/black_scholes_model.cpp

IVMSRC := $(IVMDIR)/impl_vol_mf.c $(IVMDIR)/secant_method.c

EXPORTH := $(SRCDIR)/libop.h $(SRCDIR)/option.h $(SRCDIR)/pricing_method.h \
	$(SRCDIR)/option_data.h $(SRCDIR)/pm_settings.h $(SRCDIR)/dividend.h \
	$(SRCDIR)/parameter.h $(SRCDIR)/result.h $(SRCDIR)/time_period.h

#Objects
COMMONOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))
PMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(PMSRC:.c=.o)))
IVMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(IVMSRC:.c=.o)))
QSSOBJ = $(QSSDIR)/engine/obj/release/*.o
EXTRAOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(CPPSRC:.cpp=.o))) $(QSSOBJ)


default: $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $(@) $(INC) $(CFLAGS)

$(BUILDDIR)/%.o: $(PMEUR)/%.c
	$(CC) -c $< -o $(@) $(INC) $(CFLAGS)

$(QSSOBJ): QSSSOLVER

QSSSOLVER:
	$(MAKE) -C $(QSSLIBSDIR)

$(BUILDDIR)/%.o: $(QSSDIR)/%.cpp
	$(CPPC) -c $< -o $(@)  $(INC) $(CFLAGS) $(CPPFLAGS)

$(BUILDDIR)/%.o: $(PMAM)/finite_difference/%.c
	$(CC) -c $< -o $(@) $(INC) $(CFLAGS)

$(BUILDDIR)/%.o: $(IVMDIR)/%.c
	$(CC) -c $< -o $(@) $(INC) $(CFLAGS)

$(TARGET): $(EXTRAOBJ) $(COMMONOBJ) $(PMOBJ) $(IVMOBJ)
	$(CC) -shared -o $(@) $(EXTRAOBJ) $(COMMONOBJ) $(PMOBJ) $(IVMOBJ) $(CFLAGS) $(LDFLAGS)

$(COMMONOBJ): | $(BUILDDIR)

$(PMOBJ): | $(BUILDDIR)

$(EXTRAOBJ): | $(BUILDDIR)

$(BUILDDIR):
	$(MKDIR) -p $(BUILDDIR)

export_all: $(TARGET)
	$(MKDIR) -p $(EXPORTDIR) $(EXPORTDIR)/include $(EXPORTDIR)/lib
	$(CP) $(TARGET) $(EXPORTDIR)/lib
	$(CP) $(EXPORTH) $(EXPORTDIR)/include

.PHONY : clean

clean:
	$(MAKE) -C $(QSSLIBSDIR) clean
	$(RMS) $(EXPORTDIR) $(BUILDDIR) $(TARGET)
