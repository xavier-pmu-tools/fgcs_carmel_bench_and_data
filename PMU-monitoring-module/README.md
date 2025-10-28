# PMU-monitoring-module
PMU monitoring module and examples, tested on a Jetson AGX Xavier device.

## Este repo

Este repositorio contiene dos módulos Kernel que sirven para acceder a los contadores de rendimiento de los Carmel Cores que contiene la plataforma Jetson AGX Xavier. Todda la información de como se ha accedido a los registros se encuentra en la documentación ubicada en el directorio *docu*.

El repositorio contiene dos módulos, ubicados en el directorio *module*.

1) **PMU module:** Accede a los registros disponibles del procesador. Hay que tener en cuenta que en el procesador Carmel no están disponibles todos los eventos disponibles para el ARMv8. Estos se detallan en el Technical Reference Manual (TRM) de la Serie Jetson AGX Xavier.
2) **NV PMU module**: Modulo para acceder a los eventos propios de NVIDIA implementados el procesador Carmel. Como se especifica en el TRM estos contadores son comunes para todos los procesadores. Se les denomina *Uncore events*.

En el directorio *include* se encuentran tres ficheros de cabecera.

- *carmel-pmu-events.h*: Contiene todos los eventos disponibles para los procesadores Carmel, tanto los eventos del procesador como los *Uncore*.
- *uncore-pmu-regs.h*: Contiene la definición de los registros para la obtención de los *Uncore events*.
- *pmu_cfg*: Se definen las estructuras de datos para configurar los PMUs. Define las llamadas a los módulos.


## Instalar/Desinstalar

Comandos para instalar/desinstalar módulos:

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

## Como usar

Una vez instalado, para acceder a la configuración de los registro, se creara un nuevo dispositivo en el sistema de ficheros Linux (```/dev/pmu``` para los eventos del procesador y ```/dev/nv_pmu``` para los eventos *Uncore*).

A continuación se describe como configurar desde un programa de usuario, los PMUs

**Nota:** La aplicación creada necesitara de permisos ```sudo``` para poder acceder a los registros. 

**Nota:** Asegurar (mediante afinidad por ejemplo) que lo que quieres medir se está ejecutando en la misma CPU en el que estás midiendo para una correcta lectura. 

**Nota:** Los procesos descritos aquí de detallan en la documentación del directrio *docu*. 


### Acceso a los PMUs del procesador

Primero abriremos el dispositivo instalado.

```c
#define DEVICE_PATH "/dev/pmu"

fd = open(DEVICE_PATH, O_RDWR);
if (fd == -1) {
    perror("Failed to open device file");
    return EXIT_FAILURE;
}
```

Acordarse de al final del programa cerrar el fichero.

```c
close(fd);
```

Necesitaremos una estructura de datos (definida en el fichero de cabecera *pmu_cfg.h*) para guardar la configuración del PMU. Se asignarán el evento que se desea medir, y en cual de los PMUs disponibles se desea medir.

Función ejemplo para la configuración:

```c
void set_pmu_config(struct pmu_config *cfg, uint32_t event_type, uint32_t counter_index){
    cfg->counter_index = counter_index;
    cfg->event_type = event_type;
}
```


Guardar configuración deseada:
```c
// Configure PMUs    
struct pmu_config pmu_1; 
    
set_pmu_config(&pmu_1, event_type, counter_index);
```

El primer paso será habilitar la monitorización. Mediante este paso se configura también el PMU para medir el evento deseado.

```c
if (ioctl(fd, IOCTL_ENABLE_MONITORING, &pmu_1) == -1) {
	perror("Failed to enable monitoring");
	return EXIT_FAILURE;
}
```

Empezar a monitorizar. El PMU empieza a monitorizar el evento deseado. 

```c
if (ioctl(fd, IOCTL_START_MONITORING, &pmu_1) == -1) {
	perror("Failed to start monitoring");
	return EXIT_FAILURE;
}
```

Leer contador. Este paso habrá que hacerlo las veces que se desee, por ejemplo, al inicio y al final de la tarea que se quiere monitorizar para así hacer una resta del valor final del contador y el inicial. Una vez leído el contador, se recomienda guardar el valor, sino en la siguiente lectura se sobre-escribirá.

```c
if (ioctl(fd, IOCTL_READ_COUNTER, &pmu_1) == -1) {
	perror("Failed to read counter value");
	return EXIT_FAILURE;
}
// Guardar valor leído
counter_value = pmu_1.counter_value;
```

Una vez hayamos terminado de monitorizar paramos el contador, esto deshabilitara el contador configurado.

```c
if (ioctl(fd, IOCTL_STOP_MONITORING, &pmu_1) == -1) {
	perror("Failed to stop monitoring");
	return EXIT_FAILURE;
}
```

Por último, para cerrar el programa, deshabilitaremos el PMU.

```c
if (ioctl(fd, IOCTL_DISABLE_MONITORING) == -1) {
	perror("Failed to disable monitoring");
	return EXIT_FAILURE;
}
```

### Acceso a los eventos Uncore

Para acceder a los eventos *Uncore* el proceso es similar al apartado anterior con algunas pequeñas diferencias debido a las características de estos eventos.

Al igual que antes, abrimos el dispositivo creado.

```c
#define DEVICE_PATH "/dev/nv_pmu"

fd = open(DEVICE_PATH, O_RDWR);
if (fd == -1) {
	perror("Failed to open device file");
	return EXIT_FAILURE;
}
```

Ahora guardamos la configuración deseada en la estructura de datos. Ahora la configuración es diferente, como se indica en la documentación en el directorio *docu* y en el TRM, hay que indicar el *unit group* y el *unit* que se desea.  

Función ejemplo para la configuración:

```c
void set_pmu_config(struct carmel_pmu_config *cfg, uint32_t event_type, uint8_t unit_group, uint8_t unit, uint8_t counter_index){
    cfg->counter_index = counter_index;
    cfg->event_type = event_type;
    cfg->unit_group = unit_group;
    cfg->unit = unit;
}
```

Guardar configuración deseada:

```c
// Configure PMUs    
struct carmel_pmu_config pmu_1;
    
set_pmu_config(&pmu_1, event_type, unit_group, unit, counter_index);
```

Habilitamos y configuramos el PMU:

```c
if (ioctl(fd, IOCTL_ENABLE_NV_UNCORE_MONITORING, &pmu_1) == -1) {
	perror("Failed to enable monitoring");
	return EXIT_FAILURE;
}
```

Comenzamos a contar.

```c
if (ioctl(fd, IOCTL_START_NV_UNCORE_MONITORING, &pmu_1) == -1) {
	perror("Failed to start monitoring");
	return EXIT_FAILURE;
}
```

Leemos el contador y guardamos el valor.

```c
if (ioctl(fd, IOCTL_READ_NV_UNCORE_COUNTER, &pmu_1) == -1) {
	perror("Failed to read counter value");
	return EXIT_FAILURE;
}

counter_value = pmu_1.counter_value;
```

Paramos el contador, una vez leídos los datos necesarios.

```c
if (ioctl(fd, IOCTL_STOP_NV_UNCORE_MONITORING, &pmu_1) == -1) {
	perror("Failed to stop monitoring");
	return EXIT_FAILURE;
}
```

Deshabilitamos el PMU.

```c
if (ioctl(fd, IOCTL_DISABLE_NV_UNCORE_MONITORING) == -1) {
	perror("Failed to disable monitoring");
	return EXIT_FAILURE;
}
```

Por último cerramos el directorio donde se encuentra el dispositivo creado.

```c
close(fd);
```