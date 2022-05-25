/*
* Bajo este modelo de sincronización se espera que
*
* Los escribas de los pergaminos se intercambien turnos cada vez que los estudiantes estén estudiando
*
* Los estudiantes leerán a ritmos aleatorios
*
* Siempre se mantendrá al menos un escriba en un pergamino
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

// Semaphore to check readers
sem_t students;
// Array of writers
pthread_t writers[2];
// Flag of writers
int writers_flag[2] = {0};
// The last used writer
int last = 0;
// Number of active readers
volatile int readercounter = 0;
// Array of readers
pthread_t *readers;

// Function to read for a random amount of time, effectively increasing and decreasing readercounter
void *reader(void *);
// Handle signals to free memory
void sigHandler(int);
// Handle exits to free memory
void exitHandler(int, void *);
// Writers' function
void *writer1(void *);
void *writer2(void *);

int main(int argc, char *argv[]) {

    // Handle signals and exit
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    signal(SIGQUIT, sigHandler);
    signal(SIGSEGV, sigHandler);
    on_exit(exitHandler, NULL);

    if(argc != 2) {
        fprintf(stderr, "Usage %s <num_readers>\n", argv[0]);
        exit(1);
    }

    // Variable for loops (C99)
    int i;    

    // Number of readers
    int n_readers = atoi(argv[1]);

    // Initialize semaphore
    sem_init(&students, 0, n_readers);

    pthread_create(&writers[0], NULL, writer1, NULL);
    pthread_create(&writers[1], NULL, writer2, NULL);

    readers = (pthread_t *) calloc(n_readers, sizeof(pthread_t));

    while(1) {
        usleep(500);
        printf(
            "\n1. Leer pergamino.\n"
            "2. Salir.\n\n"
            );
        int command;
        printf("Ingrese una opción: ");
        scanf("%d", &command);

        if(command == 1) {

            for(i = 0; i < n_readers; i++)
                pthread_create(&readers[i], NULL, reader, NULL);

            for(i = 0; i < n_readers; i++)
                pthread_join(readers[i], NULL);

        }else if(command == 2)
            break;
        else
            printf("Opción inválida.\n");
    }

    return 0;
}

void *reader(void *args) {

    srand(pthread_self() ^ time(NULL));
    sem_wait(&students);

    readercounter++;
    // There's atleast one reader on the critical section
    if(readercounter == 1)
        writers_flag[last] = 1;
    
    sleep(rand() % 2 + 2);
    readercounter--;
    
    // There's no readers left on the critical section
    if(readercounter == 0) {
        writers_flag[last] = 0;

        printf("\nEstudiantes terminaron de estudiar :D\n");

        if(!last)
            pthread_create(&writers[0], NULL, writer1, NULL);
        else
            pthread_create(&writers[1], NULL, writer2, NULL);

        last = !last; //The last writer
    }

    sem_post(&students);

    return args;
}

void sigHandler(int sig) {
    exit(sig);
}

void exitHandler(int status, void *arg) {
    if(arg != NULL)
        printf("%p\n", arg);
    if(readers != NULL)
        free(readers);
    sem_destroy(&students);
    printf("\nReceived status %d, cleaning...\n", status);
}

void *writer1(void *arg) {

    printf("Escriba 1 escribiendo...\n");

    while(!writers_flag[0]);

    printf("Escriba 1 dejó de escribir.\n");

    return arg;
}

void *writer2(void *arg) {

    printf("Escriba 2 escribiendo...\n");
    
    while(!writers_flag[1]);

    printf("Escriba 2 dejó de escribir.\n");  
          
    return arg;
}