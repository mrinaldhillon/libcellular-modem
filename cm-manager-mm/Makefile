include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan
COMMON_LIBDIR=$(TLR_BASE)/common
CMMANAGER_LIBDIR=$(TLR_BASE)/cm-manager-mm

CMAPI_INCDIR=$(TLR_BASE)/cm-api
INSTALL_LIBDIR=$(INSTALL_ROOT)/usr/lib/cmm-manager

#	$(COMMON_DIR)/cm_list.h \
	$(COMMON_DIR)/cm_container_of.h \
	$(CCAN_DIR)/ccan/list/list.h\
	$(CCAN_DIR)/ccan/container_of/container_of.h\
	$(CCAN_DIR)/ccan/check_type/check_type.h\
	$(CCAN_DIR)/ccan/str/str.h

INCLUDE=-I$(CMMANAGER_LIBDIR) \
	-I$(CMAPI_INCDIR) \
	-I$(CCAN_DIR)/ccan/list \
	-I$(CCAN_DIR)/ccan/container_of \
	-I$(CCAN_DIR) \
	-I /usr/include/ModemManager \
	-I /usr/include/libmm-glib


COMMON_LIBS=$(COMMON_LIBDIR)/cm_atomic.o \
	    $(COMMON_LIBDIR)/cm_ref.o \
	    $(COMMON_LIBDIR)/cm_object.o \
	    $(COMMON_LIBDIR)/cm_thread.o \
	    $(COMMON_LIBDIR)/cm_module.o \
	    $(COMMON_LIBDIR)/cm_module_loader.o

CMMANAGER_LIBS=	$(CMMANAGER_LIBDIR)/cm_modem.o \
		$(CMMANAGER_LIBDIR)/cm_modem_obj.o \
		$(CMMANAGER_LIBDIR)/cm_manager_obj_list_modems.o \
		$(CMMANAGER_LIBDIR)/cm_manager_obj_notify.o \
		$(CMMANAGER_LIBDIR)/cm_manager_obj.o \
		$(CMMANAGER_LIBDIR)/cm_manager.o \
		$(CMMANAGER_LIBDIR)/cm_bearer.o \
		$(CMMANAGER_LIBDIR)/cm_bearer_obj.o \
		$(CMMANAGER_LIBDIR)/cm_modem_obj_bearer.o \
		$(CMMANAGER_LIBDIR)/cm_bearer_properties.o \
		$(CMMANAGER_LIBDIR)/cm_bearer_properties_obj.o \
		$(CMMANAGER_LIBDIR)/cm_bearer_ip_config.o \
		$(CMMANAGER_LIBDIR)/cm_bearer_ip_config_obj.o \
		$(CMMANAGER_LIBDIR)/cm_modem_connect_properties.o \
		$(CMMANAGER_LIBDIR)/cm_modem_connect_properties_obj.o \
		$(CMMANAGER_LIBDIR)/cm_modem_connect.o






LIB_CMMANAGER=libcm-manager-mm.so

ifeq ($(DEBUG),true)
	CFLAGS += -g -DCM_DEBUG
endif

CFLAGS += -fPIC $(INCLUDE) $(shell pkg-config --cflags mm-glib)
LDFLAGS= -lpthread $(shell pkg-config --libs mm-glib)

.PHONY: all clean install

all: $(LIB_CMMANAGER)

$(LIB_CMMANAGER): $(COMMON_LIBS) $(CMMANAGER_LIBS)
	$(CC) -Wl,-soname,$(LIB_CMMANAGER) -shared $^ -o $@ $(LDFLAGS) $(CFLAGS)
-include $(CMMANAGER_LIBS:.o=.d)

$(CMMANAGER_LIBS): %.o: %.c
	$(CC) -c -o $@ $< -MD $(CFLAGS)

install: $(LIB_CMMANAGER)
	echo "install at $(INSTALL_LIBDIR)"
	-mkdir -p $(INSTALL_LIBDIR)
	install -m 0755 $^ $(INSTALL_LIBDIR)

clean:
	rm -f $(LIB_CMMANAGER) $(CMMANAGER_LIBS) $(CMMANAGER_LIBDIR)/*.d
