#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

// Function to clean an array of PIDs
void clean_array(pid_t[], int);

int main(void) {
    srand(time(NULL));
    printf("PPID, PID: %d %d\n", getppid(), getpid());
    // Variable que contiene el número de locaciones
    int n = 0;
    // Create variable for iterations (C99)
    int i;
    // Ask for array's length
    printf("Ingrese el número de locaciones posibles de Zulensky: ");
    scanf("%d", &n);
    // Arreglo que contiene los PIDs de las locaciones posibles de Zulensky
    pid_t possible_locations[n];
    // Fill array with zeros
    clean_array(possible_locations, n);
    // Create n subprocesses
    possible_locations[0] = fork();
    for(i = 1; i < n; i++) possible_locations[i] = possible_locations[i - 1] ? fork() : 0;
    
    if(possible_locations[n - 1]) {
        int found = 0;
        while(!found) {
            
        }
    }
    if(getpid() != getppid()) exit(0);
    return 0;
}

void clean_array(pid_t arr[], int length) {
    int i;
    for(i = 0; i < length; i++) {
        arr[i] = 0;
    }
}