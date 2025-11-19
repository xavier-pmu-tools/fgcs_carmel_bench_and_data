This tutorial shows how to configure processor registers to enable PMUs and monitor CPU applications. It is based on the tutorial shown in the [documentation](https://developer.arm.com/documentation/102601/0100/Setting-up-and-using-the-event-counters). Another tutorial can also be found in the following [how-to](https://trustedfirmware-a.readthedocs.io/en/latest/perf/performance-monitoring-unit.html#).

As supplementary material, see [performance registers](https://developer.arm.com/documentation/ddi0595/2020-12/AArch64-Registers/PMCR-EL0--Performance-Monitors-Control-Register?lang=en#fieldset_0-0_0) and a [list](https://arm-software.github.io/CMSIS_5/Core/html/group__pmu8__events__armv81.html#ga7505ae74c1d905f01b05dd5466c1efc0) of events.

---

### Step 0: Enable User Access

Enable user-space access to the PMUs. This step is **optional**.

**Register:** `PMUSERNR_EL0`

**Important bits:**  
- `ER [3]` → Enable user access to Event Counters.  
- `CR [2]` → Enable user access to Cycle Counters.  
- `SW [1]` → Enable user access to Software Increment.  
- `EN [0]` → Enable user access to other registers.  

---

### Step 1: Enable PMU

**Register:** `PMCR_EL0`

**Important bits:**  
- `N [15:11]` → Indicates the number of available Event Counters (6 on the Xavier).  
- `DP [5]` → Enable/Disable Cycle Counter. → Enable: '1' / Disable: '0'.  
- `P [1]` → Set to '1' to reset the counters.  
- `E [0]` → Enable/Disable all Event Counters. → Enable: '1' / Disable: '0'.  

---

### Step 2: Configure Each Event Counter

#### 2a: Select Counter

**Register:** `PMSELR_EL0`

**Important bits:**  
- `SEL [4:0]` → Selects the Event Counter `PMEVCNTR<n>_EL0`, which stores the counter value and its configuration registers.  

#### 2b: Select Event

**Register:** `PMXEVTYPER_EL0`

When the `PMSELR_0` register points to a monitor, this register points to the corresponding PMU configuration in `PMEVTYPER<n>_EL0`.

**Important bits:**  
- `evntCount [9:0]` → Write the ID of the event to monitor.  
- `[15:10]` → Extension for `FEAT_PMUv3p1`.  

---

### Step 3: Start Monitoring

#### 3a: Reset Counter

First, select the counter via `PMSELR_EL0` as in step 2a. This way, the `PMXEVCNTR_EL0` register points to `PMEVCNTR<n>_EL0`, where the counter values are stored. Write '0' to `PMXEVCNTR_EL0`.

#### 3b: Enable Event Counter

**Register:** `PMCNTENSET_EL0`

**Important bits:**  
- `P [30:0]` → Enabling a `P<n>` by setting the bit to '1' activates the corresponding counter `PMEVCNTR<n>_EL0`.  

---

### Step 4: Read Event Counter

**Register:** `PMXEVCNTR_EL0 [63:0]`

When `PMSELR_0` points to a monitor, this register corresponds to the 64-bit PMU register in `PMEVCNTR<n>_EL0`.
