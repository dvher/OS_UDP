#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

int check_winner(int player1, int player2) {
    // 1 -> Rock
    // 2 -> Paper
    // 3 -> Scissors
    if(player1 == player2) return 0;

    if(player1 == 1 && player2 == 2) return 2;
    if(player1 == 1 && player2 == 3) return 1;

    if(player1 == 2 && player2 == 1) return 1;
    if(player1 == 2 && player2 == 3) return 2;

    if(player1 == 3 && player2 == 1) return 2;
    if(player1 == 3 && player2 == 2) return 1;

    return 0;
}

char *get_player_choice(int choice) {
    switch(choice) {
        case 1: return "Rock";
        case 2: return "Paper";
        case 3: return "Scissors";
        default: return "Invalid choice";
    }
}

int main(void) {
    // Get the pid of the parent process
    pid_t pid = getpid();

    // Create variables to store both players' pipes
    int fds[2][2];

    // Create variables to hold players' names
    char nombre_jugadores[2][20];

    // Create variables to hold players' PIDs
    pid_t jugadores[2] = {0};

    // Create boolean to check if the user no longer wants to keep playing
    int continue_game = 1;

    printf("Bienvenido al campeonado de Cachipún!\n");
    printf("Hoy el árbitro será Juan, también conocido por su id %d!\n", pid);

    // Request players' names
    printf("Ingrese el nombre del primer jugador: ");
    scanf("%s", nombre_jugadores[0]);

    printf("Ingrese el nombre del segundo jugador: ");
    scanf("%s", nombre_jugadores[1]);
    // Create pipes
    if(pipe(fds[0]) == -1 || pipe(fds[1]) == -1) {
        perror("Error al crear los pipes");
        exit(1);
    }
    // Store players' PIDs
    jugadores[0] = fork();
    jugadores[1] = jugadores[0] ? fork() : 0;
    srand(getpid());
    // Main loop
    while(continue_game) {
        if (jugadores[0] && jugadores[1]) {
            // Close pipes
            if(fds[0][1]) {
                close(fds[0][1]);
                fds[0][1] = 0;
            }
            if(fds[1][1]) {
                close(fds[1][1]);
                fds[1][1] = 0;
            }
            printf("Esperando los movimientos de los jugadores...\n");
            int choice1 = 0, choice2 = 0;
            // Read players' choices
            while(!read(fds[0][0], &choice1, sizeof(int)));
            while(!read(fds[1][0], &choice2, sizeof(int)));
            printf("%s eligió %s\n", nombre_jugadores[0], get_player_choice(choice1));
            printf("%s eligió %s\n", nombre_jugadores[1], get_player_choice(choice2));
            // Check winner
            int winner = check_winner(choice1, choice2);
            int loser = winner == 1 ? 2 : 1;
            if(!winner) {
                printf("Empate! Ambos jugadores sobreviven\n");
            } else {
                printf("El ganador es %s!\n", nombre_jugadores[winner - 1]);
                // Kill loser player
                close(fds[loser -1][0]);
                kill(jugadores[loser - 1], SIGKILL);
                kill(jugadores[winner - 1], SIGCONT);
                jugadores[loser - 1] = 0;
            }
            // Ask if the user wants to keep playing
            printf("Desea seguir jugando? (1 para sí, 0 para no): ");
            scanf("%d", &continue_game);
            if(!continue_game) break;
            // Restart players
            pipe(fds[loser - 1]);
            if(winner) jugadores[loser - 1] = fork();
        }else {
            int choice = rand() % 3 + 1;
            // Close pipes
            close(fds[0][0]);
            close(fds[1][0]);

            //Write choice from player 1
            if(!jugadores[0]) {
                write(fds[1][1], &choice, sizeof(int));
            //Write choice from player 2h
            }else if(!jugadores[1]) {
                write(fds[0][1], &choice, sizeof(int));
            }
            kill(getpid(), SIGSTOP);
            printf("Continuo!!\n");
        }
    }
    return 0;
}