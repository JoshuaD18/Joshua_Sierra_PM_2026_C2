#include <stdio.h>

    /*Ejercicio 1
    El programa, al recibir como el promedio de un alumno, escribe
    aprobado si su promedio es mayor o igual a 6.

    PRO: variable de tipo real. */

void main(void)
    {
    float PRO;
    printf("Ingrese el promedio del alumno: ");
    scanf("%f", &PRO);
    if (PRO >= 6)
        printf("\nAprobado");
}
