# RDS/00008: OASIS Components Makefile Conventions

ifdef TARGET
    export TARGET_PREFIX = $(TARGET)-
endif

ifndef CROSS_COMPILE
    export CROSS_COMPILE = $(TARGET_PREFIX)
    export AS      = $(CROSS_COMPILE)as
    export LD      = $(CROSS_COMPILE)ld
    export CC      = $(CROSS_COMPILE)gcc
    export CPP     = $(CROSS_COMPILE)g++
    export AR      = $(CROSS_COMPILE)ar
    export NM      = $(CROSS_COMPILE)nm
    export STRIP   = $(CROSS_COMPILE)strip
    export OBJCOPY = $(CROSS_COMPILE)objcopy
    export OBJDUMP = $(CROSS_COMPILE)objdump
endif

export CXX = $(CROSS_COMPILE)g++

LIBPATH = -L$(INSTALL_ROOT)/usr/lib -L$(INSTALL_ROOT)/lib -Wl,-rpath-link=$(INSTALL_ROOT)/lib
DEBUG=true
CFLAGS = -Wall -Werror --std=gnu99 -D_GNU_SOURCE -O0 -g -MD -Wno-unused-function
