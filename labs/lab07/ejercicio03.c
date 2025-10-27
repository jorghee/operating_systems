#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 300
#define M 200
#define P 400
#define NUM_HILOS 12

// Matrices y variables de control global
int A[N][M];
int B[M][P];
int C[N][P];
int siguiente_fila_a_calcular = 0;
pthread_mutex_t mutex_tarea;

void inicializar_matrices();
void* worker_multiplica(void* arg);

void inicializar_matrices() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      A[i][j] = i + j;
    }
  }
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < P; j++) {
      B[i][j] = i - j;
    }
  }
}

void* worker_multiplica(void* arg) {
  int fila;
  while (1) {
    // Inicio de la sección crítica para obtener una tarea
    pthread_mutex_lock(&mutex_tarea);
    fila = siguiente_fila_a_calcular;
    siguiente_fila_a_calcular++;
    pthread_mutex_unlock(&mutex_tarea);
    // Fin de la sección crítica

    if (fila >= N) {
      break;
    }

    // Realiza el cálculo para la fila asignada (fuera de la sección crítica)
    for (int j = 0; j < P; j++) {
      C[fila][j] = 0;
      for (int k = 0; k < M; k++) {
        C[fila][j] += A[fila][k] * B[k][j];
      }
    }
  }
  return NULL;
}

int main() {
  pthread_t hilos[NUM_HILOS];
  
  inicializar_matrices();
  pthread_mutex_init(&mutex_tarea, NULL);

  for (int i = 0; i < NUM_HILOS; i++) {
    pthread_create(&hilos[i], NULL, worker_multiplica, NULL);
  }

  for (int i = 0; i < NUM_HILOS; i++) {
    pthread_join(hilos[i], NULL);
  }

  pthread_mutex_destroy(&mutex_tarea);

  printf("OK...\n");
  printf("Matriz C:\n\n");
  for (int i = 0; i < 5 && i < N; i++) {
    for (int j = 0; j < 5 && j < P; j++) {
      printf("%8d ", C[i][j]);
    }
    printf("\n");
  }

  return 0;
}
