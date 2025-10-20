#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_FILAS 3
#define NUM_COLS 3

typedef struct {
  int id_hilo;
  int fila_idx;
  float escalar;
  float (*matriz_ptr)[NUM_COLS];
} thread_data_t;

void inicializar_matriz(float matriz[NUM_FILAS][NUM_COLS]);
void imprimir_matriz(float matriz[NUM_FILAS][NUM_COLS]);
void* worker_multiplica_fila(void* arg);

int main() {
  pthread_t hilos[NUM_FILAS];
  thread_data_t datos_hilos[NUM_FILAS];
  float matriz_compartida[NUM_FILAS][NUM_COLS];
  const float ESCALAR = 5.0;

  srand(time(NULL));
  inicializar_matriz(matriz_compartida);
  
  printf("Matriz original:\n");
  imprimir_matriz(matriz_compartida);
  printf("\nIniciando hilos para multiplicar por %.2f...\n", ESCALAR);

  for (int i = 0; i < NUM_FILAS; i++) {
    datos_hilos[i].id_hilo = i + 1;
    datos_hilos[i].fila_idx = i;
    datos_hilos[i].escalar = ESCALAR;
    datos_hilos[i].matriz_ptr = matriz_compartida;

    if (pthread_create(&hilos[i], NULL, worker_multiplica_fila, &datos_hilos[i]) != 0) {
      perror("Error al crear el hilo");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < NUM_FILAS; i++) {
    if (pthread_join(hilos[i], NULL) != 0) {
      perror("Error al esperar al hilo");
      exit(EXIT_FAILURE);
    }
  }

  printf("\nOK...\n");
  printf("Matriz final:\n");
  imprimir_matriz(matriz_compartida);

  return 0;
}

void* worker_multiplica_fila(void* arg) {
  thread_data_t* data = (thread_data_t*) arg;

  printf(" -> Hilo %d: Iniciando trabajo en la fila %d.\n", data->id_hilo, data->fila_idx);
  
  sleep(1); 

  for (int j = 0; j < NUM_COLS; j++) {
    data->matriz_ptr[data->fila_idx][j] *= data->escalar;
  }

  printf(" <- Hilo %d: Trabajo en la fila %d terminado.\n", data->id_hilo, data->fila_idx);

  return NULL;
}

void inicializar_matriz(float matriz[NUM_FILAS][NUM_COLS]) {
  for (int i = 0; i < NUM_FILAS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      matriz[i][j] = (float)(rand() % 10);
    }
  }
}

void imprimir_matriz(float matriz[NUM_FILAS][NUM_COLS]) {
  for (int i = 0; i < NUM_FILAS; i++) {
    printf("  [ ");
    for (int j = 0; j < NUM_COLS; j++) {
      printf("%6.2f ", matriz[i][j]);
    }
    printf("]\n");
  }
}
