include $(TLR_BASE)/build/rds00008.mk

CMATOMIC_BIN=cm_atomic
CMREF_BIN=cm_ref
CMLOG_BIN=cm_log
CMMODULE_BIN=cm_module_loader

CMAPI_INCDIR=$(TLR_BASE)/cm-api

INCLUDE=-I$(CMAPI_INCDIR) -I$(TLR_BASE)/lib/ccan

COMMON_LIBDIR=$(TLR_BASE)/common
CMATOMIC_OBJS=$(COMMON_LIBDIR)/cm_atomic.o
CMREF_OBJS=$(CMATOMIC_OBJS) $(COMMON_LIBDIR)/cm_ref.o
CMOBJECT_OBJS=$(CMREF_OBJS) $(COMMON_LIBDIR)/cm_object.o
CMMODULE_OBJS=$(CMOBJECT_OBJS) $(COMMON_LIBDIR)/cm_module.o \
	      $(COMMON_LIBDIR)/cm_module_loader.o

CMMODULE_FOO_SO=libcm-module-foo.so
CMMODULE_FOO_SO_OBJS=libcm_module_foo.o

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += $(INCLUDE) -DCM_DEBUG

.PHONY: all clean

all: $(CMATOMIC_BIN) $(CMREF_BIN) $(CMLOG_BIN) $(CMMODULE_BIN) $(CMMODULE_FOO_SO)

$(CMATOMIC_BIN): $(CMATOMIC_OBJS)
	$(CC) $(CMATOMIC_BIN).c -o $@ $^ $(CFLAGS)
$(CMREF_BIN): $(CMREF_OBJS)
	$(CC) $(CMREF_BIN).c -o $@ $^ $(CFLAGS)
$(CMLOG_BIN):
	$(CC) $(CMLOG_BIN).c -o $@ $(CFLAGS)
$(CMMODULE_BIN): $(CMMODULE_OBJS)
	$(CC) $(CMMODULE_BIN).c -o $@ $^ $(CFLAGS) -ldl -lpthread
$(CMMODULE_FOO_SO): $(CMMODULE_FOO_SO_OBJS)
	$(CC) -Wl,-soname,$(CMMODULE_FOO_SO) -shared $^ -o $@

# Dependancy rule
-include $(CMMODULE_FOO_SO_OBJS:.o=.d)

$(CMMODULE_FOO_SO_OBJS): %.o: %.c
	$(CC) -c -o $@ $< -MD -fPIC $(CFLAGS)

clean:
	rm -f $(CMATOMIC_BIN)
	rm -f $(CMREF_BIN)
	rm -f $(CMLOG_BIN)
	rm -f $(CMMODULE_BIN)
	rm -f $(CMMODULE_FOO_SO_OBJS)
	rm -f $(CMMODULE_FOO_SO)
	rm -f ./*.d
