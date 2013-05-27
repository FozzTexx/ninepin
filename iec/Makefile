ifneq ($(KERNELRELEASE),)
obj-m	:= iec.o

else
KDIR	:= /usr/src/linux
PWD		:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
endif
