include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan
COMMON_LIBDIR=$(TLR_BASE)/common
CMMANAGER_LIBDIR=.
MODEMMANAGER_INCDIR=/usr/include/ModemManager
LIBMMGLIB_INCDIR=/usr/include/libmm-glib
GLIB_INCDIR=/usr/include/glib-2.0
GLIB_LIBINCDIR=/usr/lib64/glib-2.0/include
CMAPI_INCDIR=$(TLR_BASE)/cm-api
INSTALL_LIBDIR=$(INSTALL_ROOT)/usr/lib/cmm-manager

#	$(COMMON_DIR)/cm_list.h \
	$(COMMON_DIR)/cm_container_of.h \
	$(CCAN_DIR)/ccan/list/list.h\
	$(CCAN_DIR)/ccan/container_of/container_of.h\
	$(CCAN_DIR)/ccan/check_type/check_type.h\
	$(CCAN_DIR)/ccan/str/str.h

INCLUDE=-I$(CMAPI_INCDIR) \
	-I$(GLIB_INCDIR) \
	-I$(GLIB_LIBINCDIR) \
	-I$(LIBMMGLIB_INCDIR) \
	-I$(MODEMMANAGER_INCDIR) \
	-I$(CCAN_DIR)/ccan/list \
	-I$(CCAN_DIR)/ccan/container_of \
	-I$(CCAN_DIR)

COMMON_LIBS=$(COMMON_LIBDIR)/cm_atomic.o \
	    $(COMMON_LIBDIR)/cm_ref.o \
	    $(COMMON_LIBDIR)/cm_object.o \
	    $(COMMON_LIBDIR)/cm_module.o \
	    $(COMMON_LIBDIR)/cm_module_loader.o

CMMANAGER_LIBS=$(CMMANAGER_LIBDIR)/cm_manager.o \
		$(CMMANAGER_LIBDIR)/cm_manager_obj.o

LIB_CMMANAGER=libcm-manager-mm.so

ifeq ($(DEBUG),true)
	CFLAGS += -g -DCM_DEBUG
endif

CFLAGS += -fPIC $(INCLUDE)
LDFLAGS= -lpthread -ldl -lmm-glib -lglib-2.0

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
	rm -f $(LIB_CMMANAGER) $(CMMANAGER_LIBS) $(LIB_CMMANAGER)/*.d
