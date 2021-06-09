#
# Makefile
#
CC ?= gcc
LVGL_DIR ?= ..
CFLAGS ?= -Wall -Wshadow -Wundef -Wmaybe-uninitialized -O0 -g 
CFLAGS += -DLV_CONF_INCLUDE_SIMPLE
CFLAGS += -I$(LVGL_DIR)/
CFLAGS += -I$(LVGL_DIR)/lvgl/
CFLAGS += -I../../Inc/

LDFLAGS ?= -lSDL2 -lm
BIN = pcsim

BUILD_DIR = build

#Collect the files to compile
MAINSRC = main.c
MAINSRC += gfx_mgr.c
MAINSRC += gfx_mode_clock.c

include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk
#include $(LVGL_DIR)/lv_examples/lv_examples.mk
#include $(LVGL_DIR)/lv_examples/lv_apps/scoreboard/scoreboard.mk

#CSRCS +=$(LVGL_DIR)/mouse_cursor_icon.c

VPATH += ../../Src

OBJEXT ?= .o

AOBJS = $(addprefix $(BUILD_DIR)/,$(ASRCS:.S=$(OBJEXT)))
COBJS = $(addprefix $(BUILD_DIR)/,$(CSRCS:.c=$(OBJEXT)))

MAINOBJ = $(addprefix $(BUILD_DIR)/,$(MAINSRC:.c=$(OBJEXT)))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

## MAINOBJ -> OBJFILES

all: default 

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"
    
default: $(AOBJS) $(COBJS) $(MAINOBJ)
	$(CC) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(LDFLAGS)

clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ)

$(BUILD_DIR):
	mkdir $@	

print-%  : ; @echo $* = $($*)
