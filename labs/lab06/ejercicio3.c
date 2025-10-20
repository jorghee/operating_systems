#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TAM_VECTOR 10000000
#define MAX_HILOS 8

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
  long long* suma_parcial = malloc(sizeof(long long));
  if (suma_parcial == NULL) {
    fprintf(stderr, "Error: malloc falló para la suma parcial\n");
    return NULL;
  }
  
  *suma_parcial = 0;
  for (long long i = data->inicio; i < data->fin; i++) {
    *suma_parcial += data->vector[i];
  }
  
  return (void*) suma_parcial;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Uso: %s <num_hilos>\n", argv[0]);
    return 1;
  }
  int num_hilos = atoi(argv[1]);
  if (num_hilos < 1 || num_hilos > MAX_HILOS) {
    fprintf(stderr, "El número de hilos debe estar entre 1 y %d\n", MAX_HILOS);
    return 1;
  }

  int* vector = malloc(TAM_VECTOR * sizeof(int));
  if (!vector) { perror("malloc para vector"); exit(EXIT_FAILURE); }
  
  srand(12345);
  for (int i = 0; i < TAM_VECTOR; i++) {
    vector[i] = rand() % 10;
  }

  // --- Versión Secuencial ---
  printf("Versión Secuencial\n");
  double start_seq = obtener_tiempo();
  long long suma_secuencial = 0;
  for (int i = 0; i < TAM_VECTOR; i++) {
    suma_secuencial += vector[i];
  }
  double end_seq = obtener_tiempo();
  printf("Suma secuencial: %lld\n", suma_secuencial);
  printf("Tiempo secuencial: %.6f segundos\n\n", end_seq - start_seq);

  // --- Versión Paralela ---
  printf("Versión Paralela (%d hilos)\n", num_hilos);
  double start_par = obtener_tiempo();
  
  pthread_t hilos[num_hilos];
  suma_data_t datos_hilos[num_hilos];
  long long chunk_size = TAM_VECTOR / num_hilos;

  for (int i = 0; i < num_hilos; i++) {
    datos_hilos[i].id = i;
    datos_hilos[i].vector = vector;
    datos_hilos[i].inicio = i * chunk_size;
    datos_hilos[i].fin = (i == num_hilos - 1) ? TAM_VECTOR : (i + 1) * chunk_size;

    pthread_create(&hilos[i], NULL, worker_suma_parcial, &datos_hilos[i]);
  }

  // Recolectar resultados
  long long suma_paralela = 0;
  for (int i = 0; i < num_hilos; i++) {
    void* resultado_parcial_ptr;
    pthread_join(hilos[i], &resultado_parcial_ptr);
    
    long long* resultado_parcial = (long long*) resultado_parcial_ptr;
    suma_paralela += *resultado_parcial;
    
    free(resultado_parcial);
  }
  
  double end_par = obtener_tiempo();
  printf("Suma paralela: %lld\n", suma_paralela);
  printf("Tiempo paralelo: %.6f segundos\n\n", end_par - start_par);

  if (suma_secuencial == suma_paralela) {
    printf("Success. Ambas sumas coinciden.\n");
  } else {
    printf("Error. Las sumas no coinciden.\n");
  }

  free(vector);
  return 0;
}
