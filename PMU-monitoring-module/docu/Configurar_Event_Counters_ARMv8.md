En este tutorial se muestra como configurar los registros del procesador para habilitar los PMUs y poder monitorear aplicaciones en la CPU. Se basa en el tutorial mostrado en la  [documentación](https://developer.arm.com/documentation/102601/0100/Setting-up-and-using-the-event-counters). También puede encontrarse otro tutorial en el siguiente [howto](https://trustedfirmware-a.readthedocs.io/en/latest/perf/performance-monitoring-unit.html#).

Como material auxiliar, [performance registers](https://developer.arm.com/documentation/ddi0595/2020-12/AArch64-Registers/PMCR-EL0--Performance-Monitors-Control-Register?lang=en#fieldset_0-0_0) y una [lista](https://arm-software.github.io/CMSIS_5/Core/html/group__pmu8__events__armv81.html#ga7505ae74c1d905f01b05dd5466c1efc0) de eventos.

### Paso 0: Habilitar acceso usuario

Habilitar el acceso desde usuario a los PMUs. Este paso **no es obligatorio**.

Registro: ```PMUSERNR_EL0```

Bits importantes: 
- ```ER [3]``` → Habilitar el acceso desde usuario a Event Counters. 
- ```CR [2]``` → Habilitar el acceso desde usuario a Cycle Counters. 
- ``SW [1]`` → Habilitar el acceso desde usuario a Incremento SW. 
- ``EN [0]`` → Habilitar el acceso desde usuario a otros registros. 

### Paso 1: Habilitar PMU

Registro: ```PMCR_EL0```

Bits importantes: 
- ``N [15:11]`` → Indica el número de Event Counters disponibles (6 en caso e la Xavier).
- ``DP [5]`` → Habilitar/Deshabilitar Cycle Counter. → Habilitar: '1' / Deshabilitar: '0'. 
- ``P [1]`` → Bit a '1' para resetear los contadores.
- ``E [0]`` → Habilitar/Deshabilitar todos los Event Counters. → Habilitar: '1' / Deshabilitar: '0'.

### Paso 2: Configurar cada Event Counter

#### 2a: Seleccionar contador

Registro: ``PMSELR_EL0``

Bits importantes:
- ``SEL [4:0]`` → Selecciona el Event Counter ``PMEVCNRT<n>_EL0`` que guarda el valor del contador, y sus registros de configuración.

#### 2b: Seleccionar evento

Registro: ``PMXEVTYPER_EL0``

Cuando el registro ``PMSELR_0`` apunta a un monitor, este registro apunta a la configuración del PMU correspondiente en ``PMEVTYPER<n>_EL0``.

Bits importantes:
- ``evntCount [9:0]`` → Escribir el ID del evento que se quiere monitorizar.
- ``[15:10]`` → Extensión para ``FEAT_PMUv3p1``.

### Paso 3: Empezar a monitorizar

#### 3.a: Resetear contador

Para ello primero hay seleccionar el contador mediante ``PMSELR_EL0`` como en el paso 2a. De esta manera el registro ``PMXEVCNTR_EL0`` apunta a ``PMEVCNRT<n>_EL0`` donde se guardan los valores del contador correspondiente. Escribir '0'en el registro ``PMXEVCNTR_EL0``.
#### 3.b: Habilitar Event Counter

Registro: ``PMCNTENSET_EL0``

Bits importantes:
- ``P [30:0]`` → Habilitando un ``P<n>`` poniendo el bit a '1', se habilita el contador correspondiente ``PMEVCNRT<n>_EL0.

### Paso 4: Leer Event Counter

Registro: ``PMXEVCNTR_EL0 [63:0]``

Cuando el registro ``PMSELR_0`` apunta a un monitor, este registro apunta al registro de ``64 bits`` del PMU correspondiente en ``PMEVCNRT<n>_EL0``.

