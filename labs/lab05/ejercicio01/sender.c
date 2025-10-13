#include "common.h"

int main() {
  // ftok para generar la misma clave que el receptor
  key_t key = ftok("/tmp", CLAVE_MSG);
  if (key == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  // Obtener la cola de mensajes (no crearla)
  int msg_id = msgget(key, 0666);
  if (msg_id == -1) {
    perror("msgget - asegúrate de que el receptor se está ejecutando");
    exit(EXIT_FAILURE);
  }

  // Preparar y enviar el mensaje
  struct mi_msg msg_enviado;
  msg_enviado.mtype = 1; // Tipo de mensaje
  strncpy(msg_enviado.mtext, "Hola desde el proceso emisor!", MAX_MSG_SIZE - 1);
  msg_enviado.mtext[MAX_MSG_SIZE - 1] = '\0';

  if (msgsnd(msg_id, &msg_enviado, sizeof(msg_enviado.mtext), 0) == -1) {
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }

  printf("Mensaje enviado con éxito.\n");

  return 0;
}
