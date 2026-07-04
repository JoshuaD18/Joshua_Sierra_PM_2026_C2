#include <stdio.h>
#include <math.h>
void hanoi(FILE *archivo, int n, char ini, char fin, char ayu);
int main()
{
    int dis;
    do
    {
        printf("Cuantos discos tendra: ");
        scanf("%d", &dis);
        if(dis <= 0)
        {
        printf ("Error, esto no va servir \n");
    }
} while(dis <= 0);

FILE *archivo = fopen("Movimientos Torre de Hanoi.txt", "w");

fprintf(archivo, "\nMovimiento\n");
printf("\nMovimiento\n");
hanoi(archivo, dis, 'A','C','B');
int total_movimientos = (int)pow(2, dis) - 1;
fprintf(archivo, "Total de movimientos; %d\n", total_movimientos);
printf("Total de movimientos; %d\n", total_movimientos);
fclose(archivo);
return 0;
}
void hanoi(FILE *archivo, int n, char ini, char fin, char ayu){
    if(n == 1)
    {
        fprintf(archivo, "Mover disco 1 de %c a %c\n", ini, fin);
        printf("Mover disco 1 de %c a %c\n", ini, fin);
        return;
    }
hanoi(archivo, n - 1, ini, ayu, fin);
fprintf(archivo, "Mover disco %d de %c a %c\n", n, ini, fin);
printf("Mover disco %d de %c a %c\n", n, ini, fin);
hanoi(archivo, n - 1, ayu, fin, ini);
}

