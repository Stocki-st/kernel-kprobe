DRIVER = myprobe
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := $(DRIVER).o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
	
install:
	sudo insmod $(DRIVER).ko hook="vfs_open"
	
remove:
	sudo rmmod $(DRIVER)
	
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

all: default 
