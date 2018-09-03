#include "RR.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



Queue *queue_init()
{
  struct queue *puntero_queue;
  puntero_queue = (struct queue *) malloc(sizeof(struct queue));
  puntero_queue-> head = NULL;
  puntero_queue-> tail = NULL;
  puntero_queue-> lenght = 0;
  return puntero_queue;
}

Stack *stack_init()
{
  struct stack *puntero_stack;
  puntero_stack = (struct stack *) malloc(sizeof(struct stack));
  puntero_stack-> head = NULL;
  puntero_stack-> tail = NULL;
  puntero_stack-> lenght = 0;
  return puntero_stack;
}

Process *idle_init()
{
  struct process *puntero_proceso;
  puntero_proceso = (struct process *) malloc(sizeof(struct process));
  puntero_proceso -> PID = 0;
  puntero_proceso -> name = "IDLE";
  puntero_proceso -> estado = RUNNING;
  puntero_proceso -> rafagas = 1;
  puntero_proceso -> datos = NULL;
  puntero_proceso -> tiempo_inicio = -1;
  puntero_proceso -> tiempo_procesamiento_necesario = 0;
  puntero_proceso -> tiempo_procesamiento_ocupado = 0;
  puntero_proceso -> tiempo_waiting_necesario = 0;
  puntero_proceso -> tiempo_waiting_ocupado = 0;
  puntero_proceso -> turnaround_time = 0;
  puntero_proceso -> response_time = 0;
  puntero_proceso -> waiting_time = 0;
  puntero_proceso -> ejecucion = 0;
  puntero_proceso -> uso_cpu = 0;
  puntero_proceso -> interrupciones = 0;

  return puntero_proceso;
}


void push_inicial(Queue* queue, int PID, char *nombre, int status, Stack* dato, int rafaga, int inicio)
{
  /*faltan agregar atriutos tales como */

  Process *puntero_proceso;
  puntero_proceso = (Process *)malloc(sizeof(Process));

  if (queue->head == NULL){
    queue -> head = puntero_proceso;
  }
  puntero_proceso->next = queue -> tail;
  queue -> tail = puntero_proceso;


  puntero_proceso -> PID = PID;
  puntero_proceso -> name = nombre;
  puntero_proceso -> estado = status;
  puntero_proceso -> rafagas = rafaga;
  puntero_proceso -> datos = dato;
  puntero_proceso -> tiempo_inicio = inicio;
  puntero_proceso -> tiempo_procesamiento_necesario = 0;
  puntero_proceso -> tiempo_procesamiento_ocupado = 0;
  puntero_proceso -> tiempo_waiting_necesario = 0;
  puntero_proceso -> tiempo_waiting_ocupado = 0;
  puntero_proceso -> turnaround_time = 0;
  puntero_proceso -> response_time = 0;
  puntero_proceso -> waiting_time = 0;
  puntero_proceso -> ejecucion = 0;
  puntero_proceso -> uso_cpu = 0;
  puntero_proceso -> interrupciones = 0;
  queue -> lenght ++;

}

void push_process_datos(Stack* stack, int dato){
  Node *value;
  value = (Node *)malloc(sizeof(Node));
  value -> contenido = dato;
  if (stack->head ==NULL){
    stack -> head = value;
  }
  value -> next = stack -> tail;
  stack -> tail = value;
  stack -> lenght ++ ;

}

void pop_dato(Process* process){
  Node* dato;
  Node* dato_siguiente;
  Node* dato_eliminado;
  dato = process->datos->tail;
  dato_siguiente = dato->next;

  for (size_t i = 0; i < process->datos->lenght; i++) {

    if (dato_siguiente == process->datos->head) {

        dato_eliminado = process->datos->head;
        dato->next = process->datos->head->next;
        process->datos->head = dato;
        process->datos->lenght--;
        if (process->datos->lenght < 2){
          process->datos->tail =process->datos->head;
        }
        free(dato_eliminado);
    }
    if (dato_siguiente == NULL) {
      break;
    }
    dato = dato_siguiente;
    dato_siguiente = dato->next;
  }
}

