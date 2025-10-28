#ifndef _CARMEL_PMU_EVENTS_H_
#define _CARMEL_PMU_EVENTS_H_

// Architectural Events
#define SW_INCR                     0x00  // Instruction architecturally executed, software increment
#define INST_RETIRED                0x08  // Instruction architecturally executed
#define EXC_TAKEN                   0x09  // Exception taken
#define EXC_RETURN                  0x0A  // Instruction architecturally executed, exception return
#define CID_WRITE_RETIRED           0x0B  // Instruction architecturally executed, Write to CONTEXTIDR
#define TTBR_WRITE_RETIRED          0x1C  // Instruction architecturally executed, Write to TTBR
#define L1D_CACHE_LD                0x40  // Level-1 Data Cache access, Read
#define L1D_CACHE_ST                0x41  // Level-1 Data Cache access, Write
#define CPU_CYCLES_DUAL_EXEC        0xC0  // Cycles in dual execution mode
#define CPU_CYCLES_DUAL_EXEC_ELIGI  0xC1  // Cycles dual execution mode is eligible to execute

// Microarchitectural Events
#define L1I_CACHE_REFILL            0x01  // Level-1 Instruction Cache refill
#define L1I_TLB_REFILL              0x02  // Level-1 instruction TLB refill
#define L1D_CACHE_REFILL            0x03  // Level-1 Data Cache refill
#define L1D_CACHE                   0x04  // Level-1 Data Cache access
#define L1D_TLB_REFILL              0x05  // Level-1 data TLB refill
#define BR_MIS_PRED                 0x10  // Mispredicted or not predicted branch speculatively executed
#define CPU_CYCLES                  0x11  // CPU cycles
#define BR_PRED                     0x12  // Predictable branch speculatively executed
#define MEM_ACCESS                  0x13  // Data memory access
#define L1I_CACHE                   0x14  // Level-1 Instruction Cache access
#define L1D_CACHE_WB                0x15  // Level-1 Data Cache Write-back
#define MEMORY_ERROR                0x1A  // Local memory error
#define STALL_FRONTEND              0x23  // No operation issued due to the frontend
#define STALL_BACKEND               0x24  // No operation issued due to the backend
#define L1D_CACHE_REFILL_LD         0x42  // Level-1 Data Cache refill, Read
#define L1D_CACHE_REFILL_ST         0x43  // Level-1 Data Cache refill, Write
#define L1D_CACHE_WB_VICTIM         0x46  // Level-1 Data Cache Write-back, victim

// Uncore Events (NVIDIA Jetson AGX Xavier)
#define L2D_CACHE                   0x16  // Level-2 Data Cache access                      (Unit: L2)
#define L2D_CACHE_REFILL            0x17  // Level-2 Data Cache refill                      (Unit: L2)
#define L2D_CACHE_WB                0x18  // Level-2 Data Cache Write-back                  (Unit: L2)
#define BUS_ACCESS                  0x19  // Bus access                                     (Unit: SCF)
#define BUS_CYCLES                  0x1d  // Bus cycle                                      (Unit: SCF)
#define L3D_CACHE_ALLOCATE          0x29  // Level-3 Data Cache allocation without refill   (Unit: SCF)
#define L3D_CACHE_REFILL            0x2a  // Level-3 Data Cache refill                      (Unit: SCF)
#define L3D_CACHE                   0x2b  // Level-3 Data Cache access                      (Unit: SCF)
#define L3D_CACHE_WB                0x2c  // Level-3 Data Cache Write-back                  (Unit: SCF)

#endif // _CARMEL_PMU_EVENTS_H_