# Benchmark Suite and Measurement Tools for 'Evaluating Quantile Regression Neural Networks for Optimizing Real-Time Applications on Heterogeneous Platforms'

This repository contains the benchmark code, measurement data, and kernel modules developed and used for the experiments presented in the paper:

> [Evaluating Quantile Regression Neural Networks for Optimizing Real-Time Applications on Heterogeneous Platforms](https://doi.org/10.1016/j.future.2025.108239)

---

## 📘 Overview

The repository includes:
- **Contention data** employed in the experimental analysis.
- **Execution metrics** reported in the paper for each evaluated function and configuration.
- **Benchmark support code** used as a reference to illustrate how the functions behave in the experiments.
- **Kernel module** for accessing core and uncore Performance Monitoring Units (PMUs).

The benchmark implementations themselves are not distributed here; the repository includes the measurement data obtained from the paper and the supporting code used during the evaluation.

The **Profile Optimizer** is **not included**, as it corresponds to a real-world industrial use case that cannot be publicly released.

---

## 📂 Repository Structure

```text
.
├── contention_results/
│   ├── full_contender_setup/
│   ├── single_contender_setup/
│   └── three_contender_setup/
├── cpu_benchmarks/
│   ├── memory_benchmarks.c
│   └── memory_benchmarks.h
├── event_motitor_lectures/
│   ├── lidar_downsampling_f10.csv
│   ├── lidar_downsampling_f2.csv
│   ├── lidar_downsampling_f2_l1.csv
│   ├── lidar_downsampling_f4.csv
│   ├── lidar_downsampling_f4_l2.csv
│   ├── lidar_filtering.csv
│   ├── lidar_filtering_l1.csv
│   ├── lidar_filtering_l2.csv
│   ├── matrix_mmb.csv
│   ├── matrix_sum.csv
│   ├── matrix_transpose.csv
│   ├── matrix_transpose_multpl.csv
│   ├── profile_optimizer.csv
│   ├── rnd_acc.csv
│   ├── rnd_acc_2.csv
│   ├── rnd_acc_2_l1.csv
│   ├── rnd_acc_2_l2.csv
│   ├── v_add_int.csv
│   ├── v_add_int_l1.csv
│   ├── v_mltpl_add_int.csv
│   ├── v_mltpl_int.csv
│   ├── v_mltpl_int_l2.csv
│   ├── v_rectifier.csv
│   └── v_scale.csv
└── PMU-monitoring-module/
	├── Makefile
	├── README.md
	├── docu/
	│   ├── Carmel-CPU-Uncore-Events.md
	│   └── Configurar_Event_Counters_ARMv8.md
	├── include/
	│   ├── carmel-pmu-events.h
	│   ├── pmu_cfg.h
	│   └── uncore-pmu-regs.h
	└── module/
		├── nv_pmu_module.c
		└── pmu_module.c
```

---

## 💻 Hardware Platform

All experiments presented in this repository were conducted on an **NVIDIA Jetson AGX Xavier** platform. The evaluation focuses on the **Carmel CPU complex**, which consists of:

- **4 clusters** with 2 Carmel cores each (8 cores total)
- **Per-core cache:** 128 KB L1 instruction cache, 64 KB L1 data cache
- **Per-cluster cache:** 2 MB L2 cache (shared by 2 cores)
- **System-wide cache:** 4 MB L3 victim cache (shared by all cores)

The Carmel cores are NVIDIA's proprietary ARMv8-based design. All cores share access to main memory through the SCF (Shared Cache Fabric), creating potential contention points studied in this work.

### Performance Monitoring Units (PMUs)

The Carmel CPU complex provides **36 hardware performance events** through two categories of monitoring units:

**Core Events (27 events):** Measured by per-core PMUs, these track events occurring within individual cores:
- CPU cycles (execution, dual execution eligibility)
- Instruction and data cache accesses (L1 hit/miss/refill)
- TLB refills and translations
- Branch predictions and mispredictions
- Exception handling

**Uncore Events (9 events):** Measured by uncore PMUs, these track events occurring outside individual cores in shared resources:
- L2 and L3 cache accesses (hits, misses, refills, writebacks)
- Bus access patterns and cycles
- Memory access characteristics

The custom **PMU-monitoring-module** provides direct access to both core and uncore PMU registers, enabling comprehensive monitoring of the memory hierarchy and contention patterns across the entire CPU complex. The events are organized in 6 core PMU counters (PMU 0–5) and 3 uncore PMU counters (U-PMU 0–2), as shown in the [event_motitor_lectures](#event_motitor_lectures) section.

For detailed event definitions and configuration, see [PMU-monitoring-module/README.md](PMU-monitoring-module/README.md) and the documentation files in [PMU-monitoring-module/docu](PMU-monitoring-module/docu).

---

## 📄 Documentation

The repository includes documentation for the PMU monitoring module in [PMU-monitoring-module/README.md](PMU-monitoring-module/README.md) and the following files under [PMU-monitoring-module/docu](PMU-monitoring-module/docu):

- [Carmel-CPU-Uncore-Events.md](PMU-monitoring-module/docu/Carmel-CPU-Uncore-Events.md)
- [Configurar_Event_Counters_ARMv8.md](PMU-monitoring-module/docu/Configurar_Event_Counters_ARMv8.md)

These documents explain how the core and uncore counters are configured and how the module is used from user space.


### event_monitor_lectures

The [event_monitor_lectures](event_monitor_lectures) directory contains the CSV files with the measurements collected in the paper for each benchmark. Each file stores the event counters for a given benchmark across consecutive reads, with the benchmark-specific event mapping summarized below.

| Read | PMU 0 | PMU 1 | PMU 2 | PMU 3 | PMU 4 | PMU 5 | U-PMU 0 | U-PMU 1 | U-PMU 2 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Read 1 | CPU_CYCLES | CID_WRITE_RETIRED | TTBR_WRITE_RETIRED | EXC_TAKEN | EXC_RETURN | L1D_CACHE_LD | BUS_ACCESS | BUS_CYCLES | L2D_CACHE |
| Read 2 | CPU_CYCLES | L1D_CACHE_ST | CPU_CYCLES_DUAL_EXEC | CPU_CYCLES_DUAL_EXEC_ELIGI | L1I_CACHE_REFILL | L1I_TLB_REFILL | L3_CACHE_ALLOCATE | L3_CACHE_REFILL | L2D_CACHE_REFILL |
| Read 3 | CPU_CYCLES | L1D_CACHE_REFILL | L1D_CACHE | L1D_TLB_REFILL | BR_MIS_PRED | BR_PRED | L3D_CACHE | L3DCACHE_WB | L2D_CACHE_WB |
| Read 4 | CPU_CYCLES | MEM_ACCESS | L1I_CACHE | L1D_CACHE_WB | MEMORY_ERROR | STALL_FRONTEND | - | - | - |
| Read 5 | CPU_CYCLES | STALL_BACKEND | SW_INCR | L1D_CACHE_REFILL_LD | L1D_CACHE_REFILL_ST | L1D_CACHE_VICTIM | - | - | - |

The CSV rows follow this read order, and the columns identify the counters assigned to each PMU or uncore PMU in the experiment.

### contention_results

The [contention_results](contention_results) directory contains the CSV files used to summarize the contention scenarios evaluated in the paper. Each file lists the benchmark assigned to each CPU slot (`cpu1` to `cpu7`) together with the measured value in the first column (`Dato`).

The three subdirectories group the scenarios by contention setup:

- **[single_contender_setup](contention_results/single_contender_setup):** Contention is generated within the same cluster as the target task, sharing intra-cluster resources (L2 cache, L3 cache, and global memory).

- **[three_contender_setup](contention_results/three_contender_setup):** Contention is generated by one contender from each external cluster, sharing only inter-cluster resources (L3 cache and global memory).

- **[full_contender_setup](contention_results/full_contender_setup):** Contention is generated by all available cores, creating aggressive competition across the entire memory hierarchy. This folder also includes `tua_` variants (time-unique activity) and `tua_none.csv` (baseline).

## 🧪 Benchmark Descriptions

| Benchmark name | Description |
| --- | --- |
| rnd_acc | Performs 1M random memory accesses. |
| rnd_acc_2 | Performs 2M random memory accesses. Has an l1 and an l2 variant. |
| matrix_sum | Computes the sum of two matrices. |
| matrix_trans | Transposes a given matrix, switching its rows and columns. |
| matrix_trans_mltpl | First transposes a matrix and then multiplies it with another matrix. |
| matrix_mmb | Computes a matrix multiplication. |
| v_add_int | Computes the sum of two vectors. Has an l1 variant. |
| v_mltpl | Computes a vector multiplication. Has an l2 variant. |
| v_mltpl_add | Computes a vector multiplication followed by addition. |
| v_rect | Applies the rectifier function to input data. |
| v_scale | Scales the values of a vector by a constant factor. |
| profile_optimizer | Railway application based on a genetic algorithm for obtaining optimal speed pofile. Described in [Using MAST for modeling and response-time analysis of real-time applications with GPUs (JSA 2024)](https://doi.org/10.1016/j.sysarc.2024.103300). |
| ldr_downsampling_f2 | Reduces the resolution of LIDAR point-cloud data by a factor of 2. Has an l1 variant. |
| ldr_downsampling_f4 | Reduces the resolution of LIDAR point-cloud data by a factor of 4. Has an l2 variant. |
| ldr_downsampling_f10 | Reduces the resolution of LIDAR point-cloud data by a factor of 10. |
| ldr_filtering | Applies filtering to LIDAR point-cloud data to eliminate ground and distant objects. Has an l1 and an l2 variant. |

The CSV files in [event_monitor_lectures](event_monitor_lectures) correspond to the measurements collected for these benchmarks under the reported monitoring configurations.

---

## Contact

For questions or feedback, please contact: [iosu.gomez@ikerlan.com]

---

## 📚 Citation

If you use this code or data in your research, please cite the corresponding paper:

```bibtex
@article{GOMEZ2026108239,
title = {Evaluating quantile regression neural networks for optimizing real-time applications on heterogeneous platforms},
journal = {Future Generation Computer Systems},
volume = {176},
pages = {108239},
year = {2026},
issn = {0167-739X},
doi = {https://doi.org/10.1016/j.future.2025.108239},
url = {https://www.sciencedirect.com/science/article/pii/S0167739X25005333},
author = {Iosu Gomez and David Fonts and Sergi Vilardell and Unai {Díaz De Cerio} and Juan M. Rivas and Enrico Mezzetti and J. Javier Gutiérrez and Francisco J. Cazorla},
}
```
