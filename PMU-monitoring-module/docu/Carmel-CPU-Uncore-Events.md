Este manual esta basado en la información proporcionada en el [Technical Reference Manual](https://developer.download.nvidia.com/assets/embedded/secure/jetson/xavier/docs/Xavier_TRM_DP09253002.pdf?oYNqbxAzVwOc6cQethyAWdm8sy2Fza6DNhZi6uEG_4ffeTV4xacQzYtahPIuAwdRi9CMWUiS7GbV09R-wS89cAQeEsYUrNuQ71KKDQHe3XYcuEM4pIuyGxK5wd1pQLeo--pa2Mfi_WNjaaVTpj_cuXNLVxdkXXUUdJN5J1Dj_PBBbg==&t=eyJscyI6ImdzZW8iLCJsc2QiOiJodHRwczovL3d3dy5nb29nbGUuY29tLyJ9) de la serie Jetson Xavier. 

### Paso 1: Habilitar monitores

Registro: ```NV_PMCR_EL0``` 

Bits importantes: 
- ``N [15:11]`` → Indica el número de Uncore Counters disponibles (2 unidades).
- ``P [1]`` → Bit a '1' para resetear los contadores.
- ``E [0]`` → Habilitar/Deshabilitar todos los Event Counters. → Habilitar: '1' / Deshabilitar: '0'.

### Paso 2: Configurar cada Uncore Counter

#### 2a: Seleccionar Unit Group

Registro: ``NV_PMSELR_EL0``

| Unidad | Descripción             | # de unidades | código |
| ------ | ----------------------- | ------------- | ------ |
| SCF    | System Coherency Fabric | 1             | '0'    |
| L2     | L2 cache                | 4             | '1'    |
Cada unidad hay 2 contadores, por lo que solo se pueden monitorizar dos valores al mismo tiempo.

Bits importantes:
- ``G [15:8]`` → Seleccionar Unit Group ('0' : SCF, '1' : L2).
- ``U [7:0]`` →  Seleccionar unidad (tiene que ser menor que el número de unidades en el Unit Groups seleccionado).

Número de unidades en un unit group = SCF: 1, L2: 4.
Número de contadores para cada unidad = SCF: 2, L2: 2.

#### 2b: Comprobar configuración seleccionada (Opcional)

Registro: ``NV_PMCRN_EL0`` (Registro solo de lectura)

Bits importantes:
- ``Counters [15:8]`` → Número de contadores en cada Unit del Unit Group seleccionado (SCF : 2, L2 : 2).
- ``Units [7:0]`` →  Número de unidades en el Unit Group seleccionado (SCF : 1, L2 : 4).

#### 2c: Seleccionar evento

Registro: ``NV_PMEVTYPER<n>_EL0``

Bits importantes:
- ``evtCount [9:0]`` → Select the event to count.
### Paso 3: Habilitar Counter

Registro: ``NV_PCNTENSET_EL0``

Habilita contador en el Unit seleccionado, puede habilitar múltiples contadores.

Bits importantes:
- ``P [1:0]`` → Select the event to count.

### Paso 4: Leer Uncore Counter

Registro: ``NV_PMEVCNTR<n>_EL0 [32:0]``

Registro de contadores para cada Uncore Perform Unit. 

