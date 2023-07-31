#  SPDX-License-Identifier: BSD-3-Clause
#  Copyright (C) 2015 Intel Corporation.
#  All rights reserved.
#

SPDK_ROOT_DIR := $(abspath $(CURDIR)/../spdk)
include $(SPDK_ROOT_DIR)/mk/spdk.common.mk
include $(SPDK_ROOT_DIR)/mk/spdk.modules.mk

APP = main

C_SRCS := \
./main.c \
./lib/pio.c \
./lib/shm.c \
./lib/spdk.c \
./lib/atomic.c \
./lib/work_queue.c \
./lib/ssd_cache_api.c  \
./lib/ssd_cache_target.c \
./lib/ssd_cache_mapping.c \

CFLAGS += -I$(abspath $(CURDIR)/inc)
CFLAGS += -g -O2

SPDK_LIB_LIST = $(ALL_MODULES_LIST) event event_bdev

include $(SPDK_ROOT_DIR)/mk/spdk.app.mk