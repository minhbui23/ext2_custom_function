# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the linux ext2-filesystem routines.
#

KDIR := /usr/src/linux-source-6.8.0/linux-source-6.8.0
PWD := $(shell pwd)

obj-$(CONFIG_EXT2_FS) += ext2.o

ext2-y := ext2_log.o balloc.o dir.o file.o ialloc.o inode.o ext2_check_free_space.o \
	  ioctl.o namei.o super.o symlink.o trace.o 

# For tracepoints to include our trace.h from tracepoint infrastructure
CFLAGS_trace.o := -I$(src)

ext2-$(CONFIG_EXT2_FS_XATTR)	 += xattr.o xattr_user.o xattr_trusted.o
ext2-$(CONFIG_EXT2_FS_POSIX_ACL) += acl.o
ext2-$(CONFIG_EXT2_FS_SECURITY)	 += xattr_security.o

obj-m += test_check_free_space.o

#obj-m += ext2_quota.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

debug:
	@echo "KDIR = $(KDIR)"
	@echo "PWD = $(PWD)"
