include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan
COMMON_LIBDIR=$(TLR_BASE)/common
CMMMANAGER_LIBDIR=$(TLR_BASE)/cmm-manager

CMAPI_INCDIR=$(TLR_BASE)/cm-api
INSTALL_LIBDIR=$(INSTALL_ROOT)/usr/lib

#	$(COMMON_DIR)/cm_list.h \
	$(COMMON_DIR)/cm_container_of.h \
	$(CCAN_DIR)/ccan/list/list.h\
	$(CCAN_DIR)/ccan/container_of/container_of.h\
	$(CCAN_DIR)/ccan/check_type/check_type.h\
	$(CCAN_DIR)/ccan/str/str.h

INCLUDE=-I$(CMMMANAGER_LIBDIR) \
	-I$(CMAPI_INCDIR) \
	-I$(CCAN_DIR)/ccan/list \
	-I$(CCAN_DIR)/ccan/container_of \
	-I$(CCAN_DIR)

COMMON_LIBS=$(COMMON_LIBDIR)/cm_atomic.o \
	    $(COMMON_LIBDIR)/cm_ref.o \
	    $(COMMON_LIBDIR)/cm_object.o \
	    $(COMMON_LIBDIR)/cm_thread.o \
	    $(COMMON_LIBDIR)/cm_module.o \
	    $(COMMON_LIBDIR)/cm_module_loader.o

CMMMANAGER_LIBS=$(CMMMANAGER_LIBDIR)/cmm_manager.o \
		$(CMMMANAGER_LIBDIR)/cmm_manager_obj.o

LIB_CMMMANAGER=libcmm-manager.so

ifeq ($(DEBUG),true)
	CFLAGS += -g -DCM_DEBUG
endif

CFLAGS += -fPIC $(INCLUDE) 
LDFLAGS= -lpthread -ldl

.PHONY: all clean install

all: $(LIB_CMMMANAGER)

$(LIB_CMMMANAGER): $(COMMON_LIBS) $(CMMMANAGER_LIBS)
	$(CC) -Wl,-soname,$(LIB_CMMMANAGER) -shared $^ -o $@ $(LDFLAGS) $(CFLAGS) 

-include $(CMMMANAGER_LIBS:.o=.d)

$(CMMMANAGER_LIBS): %.o: %.c
	$(CC) -c -o $@ $< -MD $(CFLAGS) -DCMM_MANAGER_LIBDIR=\"$(INSTALL_LIBDIR)/cmm-manager\"

install: $(LIB_CMMMANAGER)
	echo "install at $(INSTALL_LIBDIR)"
	-mkdir -p $(INSTALL_LIBDIR)
	install -m 0755 $^ $(INSTALL_LIBDIR)

clean:
	rm -f $(LIB_CMMMANAGER) $(CMMMANAGER_LIBS) $(LIB_CMMMANAGER)/*.d
