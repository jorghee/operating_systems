#define _XOPEN_SOURCE 700
#include "tictactoe.h"

int g_shm_id;
int g_msg_id;
struct SharedBoard *g_board;

void cleanup_resources(int signum) {
  (void)signum;
  printf("\nTerminación detectada (Jugador X). Limpiando recursos...\n");
  if (g_board) shmdt(g_board);
  shmctl(g_shm_id, IPC_RMID, NULL);
  msgctl(g_msg_id, IPC_RMID, NULL);
  exit(0);
}

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
  signal(SIGINT, cleanup_resources);
  signal(SIGTERM, cleanup_resources);

  key_t shm_key = ftok("/tmp", SHM_KEY);
  key_t msg_key = ftok("/tmp", MSG_KEY);

  g_shm_id = shmget(shm_key, sizeof(struct SharedBoard), 0666 | IPC_CREAT);
  if (g_shm_id == -1) { perror("shmget"); exit(1); }

  g_board = (struct SharedBoard *)shmat(g_shm_id, NULL, 0);
  if (g_board == (void *)-1) { perror("shmat"); exit(1); }

  g_msg_id = msgget(msg_key, 0666 | IPC_CREAT);
  if (g_msg_id == -1) { perror("msgget"); exit(1); }

  initialize_board(g_board);
  printf("Juego de Tres en Raya. Eres el Jugador X.\n");
  printf("Esperando que el Jugador O se una...\n");

  while (g_board->game_state == 'P') {
    print_board(g_board);
    get_player_move(g_board, 'X');
    g_board->turn_count++;

    if (check_win(g_board, 'X')) {
      g_board->game_state = 'X';
    } else if (g_board->turn_count == BOARD_SIZE * BOARD_SIZE) {
      g_board->game_state = 'D';
    }
    
    send_message(g_msg_id, (g_board->game_state == 'P') ? P_O_TURN : GAME_OVER);
    if (g_board->game_state != 'P') break;
    
    printf("Esperando al Jugador O...\n");
    wait_for_message(g_msg_id, P_X_TURN);
  }

  print_board(g_board);
  if (g_board->game_state == 'X') printf("¡Ganaste!\n");
  else if (g_board->game_state == 'O') printf("Perdiste. Gana el Jugador O.\n");
  else printf("¡Es un empate!\n");

  cleanup_resources(0);
  return 0;
}
