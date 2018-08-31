#pragma once

typedef struct process Process;

typedef struct queue Queue;
typedef struct stack Stack;
typedef struct node Node;
typedef struct cpu CPU;


enum estado{
  RUNNING = 0,
  READY = 1,
  WAITING = 2,
  FINISHED = 3
} Estado;

struct queue
{
  struct process *head;
  struct process *tail;
    int lenght;

};

struct stack
{
  struct node *head;
  struct node *tail;
    int lenght;
};


struct node
{
  struct node *next;
  int contenido;
};

struct process
{
  int uso_cpu;
  int interrupciones;
  int ejecucion;
  int turnaround_time;
  int response_time;
  int waiting_time;
  int PID;
  char *name;
  int estado;
  int tiempo_inicio;
  int rafagas;
  int tiempo_procesamiento_necesario;
  int tiempo_waiting_necesario;
  int tiempo_procesamiento_ocupado;
  int tiempo_waiting_ocupado;
  struct stack *datos;
  struct process *next;
};

struct cpu{
  struct process *proceso_in;
  int tiempo_dentro_cpu;
  int OCUPADO;
};

void push_process_datos(Stack* stack, int dato);

void push_scheduling(Queue* queue, Process* process);

void push_inicial(Queue* queue, int PID, char *nombre, int estado, Stack* dato, int rafaga, int inicio);

void push_cpu_scheduler(CPU *cpu, Queue* scheduler, int simulacion);

void add_scheduler(Queue* scheduler, Queue* arrayProcess, int simulacion);

void add_CPU(CPU *cpu, Queue* scheduler,int simulacion);

void pop_dato(Process* process);

void push_cpu_waiting(Queue* waiting, CPU *cpu,int simulacion);

void delete_process(Queue* array, CPU* cpu, int simulacion);

void push_waiting_scheduler(Queue* waiting, Queue *scheduler, int simulacion);

void actualizar_tiempo(Queue* waiting);

void actualizar_tiempo_cpu(CPU *cpu);

void actualizar_turnaround_tieme(CPU *cpu, Queue* waiting, Queue* scheduler);

void actualizar_response_tiempo(Queue* scheduler);

void actualizar_waiting_tiempo(Queue* scheduler, Queue* waiting);

Process *idle_init();

Queue *queue_init();

Stack *stack_init();
