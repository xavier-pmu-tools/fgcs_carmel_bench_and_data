# PMU-monitoring-module
PMU monitoring module and examples, tested on a Jetson AGX Xavier device.

## This repo

This repository contains two Kernel modules that are used to access the performance counters of the Carmel Cores that the Jetson AGX Xavier platform contains. All the information about how the registers have been accessed is found in the documentation located in the *docu* directory.

The repository contains two modules, located in the *module* directory.

1) **PMU module:** Accesses the available registers of the processor. It must be taken into account that in the Carmel processor not all the events available for ARMv8 are available. These are detailed in the Technical Reference Manual (TRM) of the Jetson AGX Xavier Series.
2) **NV PMU module**: Module to access NVIDIA's own events implemented in the Carmel processor. As specified in the TRM these counters are common for all the processors. They are called *Uncore events*.

In the *include* directory there are three header files.

- *carmel-pmu-events.h*: Contains all the events available for the Carmel processors, both the processor events and the *Uncore*.
- *uncore-pmu-regs.h*: Contains the definition of the registers for obtaining the *Uncore events*.
- *pmu_cfg*: The data structures to configure the PMUs are defined. It defines the calls to the modules.



## Install/Uninstall

Commands to install/uninstall modules:

```bash
# Compile modules
make 
# Load modules
make load
# Unload modules
make unload
# Clean
make clean
```

## How to use

Once installed, to access the register configuration, a new device will be created in the Linux file system (```/dev/pmu``` for processor events and ```/dev/nv_pmu``` for *Uncore* events).

Below is a description of how to configure the PMUs from a user program.

**Note:** The created application will need ```sudo``` permissions to access the registers. 

**Note:** Ensure (using affinity, for example) that what you want to measure is running on the same CPU on which you are measuring for a correct reading. 

**Note:** The processes described here are detailed in the documentation in the *docu* directory.



### Accessing the processor PMUs

First, we will open the installed device.

```c
#define DEVICE_PATH "/dev/pmu"

fd = open(DEVICE_PATH, O_RDWR);
if (fd == -1) {
    perror("Failed to open device file");
    return EXIT_FAILURE;
}
```

Remember to close the file at the end of the program.

```c
close(fd);
```

We will need a data structure (defined in the header file pmu_cfg.h) to store the PMU configuration. The event to be measured and which of the available PMUs will be used are assigned.

Example function for the configuration:

```c
void set_pmu_config(struct pmu_config *cfg, uint32_t event_type, uint32_t counter_index){
    cfg->counter_index = counter_index;
    cfg->event_type = event_type;
}
```


Store the desired configuration:
```c
// Configure PMUs    
struct pmu_config pmu_1; 
    
set_pmu_config(&pmu_1, event_type, counter_index);
```

The first step will be to enable monitoring. With this step, the PMU is also configured to measure the desired event.

```c
if (ioctl(fd, IOCTL_ENABLE_MONITORING, &pmu_1) == -1) {
	perror("Failed to enable monitoring");
	return EXIT_FAILURE;
}
```

Start monitoring. The PMU begins to monitor the desired event. 

```c
if (ioctl(fd, IOCTL_START_MONITORING, &pmu_1) == -1) {
	perror("Failed to start monitoring");
	return EXIT_FAILURE;
}
```

Read counter. This step should be done as many times as needed, for example, at the beginning and end of the task you want to monitor to subtract the final counter value from the initial one. Once the counter is read, it is recommended to store the value; otherwise, it will be overwritten in the next reading.

```c
if (ioctl(fd, IOCTL_READ_COUNTER, &pmu_1) == -1) {
	perror("Failed to read counter value");
	return EXIT_FAILURE;
}
// Guardar valor leído
counter_value = pmu_1.counter_value;
```

Once we have finished monitoring, we stop the counter. This will disable the configured counter.

```c
if (ioctl(fd, IOCTL_STOP_MONITORING, &pmu_1) == -1) {
	perror("Failed to stop monitoring");
	return EXIT_FAILURE;
}
```

Finally, to close the program, we will disable the PMU.

```c
if (ioctl(fd, IOCTL_DISABLE_MONITORING) == -1) {
	perror("Failed to disable monitoring");
	return EXIT_FAILURE;
}
```

### Accessing the Uncore events

To access the *Uncore* events, the process is similar to the previous section with some small differences due to the characteristics of these events.

As before, we open the created device.

```c
#define DEVICE_PATH "/dev/nv_pmu"

fd = open(DEVICE_PATH, O_RDWR);
if (fd == -1) {
	perror("Failed to open device file");
	return EXIT_FAILURE;
}
```

Now we store the desired configuration in the data structure. The configuration is different now: as indicated in the documentation in the docu directory and in the TRM, you must specify the *unit group* and the *unit* you want.  

Example function for the configuration:

```c
void set_pmu_config(struct carmel_pmu_config *cfg, uint32_t event_type, uint8_t unit_group, uint8_t unit, uint8_t counter_index){
    cfg->counter_index = counter_index;
    cfg->event_type = event_type;
    cfg->unit_group = unit_group;
    cfg->unit = unit;
}
```

Store the desired configuration:

```c
// Configure PMUs    
struct carmel_pmu_config pmu_1;
    
set_pmu_config(&pmu_1, event_type, unit_group, unit, counter_index);
```

We enable and configure the PMU:

```c
if (ioctl(fd, IOCTL_ENABLE_NV_UNCORE_MONITORING, &pmu_1) == -1) {
	perror("Failed to enable monitoring");
	return EXIT_FAILURE;
}
```

We start counting.

```c
if (ioctl(fd, IOCTL_START_NV_UNCORE_MONITORING, &pmu_1) == -1) {
	perror("Failed to start monitoring");
	return EXIT_FAILURE;
}
```

We read the counter and store the value.

```c
if (ioctl(fd, IOCTL_READ_NV_UNCORE_COUNTER, &pmu_1) == -1) {
	perror("Failed to read counter value");
	return EXIT_FAILURE;
}

counter_value = pmu_1.counter_value;
```

We stop the counter once the necessary data has been read.

```c
if (ioctl(fd, IOCTL_STOP_NV_UNCORE_MONITORING, &pmu_1) == -1) {
	perror("Failed to stop monitoring");
	return EXIT_FAILURE;
}
```

We disable the PMU.

```c
if (ioctl(fd, IOCTL_DISABLE_NV_UNCORE_MONITORING) == -1) {
	perror("Failed to disable monitoring");
	return EXIT_FAILURE;
}
```

Finally, we close the directory where the created device is located.

```c
close(fd);
```
