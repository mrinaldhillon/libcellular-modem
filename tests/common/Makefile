include $(TLR_BASE)/build/rds00008.mk

CMATOMIC_BIN=cm_atomic
CMREF_BIN=cm_ref
CMLOG_BIN=cm_log

INCLUDE=-I$(TLR_BASE)/common -I$(TLR_BASE)/lib/ccan

COMMON_LIBS=$(TLR_BASE)/common
CMATOMIC_OBJS=$(COMMON_LIBS)/cm_atomic.o
CMREF_OBJS=$(CMATOMIC_OBJS) $(COMMON_LIBS)/cm_ref.o

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += $(INCLUDE) -DCM_DEBUG

.PHONY: all clean

all: $(CMATOMIC_BIN) $(CMREF_BIN) $(CMLOG_BIN)

$(CMATOMIC_BIN): $(CMATOMIC_OBJS)
	$(CC) $(CMATOMIC_BIN).c -o $@ $^ $(CFLAGS)
$(CMREF_BIN): $(CMREF_OBJS)
	$(CC) $(CMREF_BIN).c -o $@ $^ $(CFLAGS)
$(CMLOG_BIN):
	$(CC) $(CMLOG_BIN).c -o $@ $(CFLAGS)

clean:
	rm -f $(CMATOMIC_BIN)
	rm -f $(CMREF_BIN)
	rm -f $(CMLOG_BIN)
	rm -f ./*.d
