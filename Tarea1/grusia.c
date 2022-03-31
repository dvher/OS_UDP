#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

// Function to clean an array of PIDs
void clean_array(pid_t[], int);
// Function to check if the process is the parent
int isParent(pid_t[], int);
// Function to resume the process
void resumeProcess(pid_t[], int, int);

int main(void) {
    srand(time(NULL));
    // Number of locations
    int n = 0;
    // Create variable for iterations (C99)
    int i;
    // Ask for array's length
    printf("Ingrese el número de locaciones posibles de Zulensky: ");
    scanf("%d", &n);
    // Zulensky's location
    int location = rand() % n;
    // Array containing all possible locations of Zulensky
    pid_t possible_locations[n];
    // Fill array with zeros
    clean_array(possible_locations, n);
    // Create n subprocesses
    possible_locations[0] = fork();
    for(i = 1; i < n; i++) possible_locations[i] = possible_locations[i - 1] ? fork() : 0;
    
    if(isParent(possible_locations, n)) {
        int found = 0;
        int j = 0;
        while(!found) {
            if(j == location) {
               found = 1;
               printf("Zulensky ha sido encontrado.\n");
               printf("Eliminando el resto de locaciones de la lista...\n");
               kill(possible_locations[j], SIGKILL);
               resumeProcess(possible_locations, n, j);
            } else {
               kill(possible_locations[j], SIGINT);
               possible_locations[j] = 0;
               //printf("Zulensky no se encontraba en esta locación, relocalizando...\n");
               possible_locations[j] = fork();
            }
            j = (j + 1) % n;
        }
    } else
        kill(getpid(), SIGSTOP);
    return 0;
}

void clean_array(pid_t arr[], int length) {
    int i;
    for(i = 0; i < length; i++) {
        arr[i] = 0;
    }
}

int isParent(pid_t arr[], int length) {
    int i;
    for(i = 0; i < length; i++)
        if(!arr[i]) return 0;
    return 1;
}

void resumeProcess(pid_t arr[], int length, int dead) {
    int i;
    for(i = 0; i < length; i++) {
        if(i != dead) kill(arr[i], SIGCONT);
    }
}
