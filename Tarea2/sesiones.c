#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {

    int id;
    int hora_duracion;
    int dia_limite_termino;
    int hora_limite_termino;

} asignatura;

int main(void) {

    int i;

    // Number of asignatures
    int K;

    printf("Ingrese la cantidad de asignaturas: ");
    scanf("%d", &K);

    asignatura datos_asignaturas[K];

    for(i = 0; i < K; i++) {

        printf("Ingrese el id de la asignatura %d: ", i+1);
        scanf("%d", &datos_asignaturas[i].id);

        printf("Ingrese la duración total (en horas) del estudio semanal de la asignatura: ");
        scanf("%d", &datos_asignaturas[i].hora_duracion);

        printf("Ingrese el día límite para terminar el estudio de esta asignatura: ");
        scanf("%d", &datos_asignaturas[i].dia_limite_termino);

        printf("Ingrese la hora límite de término para el estudio de la asignatura: ");
        scanf("%d", &datos_asignaturas[i].hora_limite_termino);

    }

    return 0;

}