TARGET =
TLR_BASE := $(shell cd ..; pwd)
export TLR_BASE

include $(TLR_BASE)/build/makefile.globals

TARGETS += ../common ../tests
# ../src

TARGETS_CLEAN = $(addsuffix .clean,$(TARGETS) $(CLEAN_TARGETS))
TARGETS_INSTALL = $(addsuffix .install,$(TARGETS) )

export TARGET
export INSTALL_ROOT

.PHONY: all
all: $(TARGETS)

.PHONY: $(TARGETS)

#../src:
#	$(MAKE) -C $@ all install
../common:
	$(MAKE) -C $@ all

../tests:
	$(MAKE) -C $@ all

#../plugins:
#	$(MAKE) -C $@ all install

.PHONY: clean
clean: $(TARGETS_CLEAN)

.PHONY: $(TARGETS_CLEAN)
$(TARGETS_CLEAN):
	$(MAKE) -C $(basename $@) clean

.PHONY: install
install: $(TARGETS_INSTALL)

.PHONY: $(TARGETS_INSTALL)
$(TARGETS_INSTALL):
	$(MAKE) -C $(basename $@) install