void add_CPU(CPU *cpu, Queue* scheduler, int simulacion){

  Process* proceso;
  Process* proceso_siguiente;
  Process* proceso_eliminado;

  proceso = scheduler->tail;

  proceso_siguiente = proceso->next;

  for (size_t i = 0; i < scheduler->lenght; i++) {
    if (scheduler->lenght == 1) {

      scheduler->head = proceso->next;
      scheduler->tail = proceso->next;
      proceso_eliminado = proceso;
      cpu->proceso_in = proceso_eliminado;
      cpu->proceso_in -> estado = RUNNING;
      printf("[t= %d] El proceso: %s cambió su estado a RUNNING \n", simulacion, cpu->proceso_in->name);
      cpu->proceso_in->uso_cpu++;
      if (cpu->proceso_in->ejecucion==0) {
        cpu->proceso_in->ejecucion=1;
      }
      cpu->OCUPADO=1;
      scheduler->lenght--;

    }
    else if (scheduler->lenght > 1 && proceso_siguiente == scheduler->head){

        cpu->proceso_in = scheduler->head;
        proceso->next = NULL;
        scheduler->head = proceso;
        cpu->tiempo_dentro_cpu = 0;
        cpu->proceso_in -> estado = RUNNING;
        printf("[t= %d] El proceso: %s cambió su estado a RUNNING \n", simulacion, cpu->proceso_in->name);
        cpu->proceso_in->uso_cpu++;
        if (cpu->proceso_in->ejecucion==0) {
          cpu->proceso_in->ejecucion=1;
        }
        cpu->OCUPADO=1;
        scheduler->lenght--;

    }
    if (proceso_siguiente==NULL) {
      break;
    }
    proceso=proceso_siguiente;
    proceso_siguiente=proceso->next;
  }
}

void add_scheduler(Queue* scheduler, Queue* arrayProcess, int simulacion){
  Process* process_inicio;
  Process* process_siguiente;

  process_inicio = arrayProcess->tail;
  process_siguiente = process_inicio->next;

  for (size_t i = 0; i < arrayProcess->lenght; i++) {
    if (process_inicio->tiempo_inicio == simulacion) {
      Process *puntero_proceso;
      puntero_proceso = (Process *)malloc(sizeof(Process));

      if (scheduler->head == NULL){
        puntero_proceso->next=NULL;
        scheduler -> head = puntero_proceso;
        scheduler->tail = puntero_proceso;
      }
      else{
        puntero_proceso->next=scheduler->tail;
        scheduler->tail=puntero_proceso;
      }
      puntero_proceso -> PID = process_inicio->PID;
      puntero_proceso -> name = process_inicio->name;
      puntero_proceso -> estado = READY;
      puntero_proceso -> rafagas = process_inicio->rafagas;
      puntero_proceso -> datos = process_inicio->datos;
      puntero_proceso -> tiempo_inicio = process_inicio-> tiempo_inicio;
      scheduler -> lenght ++;
      printf("[t= %d]El proceso: %s se ha creado\n", simulacion, puntero_proceso->name);
    }
    if (process_siguiente==NULL) {
      break;
    }
    process_inicio = process_siguiente;
    process_siguiente = process_inicio->next;
  }
}

void push_cpu_scheduler(CPU *cpu, Queue* scheduler, int simulacion)
{
  cpu->proceso_in->next = scheduler->tail;
  scheduler->tail = cpu->proceso_in;
  scheduler->tail->estado = READY;
  scheduler->lenght++;
  printf("[t= %d] El proceso: %s cambió su estado a READY \n", simulacion, scheduler->tail->name);
  cpu->proceso_in = NULL;
  cpu->tiempo_dentro_cpu = 0;
  cpu->OCUPADO=0;

}

void push_cpu_waiting(Queue* waiting, CPU *cpu, int simulacion){

  cpu->proceso_in->next = waiting->tail;
  if (waiting->head==NULL) {
    waiting->head = cpu->proceso_in;
  }
  waiting->tail = cpu->proceso_in;
  waiting->tail->estado = WAITING;
  pop_dato(waiting->tail);
  waiting->tail->tiempo_procesamiento_ocupado = 0;
  waiting->tail->tiempo_waiting_ocupado = 0;
  printf("[t= %d] El proceso: %s cambió su estado a WAITING \n", simulacion, waiting->tail->name);

  cpu->proceso_in = NULL;
  cpu->tiempo_dentro_cpu = 0;
  waiting->lenght++;
  cpu->OCUPADO=0;
}

