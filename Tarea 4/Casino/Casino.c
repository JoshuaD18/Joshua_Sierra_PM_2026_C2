#include <stdio.h>
#include <stdlib.h>

int main()

{
int opcion;
int jugadas = 0;
int apuestas [5];
int dinero[5];
int numero_ganador;
int ganador = 0;
int randomizador = 18;
int i;

do
{
    randomizador = randomizador + opcion + 7;
    printf("\nCasino\n");
    printf("1. Jugar\n");
    printf("2. Elegir cantidad de jugadas\n");
    printf("3 Salir\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch(opcion)
    {
    case 1:
        if (jugadas > 0)
        {
            ganador = (randomizador % 40) + 1;

            for (i = 0; i < jugadas; i++)
            {
                printf("\nJugada %d\n", i + 1);
                do
                {
                printf("Juegue un numero del 1 al 40: ");
                scanf("%d", &apuestas[i]);

                if (apuestas[i] < 1 || apuestas[i] > 40)
                {
                    printf("Tiene que ser un numero entre 1 y 40.\n");
                }
                }
                while (apuestas[i] < 1 || apuestas[i] > 40);

                printf("Cantidad a apostar: ");
                scanf("%d", &dinero[i]);

                randomizador = randomizador + apuestas[i];
            }

            printf("\nY el numero ganador es\n");
            printf("Numero ganador: %d\n", ganador);

            numero_ganador = 0;

            for (i = 0; i < jugadas; i++)
            {
                if(apuestas[i] == ganador)
                {
                    printf("\nHa salido su numero jugado\n");
                    printf("Dinero apostado: %d | Premio: %d\n", dinero[i], dinero[i] * 1000);
                    numero_ganador = 1;
                }
            }
            if (numero_ganador == 0)
            {
                printf("\nNinguno de los numeros seleccionados es el ganador. Suerte la proxima\n");
            }
        }
        else
        {
            printf("Debes elegir primero la cantidad de jugadas en la opcion 2.\n");
        }
        break;

    case 2:
        printf("\nCuantas jugadas quiere realizar? ");
        scanf("%d", &jugadas);


        if (jugadas > 5 || jugadas <= 0)
        {
            printf("El numero de jugadas debe ser entre 1 y 5\n");
            jugadas = 0;
        }
        else
        {
        printf("Bien, se registraron %d jugadas.\n", jugadas);
        }
        break;
    }
}
while (opcion != 3);

return 0;
}
