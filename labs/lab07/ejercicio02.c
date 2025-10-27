#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ITEMS 15

// Recurso compartido
int buffer;
int recurso_disponible = 0;
pthread_mutex_t mutex_buffer;

void* productor(void* arg);
void* consumidor(void* arg);

void* productor(void* arg) {
  for (int i = 0; i < NUM_ITEMS; i++) {
    while (1) {
      pthread_mutex_lock(&mutex_buffer);
      if (recurso_disponible == 0) {
        buffer = i;
        printf("Productor produce -> %d\n", buffer);
        recurso_disponible = 1;
        pthread_mutex_unlock(&mutex_buffer);
        break; // Sale del bucle de espera
      }
      pthread_mutex_unlock(&mutex_buffer);
      usleep(1000); // Cede el CPU para no saturar
    }
  }
  return NULL;
}

void* consumidor(void* arg) {
  for (int i = 0; i < NUM_ITEMS; i++) {
    while (1) {
      pthread_mutex_lock(&mutex_buffer);
      if (recurso_disponible == 1) {
        int valor = buffer;
        printf("Consumidor consume <- %d\n", valor);
        recurso_disponible = 0;
        pthread_mutex_unlock(&mutex_buffer);
        break; // Sale del bucle de espera
      }
      pthread_mutex_unlock(&mutex_buffer);
      usleep(1000); // Cede el CPU
    }
  }
  return NULL;
}

int main() {
  pthread_t hilo_prod, hilo_cons;

  pthread_mutex_init(&mutex_buffer, NULL);

  pthread_create(&hilo_prod, NULL, productor, NULL);
  pthread_create(&hilo_cons, NULL, consumidor, NULL);

  pthread_join(hilo_prod, NULL);
  pthread_join(hilo_cons, NULL);

  pthread_mutex_destroy(&mutex_buffer);

  printf("Proceso finalizado.\n");
  return 0;
}