void push_waiting_scheduler(Queue* waiting, Queue *scheduler, int simulacion){
  if(waiting->lenght>0){
    Process* proceso;
    Process* proceso_siguiente;
    Process* proceso_anterior;
    Process* proceso_elegido;

    proceso = waiting->tail;
    proceso_siguiente = proceso->next;
    proceso_anterior = proceso;

    if (waiting->lenght==1 && proceso->datos->head->contenido==proceso->tiempo_waiting_ocupado) {
      waiting->tail = proceso->next;
      waiting->head = proceso->next;
      proceso->next = scheduler->tail;
      scheduler->tail = proceso;
      if (scheduler->head==NULL) {
            scheduler->head=proceso;
          }
      scheduler->tail->estado = READY;
      printf("[t= %d] El proceso: %s cambió su estado a READY \n", simulacion, scheduler->tail->name);
      scheduler->tail->tiempo_procesamiento_ocupado = 0;
scheduler->tail->tiempo_waiting_ocupado = 0;
      pop_dato(scheduler->tail);
      scheduler->lenght++;
      waiting->lenght--;
    }

    else if (waiting->lenght>1) {
      for (size_t i = 0; i < waiting->lenght; i++) {
        if ( proceso == waiting->tail && proceso->datos->head->contenido==proceso->tiempo_waiting_ocupado) {
          waiting->tail = proceso->next;
          proceso->next = scheduler->tail;
          scheduler->tail = proceso;
          if (scheduler->head==NULL) {
            scheduler->head=proceso;
          }
          scheduler->tail->estado = READY;
          printf("[t= %d] El proceso: %s cambió su estado a READY \n", simulacion, scheduler->tail->name);
          scheduler->tail->tiempo_procesamiento_ocupado = 0;
scheduler->tail->tiempo_waiting_ocupado = 0;
          pop_dato(scheduler->tail);
          scheduler->lenght++;
          waiting->lenght--;
          proceso = waiting->tail;
          proceso_siguiente = proceso->next;
          proceso_anterior = proceso;
        }

        else {
          proceso_elegido = proceso_siguiente;
          proceso->next = proceso_siguiente->next;
          proceso_elegido->next=scheduler->tail;
          scheduler->tail = proceso_elegido;
          if (scheduler->head==NULL) {
            scheduler->head=proceso_elegido;
          }
          scheduler->tail->estado = READY;
          printf("[t= %d] El proceso: %s cambió su estado a READY \n", simulacion, scheduler->tail->name);
          pop_dato(scheduler->tail);
          scheduler->tail->tiempo_procesamiento_ocupado = 0;
scheduler->tail->tiempo_waiting_ocupado = 0;
          scheduler->lenght++;
          waiting->lenght--;
          proceso = waiting->tail;
          proceso_siguiente = proceso->next;
        }
      }
    }
  }
}

/*
void push_waiting_scheduler(Queue* waiting, Queue *scheduler, int simulacion){
  if(waiting->lenght>0){
    Process* proceso;
    Process* proceso_siguiente;
    Process* proceso_anterior;
    Process* proceso_ready;
    proceso = waiting->tail;
    proceso_anterior = waiting->tail;
    proceso_siguiente = proceso->next;

    for (size_t i = 0; i < scheduler->lenght; i++) {
      printf("----------------------------------\n" );
      if (scheduler->lenght>1) {

        if (proceso == waiting->tail && proceso->datos->head->contenido == proceso->tiempo_waiting_ocupado ) {
          printf("entre 1\n" );
          waiting->tail = proceso->next;
          waiting->lenght--;
          proceso_ready = proceso;
          proceso_ready->next=scheduler->tail;
          scheduler->tail=proceso_ready;
          scheduler->tail->estado = READY;
          printf("[t= %d] El proceso: %s cambió su estado a READY \n", simulacion, scheduler->tail->name);

          pop_dato(scheduler->tail);

          scheduler->tail->tiempo_procesamiento_ocupado = 0;
          scheduler->tail->tiempo_waiting_ocupado = 0;
          scheduler->lenght++;
        }
        else{

          if (proceso_siguiente->datos->head->contenido == proceso_siguiente->tiempo_waiting_ocupado) {
            printf("entre 2\n" );
            proceso_ready = proceso_siguiente;
            proceso_ready->next=scheduler->tail;
            scheduler->tail=proceso_ready;
            scheduler->tail->estado = READY;
            waiting->lenght--;
            printf("[t= %d] El proceso: %s cambió su estado a READY \n", simulacion, scheduler->tail->name);
            proceso->next = proceso_siguiente->next;
            pop_dato(scheduler->tail);
            scheduler->tail->tiempo_procesamiento_ocupado = 0;
            scheduler->tail->tiempo_waiting_ocupado = 0;
            scheduler->lenght++;

          }
        }
      }
      else{
      }

      if (proceso_siguiente == NULL) {
        break;
      }
      else{
      proceso = proceso_siguiente;
      proceso_siguiente = proceso->next;
      }
    }
  }
}

*/
void delete_process(Queue* array, CPU* cpu, int simulacion){
  printf("[t= %d] El proceso: %s is FINISHED \n", simulacion, cpu->proceso_in->name);

  if (array->head==NULL) {
    array->head=cpu->proceso_in;
    array->tail=cpu->proceso_in;
  }
  else{
    cpu->proceso_in->next = array->tail;
    array->tail= cpu->proceso_in;
  }
  array->lenght++;
  cpu->proceso_in = NULL;
  cpu->tiempo_dentro_cpu = 0;
  cpu->OCUPADO=0;
}

