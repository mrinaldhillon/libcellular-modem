include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan
COMMON_DIR=$(TLR_BASE)/common

INCLUDES=$(COMMON_DIR)/cm_err.h \
	$(COMMON_DIR)/cm_log.h \
	./cm_manager.h \
	./cm_modem.h \
	./cm_modem_state.h \
	./cm_bearer.h \
	./cm_bearer_properties.h \
	./cm_bearer_ip_config.h \
	./cm_bearer_stats.h \
	./cm_bearer_state.h \
	./cm_ip_type.h \
	./cm_signal.h \
	./cm_signal_type.h \
	./cm_sim.h \
	./cm_messaging.h
	

#	$(COMMON_DIR)/cm_list.h \
	$(COMMON_DIR)/cm_container_of.h \
	$(CCAN_DIR)/ccan/list/list.h\
	$(CCAN_DIR)/ccan/container_of/container_of.h\
	$(CCAN_DIR)/ccan/check_type/check_type.h\
	$(CCAN_DIR)/ccan/str/str.h

INCLUDE=-I. -I$(COMMON_DIR) \
	-I $(CCAN_DIR)/ccan/list \
	-I $(CCAN_DIR)/ccan/container_of \
	-I $(CCAN_DIR)


COMMON_LIBS=$(COMMON_DIR)/cm_atomic.o $(COMMON_DIR)/cm_ref.o
LIBOBJS = ./cm_plugin_handler.o ./cm_manager.o
LIBRARY=libcm-manager.so

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += -fPIC $(INCLUDE) -DCM_DEBUG

.PHONY: all clean install

all: $(LIBRARY)

$(LIBRARY):$(LIBOBJS) $(COMMON_LIBS)
	echo "install root $(INSTALL_ROOT)"
	$(CC) -Wl,-soname,$(LIBRARY) -shared $^ -o $@ $(LDFLAGS) -lpthread -ldl

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
