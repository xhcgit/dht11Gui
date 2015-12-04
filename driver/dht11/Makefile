obj-m := dht11.o
KDIR := /home/linux-tq2440
all:
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux- ARCH=arm
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.bak *.order

