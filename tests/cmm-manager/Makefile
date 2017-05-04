include $(TLR_BASE)/build/rds00008.mk

CMM_MANAGER_BIN=cmm_manager
INCLUDE=-I$(INSTALL_ROOT)/usr/include 


ifeq ($(DEBUG),true)
CFLAGS += -g -DCM_DEBUG
endif

.PHONY: all clean

all: $(CMM_MANAGER_BIN) 

$(CMM_MANAGER_BIN):
	$(CC) $(CMM_MANAGER_BIN).c -o $@ $(INCLUDE) $(CFLAGS) -L $(INSTALL_ROOT)/usr/lib/ -lcmm-manager

clean:
	rm -f $(CMM_MANAGER_BIN) ./*.d
