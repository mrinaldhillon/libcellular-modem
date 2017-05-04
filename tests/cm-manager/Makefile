include $(TLR_BASE)/build/rds00008.mk

CM_MANAGER_BIN=cm_manager

ifeq ($(DEBUG),true)
CFLAGS += -g -DCM_DEBUG
endif

CFLAGS += -I$(INSTALL_ROOT)/usr/include 

.PHONY: all clean

all: $(CM_MANAGER_BIN) 

$(CM_MANAGER_BIN):
	$(CC) $(CM_MANAGER_BIN).c -o $@ $(CFLAGS) -L $(INSTALL_ROOT)/usr/lib/ -lcm-manager

clean:
	rm -f $(CM_MANAGER_BIN) ./*.d
