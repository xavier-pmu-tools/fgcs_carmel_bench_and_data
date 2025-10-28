obj-m += module/pmu_module.o
obj-m += module/nv_pmu_module.o

KERNEL_MODULE_1 := pmu_module
KERNEL_MODULE_2 := nv_pmu_module

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo insmod module/$(KERNEL_MODULE_1).ko
	sudo insmod module/$(KERNEL_MODULE_2).ko

unload:
	sudo rmmod $(KERNEL_MODULE_1)
	sudo rmmod $(KERNEL_MODULE_2)

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules