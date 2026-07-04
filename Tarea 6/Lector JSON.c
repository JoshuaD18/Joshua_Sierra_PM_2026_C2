#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[100];
} Alumno;

int contar_elementos_json(const char *nombre_archivo) {
    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return 0;

    int cont = 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), f)) {
        if (strstr(linea, "}")) {
            cont++;
        }
    }
    fclose(f);
    return cont;
}

void cargar_datos_json(const char *nombre_archivo, Alumno *array, int n) {
    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return;

    char linea[256];
    int i = -1;

    while (fgets(linea, sizeof(linea), f)) {
        if (strstr(linea, "{")) {
            i++;
            continue;
        }
        if (i >= 0 && i < n) {
            char *pos;
            if ((pos = strstr(linea, "\"nombre\""))) {
                sscanf(pos, "\"nombre\": \"%[^\"]\"", array[i].nombre);
            } else if ((pos = strstr(linea, "\"apellido\""))) {
                sscanf(pos, "\"apellido\": \"%[^\"]\"", array[i].apellido);
            } else if ((pos = strstr(linea, "\"promedio\""))) {
                sscanf(pos, "\"promedio\": %f", &array[i].promedio);
            } else if ((pos = strstr(linea, "\"materia\""))) {
                sscanf(pos, "\"materia\": \"%[^\"]\"", array[i].materia);
            }
        }
    }
    fclose(f);
}

int main(void) {
    const char *archivo = "alumnos.json";
    int n = contar_elementos_json(archivo);

    if (n == 0) {
        printf("No se encontraron elementos o el archivo no existe.\n");
        return 1;
    }

    printf("Cantidad de alumnos: %d\n", n);

    Alumno *alumnos = (Alumno *)malloc(n * sizeof(Alumno));
    if (!alumnos) {
        printf("Error de asignacion de memoria.\n");
        return 1;
    }

    cargar_datos_json(archivo, alumnos, n);

    printf(" LISTA DE ALUMNOS DESDE JSON \n");
    for (int i = 0; i < n; i++) {
        printf("Alumno %d:\n", i + 1);
        printf("  Nombre: %s %s\n", alumnos[i].nombre, alumnos[i].apellido);
        printf("  Materia: %s\n", alumnos[i].materia);
        printf("  Promedio: %.1f\n\n", alumnos[i].promedio);
    }

    free(alumnos);
    return 0;
}
