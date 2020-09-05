### Simulador del algoritmo de planificación por pandilla


#### Descripción
Implementación en C del algoritmo de planificación por pandilla utilizando Round Robin como criterio de planificación. Se utilizaron estructuras y memoria dinámica.


#### Modelo de la simulación

###### Lista ligada

Se implementó una lista ligada utilizando la estructura ```node_t```. La lista ligada se utiliza para almacenar los procesos de una forma continua y ordenada. De igual forma se implementaron funciones auxiliares para añadir nodos al final y al inicio de la lista, para eliminar el nodo inicial y para pasar el nodo inicial hacia la cola.

```C
typedef struct node{
  process data;
  struct node *next;
}node_t;
```
```C
node_t *dummy_head = (node_t *) malloc(sizeof(node_t));
```

```C
void pop(node_t **head){}

void push(node_t **head, process *p){}

void push_to_end(node_t *head, process *p){}

void pop_and_push(node_t **head){}

```

###### Estructura Process

Se implementó una estructura para representar un proceso del S.O. Los procesos se almacenan en los nodos de la lista ligada. La cantidad de procesos a simular es elección del usuario.

```C
typedef struct{
  int id;
  int burst_time;
  int gang;
}process;
```

###### Estructura CPU

Se implementó una estructura para representar un CPU de la computadora. Los CPUS se almacenan en un arreglo dinámico. La cantidad de CPUS a simular es elección del usuario. Cada CPU apunta a un proceso para representar el trabajo del procesador.

```C
typedef struct{
  int number;
  process p;
}cpu;
```

```C
cpu *cpu_head = (cpu*) malloc(sizeof(cpu) * n_of_cpus);
cpu *cpu_tail = cpu_head + n_of_cpus;
```

#### Implementación del algoritmo

Mientras existan nodos en la lista ligada, existen procesos sin terminar su ejecución. Se asignan los procesos almacenados en la lista ligada a los CPUs almacenados en el arreglo dinámico. Tras asignar los procesos a los CPUs, se imprime la información de los mismos, es decir, qué proceso está en qué CPU. Se simula el tiempo de procesamiento a través de la función sleep. Posterior se evalua si la cantidad de procesos sin terminar es mayor al número de CPU; si es mayor se evita leer procesos en espera, si es menor se evita leer CPUs vacios. Finalmente se limpian los CPUs para simular el cambio de estado de los procesos. Lo anterior se repite mientras existan procesos activos.
<br>
<br>
Es importante mencionar la importancia de que los procesos de la misma pandilla estén juntos en la lista ligada, pues de otra manera la implementación no puede asegurar una correcta planificación por pandilla. En este momento, la única forma de asegurar esto es que el usuario ingrese los procesos de la misma pandilla de manera conjunta.

```C
while(list_length(dummy_head->next) > 0){
  assign_process_to_cpu(&cpu_head, &cpu_tail, dummy_head->next);
  print_cpu_info(cpu_head, cpu_tail);
  printf("******************************************************\n");
  sleep(Q); //Simula el tiempo de procesamiento
  if(list_length(dummy_head->next) > n_of_cpus){ //Evita leer procesos en espera
    for(int i = 0; i < n_of_cpus; ++i){
      substract_quantum_from_burst(&dummy_head->next, Q);
    }
  }else{//Evita llenar CPUs vacios
    for(int i = 0; i < list_length(dummy_head->next); ++i){
      substract_quantum_from_burst(&dummy_head->next, Q);
    }
  }
  clean_cpu(&cpu_head, &cpu_tail); //Establece todos los CPUS a 0
}
```

###### assign_process_to_cpu()
Asigna al arreglo de CPUs los procesos guardados en la lista ligada comenzando por el nodo inicial y continuando hasta que se agoten los CPUs disponibles o los procesos a asignar.

```C
void assign_process_to_cpu(cpu **cpu_head, cpu **cpu_tail, node_t *head){}
```

###### print_cpu_info()
Imprime los procesos ligados a cada CPU. De cada proceso indica su pandilla, su id y su tiempo requerido de CPU restante.

```C
void print_cpu_info(cpu *cpu_head, cpu *cpu_tail){}
```

###### substract_quantum_from_burst()
Resta el quantum al burst_time del proceso pasado como argumento. Si el burst_time resulta igual o menor a 0 elimina el nodo inicial. Si el burst_time es mayor a 0 pasa el nodo inicial al final de la lista ligada.

```C
void substract_quantum_from_burst(node_t **process, int quantum){}
```

###### clean_cpu()
Iguala todos los atributos de los procesos en CPU a -1 para simular que los procesos han sido retirados del procesador.

```C
void clean_cpu(cpu **cpu_head, cpu **cpu_tail){}
```

###### list_length()
Regresa un int con el tamaño de la lista ligada pasada como argumento.

```C
int list_length(node_t *head){}
```


### Arbol de Archivos

#### Descripción
Implementación en C de un programa que parta desde un directorio y decienda por el arbol de archivos registrando los tamaños de los archivos que encuentre

#### Metodos utilizados

###### listFilesRecursively()
Comienza  abrir los directorios usando funciones recursivas

```C
long long int* listFilesRecursively(char *path, long long int * vector);
```

###### statFileType()
Regresa el tipo de archivo

```C
unsigned char statFileType(char *fname);
```

###### stat()
Creacion de estructura de stats

```C
int stat(const char*path, struct stat *buf);
```

###### revisarUrnas()
Continua la creacion de urnas y crea valores para ellas 

```C
long long int *revisarUrnas( long long int * vector, int valor);
```

###### getFullName()
Toma todo el directorio

```C
char *getFullName(char *ruta, struct dirent *ent);
```

###### printVector()
Hace impresion del vector de los valores

```C
void printVector(long long int *vector);
```

###### getUrna()
Funcion get para regresar la urna

```C
long long int getUrna(long long int *vector);
```

###### getNumero()
Funcion que regresa la cantidad de urnas

```C
long long int getNumero(long long int *vector);
```


###### setNumero()
Genera un nuevo numero de urnas colectadas con set

```C
long long int *setNumero(long long int *vector, int nuevonum);
```

###### clearNew()
Funcion para prevenir fallas

```C
long long int *clearNew(long long int *vector, int lastvalue);
```

###### accumulate()
crea la acumulacion de valores

```C
long long int *accumulate(long long int *vector, int position);
```

###### maximoArchivos()
reconoce a la urna con la mayor cantidad de archivos

```C
int maximoArchivos (long long int *vector);
```

###### imprimirHistograma()
Imprime el hisotriagrama 

```C
void imprimirHistograma(long long int *vector);
```

