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
QSSDIR   := ../qss-solver
QSSLIBSDIR := $(QSSDIR)/libs
BUILDDIR := $(SRCDIR)/obj


#Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O0 -g -fPIC
CPPFLAGS  := -std=gnu++11
LDFLAGS  	:=
INC       := -I$(SRCDIR) -I$(QSSDIR)/..
.SUFFIXES: .c

#Source files
COMMONSRC := $(SRCDIR)/option_data.c $(SRCDIR)/pricing_data.c $(SRCDIR)/result.c \
	$(SRCDIR)/pricing_method.c $(SRCDIR)/option.c $(SRCDIR)/dividend.c

PMSRC := $(PMEUR)/european_analytic.c \
	$(PMAM)/finite_difference/american_finite_difference.c

CPPSRC := $(QSSDIR)/black_scholes_model.cpp

#Objects
COMMONOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))
PMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(PMSRC:.c=.o)))
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

$(TARGET): $(EXTRAOBJ) $(COMMONOBJ) $(PMOBJ)
	gcc -shared -o $(@) $(EXTRAOBJ) $(COMMONOBJ) $(PMOBJ)
	#@ar rc $(@) $(EXTRAOBJ) $(COMMONOBJ) $(PMOBJ)
	#ranlib $(@)

$(COMMONOBJ): | $(BUILDDIR)

$(PMOBJ): | $(BUILDDIR)

$(EXTRAOBJ): | $(BUILDDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

clean:
	cd $(QSSLIBSDIR) && $(MAKE) clean
	$(RMS) $(BUILDDIR) $(TARGET) *.a
