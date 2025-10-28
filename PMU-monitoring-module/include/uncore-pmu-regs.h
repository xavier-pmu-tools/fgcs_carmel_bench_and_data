#ifndef _UNCORE_PMU_REGS_
#define _UNCORE_PMU_REGS_

/* Register code: s(op0)_(op1)_c(CRn)_c(CRm)_(op2) */

// Global registers
#define NV_PMSELR_EL0       s3_3_c15_c5_1

// Group registers
#define NV_PMCEID0_EL0      s3_3_c15_c4_5
#define NV_PMCEID1_EL0      s3_3_c15_c4_6
#define NV_PMCRN_EL0        s3_3_c15_c4_7

// Unit registers
#define NV_PMCNTENSET_EL0   s3_3_c15_c4_0
#define NV_PMCNTENCLR_EL0   s3_3_c15_c4_1
#define NV_PMOVSSET_EL0     s3_3_c15_c4_2
#define NV_PMOVSCLR_EL0     s3_3_c15_c4_3
#define NV_PMCR_EL0         s3_3_c15_c4_4
#define NV_PMINTENSET_EL1   s3_0_c15_c2_0
#define NV_PMINTENCLR_EL1   s3_0_c15_c2_1
#define NV_MDCR_EL2         s3_4_c15_c2_0

// Counter registers
#define NV_PMEVCNTR0_EL0    s3_3_c15_c0_0
#define NV_PMEVCNTR1_EL0    s3_3_c15_c0_1
#define NV_PMEVCNTR2_EL0    s3_3_c15_c0_2
#define NV_PMEVCNTR3_EL0    s3_3_c15_c0_3
#define NV_PMEVCNTR4_EL0    s3_3_c15_c0_4
#define NV_PMEVCNTR5_EL0    s3_3_c15_c0_5
#define NV_PMEVCNTR6_EL0    s3_3_c15_c0_6
#define NV_PMEVCNTR7_EL0    s3_3_c15_c0_7
#define NV_PMEVTYPER0_EL0   s3_3_c15_c2_0
#define NV_PMEVTYPER1_EL0   s3_3_c15_c2_1
#define NV_PMEVTYPER2_EL0   s3_3_c15_c2_2
#define NV_PMEVTYPER3_EL0   s3_3_c15_c2_3
#define NV_PMEVTYPER4_EL0   s3_3_c15_c2_4
#define NV_PMEVTYPER5_EL0   s3_3_c15_c2_5
#define NV_PMEVTYPER6_EL0   s3_3_c15_c2_6
#define NV_PMEVTYPER7_EL0   s3_3_c15_c2_7

#endif // _UNCORE_PMU_REGS_