#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_FILAS 3
#define NUM_COLS 3
#define NUM_WORKERS 3

typedef struct {
  int id_hilo;
  int fila_idx;
  float escalar;
  float (*matriz_ptr)[NUM_COLS];
} worker_data_t;

typedef struct {
  float (*matriz_ptr)[NUM_COLS];
  volatile bool* done_flag;
} printer_data_t;

pthread_mutex_t matriz_mutex = PTHREAD_MUTEX_INITIALIZER;

void inicializar_matriz(float matriz[NUM_FILAS][NUM_COLS]);
void imprimir_matriz(const char* titulo, float matriz[NUM_FILAS][NUM_COLS]);
void* worker_multiplica_fila(void* arg);
void* worker_impresor(void* arg);

int main() {
  pthread_t hilos_workers[NUM_WORKERS];
  pthread_t hilo_impresor;
  worker_data_t datos_workers[NUM_WORKERS];
  printer_data_t datos_impresor;

  float matriz_compartida[NUM_FILAS][NUM_COLS];
  volatile bool trabajo_terminado = false;
  const float ESCALAR = 5.0;

  srand(time(NULL));
  inicializar_matriz(matriz_compartida);
  
  imprimir_matriz("Matriz original:", matriz_compartida);
  printf("\nIniciando hilos...\n\n");

  datos_impresor.matriz_ptr = matriz_compartida;
  datos_impresor.done_flag = &trabajo_terminado;
  if (pthread_create(&hilo_impresor, NULL, worker_impresor, &datos_impresor) != 0) {
    perror("Error al crear el hilo impresor");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < NUM_WORKERS; i++) {
    datos_workers[i].id_hilo = i + 1;
    datos_workers[i].fila_idx = i;
    datos_workers[i].escalar = ESCALAR;
    datos_workers[i].matriz_ptr = matriz_compartida;

    if (pthread_create(&hilos_workers[i], NULL, worker_multiplica_fila, &datos_workers[i]) != 0) {
      perror("Error al crear el hilo trabajador");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < NUM_WORKERS; i++) {
    pthread_join(hilos_workers[i], NULL);
  }

  trabajo_terminado = true;
  pthread_join(hilo_impresor, NULL);

  printf("\nOK...\n");
  imprimir_matriz("Matriz final:", matriz_compartida);

  return 0;
}

void* worker_multiplica_fila(void* arg) {
  worker_data_t* data = (worker_data_t*) arg;
  printf(" -> Worker %d: trabajando en fila %d\n", data->id_hilo, data->fila_idx);
  
  sleep(data->id_hilo); 

  pthread_mutex_lock(&matriz_mutex);
  for (int j = 0; j < NUM_COLS; j++) {
    data->matriz_ptr[data->fila_idx][j] *= data->escalar;
  }
  pthread_mutex_unlock(&matriz_mutex);

  printf(" <- Worker %d: finalizado\n", data->id_hilo);
  return NULL;
}

void* worker_impresor(void* arg) {
  printer_data_t* data = (printer_data_t*) arg;
  int iteracion = 1;

  while (!(*data->done_flag)) {
    char titulo[50];
    sprintf(titulo, "Estado de la matriz (iteración %d):", iteracion++);
    
    pthread_mutex_lock(&matriz_mutex);
    imprimir_matriz(titulo, data->matriz_ptr);
    pthread_mutex_unlock(&matriz_mutex);

    sleep(1);
  }

  printf(" -> Hilo impresor: Terminando...\n");
  return NULL;
}

void inicializar_matriz(float matriz[NUM_FILAS][NUM_COLS]) {
  for (int i = 0; i < NUM_FILAS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      matriz[i][j] = (float)(rand() % 10 + 1);
    }
  }
}

void imprimir_matriz(const char* titulo, float matriz[NUM_FILAS][NUM_COLS]) {
  printf("%s\n", titulo);
  for (int i = 0; i < NUM_FILAS; i++) {
    printf("  [ ");
    for (int j = 0; j < NUM_COLS; j++) {
      printf("%6.2f ", matriz[i][j]);
    }
    printf("]\n");
  }
  printf("\n");
}
