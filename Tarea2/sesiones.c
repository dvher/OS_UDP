#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

// Store all data for a class
typedef struct {

    char nombre[64];
    int hora_duracion;
    int dia_limite_termino;
    int hora_limite_termino;

} asignatura;

// Function to add class to array
void *addToSchedule(void *);
// Function to clear heap
void clearClasses(int);
// Handle signals to avoid memory leaks, still may be possible with SIGKILL
void sigHandler(int);
// Handle exit to clear up residual memory
void exitHandler(int, void *);
// Calculate deadline based on day and hour
int calculateDeadline(asignatura *);
// Sort classes by deadline
void sortClassesByDeadline(void);

// Store all classes along with their data
asignatura **datos_asignaturas;
int length_classes = 0;

int main(void) {

    // Loop variable (C99)
    int i;

    // Handle signals and exit
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    signal(SIGQUIT, sigHandler);
    signal(SIGSEGV, sigHandler);
    on_exit(exitHandler, NULL);

    printf("Ingrese la cantidad de asignaturas: ");
    scanf("%d", &length_classes);

    // Initialize array with NULL values
    datos_asignaturas = (asignatura **) calloc(length_classes, sizeof(asignatura *));

    // Check if array was allocated, if not exit
    if(datos_asignaturas == NULL) {
        fprintf(stderr, "No se pudieron almacenar las asignaturas.\n");
        exit(1);
    }

    // Use threads to store all classes
    for(i = 0; i < length_classes; i++) {

        pthread_t new_thread;
        int ret_code = pthread_create(&new_thread, NULL, addToSchedule, (void *) &i);
        pthread_join(new_thread, NULL);
        if(ret_code != 0) {
            fprintf(stderr, "Received error code %d when creating thread.\n", ret_code);
            exit(ret_code);
        }

    }

    clearClasses(-1);

    return 0;

}

void *addToSchedule(void *class) {

    int i = *(int *) class;
    datos_asignaturas[i] = malloc(sizeof(asignatura *));

    // Clear trailing \n character
    getchar();

    printf("\n");

    printf("Ingrese el nombre de la asignatura: ");
    fgets(datos_asignaturas[i]->nombre, 63, stdin);

    //Clear input
    while(getchar() != '\n'){
        printf("Logrado.\n");
    };

    exit(10);

    printf("Ingrese la duración total (en horas) del estudio semanal de la asignatura: ");
    scanf("%d", &datos_asignaturas[i]->hora_duracion);

    while(datos_asignaturas[i]->hora_duracion > 24*7 || datos_asignaturas[i]->hora_duracion <= 0) {
        printf("Por favor ingrese un número entre 1 y %d: ", 24*7);
        scanf("%d", &datos_asignaturas[i]->hora_duracion);
    }

    printf("Ingrese el día límite para terminar el estudio de esta asignatura: ");
    scanf("%d", &datos_asignaturas[i]->dia_limite_termino);

    while(datos_asignaturas[i]->dia_limite_termino < 0 || datos_asignaturas[i]->dia_limite_termino > 6) {
        printf("Por favor ingrese un número entre 0 y 6: ");
        scanf("%d", &datos_asignaturas[i]->dia_limite_termino);
    }

    printf("Ingrese la hora límite de término para el estudio de la asignatura: ");
    scanf("%d", &datos_asignaturas[i]->hora_limite_termino);

    while(datos_asignaturas[i]->hora_limite_termino > 23 || datos_asignaturas[i]->hora_limite_termino < 0) {
        printf("Por favor ingrese un número entre 0 y 23: ");
        scanf("%d", &datos_asignaturas[i]->hora_limite_termino);
    }

    return NULL;
}

void clearClasses(int class_to_remove) {
    if(datos_asignaturas == NULL)
        return;
    if(class_to_remove < 0) {
        int i;
        for(i = 0; i < length_classes && datos_asignaturas[i] != NULL; i++) {
            free(datos_asignaturas[i]);
            datos_asignaturas[i] = NULL;
        }
        free(datos_asignaturas);
        datos_asignaturas = NULL;
    }else if(class_to_remove > length_classes)
        fprintf(stderr, "Cannot free space unoccupied.\n");
    else{
        if(datos_asignaturas[class_to_remove] != NULL)
            free(datos_asignaturas[class_to_remove]);
        fprintf(stderr, "Cannot free space unoccupied.\n");
    }
}

void sigHandler(int sig) {
    printf("\nReceived signal n°%d, cleaning...\n", sig);
    exit(sig);
}

void exitHandler(int status, void *arg) {
    if(arg != NULL) {
        printf("Argument: %p\n", arg);
    }
    printf("\nExiting with status %d\n", status);
    clearClasses(-1);

}

int calculateDeadline(asignatura *asign) {
    if(asign == NULL)
        return -1;
    return 24 * asign->dia_limite_termino + asign->hora_limite_termino;
}

void sortClassesByDeadline(void) {

    int i, j;

    for(i = 0; i < length_classes; i++) {
        for(j = 0; i < length_classes - i - 1; j++) {
            if(calculateDeadline(datos_asignaturas[j]) > calculateDeadline(datos_asignaturas[j+1])) {
                asignatura *aux = datos_asignaturas[j];
                datos_asignaturas[j] = datos_asignaturas[j+1];
                datos_asignaturas[j+1] = aux;
            }
        }
    }

}