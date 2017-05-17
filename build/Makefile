TARGET =
TLR_BASE := $(shell cd ..; pwd)
export TLR_BASE

include $(TLR_BASE)/build/makefile.globals

TARGETS += ../common ../cm-manager-mm ../tests

TARGETS_CLEAN = $(addsuffix .clean,$(TARGETS) $(CLEAN_TARGETS))
TARGETS_INSTALL = $(addsuffix .install,$(TARGETS) )

export TARGET
export INSTALL_ROOT

.PHONY: all
all: $(TARGETS)

.PHONY: $(TARGETS)
../common:
	$(MAKE) -C $@ all
../cm-manager-mm:
	$(MAKE) -C $@ all
../tests:
	$(MAKE) -C $@ all
../cm-manager:
	$(MAKE) -C $@ all

#../cmm-manager:
#	$(MAKE) -C $@ all

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
	mkdir -p $(INSTALL_ROOT)/usr/include
	cp -r $(TLR_BASE)/cm-api $(INSTALL_ROOT)/usr/include/libcm-api
