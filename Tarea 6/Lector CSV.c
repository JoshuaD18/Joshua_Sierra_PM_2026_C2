#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[100];
} Alumno;

int contar_elementos_csv(const char *nombre_archivo) {
    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return 0;

    int cont = 0;
    char linea[256];

    if (fgets(linea, sizeof(linea), f)) {
        while (fgets(linea, sizeof(linea), f)) {
            if (strlen(linea) > 3) {
                cont++;
            }
        }
    }
    fclose(f);
    return cont;
}

void cargar_datos_csv(const char *nombre_archivo, Alumno *array, int n) {
    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return;

    char linea[256];
    fgets(linea, sizeof(linea), f);

    for (int i = 0; i < n; i++) {
        if (fgets(linea, sizeof(linea), f)) {
            linea[strcspn(linea, "\r\n")] = 0;

            char *token = strtok(linea, ",");
            if (token) strcpy(array[i].nombre, token);

            token = strtok(NULL, ",");
            if (token) strcpy(array[i].apellido, token);

            token = strtok(NULL, ",");
            if (token) array[i].promedio = (float)atof(token);

            token = strtok(NULL, ",");
            if (token) strcpy(array[i].materia, token);
        }
    }
    fclose(f);
}

int main(void) {
    const char *archivo = "alumnos.csv";
    int n = contar_elementos_csv(archivo);

    if (n == 0) {
        printf("No se encontro el archivo.\n");
        return 1;
    }

    printf("Cantidad de alumnos detectados en CSV: %d\n", n);

    Alumno *alumnos = (Alumno *)malloc(n * sizeof(Alumno));
    if (!alumnos) {
        printf("Error de asignacion de memoria.\n");
        return 1;
    }

    cargar_datos_csv(archivo, alumnos, n);

    printf("LISTA DE ALUMNOS DESDE CSV\n");
    for (int i = 0; i < n; i++) {
        printf("Alumno %d:\n", i + 1);
        printf("  Nombre: %s %s\n", alumnos[i].nombre, alumnos[i].apellido);
        printf("  Materia: %s\n", alumnos[i].materia);
        printf("  Promedio: %.1f\n\n", alumnos[i].promedio);
    }

    free(alumnos);
    return 0;
}
