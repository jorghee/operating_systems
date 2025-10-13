#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>

#define SHM_KEY 6000
#define MSG_KEY 6001
#define BOARD_SIZE 3

// Tipos de mensaje para sincronización
#define P_X_TURN 1
#define P_O_TURN 2
#define GAME_OVER 3

// Estructura para la memoria compartida
struct SharedBoard {
  char board[BOARD_SIZE][BOARD_SIZE];
  int turn_count;
  char game_state; // 'P': Playing, 'X': X wins, 'O': O wins, 'D': Draw
};

// Estructura para la cola de mensajes
struct GameMessage {
  long mtype;
  char mtext[1]; // No necesitamos contenido, solo el tipo
};

void initialize_board(struct SharedBoard *board);
void print_board(const struct SharedBoard *board);
int check_win(const struct SharedBoard *board, char player);
void get_player_move(struct SharedBoard *board, char player);
void send_message(int msg_id, long type);
void wait_for_message(int msg_id, long type);

#endif
