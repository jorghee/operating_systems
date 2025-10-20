#include <stdio.h>
#include <pthread.h>

// Versión sin sincronización

volatile int contador_global = 0;

void* incrementador(void* arg) {
  for (int i = 0; i < 100000; i++) {
    contador_global++;
  }
  return NULL;
}

void* decrementador(void* arg) {
  for (int i = 0; i < 100000; i++) {
    contador_global--;
  }
  return NULL;
}

// --- Versión con sincronización ---

int contador_sincronizado = 0;
pthread_mutex_t contador_mutex = PTHREAD_MUTEX_INITIALIZER;

void* incrementador_sinc(void* arg) {
  for (int i = 0; i < 100000; i++) {
    pthread_mutex_lock(&contador_mutex);
    contador_sincronizado++;
    pthread_mutex_unlock(&contador_mutex);
  }
  return NULL;
}

void* decrementador_sinc(void* arg) {
  for (int i = 0; i < 100000; i++) {
    pthread_mutex_lock(&contador_mutex);
    contador_sincronizado--;
    pthread_mutex_unlock(&contador_mutex);
  }
  return NULL;
}


int main() {
  pthread_t h1, h2;

  printf("> Sin sincronización\n");
  // Repetimos varias veces para ver la inconsistencia
  for (int i = 0; i < 5; i++) {
    contador_global = 0;
    pthread_create(&h1, NULL, incrementador, NULL);
    pthread_create(&h2, NULL, decrementador, NULL);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    printf("Ejecución %d: Resultado final = %d (esperado: 0)\n", i + 1, contador_global);
  }

  printf("\n> Con sincronización (Mutex)\n");
  for (int i = 0; i < 5; i++) {
    contador_sincronizado = 0;
    pthread_create(&h1, NULL, incrementador_sinc, NULL);
    pthread_create(&h2, NULL, decrementador_sinc, NULL);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    printf("Ejecución %d: Resultado final = %d (esperado: 0)\n", i + 1, contador_sincronizado);
  }

  return 0;
}
