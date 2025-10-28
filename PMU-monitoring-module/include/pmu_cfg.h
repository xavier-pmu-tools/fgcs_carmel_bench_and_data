#ifndef _PMU_CFG_H_
#define _PMU_CFG_H_

#define PMU_0 0
#define PMU_1 1
#define PMU_2 2
#define PMU_3 3
#define PMU_4 4
#define PMU_5 5

struct pmu_config {
    uint32_t event_type;
    uint32_t counter_index;
    uint64_t counter_value;
};

struct carmel_pmu_config{
    uint32_t event_type;
    uint32_t counter_value;
    uint8_t unit_group;  // SCF: '0', L2: '1'
    uint8_t unit;
    uint8_t counter_index;  
};

#define IOCTL_START_MONITORING _IOW(0, 1, struct pmu_config)
#define IOCTL_STOP_MONITORING _IOW(0, 2, struct pmu_config)
#define IOCTL_ENABLE_MONITORING _IOW(0, 3, struct pmu_config)
#define IOCTL_DISABLE_MONITORING _IO(0, 4)
#define IOCTL_READ_COUNTER _IOR(0, 5, struct pmu_config)

#define IOCTL_START_NV_UNCORE_MONITORING _IOW(0, 1, struct carmel_pmu_config)
#define IOCTL_STOP_NV_UNCORE_MONITORING _IOW(0, 2, struct carmel_pmu_config)
#define IOCTL_ENABLE_NV_UNCORE_MONITORING _IOW(0, 3, struct carmel_pmu_config)
#define IOCTL_DISABLE_NV_UNCORE_MONITORING _IO(0, 4)
#define IOCTL_READ_NV_UNCORE_COUNTER _IOR(0, 5, struct carmel_pmu_config)

#define SCF_UNIT_GROUP 0
#define L2_UNIT_GROUP 1

#endif // _PMU_CFG_H_