#include "tictactoe.h"

void initialize_board(struct SharedBoard *board) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      board->board[i][j] = ' ';
    }
  }
  board->turn_count = 0;
  board->game_state = 'P';
}

void print_board(const struct SharedBoard *board) {
  printf("\n  0 1 2\n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    printf("%d %c|%c|%c\n", i, board->board[i][0], board->board[i][1], board->board[i][2]);
    if (i < BOARD_SIZE - 1) printf("  -----\n");
  }
  printf("\n");
}

int check_win(const struct SharedBoard *board, char player) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (board->board[i][0] == player && board->board[i][1] == player && board->board[i][2] == player) return 1;
    if (board->board[0][i] == player && board->board[1][i] == player && board->board[2][i] == player) return 1;
  }
  if (board->board[0][0] == player && board->board[1][1] == player && board->board[2][2] == player) return 1;
  if (board->board[0][2] == player && board->board[1][1] == player && board->board[2][0] == player) return 1;
  return 0;
}

void get_player_move(struct SharedBoard *board, char player) {
  int row, col;
  while (1) {
    printf("Jugador %c, introduce tu jugada (fila columna): ", player);
    if (scanf("%d %d", &row, &col) != 2) {
      // Limpiar buffer de entrada si la entrada no es válida
      while(getchar() != '\n');
      printf("Entrada inválida. Introduce dos números.\n");
      continue;
    }
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board->board[row][col] == ' ') {
      board->board[row][col] = player;
      break;
    }
    printf("Movimiento inválido. Inténtalo de nuevo.\n");
  }
}

void send_message(int msg_id, long type) {
  struct GameMessage msg;
  msg.mtype = type;
  if (msgsnd(msg_id, &msg, 0, 0) == -1) {
    perror("msgsnd");
    exit(1);
  }
}

void wait_for_message(int msg_id, long type) {
  struct GameMessage msg;
  if (msgrcv(msg_id, &msg, 0, type, 0) == -1) {
    // Si la cola fue eliminada por el otro jugador, salimos limpiamente.
    if(errno == EIDRM) {
        printf("El otro jugador ha terminado y limpiado los recursos.\n");
        exit(0);
    }
    perror("msgrcv");
    exit(1);
  }
}

int main() {
  key_t shm_key = ftok("/tmp", SHM_KEY);
  key_t msg_key = ftok("/tmp", MSG_KEY);

  int shm_id = shmget(shm_key, sizeof(struct SharedBoard), 0666);
  if (shm_id == -1) { perror("shmget"); exit(1); }

  struct SharedBoard *board = (struct SharedBoard *)shmat(shm_id, NULL, 0);
  if (board == (void *)-1) { perror("shmat"); exit(1); }

  int msg_id = msgget(msg_key, 0666);
  if (msg_id == -1) { perror("msgget"); exit(1); }
  
  printf("Juego de Tres en Raya. Eres el Jugador O.\n");

  while (1) {
    printf("Esperando al Jugador X...\n");
    wait_for_message(msg_id, P_O_TURN);

    if (board->game_state != 'P') break;

    print_board(board);
    get_player_move(board, 'O');
    board->turn_count++;

    if (check_win(board, 'O')) {
      board->game_state = 'O';
    } else if (board->turn_count == BOARD_SIZE * BOARD_SIZE) {
      board->game_state = 'D';
    }

    send_message(msg_id, (board->game_state == 'P') ? P_X_TURN : GAME_OVER);
    if (board->game_state != 'P') break;
  }

  print_board(board);
  if (board->game_state == 'O') printf("¡Ganaste!\n");
  else if (board->game_state == 'X') printf("Perdiste. Gana el Jugador X.\n");
  else printf("¡Es un empate!\n");

  shmdt(board);
  return 0;
}
