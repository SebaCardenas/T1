#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "RR.h"


int main(int argc, char *argv[])
{	int quantum;
	int simulacion;
	int instrucciones;
	int instrucciones_terminadas;

	if (argc != 3 && argc != 4)
	{
		printf("Modo de uso: ./scheduler test.txt output.csv quantum\n");
		return 0;
	}

	/* Abrimos el archivo input en modo de lectura */
	FILE *input_file = fopen(argv[1], "r");

	/* Abrimos el archivo output en modo de escritura */
	FILE *output_file = fopen(argv[2], "w");

	/* Revisa que el archivo fue abierto correctamente */
	if (!input_file)
	{
		printf("¡El archivo %s no existe!\n", argv[1]);
		return 2;
	}
	if (argv[3]==NULL) {
		quantum = 3;
	}

	else{
		quantum = atoi(argv[3]);
	}

	printf("quantum: %d\n", quantum);

	Queue *cola;
	Stack *stack;

	char c[256];
	int contador2 = 0;
	char palabra[256];

	bool process_name;
	int ID;
	int rafaga_stack;
	int contenido_proceso;
	int contador_proceso;
	int inicio_process;


	cola = queue_init();

	ID = 0;
	instrucciones = 0;
	contador_proceso = 0;
	while( fgets ( c, sizeof c, input_file ) != NULL )
  { memset(palabra, 0, sizeof palabra);
		stack = stack_init();
		process_name = 1;
		char *puntero_palabra;
		puntero_palabra = (char *) malloc(sizeof(char));
		for (size_t i = 0; i < strlen(c)+2; i++) {

			if (c[i] == ' ' || c[i] == '\n' ) {
				if (process_name == 1){

						strcpy(puntero_palabra,palabra);
						process_name = 0;
				}
				else{

				if (contador_proceso == 1) {
						inicio_process = atoi(palabra);
					}
				if (contador_proceso == 2) {
					rafaga_stack = atoi(palabra);
				}
				if (contador_proceso>2) {

				contenido_proceso = atoi(palabra);


				push_process_datos(stack,contenido_proceso);
}
				}
				contador2=0;
				memset(palabra, 0, sizeof palabra);
				contador_proceso++;
			}
			else{
				palabra[contador2] = c[i];
				contador2++;
			}

		}
		contador_proceso = 0;
		push_inicial(cola, ID,puntero_palabra, WAITING, stack, rafaga_stack, inicio_process);
		contador2 = 0;
		ID++;
		instrucciones++;


	}
	Process *idle;
	Queue *scheduler;
	Queue *waiting;
	CPU *cpu;
	Queue *Array;
	int terminar;

	idle= idle_init();
	Array = queue_init();



/*
	Process *proceso_actual = cola->tail;
	Process *aux2;
	Node *dato_actual = cola->tail->datos->tail;
	Node *aux;

	for (size_t i = 0; i < cola->lenght; i++) {
			printf("nombre: %s id:_%d tiempo inicio: %d rafagas: %d estado: %d\n", proceso_actual->name,proceso_actual->PID,proceso_actual->tiempo_inicio,proceso_actual->rafagas, proceso_actual->estado);

			for (size_t j = 0; j < proceso_actual->datos->lenght; j++) {

				printf("%d \n", dato_actual-> contenido);
				aux = dato_actual->next;
				dato_actual = aux;

			}
			printf("\n" );
			if (proceso_actual->next ==NULL) {
				break;
			}
			aux2 = proceso_actual->next;
			dato_actual = aux2->datos->tail;
			proceso_actual = aux2;

	}*/
	simulacion = 0;
	scheduler = queue_init();
	waiting = queue_init();
	cpu =(CPU *)malloc(sizeof(CPU));
	cpu->OCUPADO = 0;

	bool CPU_ocupada;
	CPU_ocupada = 0;
	instrucciones_terminadas = 0;
	terminar = 0;
	while (true) {

		add_scheduler(scheduler, cola, simulacion);

		push_waiting_scheduler(waiting, scheduler, simulacion);

		if(cpu->OCUPADO == 0){
			if (scheduler->lenght>0) {
				add_CPU(cpu, scheduler, simulacion);
			}
		}
		else{
			if (cpu->proceso_in->datos->head->contenido == cpu->proceso_in->tiempo_procesamiento_ocupado) {
				if (cpu->proceso_in->tiempo_procesamiento_ocupado%quantum==0) {
					cpu->proceso_in->interrupciones++;
				}
				if (cpu->proceso_in->datos->lenght==1) {
					delete_process(Array, cpu, simulacion);
					terminar++;
					cpu->OCUPADO=0;
				}
				else {
					push_cpu_waiting(waiting, cpu, simulacion);
					cpu->OCUPADO=0;
				}

			}
			else if (cpu->proceso_in->tiempo_procesamiento_ocupado%quantum==0) {
				cpu->proceso_in->interrupciones++;
				push_cpu_scheduler(cpu, scheduler, simulacion);
				cpu->OCUPADO=0;
			}

		}
		if(cpu->OCUPADO == 0){
			if (scheduler->lenght>0 ) {
				add_CPU(cpu, scheduler, simulacion);
			}
		}




		actualizar_tiempo(waiting);
		actualizar_tiempo_cpu(cpu);
		actualizar_turnaround_tieme(cpu, waiting, scheduler);
		actualizar_response_tiempo(scheduler);
		actualizar_waiting_tiempo(scheduler, waiting);
		simulacion++;
		if (terminar==cola->lenght) {
			break;
		}
	}


	Process* p;
	Process* pp;
	p=Array->tail;
	pp = p->next;

	for (size_t i = 0; i < Array->lenght; i++) 	{

		fprintf(output_file, "%s,%d, %d, %d, %d, %d\n",p->name,p->uso_cpu, p->interrupciones, p->turnaround_time,p->response_time,p->waiting_time);
		if(pp ==NULL){break;}
		p = pp;
		pp = p->next;
	}

	fclose(input_file);
	fclose(output_file);


	///////////////// Recuerda que antes de acabar tu programa debes liberar toda la memoria reservada ///////////////////


	/* Esta linea indica que el programa termino sin errores*/
	return 0;
}
