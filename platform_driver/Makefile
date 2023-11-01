KEVN := $(shell uname -r)
PWD  := $(shell pwd)
KERN_DIR := /lib/modules/$(KEVN)/build

obj-m += dirver.o
obj-m += device.o

all:
	make	-C $(KERN_DIR) M=$(PWD) modules
clean:
	make	-C $(KERN_DIR) M=$(PWD) clean