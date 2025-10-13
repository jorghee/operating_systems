#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#define CLAVE_MSG 5678
#define MAX_MSG_SIZE 100

struct mi_msg {
  long mtype;
  char mtext[MAX_MSG_SIZE];
};

int main() {
  key_t key = ftok("/tmp", CLAVE_MSG);
  if (key == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  // Crear una nueva cola de mensajes
  int msg_id = msgget(key, 0666 | IPC_CREAT);
  if (msg_id == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  printf("Cola de mensajes creada con ID: %d\n", msg_id);

  printf("Intentando recibir mensaje con IPC_NOWAIT (la cola está vacía)...\n");
  struct mi_msg msg;

  // Intento de recepción no bloqueante. La cola está vacía, por lo que debe fallar.
  if (msgrcv(msg_id, &msg, sizeof(msg.mtext), 1, IPC_NOWAIT) == -1) {
    // Comprobamos el error esperado
    if (errno == ENOMSG) {
      printf("Éxito: msgrcv falló con errno = ENOMSG, como se esperaba.\n");
      printf("Descripción del error: %s\n", strerror(errno));
    } else {
      // Otro error inesperado
      perror("msgrcv falló con un error inesperado");
    }
  } else {
    printf("Error: msgrcv tuvo éxito cuando debería haber fallado.\n");
  }
  
  printf("\nAhora intentando una lectura bloqueante (el programa se colgará)...\n");
  printf("Presiona Ctrl+C para terminar.\n");

  // Intento de recepción bloqueante para mostrar la diferencia.
  // Esta línea nunca retornará si no se envía un mensaje.
  if (msgrcv(msg_id, &msg, sizeof(msg.mtext), 1, 0) == -1) {
      perror("msgrcv bloqueante");
  }

  // Limpieza (este código no se alcanzará en el flujo normal)
  if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
    perror("msgctl");
  }

  return 0;
}
