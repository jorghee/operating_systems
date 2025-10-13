#include "common.h"

// ID global para que el manejador de señales pueda acceder a él
int msg_id;

void cleanup(int signum) {
  (void)signum;
  printf("\nRecibida señal de terminación. Limpiando cola de mensajes...\n");
  if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
    perror("msgctl cleanup");
  }
  exit(EXIT_SUCCESS);
}

int main() {
  signal(SIGINT, cleanup);
  signal(SIGTERM, cleanup);

  // ftok para generar una clave única
  key_t key = ftok("/tmp", CLAVE_MSG);
  if (key == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  // Usamos IPC_CREAT para crear la cola si no existe
  msg_id = msgget(key, 0666 | IPC_CREAT);
  if (msg_id == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  printf("Cola de mensajes creada/obtenida. ID: %d\n", msg_id);
  printf("Receptor esperando mensajes...\n");

  struct mi_msg msg_recibido;
  // Esperamos un mensaje de tipo 1
  if (msgrcv(msg_id, &msg_recibido, sizeof(msg_recibido.mtext), 1, 0) == -1) {
    perror("msgrcv");
    // No salimos sin limpiar
    cleanup(0);
  }

  printf("Mensaje recibido: '%s'\n", msg_recibido.mtext);

  // Limpiar la cola de mensajes
  printf("Limpiando la cola de mensajes...\n");
  if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
    perror("msgctl");
    exit(EXIT_FAILURE);
  }

  printf("Cola de mensajes eliminada.\n");

  return 0;
}
