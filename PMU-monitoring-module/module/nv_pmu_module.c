#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>

#include "../include/pmu_cfg.h"
#include "../include/uncore-pmu-regs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Iosu Gomez Iturrioz");
MODULE_DESCRIPTION("Kernel module to control NVIDIA event monitoring.");
MODULE_VERSION("0.1");

static uint64_t read_event_counter(struct carmel_pmu_config conf) {
    uint32_t unit_group_config = (conf.unit_group << 8) | (conf.unit & 0xFF);
    uint32_t counter;
    asm volatile("msr " __stringify(NV_PMSELR_EL0) ", %0" : : "r"(unit_group_config));
    switch (conf.counter_index) {
        case 0:
            asm volatile("mrs %0, " __stringify(NV_PMEVCNTR0_EL0) "" : "=r"(counter));
            break;
        case 1:
            asm volatile("mrs %0, " __stringify(NV_PMEVCNTR1_EL0) "" : "=r"(counter));
            break;
        default:
            return -EINVAL;
    }
    return counter;
}

static void configure_pmu(struct carmel_pmu_config conf) {
    uint32_t unit_group_config = (conf.unit_group << 8) | (conf.unit & 0xFF);
    asm volatile("msr " __stringify(NV_PMCR_EL0) ", %0" : : "r"(1));
    asm volatile("msr " __stringify(NV_PMSELR_EL0) ", %0" : : "r"(unit_group_config));
    switch (conf.counter_index) {
        case 0:
            asm volatile("msr " __stringify(NV_PMEVTYPER0_EL0) ", %0" : : "r"(conf.event_type & 0xFF));
            break;
        case 1:
            asm volatile("msr " __stringify(NV_PMEVTYPER1_EL0) ", %0" : : "r"(conf.event_type & 0xFF));
            break;
        default:
            break;
    }
}

static void enable_counters(uint8_t counter_index) {
    asm volatile("msr " __stringify(NV_PMCNTENSET_EL0) ", %0" : : "r"(1 << counter_index));  // Enable event counter counter_index
}

static void disable_counters(uint8_t counter_index) {
    asm volatile("msr " __stringify(NV_PMCNTENCLR_EL0) ", %0" : : "r"(1 << counter_index));   // Disable event counter counter_index
}

static void reset_counters(struct carmel_pmu_config conf) {
    uint32_t unit_group_config = (conf.unit_group << 8) | (conf.unit & 0xFF);
    asm volatile("msr " __stringify(NV_PMSELR_EL0) ", %0" : : "r"(unit_group_config));
    switch (conf.counter_index) {
        case 0:
            asm volatile("msr " __stringify(NV_PMEVCNTR0_EL0) ", %0" : : "r"(0));
            break;
        case 1:
            asm volatile("msr " __stringify(NV_PMEVCNTR1_EL0) ", %0" : : "r"(0));
            break;
        default:
            break;
    }
}

static long pmu_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {

    struct carmel_pmu_config config;

    switch (cmd) {
        case IOCTL_ENABLE_NV_UNCORE_MONITORING:
            if (copy_from_user(&config, (struct carmel_pmu_config *)arg, sizeof(config))) {
                return -EFAULT;
            }
            configure_pmu(config);
            printk(KERN_INFO "Uncore: monitor configured for counter %d with event type %x\n", config.counter_index, config.event_type);
            break;
        case IOCTL_START_NV_UNCORE_MONITORING:
            if (copy_from_user(&config, (struct carmel_pmu_config *)arg, sizeof(config))) {
                return -EFAULT;
            }
            reset_counters(config);
            enable_counters(config.counter_index);
            printk(KERN_INFO "Uncore: Monitoring started on counter %d\n", config.counter_index);
            break;
        case IOCTL_STOP_NV_UNCORE_MONITORING:
            if (copy_from_user(&config, (struct carmel_pmu_config *)arg, sizeof(config))) {
                return -EFAULT;
            }
            disable_counters(config.counter_index);
            printk(KERN_INFO "Uncore: Monitoring stopped on counter %d\n", config.counter_index);
            break;
        case IOCTL_DISABLE_NV_UNCORE_MONITORING:
            asm volatile("msr " __stringify(NV_PMCR_EL0) ", %0" : : "r"(0));  // Disable PMU
            printk(KERN_INFO "Uncore monitor disabled\n");
            break;
        case IOCTL_READ_NV_UNCORE_COUNTER:
            if (copy_from_user(&config, (struct carmel_pmu_config *)arg, sizeof(config))) {
                return -EFAULT;
            }
            config.counter_value = read_event_counter(config);
            if (copy_to_user((struct carmel_pmu_config *)arg, &config, sizeof(config))) {
                return -EFAULT;
            }
            printk(KERN_INFO "Uncore: Counter value read: %u\n", config.counter_value);
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
    major_number = register_chrdev(0, "nv_pmu", &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "PMU module failed to register a major number\n");
        return major_number;
    }

    pmu_class = class_create(THIS_MODULE, "nv_pmu");
    if (IS_ERR(pmu_class)) {
        unregister_chrdev(major_number, "nv_pmu");
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(pmu_class);
    }

    pmu_device = device_create(pmu_class, NULL, MKDEV(major_number, 0), NULL, "nv_pmu");
    if (IS_ERR(pmu_device)) {
        class_destroy(pmu_class);
        unregister_chrdev(major_number, "nv_pmu");
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
    unregister_chrdev(major_number, "nv_pmu");
    printk(KERN_INFO "PMU module unloaded\n");
}

module_init(pmu_module_init);
module_exit(pmu_module_exit);
