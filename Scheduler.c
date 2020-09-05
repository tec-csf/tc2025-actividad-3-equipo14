/*

  Octavio Garduza, Christian Dalma

  Actividad 3: Administración de un SO
  Ejercicio 1

  Implementación de un simulador del algoritmo de planificación por pandilla
  utilizando Round Robin como criterio de planificación.


  Para implementar la estructura node_t y las funciones auxiliares de la
  lista ligada nos basamos en el siguiente tutorial:
  https://www.learn-c.org/en/Linked_lists
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define Q 4 //Quantum por default


typedef struct{
  int id;
  int burst_time;
  int gang;
}process;

typedef struct{
  int number;
  process p;
}cpu;

typedef struct node{
  process data;
  struct node *next;
}node_t;

//Establece el id, burst_time y gang del proceso
void set_process_info(node_t **head);
//Establece los CPUs del 1 al n
void set_cpu_info(cpu **cpu_head, cpu **cpu_tail);


//Funciones auxiliares para la lista ligada
void pop(node_t **head);
void push(node_t **head, process *p);
void push_to_end(node_t *head, process *p);

//Implementación propia
void pop_and_push(node_t **head);

//Funciones de calendarización
void assign_process_to_cpu(cpu **cpu_head, cpu **cpu_tail, node_t *head);
void substract_quantum_from_burst(node_t **process, int quantum);
int list_length(node_t *head);
void clean_cpu(cpu **cpu_head, cpu **cpu_tail);
void print_cpu_info(cpu *cpu_head, cpu *cpu_tail);

int main(int argc, const char * argv[]){

  node_t *dummy_head;
  cpu *cpu_head;
  cpu *cpu_tail;

  int n_of_processes = 0;
  int n_of_cpus = 0;

  printf("\n\nSIMULADOR DEL ALGORITMO DE PLANIFICACIÓN POR PANDILLA\n");
  printf("---------------------------------------------------------\n\n");

  printf("¿Cuántos procesos desea simular?: ");
  scanf("%d", &n_of_processes);

  printf("\n¿Cuántos CPUs desea simular?: ");
  scanf("%d", &n_of_cpus);

  dummy_head = (node_t *) malloc(sizeof(node_t));

  cpu_head = (cpu*) malloc(sizeof(cpu) * n_of_cpus);
  cpu_tail = cpu_head + n_of_cpus;

  set_cpu_info(&cpu_head, &cpu_tail);

  printf("\n\n                 INGRESAR PROCESOS                   \n");
  printf("---------------------------------------------------------\n");
  printf("\n -Las pandillas deben de ingresarse de manera conjunta\n");
  printf("\n -Pandilla: Se recomienda listar las pandillas del 1 a la n\n");
  printf("\n -ID del proceso: Se recomienda listar los procesos de 1 a n por cada pandilla\n");
  printf("\n -Tiempo de CPU: Total en segundos que requiere el proceso de CPU\n\n");
  for(int i = 0; i < n_of_processes; ++i){
    set_process_info(&dummy_head);
    printf("\n\n");
  }

  printf("\n\n          IMPRESION DE LOS PROCESOS EN CPU           \n");
  printf("---------------------------------------------------------\n");
  printf("\n -Los CPUS que no están en uso se representan con procesos negativos \n");
  printf("\n -El tiempo de CPU restante es previo a que el proceso utilice el CPU en cada ronda\n");
  printf("\n\nPresione cualquir tecla para comenzar\n");
  getchar();
  getchar();

  //Mientras que haya procesos sin acabar
  printf("******************************************************\n");
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

  //Liberar memoria
  free(dummy_head);
  free(cpu_head);
}//Fin de main

void set_process_info(node_t **head){
  process *p = (process*) malloc(sizeof(process));

  printf("Ingrese la pandilla a la que pertenece el proceso: ");
  scanf("%d", &p->gang);

  printf("Ingrese el ID del proceso: ");
  scanf("%d", &p->id);

  printf("Ingrese el tiempo de cpu requerido para el proceso: ");
  scanf("%d", &p->burst_time);

  push_to_end(*head, p);
}

void set_cpu_info(cpu **cpu_head, cpu **cpu_tail){
  cpu *aux;
  int n = 1;
  for(aux = *cpu_head; aux < *cpu_tail; ++aux) {
    aux->number = n;
    ++n;
  }
}

void pop(node_t **head){
  node_t* next_node;
  next_node = (*head)->next;
  (*head)->data.id = 0;
  (*head)->data.burst_time = 0;
  (*head)->data.gang = 0;
  free(*head);
  *head = next_node;
}

void push(node_t **head, process *p){
  node_t* new_node;
  new_node = (node_t*) malloc(sizeof(node_t));

  new_node->data = *p;
  new_node->next = *head;
  *head = new_node;
}

void push_to_end(node_t *head, process *p){
  node_t *current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = (node_t *) malloc(sizeof(node_t));
  current->next->data = *p;
  current->next->next = NULL;
}

void pop_and_push(node_t **head){
  process *p = (process*) malloc(sizeof(process));
  p->id = (*head)->data.id;
  p->burst_time = (*head)->data.burst_time;
  p->gang = (*head)->data.gang;
  push_to_end(*head, p);
  pop(head);
}


/*
 * Function:  assign_process_to_cpu
 * --------------------
 * Asigna al arreglo de CPUs los procesos guardados en la lista ligada
 * comenzando por el nodo inicial y continuando hasta que se agoten los CPUs
 * disponibles o los procesos a asignar.
 *
 *  cpu_head: Apuntador al apuntador del inicio del arreglo de cpus
 *  cpu_tail: Apuntador al apuntador del final del arreglo de cpus
 *  head: Apuntador al nodo inicial de la lista ligada
 *
 */
