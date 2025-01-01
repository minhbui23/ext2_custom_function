KDIR := /usr/src/linux-source-6.8.0/linux-source-6.8.0
PWD := $(shell pwd)


obj-m += ext2_quota.o


all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

debug:
	@echo "KDIR = $(KDIR)"
	@echo "PWD = $(PWD)"
