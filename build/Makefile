#compiler and linker
CC := gcc

RMS := rm -rf

TARGET := libop.a

#Directories
SRCDIR   := ../src
PMDIR		 := $(SRCDIR)/pricing_methods
BUILDDIR := $(SRCDIR)/obj


#Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2
LDFLAGS  	:= -lm
INC       := -I$(SRCDIR)
.SUFFIXES: .c

#Source files
COMMONSRC := $(SRCDIR)/option_data.c $(SRCDIR)/pricing_data.c $(SRCDIR)/result.c \
	$(SRCDIR)/pricing_method.c $(SRCDIR)/option.c

PMSRC := $(PMDIR)/european_analytic.c

#Objects
COMMONOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))
PMOBJ = $(addprefix $(BUILDDIR)/, $(notdir $(PMSRC:.c=.o)))

DEPS = $(COMMONOBJ:.o=.d) $(PMOBJ:.o=.d)

default: $(TARGET)

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o : $(PMDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(COMMONOBJ) $(PMOBJ)
	@ar rvs $(@) $(COMMONOBJ) $(PMOBJ)

$(COMMONOBJ): | $(BUILDDIR)

$(PMOBJ): | $(BUILDDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

-include $(DEPS)

clean:
	$(RMS) $(BUILDDIR) $(TARGET)
