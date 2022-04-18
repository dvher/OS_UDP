#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

// Waiting time between searches
#define SLEEP_TIME 500000

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
    // Array containing all possible locations of Zulensky
    pid_t possible_locations[n];
    // Create variable for Zulensky's location
    int location = rand() % n;
    // Fill array with zeros
    clean_array(possible_locations, n);
    // Create n subprocesses
    possible_locations[0] = fork();
    for(i = 1; i < n; i++) possible_locations[i] = possible_locations[i - 1] ? fork() : 0;
    
    // If current process is parent
    if(isParent(possible_locations, n)) {
        int j;
        int found = 0;
        // While Zulensky isn't found, keep iterating through the array
        // Multiple conditions are made to be sure that a child process won't access it from the inside.
        while(!found && isParent(possible_locations, n)) {
            j = rand() % n;
            printf("\nBuscando en locación %d...\n", possible_locations[j]);
            if(j == location) {
               found = 1;
               printf("Zulensky ha sido encontrado.\n");
               printf("Eliminando el resto de locaciones de la lista...\n");
               // Kill Zulensky's location
               kill(possible_locations[j], SIGKILL);
               // Allow all subprocesses to stop running on their own
               resumeProcess(possible_locations, n, j);
            } else {
               // Allow the process to keep living and get it out of the array
               kill(possible_locations[j], SIGCONT);
               possible_locations[j] = 0;
               printf("Zulensky no se encontraba en esta locación, relocalizando...\n");
               // Sleep until time is done to keep searching
               usleep(SLEEP_TIME);
               // Crete a new process for a possible new location
               possible_locations[j] = fork();
               if(!possible_locations[j]) kill(getpid(), SIGSTOP);
            }
        }
        // Wait  until every child is finished to stop running
        wait(NULL);
    } else
        // If the process is child, stop running until called for
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
