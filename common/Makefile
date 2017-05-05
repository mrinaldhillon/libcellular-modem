include $(TLR_BASE)/build/rds00008.mk

CCAN_DIR=$(TLR_BASE)/lib/ccan
CMAPI_INCDIR=$(TLR_BASE)/cm-api
COMMON_INCDIR=$(CMAPI_INCDIR)

COMMON_LIBDIR=.

INCLUDE=-I$(COMMON_INCDIR) -I$(CCAN_DIR)

COMMON_LIBS=$(COMMON_LIBDIR)/cm_atomic.o $(COMMON_LIBDIR)/cm_ref.o \
	$(COMMON_LIBDIR)/cm_object.o $(COMMON_LIBDIR)/cm_module.o \
	$(COMMON_LIBDIR)/cm_module_loader.o

ifeq ($(DEBUG),true)
CFLAGS += -g  -DCM_DEBUG
endif

CFLAGS += -fPIC $(INCLUDE)

.PHONY: all clean install

all: $(COMMON_LIBS)

-include $(COMMON_LIBS:.o=.d)
$(COMMON_LIBS): %.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
clean:
	rm -f $(COMMON_LIBS) $(COMMON_LIBDIR)/*.d
