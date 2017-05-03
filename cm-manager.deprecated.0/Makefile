include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan
COMMON_DIR=$(TLR_BASE)/common
CMMODEM_DIR=$(TLR_BASE)/cm-modem

INCLUDES=$(COMMON_DIR)/cm_err.h \
	$(COMMON_DIR)/cm_log.h \
	./cm_manager.h \
	$(CMMODEM_DIR)/cm_modem.h \
	$(CMMODEM_DIR)/cm_modem_state.h \
	$(CMMODEM_DIR)/cm_bearer.h \
	$(CMMODEM_DIR)/cm_bearer_properties.h \
	$(CMMODEM_DIR)/cm_bearer_ip_config.h \
	$(CMMODEM_DIR)/cm_bearer_stats.h \
	$(CMMODEM_DIR)/cm_bearer_state.h \
	$(CMMODEM_DIR)/cm_ip_type.h \
	$(CMMODEM_DIR)/cm_signal.h \
	$(CMMODEM_DIR)/cm_signal_type.h \
	$(CMMODEM_DIR)/cm_sim.h \
	$(CMMODEM_DIR)/cm_messaging.h
	

#	$(COMMON_DIR)/cm_list.h \
	$(COMMON_DIR)/cm_container_of.h \
	$(CCAN_DIR)/ccan/list/list.h\
	$(CCAN_DIR)/ccan/container_of/container_of.h\
	$(CCAN_DIR)/ccan/check_type/check_type.h\
	$(CCAN_DIR)/ccan/str/str.h

INCLUDE=-I. -I$(COMMON_DIR) \
	-I$(TLR_BASE)/cm-modem \
	-I$(CCAN_DIR)/ccan/list \
	-I$(CCAN_DIR)/ccan/container_of \
	-I$(CCAN_DIR)

COMMON_LIBS=$(COMMON_DIR)/cm_atomic.o \
	    $(COMMON_DIR)/cm_ref.o \
	    $(COMMON_DIR)/cm_object.o \
	    $(COMMON_DIR)/cm_module.o \
	    $(COMMON_DIR)/cm_module_loader.o

LIBOBJS=./cm_manager.o
LIBRARY=libcm-manager.so
CFLAGS=
ifeq ($(DEBUG),true)
CFLAGS += -g -DCM_DEBUG
endif

CFLAGS += -fPIC $(INCLUDE)
LDFLAGS= -lpthread -ldl

.PHONY: all clean install

all: $(LIBRARY)

$(LIBRARY): $(LIBOBJS) $(COMMON_LIBS)
	echo "install root $(INSTALL_ROOT)"
	$(CC) -Wl,-soname,$(LIBRARY) -shared $^ -o $@ $(LDFLAGS)

-include $(LIBOBJS:.o=.d)

$(LIBOBJS): %.o: %.c
	$(CC) -c -o $@ $< -MD $(CFLAGS)

install: $(LIBRARY)
	-mkdir -p $(INSTALL_ROOT)/usr/lib
	-mkdir -p $(INSTALL_ROOT)/usr/include
	install -m 0755 $^ $(INSTALL_ROOT)/usr/lib
	install -m 0644 $(INCLUDES) $(INSTALL_ROOT)/usr/include
#	cp -r $(CCAN_DIR)/ccan $(INSTALL_ROOT)/usr/include


clean:
	rm -f $(LIBRARY) $(LIBOBJS) $(LIBS_DIR)/*.d
