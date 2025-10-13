#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define CLAVE_MSG 1234
#define MAX_MSG_SIZE 100

struct mi_msg {
  long mtype;
  char mtext[MAX_MSG_SIZE];
};

#endif
