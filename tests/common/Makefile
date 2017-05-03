include $(TLR_BASE)/build/rds00008.mk

CMATOMIC_BIN=cm_atomic
CMREF_BIN=cm_ref
CMLOG_BIN=cm_log
CMMODULE_BIN=cm_module_loader

INCLUDE=-I$(TLR_BASE)/common -I$(TLR_BASE)/lib/ccan

COMMON_LIBS=$(TLR_BASE)/common
CMATOMIC_OBJS=$(COMMON_LIBS)/cm_atomic.o
CMREF_OBJS=$(CMATOMIC_OBJS) $(COMMON_LIBS)/cm_ref.o
CMOBJECT_OBJS=$(CMREF_OBJS) $(COMMON_LIBS)/cm_object.o
CMMODULE_OBJS=$(CMOBJECT_OBJS) $(COMMON_LIBS)/cm_module.o \
	      $(COMMON_LIBS)/cm_module_loader.o

CMMODULE_EXAMPLE_SO=libcm-module-example.so
CMMODULE_EXAMPLE_SO_OBJS=libcm_module_example.o

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += $(INCLUDE) -DCM_DEBUG

.PHONY: all clean

all: $(CMATOMIC_BIN) $(CMREF_BIN) $(CMLOG_BIN) $(CMMODULE_BIN) $(CMMODULE_EXAMPLE_SO)

$(CMATOMIC_BIN): $(CMATOMIC_OBJS)
	$(CC) $(CMATOMIC_BIN).c -o $@ $^ $(CFLAGS)
$(CMREF_BIN): $(CMREF_OBJS)
	$(CC) $(CMREF_BIN).c -o $@ $^ $(CFLAGS)
$(CMLOG_BIN):
	$(CC) $(CMLOG_BIN).c -o $@ $(CFLAGS)
$(CMMODULE_BIN): $(CMMODULE_OBJS)
	$(CC) $(CMMODULE_BIN).c -o $@ $^ $(CFLAGS) -ldl -lpthread
$(CMMODULE_EXAMPLE_SO): $(CMMODULE_EXAMPLE_SO_OBJS)
	$(CC) -Wl,-soname,$(CMMODULE_EXAMPLE_SO) -shared $^ -o $@

# Dependancy rule
-include $(CMMODULE_EXAMPLE_SO_OBJS:.o=.d)

$(CMMODULE_EXAMPLE_SO_OBJS): %.o: %.c
	$(CC) -c -o $@ $< -MD -fPIC $(CFLAGS)

clean:
	rm -f $(CMATOMIC_BIN)
	rm -f $(CMREF_BIN)
	rm -f $(CMLOG_BIN)
	rm -f $(CMMODULE_BIN)
	rm -f $(CMMODULE_EXAMPLE_SO_OBJS)
	rm -f $(CMMODULE_EXAMPLE_SO)
	rm -f ./*.d
