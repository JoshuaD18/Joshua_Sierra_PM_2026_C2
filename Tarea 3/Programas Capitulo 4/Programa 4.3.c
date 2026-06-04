#include <stdio.h>

void f1 (void);
int K_Global = 5;

void main(void)
{
    int I;
    for (I = 1; I <= 3; I++)
        f1();
}
void f1(void)

{
    int K = 2;
    K += K;
    printf("\n\nEl valor de la variable local es:   %d", K);
    K_Global = K_Global + K;
    printf("\nEl valor de la variable global es: %d", K_Global);
}

/* Profesor, hice un cambio con ::K a K_Global, porque los ::
no me funcionaban, investigue y segun lo que vi funcionan solo en c++ */
