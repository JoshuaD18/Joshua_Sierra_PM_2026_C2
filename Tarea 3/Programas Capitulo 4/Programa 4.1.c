#include <stdio.h>
/* Cubo - 1.*/

int cubo(void);
int I;

void main(void)
{
int CUB;
for (I = 1; I <= 10; I++)
{
    CUB = cubo();
    printf("\nEl cubo de %d es:    %d", I, CUB);
}
}

int cubo(void)
/*  La funcion calcula el cubo de la variable global I. */
{
    return (I*I*I);
}
