obj-m := bin_handler.o

KERNEL_DIR = /lib/modules/6.7.4/build
SUBDIR = `pwd`

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(SUBDIR) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(SUBDIR) clean
