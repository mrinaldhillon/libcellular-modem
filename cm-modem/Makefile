include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan
COMMON_LIBDIR=$(TLR_BASE)/common
CMAPI_INCDIR=$(TLR_BASE)/cm-api

#	$(COMMON_LIBDIR)/cm_list.h \
	$(COMMON_LIBDIR)/cm_container_of.h \
	$(CCAN_DIR)/ccan/list/list.h\
	$(CCAN_DIR)/ccan/container_of/container_of.h\
	$(CCAN_DIR)/ccan/check_type/check_type.h\
	$(CCAN_DIR)/ccan/str/str.h

INCLUDE=-I$(CMAPI_INCDIR) \
	-I $(CCAN_DIR)/ccan/list \
	-I $(CCAN_DIR)/ccan/container_of \
	-I $(CCAN_DIR)

COMMON_LIBS=$(COMMON_LIBDIR)/cm_atomic.o \
	    $(COMMON_LIBDIR)/cm_ref.o \
	    $(COMMON_LIBDIR)/cm_object.o

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
