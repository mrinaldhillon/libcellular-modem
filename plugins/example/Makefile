include $(TLR_BASE)/build/rds00008.mk

INCLUDES_DIR=.
LIBCM_MANAGER_INC_DIR=$(TLR_BASE)/cm-manager
LIBS_DIR=.
COMMON_DIR=$(TLR_BASE)/common
COMMON_LIBS=$(COMMON_DIR)/cm_atomic.o $(COMMON_DIR)/cm_ref.o

INCLUDE=-I$(LIBCM_MANAGER_INC_DIR) -I$(INCLUDES_DIR)\
	-I$(TLR_BASE)/lib/ccan\
	-I$(COMMON_DIR)

LIBOBJS=$(LIBS_DIR)/cm_manager_iface_example.o

LIBRARY=libcm-manager-plugin-example.so
LIBRARY1=libcm-manager-plugin-example1.so

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += -fPIC $(INCLUDE) -DCM_DEBUG
#TBD: should libcellular-device link with libqmi?
# currently application will have to link both cellular-device and qmi
# i.e. -lcellular-device -lqmi

.PHONY: all clean install

all: $(LIBRARY)

$(LIBRARY):$(LIBOBJS) $(COMMON_LIBS)
	echo "install root $(INSTALL_ROOT)"
	$(CC) -Wl,-soname,$(LIBRARY) -shared $^ -o $@ $(LDFLAGS) -lpthread 
$(LIBRARY1):$(LIBOBJS) $(COMMON_LIBS)
	echo "install root $(INSTALL_ROOT)"
	$(CC) -Wl,-soname,$(LIBRARY) -shared $^ -o $@ $(LDFLAGS) -lpthread

# Dependancy rule
-include $(LIBOBJS:.o=.d)

$(LIBOBJS): %.o: %.c
	$(CC) -c -o $@ $< -MD $(CFLAGS)

$(LIBOBJS): %.o: %.c
	$(CC) -c -o $@ $< -MD $(CFLAGS)


install: $(LIBRARY) $(LIBRARY1)
	-mkdir -p $(INSTALL_ROOT)/usr/lib/cm-manager
	install -m 0755 $^ $(INSTALL_ROOT)/usr/lib/cm-manager

clean:
	rm -f $(LIBRARY) $(LIBRARY1) $(LIBOBJS) $(LIBS_DIR)/*.d
