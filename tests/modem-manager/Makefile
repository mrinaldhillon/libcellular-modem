include $(TLR_BASE)/build/rds00008.mk

ifeq ($(DEBUG),true)
CFLAGS += -g
endif

CFLAGS += $(INCLUDE) $(shell pkg-config --cflags mm-glib)
LDFLAGS= $(shell pkg-config --libs mm-glib)

.PHONY: all clean

MODEM_MANAGER_TESTBIN=mm_test
MODEM_MANAGER_SIMPLE_TESTBIN=mm_test_simple

all: $(MODEM_MANAGER_TESTBIN) $(MODEM_MANAGER_SIMPLE_TESTBIN)


$(MODEM_MANAGER_TESTBIN):
	$(CC) $(MODEM_MANAGER_TESTBIN).c -o $@ $(CFLAGS) $(LDFLAGS)
$(MODEM_MANAGER_SIMPLE_TESTBIN):
	$(CC) $(MODEM_MANAGER_SIMPLE_TESTBIN).c -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(MODEM_MANAGER_TESTBIN)
	rm -f $(MODEM_MANAGER_SIMPLE_TESTBIN)
