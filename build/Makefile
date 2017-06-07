#compiler and linker
CC 	:= gcc
CPPC := g++
MAKE := make
RMS := rm -rf

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


#Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2 -fPIC
CPPFLAGS  := -std=gnu++11
LDFLAGS  	:=
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

#Objects
COMMONOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))
PMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(PMSRC:.c=.o)))
IVMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(IVMSRC:.c=.o)))
QSSLIBS = $(QSSLIBSDIR)/libqss.a $(QSSLIBSDIR)/libtimestep.a
QSSOBJ = $(QSSDIR)/engine/obj/release/*.o
EXTRAOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(CPPSRC:.cpp=.o))) $(QSSOBJ)


default: $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o: $(PMEUR)/%.c
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(QSSOBJ):
	@cd $(QSSLIBSDIR) && $(MAKE)

$(BUILDDIR)/%.o: $(QSSDIR)/%.cpp
	$(CPPC) $(INC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(BUILDDIR)/%.o: $(PMAM)/finite_difference/%.c
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o: $(IVMDIR)/%.c
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(EXTRAOBJ) $(COMMONOBJ) $(PMOBJ) $(IVMOBJ)
	gcc -shared -o $(@) $(EXTRAOBJ) $(COMMONOBJ) $(PMOBJ) $(IVMOBJ)

$(COMMONOBJ): | $(BUILDDIR)

$(PMOBJ): | $(BUILDDIR)

$(EXTRAOBJ): | $(BUILDDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

clean:
	cd $(QSSLIBSDIR) && $(MAKE) clean
	$(RMS) $(BUILDDIR) $(TARGET) *.a
