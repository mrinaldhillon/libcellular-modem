include $(TLR_BASE)/build/rds00008.mk

CM_MANAGER_IFACE_BIN=cm_manager_iface

INCLUDE=-I$(TLR_BASE)/cm-manager -I$(TLR_BASE)/common -I$(TLR_BASE)/lib/ccan

COMMON_LIBS=$(TLR_BASE)/common
CM_MANAGER_LIBS=$(TLR_BASE)/cm-manager

OBJS= $(COMMON_LIBS)/cm_atomic.o $(COMMON_LIBS)/cm_ref.o \
      $(COMMON_LIBS)/cm_object.o $(CM_MANAGER_LIBS)/cm_plugin_handler.o

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += -rdynamic $(INCLUDE) -DCM_DEBUG 

.PHONY: all clean

all: $(CM_MANAGER_IFACE_BIN) 

$(CM_MANAGER_IFACE_BIN): $(OBJS)
	$(CC) $(CM_MANAGER_IFACE_BIN).c -o $@ $^ $(CFLAGS) -ldl -lpthread

clean:
	rm -f $(CM_MANAGER_IFACE_BIN) ./*.d
