#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>

#include "../include/pmu_cfg.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Iosu Gomez Iturrioz");
MODULE_DESCRIPTION("Kernel module to control event monitoring.");
MODULE_VERSION("0.1");

static uint64_t read_event_counter(uint32_t counter_index) {
    uint64_t counter;
    asm volatile("msr pmselr_el0, %0" : : "r"(counter_index));
    asm volatile("mrs %0, pmxevcntr_el0" : "=r"(counter));
    return counter;
}

static void configure_pmu(struct pmu_config conf) {
    asm volatile("msr pmuserenr_el0, %0" : : "r"(1));  // Enable user access to PMU
    asm volatile("msr pmcr_el0, %0" : : "r"(1));       // Enable PMU
    asm volatile("msr pmselr_el0, %0" : : "r"(conf.counter_index)); //Select event counter counter_index
    asm volatile("msr pmxevtyper_el0, %0" : : "r"(conf.event_type));  // Example event: CPU cycles
}

static void enable_counters(uint32_t counter_index) {
    asm volatile("msr pmcntenset_el0, %0" : : "r"(1 << counter_index));  // Enable event counter counter_index    
}

static void disable_counters(uint32_t counter_index) {
    asm volatile("msr pmcntenclr_el0, %0" : : "r"(1 << counter_index));  // Disable event counter counter_index
}

static void reset_counters(uint32_t counter_index) {
    asm volatile("msr pmovsclr_el0, %0" : : "r"(1 << counter_index));    // Reset overflow flags
    asm volatile("msr pmselr_el0, %0" : : "r"(counter_index)); // Reset event counter counter_index
    asm volatile("msr pmxevcntr_el0, %0" : : "r"(0));
    //asm volatile("msr pmevcntr0_el0, %0" : : "r"(0));        // Reset event counter counter_index
}

static long pmu_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {

    struct pmu_config config;

    switch (cmd) {
        case IOCTL_ENABLE_MONITORING:
            if (copy_from_user(&config, (struct pmu_config *)arg, sizeof(config))) {
                return -EFAULT;
            }
            configure_pmu(config);
            printk(KERN_INFO "Configured PMU: Counter index %d, Event type %x\n", config.counter_index, config.event_type);
            break;
        case IOCTL_START_MONITORING:
            if (copy_from_user(&config, (struct pmu_config *)arg, sizeof(config))) {
                return -EFAULT;
            }
            reset_counters(config.counter_index);
            enable_counters(config.counter_index);
            printk(KERN_INFO "PMU: Monitoring started on counter %d\n", config.counter_index);
            break;
        case IOCTL_STOP_MONITORING:
            if (copy_from_user(&config, (struct pmu_config *)arg, sizeof(config))) {
                return -EFAULT;
            }
            disable_counters(config.counter_index);
            printk(KERN_INFO "PMU: Monitoring stopped on counter %d\n", config.counter_index);
            break;
        case IOCTL_DISABLE_MONITORING:
            asm volatile("msr pmcr_el0, %0" : : "r"(0));  // Disable PMU
            printk(KERN_INFO "PMU disabled\n");
            break;
        case IOCTL_READ_COUNTER:
            if (copy_from_user(&config, (struct pmu_config *)arg, sizeof(config))) {
                return -EFAULT;
            }
            config.counter_value = read_event_counter(config.counter_index);
            if (copy_to_user((struct pmu_config *)arg, &config, sizeof(config))) {
                return -EFAULT;
            }
            printk(KERN_INFO "PMU Counter value read: %llu\n", config.counter_value);
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .unlocked_ioctl = pmu_ioctl,
};

static int major_number;
static struct class *pmu_class = NULL;
static struct device *pmu_device = NULL;

static int __init pmu_module_init(void) {
    major_number = register_chrdev(0, "pmu", &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "PMU module failed to register a major number\n");
        return major_number;
    }

    pmu_class = class_create(THIS_MODULE, "pmu");
    if (IS_ERR(pmu_class)) {
        unregister_chrdev(major_number, "pmu");
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(pmu_class);
    }

    pmu_device = device_create(pmu_class, NULL, MKDEV(major_number, 0), NULL, "pmu");
    if (IS_ERR(pmu_device)) {
        class_destroy(pmu_class);
        unregister_chrdev(major_number, "pmu");
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(pmu_device);
    }

    printk(KERN_INFO "PMU module loaded with device major number %d\n", major_number);
    return 0;
}

static void __exit pmu_module_exit(void) {
    device_destroy(pmu_class, MKDEV(major_number, 0));
    class_unregister(pmu_class);
    class_destroy(pmu_class);
    unregister_chrdev(major_number, "pmu");
    printk(KERN_INFO "PMU module unloaded\n");
}

module_init(pmu_module_init);
module_exit(pmu_module_exit);
