#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TAM_VECTOR 10000000
#define MAX_HILOS 8

// Variables globales para la suma y el mutex
long long suma_paralela = 0;
pthread_mutex_t mutex_suma;

typedef struct {
  int id;
  int* vector;
  long long inicio;
  long long fin;
} suma_data_t;

double obtener_tiempo() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec / 1e9;
}

void* worker_suma_parcial(void* arg) {
  suma_data_t* data = (suma_data_t*) arg;
  long long suma_local = 0;

  for (long long i = data->inicio; i < data->fin; i++) {
    suma_local += data->vector[i];
  }

  // Inicio de la sección crítica
  pthread_mutex_lock(&mutex_suma);
  suma_paralela += suma_local;
  pthread_mutex_unlock(&mutex_suma);
  // Fin de la sección crítica

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Uso: %s <numero_de_hilos>\n", argv[0]);
    return 1;
  }
  int num_hilos = atoi(argv[1]);
  if (num_hilos < 1 || num_hilos > MAX_HILOS) {
    fprintf(stderr, "El número de hilos debe estar entre 1 y %d\n", MAX_HILOS);
    return 1;
  }

  int* vector = malloc(TAM_VECTOR * sizeof(int));
  if (vector == NULL) return 1;
  srand(12345);
  for (int i = 0; i < TAM_VECTOR; i++) vector[i] = rand() % 10;
  
  // Suma secuencial para verificación
  double start_seq = obtener_tiempo();
  long long suma_secuencial = 0;
  for (int i = 0; i < TAM_VECTOR; i++) suma_secuencial += vector[i];
  double end_seq = obtener_tiempo();
  printf("Tiempo secuencial: \t%.4f segundos\n", end_seq - start_seq);

  // Inicialización del mutex
  pthread_mutex_init(&mutex_suma, NULL);

  pthread_t hilos[num_hilos];
  suma_data_t datos_hilos[num_hilos];
  long long chunk_size = TAM_VECTOR / num_hilos;

  double start_par = obtener_tiempo();
  for (int i = 0; i < num_hilos; i++) {
    datos_hilos[i].id = i;
    datos_hilos[i].vector = vector;
    datos_hilos[i].inicio = i * chunk_size;
    datos_hilos[i].fin = (i == num_hilos - 1) ? TAM_VECTOR : (i + 1) * chunk_size;
    pthread_create(&hilos[i], NULL, worker_suma_parcial, &datos_hilos[i]);
  }

  for (int i = 0; i < num_hilos; i++) {
    pthread_join(hilos[i], NULL);
  }
  double end_par = obtener_tiempo();
  printf("Tiempo paralelo:   \t%.4f segundos\n", end_par - start_par);

  // Destrucción del mutex
  pthread_mutex_destroy(&mutex_suma);

  printf("Suma secuencial: \t%lld\n", suma_secuencial);
  printf("Suma paralela:   \t%lld\n", suma_paralela);
  if (suma_secuencial == suma_paralela) {
    printf("Success.\n");
  } else {
    printf("Error.\n");
  }

  free(vector);
  return 0;
}
