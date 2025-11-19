This manual is based on the information provided in the [Technical Reference Manual](https://developer.download.nvidia.com/assets/embedded/secure/jetson/xavier/docs/Xavier_TRM_DP09253002.pdf?oYNqbxAzVwOc6cQethyAWdm8sy2Fza6DNhZi6uEG_4ffeTV4xacQzYtahPIuAwdRi9CMWUiS7GbV09R-wS89cAQeEsYUrNuQ71KKDQHe3XYcuEM4pIuyGxK5wd1pQLeo--pa2Mfi_WNjaaVTpj_cuXNLVxdkXXUUdJN5J1Dj_PBBbg==&t=eyJscyI6ImdzZW8iLCJsc2QiOiJodHRwczovL3d3dy5nb29nbGUuY29tLyJ9) of the Jetson Xavier series.

### Step 1: Enable Monitors

Register: ```NV_PMCR_EL0``` 

Important bits: 
- ``N [15:11]`` → Indicates the number of available Uncore Counters (2 units).
- ``P [1]`` → Set to '1' to reset the counters.
- ``E [0]`` → Enable/Disable all Event Counters. → Enable: '1' / Disable: '0'.

### Step 2: Configure Each Uncore Counter

#### 2a: Select Unit Group

Register: ``NV_PMSELR_EL0``

| Unit | Description             | # of units | code |
| ---- | ----------------------- | ---------- | ---- |
| SCF  | System Coherency Fabric | 1          | '0'  |
| L2   | L2 cache                | 4          | '1'  |

Each unit has 2 counters, so only two values can be monitored at the same time.

Important bits:
- ``G [15:8]`` → Select Unit Group ('0' : SCF, '1' : L2).
- ``U [7:0]`` → Select unit (must be less than the number of units in the selected Unit Group).

Number of units in a Unit Group = SCF: 1, L2: 4.  
Number of counters per unit = SCF: 2, L2: 2.

#### 2b: Verify Selected Configuration (Optional)

Register: ``NV_PMCRN_EL0`` (Read-only register)

Important bits:
- ``Counters [15:8]`` → Number of counters per unit in the selected Unit Group (SCF: 2, L2: 2).
- ``Units [7:0]`` → Number of units in the selected Unit Group (SCF: 1, L2: 4).

#### 2c: Select Event

Register: ``NV_PMEVTYPER<n>_EL0``

Important bits:
- ``evtCount [9:0]`` → Select the event to count.

### Step 3: Enable Counter

Register: ``NV_PCNTENSET_EL0``

Enables counter in the selected Unit; multiple counters can be enabled.

Important bits:
- ``P [1:0]`` → Select the event to count.

### Step 4: Read Uncore Counter

Register: ``NV_PMEVCNTR<n>_EL0 [32:0]``

Counter register for each Uncore Performance Unit.
