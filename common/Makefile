include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan/

INCLUDE=-I. -I$(CCAN_DIR)

LIBOBJS=./cm_atomic.o ./cm_ref.o ./cm_object.o ./cm_module.o ./cm_module_loader.o

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += -fPIC $(INCLUDE) -DCM_DEBUG

.PHONY: all clean

all: $(LIBOBJS)

-include $(LIBOBJS:.o=.d)
$(LIBOBJS): %.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(LIBOBJS) $(LIBS_DIR)/*.d