void actualizar_tiempo(Queue* waiting){
  if (waiting->lenght>0){
    Process* proceso;
    Process* process_siguiente;
    proceso = waiting->tail;
    process_siguiente = proceso->next;
    for (size_t i = 0; i < waiting->lenght; i++) {
      proceso->tiempo_waiting_ocupado++;
      proceso->tiempo_procesamiento_ocupado++;
      if (proceso->next==NULL) {
        break;
      }
      proceso = process_siguiente;
      process_siguiente = proceso->next;
    }
  }
}


void actualizar_tiempo_cpu(CPU *cpu){
  if (cpu->proceso_in!=NULL) {
  cpu->proceso_in->tiempo_procesamiento_ocupado++;
  cpu->proceso_in->tiempo_waiting_ocupado++;
  }
}

void actualizar_turnaround_tieme(CPU *cpu, Queue* waiting, Queue* scheduler){
  if (waiting->lenght>0){
    Process* proceso;
    Process* process_siguiente;
    proceso = waiting->tail;
    process_siguiente = proceso->next;
    for (size_t i = 0; i < waiting->lenght; i++) {
      proceso->turnaround_time++;
      if (proceso->next==NULL) {
        break;
      }
      proceso = process_siguiente;
      process_siguiente = proceso->next;
    }
  }
  if (scheduler->lenght>0){
    Process* proceso;
    Process* process_siguiente;
    proceso = scheduler->tail;
    process_siguiente = proceso->next;
    for (size_t i = 0; i < scheduler->lenght; i++) {
      proceso->turnaround_time++;
      if (proceso->next==NULL) {
        break;
      }
      proceso = process_siguiente;
      process_siguiente = proceso->next;
    }
  }
  if (cpu->proceso_in!=NULL) {
  cpu->proceso_in->turnaround_time++;
  }
}

void actualizar_response_tiempo(Queue* scheduler){
  if (scheduler->lenght>0){
    Process* proceso;
    Process* process_siguiente;
    proceso = scheduler->tail;
    process_siguiente = proceso->next;
    for (size_t i = 0; i < scheduler->lenght; i++) {
      if (proceso->ejecucion==0) {
        proceso->response_time++;
      }
      if (proceso->next==NULL) {
        break;
      }
      proceso = process_siguiente;
      process_siguiente = proceso->next;
    }
  }
}

void actualizar_waiting_tiempo(Queue* scheduler, Queue* waiting){
  if (waiting->lenght>0){
    Process* proceso;
    Process* process_siguiente;
    proceso = waiting->tail;
    process_siguiente = proceso->next;
    for (size_t i = 0; i < waiting->lenght; i++) {
      proceso->waiting_time++;
      if (proceso->next==NULL) {
        break;
      }
      proceso = process_siguiente;
      process_siguiente = proceso->next;
    }
  }
  if (scheduler->lenght>0){
    Process* proceso;
    Process* process_siguiente;
    proceso = scheduler->tail;
    process_siguiente = proceso->next;
    for (size_t i = 0; i < scheduler->lenght; i++) {
      proceso->waiting_time++;
      if (proceso->next==NULL) {
        break;
      }
      proceso = process_siguiente;
      process_siguiente = proceso->next;
    }
  }
}
