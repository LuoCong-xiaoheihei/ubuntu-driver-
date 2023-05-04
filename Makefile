
KEVN := $(shell uname -r)
PWD  := $(shell pwd)
KERN_DIR := /lib/modules/$(KEVN)/build

obj-m += hello.o
obj-m += hello_world.o
obj-m += filectl.o
obj-m += private_data.o

all:
	make	-C $(KERN_DIR) M=$(PWD) modules
	$(CROSS_COMPILE)gcc -o file_test file_test.c 
	$(CROSS_COMPILE)gcc -o private_test private_test.c
clean:
	make	-C $(KERN_DIR) M=$(PWD) clean


