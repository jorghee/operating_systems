CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: ex1 ex2 ex3

ex1: sender receiver
ex2: nowait_example
ex3: player_x player_o

# --- Ejercicio 1 ---
sender: ejercicio1/sender.c ejercicio1/common.h
	$(CC) $(CFLAGS) -o sender ejercicio1/sender.c

receiver: ejercicio1/receiver.c ejercicio1/common.h
	$(CC) $(CFLAGS) -o receiver ejercicio1/receiver.c

# --- Ejercicio 2 ---
nowait_example: ejercicio2/nowait_example.c
	$(CC) $(CFLAGS) -o nowait_example ejercicio2/nowait_example.c

# --- Ejercicio 3 ---
player_x: ejercicio3/player_x.c ejercicio3/tictactoe.h
	$(CC) $(CFLAGS) -o player_x ejercicio3/player_x.c

player_o: ejercicio3/player_o.c ejercicio3/tictactoe.h
	$(CC) $(CFLAGS) -o player_o ejercicio3/player_o.c

# --- Limpieza ---
clean:
	rm -f sender receiver nowait_example player_x player_o

.PHONY: all clean ex1 ex2 ex3
