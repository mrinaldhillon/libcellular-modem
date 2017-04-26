include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan/ccan

INCLUDE=-I. -I$(CCAN_DIR)/list \
	-I$(CCAN_DIR)/container_of

LIBOBJS=./cm_atomic.o ./cm_ref.o

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += -fPIC $(INCLUDE)

.PHONY: all clean

all: $(LIBOBJS)

-include $(LIBOBJS:.o=.d)
$(LIBOBJS): %.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(LIBOBJS) $(LIBS_DIR)/*.d