void assign_process_to_cpu(cpu **cpu_head, cpu **cpu_tail, node_t *head){
  node_t* current_process = head;
  cpu *current_cpu = *cpu_head;
  int count = 0;
  while(current_process != NULL && current_cpu < *cpu_tail){
    current_cpu->p = current_process->data;
    current_process = current_process->next;
    current_cpu++;
    count++;
  }
}

/*
 * Function:  clean_cpu
 * --------------------
 * Iguala todos los atributos de los procesos en CPU a -1 para simular que
 * los procesos han sido retirados del procesador.
 *
 *  cpu_head: Apuntador al apuntador del inicio del arreglo de cpus
 *  cpu_tail: Apuntador al apuntador del final del arreglo de cpus
 *
 */
void clean_cpu(cpu **cpu_head, cpu **cpu_tail){
  cpu* aux = *cpu_head;
  while(aux++ < *cpu_tail){
    aux->p.id = -1;
    aux->p.burst_time = -1;
    aux->p.gang = -1;
  }
}

/*
 * Function:  substract_quantum_from_burst
 * --------------------
 * Resta el quantum al burst_time del proceso pasado como argumento. Si el
 * burst_time resulta igual o menor a 0 elimina el nodo inicial.
 * Si el burst_time es mayor a 0 pasa el nodo inicial al final de la lista ligada.
 *
 *  cpu_head: Apuntador al apuntador del inicio del arreglo de cpus
 *  cpu_tail: Apuntador al apuntador del final del arreglo de cpus
 *
 */
void substract_quantum_from_burst(node_t **process, int quantum){
  (*process)->data.burst_time -= quantum;
  if((*process)->data.burst_time <= 0){
    pop(process);
  }else{
    pop_and_push(process);
  }
}

/*
 * Function:  list_length
 * --------------------
 *
 * Regresa el tamaño de la lista ligada
 *
 *  cpu_head: Apuntador al inicio de la lista ligada.
 *
 * returns: int del tamaño de la lista ligada.
 *
*/
int list_length(node_t *head){
    node_t *current = head;
    int c = 0;
    while(current != NULL){
      c++;
      current = current->next;
    }
    return(c);
}

/*
 * Function:  print_cpu_info
 * --------------------
 *
 * Imprime los procesos ligados a cada CPU
 *
 * cpu_head: Apuntador al inicio del arreglo de CPUs.
 * cpu_tail: Apuntador al final del arreglo de CPUs.
 *
*/
void print_cpu_info(cpu *cpu_head, cpu *cpu_tail){
  cpu * aux;
  for(aux = cpu_head; aux < cpu_tail; ++aux){
    printf("\nCPU: %d\n\nPandilla: %d\nProceso: %d\nTiempo de CPU restante: %d\n",
      aux->number,
      aux->p.gang,
      aux->p.id,
      aux->p.burst_time);
    printf("\n");
  }
}
