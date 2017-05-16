#compiler and linker
CC 	:= gcc
MAKE := make
RMS := rm -rf

TARGET := libop.a


#Directories
SRCDIR   := ../src
PMDIR		 := $(SRCDIR)/pricing_methods
PMEUR  	 := $(PMDIR)/european
PMAM     := $(PMDIR)/american
QSSDIR   := $(PMAM)/finite_difference/qss-solver
BUILDDIR := $(SRCDIR)/obj


#Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2
LDFLAGS  	:= -lm
INC       := -I$(SRCDIR) -I$(QSSDIR)/..
.SUFFIXES: .c

#Source files
COMMONSRC := $(SRCDIR)/option_data.c $(SRCDIR)/pricing_data.c $(SRCDIR)/result.c \
	$(SRCDIR)/pricing_method.c $(SRCDIR)/option.c

PMSRC := $(PMEUR)/european_analytic.c \
	$(PMAM)/finite_difference/american_finite_difference.c

#Objects
COMMONOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))
PMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(PMSRC:.c=.o)))

#DEPS = $(COMMONOBJ:.o=.d) $(PMOBJ:.o=.d)

default: $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	#$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o: $(PMEUR)/%.c
	#$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/black_scholes_model.o:
	@cd $(QSSDIR) && $(MAKE)
	@mv $(QSSDIR)/black_scholes_model.o $@

$(BUILDDIR)/%.o: $(PMAM)/finite_difference/%.c $(BUILDDIR)/black_scholes_model.o 
	#$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(COMMONOBJ) $(PMOBJ)
	@ar rvs $(@) $(COMMONOBJ) $(PMOBJ)

$(COMMONOBJ): | $(BUILDDIR)

$(PMOBJ): | $(BUILDDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

#-include $(DEPS)

clean:
	cd $(QSSDIR) && $(MAKE) clean
	$(RMS) $(BUILDDIR) $(TARGET)
