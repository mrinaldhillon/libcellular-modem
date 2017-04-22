include $(TLR_BASE)/build/rds00008.mk

INCLUDES_DIR=.
LIBS_DIR=.

INCLUDE=-I$(INCLUDES_DIR)

LIBOBJS = $(LIBS_DIR)/cm_atomic.o $(LIBS_DIR)/cm_ref.o

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += $(INCLUDE)

.PHONY: all clean

all: $(LIBOBJS)

$(LIBOBJS): %.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

#-include $(LIBOBJS:.o=.d)
#$(LIBOBJS): %.o: %.c
#	$(CC) -c -o $@ $< -MD $(CFLAGS)

clean:
	rm -f $(LIBOBJS) $(LIBS_DIR)/*.d
