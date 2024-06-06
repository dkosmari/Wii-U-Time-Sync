#-------------------------------------------------------------------------------
.SUFFIXES:
#-------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)

include $(DEVKITPRO)/wups/share/wups_rules

WUMS_ROOT := $(DEVKITPRO)/wums
WUT_ROOT := $(DEVKITPRO)/wut

#-------------------------------------------------------------------------------
# PLUGIN_NAME sets the name of the plugin.
# PLUGIN_DESCRIPTION sets the description of the plugin.
# PLUGIN_VERSION sets the version of the plugin.
# PLUGIN_AUTHOR sets the author of the plugin.
# PLUGIN_LICENSE sets the license of the plugin.
#-------------------------------------------------------------------------------
PLUGIN_NAME        := Time Sync
PLUGIN_DESCRIPTION := A plugin that synchronizes the system clock to the Internet.
PLUGIN_VERSION     := v4.1
PLUGIN_AUTHOR      := Nightkingale, Daniel K. O.
PLUGIN_LICENSE     := MIT

#-------------------------------------------------------------------------------
# TARGET is the name of the output.
# BUILD is the directory where object files & intermediate files will be placed.
# SOURCES is a list of directories containing source code.
# DATA is a list of directories containing data files.
# INCLUDES is a list of directories containing header files.
#-------------------------------------------------------------------------------
TARGET   := Time_Sync
BUILD    := build
SOURCES  := source source/net source/wupsxx
DATA     := data
INCLUDES := include

# Be verbose by default.
V ?= 1

#-------------------------------------------------------------------------------
# options for code generation
#-------------------------------------------------------------------------------
WARN_FLAGS := -Wall -Wextra -Werror -Wundef -Wpointer-arith -Wcast-align

OPTFLAGS := -O2 -fipa-pta -ffunction-sections

CFLAGS := $(WARN_FLAGS) $(OPTFLAGS) $(MACHDEP)

CXXFLAGS := $(CFLAGS) -std=c++23

DEFINES := '-DPLUGIN_NAME="$(PLUGIN_NAME)"'                   \
           '-DPLUGIN_DESCRIPTION="$(PLUGIN_DESCRIPTION)"'     \
           '-DPLUGIN_VERSION="$(PLUGIN_VERSION)"'             \
           '-DPLUGIN_AUTHOR="$(PLUGIN_AUTHOR)"'               \
           '-DPLUGIN_LICENSE="$(PLUGIN_LICENSE)"'

# Note: INCLUDE will be defined later, so CPPFLAGS has to be of the recursive flavor.
CPPFLAGS = $(INCLUDE) -D__WIIU__ -D__WUT__ -D__WUPS__  $(DEFINES)

ASFLAGS	:= -g $(ARCH)

LDFLAGS	= -g \
          $(ARCH) \
          $(RPXSPECS) \
          $(WUPSSPECS) \
          -Wl,-Map,$(notdir $*.map) \
          $(CXXFLAGS)

LIBS := -lcurlwrapper -lnotifications -lwups -lwut

#-------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level
# containing include and lib
#-------------------------------------------------------------------------------
LIBDIRS	:= $(WUMS_ROOT) $(WUPS_ROOT) $(WUT_ROOT) $(PORTLIBS)

#-------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#-------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#-------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#-------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#-------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#-------------------------------------------------------------------------------
	export LD	:=	$(CC)
#-------------------------------------------------------------------------------
else
#-------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES 	:=	$(OFILES_BIN) $(OFILES_SRC)
export HFILES_BIN	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean all

#-------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	$(MAKE) -C $(BUILD) -f $(CURDIR)/Makefile V=$(V)

#-------------------------------------------------------------------------------
clean:
	$(info clean ...)
	$(RM) -r $(BUILD) $(TARGET).wps $(TARGET).elf

#-------------------------------------------------------------------------------
else
.PHONY:	all

DEPENDS	:=	$(OFILES:.o=.d)

#-------------------------------------------------------------------------------
# main targets
#-------------------------------------------------------------------------------
all	:	$(OUTPUT).wps

$(OUTPUT).wps	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SRC)	: $(HFILES_BIN)

#-------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#-------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#-------------------------------------------------------------------------------
	$(info $(notdir $<))
	$(bin2o)

-include $(DEPENDS)

#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------


.PHONY: upload
upload: all
	curl --upload-file "$(TARGET).wps" "ftp://wiiu:/fs/vol/external01/wiiu/environments/aroma/plugins/"
	[ ! -f "$(PLUGIN_NAME).json" ] || curl --upload-file "$(PLUGIN_NAME).json" "ftp://wiiu:/fs/vol/external01/wiiu/environments/aroma/plugins/config/"


.PHONY: company
company: Makefile
	echo $(INCLUDE) -D__WIIU__ -D__WUT__ -D__WUPS__ | xargs -n1 | sort -u > compile_flags.txt
