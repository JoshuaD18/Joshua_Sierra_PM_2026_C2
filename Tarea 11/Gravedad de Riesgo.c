#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXTO 100
#define MAX_FILENAME 150

typedef struct {
    int id;
    char bien[MAX_TEXTO];
    char riesgo[MAX_TEXTO];
    char dano[MAX_TEXTO];

    int F;
    int S;
    int P;
    int E;
    int A;
    int B;

    int I;
    int D;
    int C;
    int Pb;
    int ER;
    char clasificacion[MAX_TEXTO];
} RiesgoMosler;

void calcularRiesgo(RiesgoMosler *r);
void pedirTexto(const char *mensaje, char *buffer, int maxLen);
int pedirValor(const char *mensaje, int min, int max);

void crearRiesgo(RiesgoMosler **lista, int *cantidad);
void modificarRiesgo(RiesgoMosler *lista, int cantidad);
void borrarRiesgo(RiesgoMosler **lista, int *cantidad);
void imprimirRiesgos(const RiesgoMosler *lista, int cantidad);

void guardarEnBinario(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo);
void cargarDesdeBinario(RiesgoMosler **lista, int *cantidad, const char *nombreArchivo);
void guardarEnJSON(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo);
void guardarEnXML(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo);

void liberarMemoria(RiesgoMosler **lista, int *cantidad);

int main() {
    RiesgoMosler *listaRiesgos = NULL;
    int cantidadRiesgos = 0;
    int opcion;
    char archivoActual[MAX_FILENAME] = "riesgos_mosler.dat";

    do {
        printf("\n============================================\n");
        printf("   SISTEMA DE GESTION DE RIESGOS - MOSLER   \n");
        printf("============================================\n");
        printf("Archivo Binario Actual: [%s]\n", archivoActual);
        printf("Riesgos en Memoria Dinamica: %d\n", cantidadRiesgos);
        printf("--------------------------------------------\n");
        printf("1. Crear nuevo riesgo\n");
        printf("2. Modificar parametros de un riesgo\n");
        printf("3. Recalcular todos los riesgos\n");
        printf("4. Imprimir riesgos en pantalla\n");
        printf("5. Borrar un riesgo\n");
        printf("6. Seleccionar/Cambiar nombre de archivo binario\n");
        printf("7. Cargar desde Archivo Binario (.dat)\n");
        printf("8. Guardar en Archivo Binario (.dat)\n");
        printf("9. Exportar a JSON\n");
        printf("10. Exportar a XML\n");
        printf("11. Salir\n");
        printf("--------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                crearRiesgo(&listaRiesgos, &cantidadRiesgos);
                break;
            case 2:
                modificarRiesgo(listaRiesgos, cantidadRiesgos);
                break;
            case 3:
                for (int i = 0; i < cantidadRiesgos; i++) {
                    calcularRiesgo(&listaRiesgos[i]);
                }
                printf("\n[SUCCESS] Todos los riesgos han sido recalculados correctamente.\n");
                break;
            case 4:
                imprimirRiesgos(listaRiesgos, cantidadRiesgos);
                break;
            case 5:
                borrarRiesgo(&listaRiesgos, &cantidadRiesgos);
                break;
            case 6:
                pedirTexto("Ingrese el nombre del archivo binario (ej: datos.dat): ", archivoActual, MAX_FILENAME);
                printf("[INFO] Archivo objetivo actualizado a: %s\n", archivoActual);
                break;
            case 7:
                cargarDesdeBinario(&listaRiesgos, &cantidadRiesgos, archivoActual);
                break;
            case 8:
                guardarEnBinario(listaRiesgos, cantidadRiesgos, archivoActual);
                break;
            case 9: {
                char archivoJSON[MAX_FILENAME];
                pedirTexto("Ingrese el nombre del archivo JSON (ej: riesgos.json): ", archivoJSON, MAX_FILENAME);
                guardarEnJSON(listaRiesgos, cantidadRiesgos, archivoJSON);
                break;
            }
            case 10: {
                char archivoXML[MAX_FILENAME];
                pedirTexto("Ingrese el nombre del archivo XML (ej: riesgos.xml): ", archivoXML, MAX_FILENAME);
                guardarEnXML(listaRiesgos, cantidadRiesgos, archivoXML);
                break;
            }
            case 11:
                printf("\nLiberando recursos de memoria y saliendo...\n");
                liberarMemoria(&listaRiesgos, &cantidadRiesgos);
                break;
            default:
                printf("\n[ERROR] Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 11);

    return 0;
}

void pedirTexto(const char *mensaje, char *buffer, int maxLen) {
    printf("%s", mensaje);
    fgets(buffer, maxLen, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

int pedirValor(const char *mensaje, int min, int max) {
    int valor;
    do {
        printf("%s (%d-%d): ", mensaje, min, max);
        if (scanf("%d", &valor) != 1) {
            while (getchar() != '\n');
            valor = -1;
        }
    } while (valor < min || valor > max);
    getchar();
    return valor;
}

void calcularRiesgo(RiesgoMosler *r) {
    r->I = r->F * r->S;
    r->D = r->P * r->E;
    r->C = r->I + r->D;
    r->Pb = r->A * r->B;
    r->ER = r->C * r->Pb;

    if (r->ER <= 250) {
        strcpy(r->clasificacion, "Muy Bajo");
    } else if (r->ER <= 500) {
        strcpy(r->clasificacion, "Pequeno");
    } else if (r->ER <= 750) {
        strcpy(r->clasificacion, "Medio");
    } else if (r->ER <= 1000) {
        strcpy(r->clasificacion, "Elevado");
    } else {
        strcpy(r->clasificacion, "Critico");
    }
}

void crearRiesgo(RiesgoMosler **lista, int *cantidad) {
    RiesgoMosler *temp = realloc(*lista, (*cantidad + 1) * sizeof(RiesgoMosler));
    if (temp == NULL) {
        printf("\n[ERROR] Error al asignar memoria dinamica.\n");
        return;
    }
    *lista = temp;

    RiesgoMosler *nuevo = &((*lista)[*cantidad]);
    nuevo->id = *cantidad + 1;

    printf("\n--- NUEVO RIESGO (FASE 1) ---\n");
    pedirTexto("Ingrese el Activo/Bien: ", nuevo->bien, MAX_TEXTO);
    pedirTexto("Ingrese la Amenaza/Riesgo: ", nuevo->riesgo, MAX_TEXTO);
    pedirTexto("Ingrese la descripcion del Dano: ", nuevo->dano, MAX_TEXTO);

    printf("\n--- CRITERIOS FASE 2 (Escala 1 a 5) ---\n");
    nuevo->F = pedirValor("1. Criterio de Funcion (F)", 1, 5);
    nuevo->S = pedirValor("2. Criterio de Sustitucion (S)", 1, 5);
    nuevo->P = pedirValor("3. Criterio de Profundidad (P)", 1, 5);
    nuevo->E = pedirValor("4. Criterio de Extension (E)", 1, 5);
    nuevo->A = pedirValor("5. Criterio de Agresion (A)", 1, 5);
    nuevo->B = pedirValor("6. Criterio de Vulnerabilidad (B)", 1, 5);

    calcularRiesgo(nuevo);
    (*cantidad)++;
    printf("\n[SUCCESS] Riesgo ID #%d creado correctamente.\n", nuevo->id);
}

void modificarRiesgo(RiesgoMosler *lista, int cantidad) {
    if (cantidad == 0) {
        printf("\n[INFO] No hay riesgos para modificar.\n");
        return;
    }

    int id = pedirValor("\nIngrese el ID a modificar", 1, cantidad);
    RiesgoMosler *r = &lista[id - 1];

    printf("\nModificando Riesgo ID #%d:\n", r->id);
    pedirTexto("Nuevo Activo/Bien: ", r->bien, MAX_TEXTO);
    pedirTexto("Nueva Amenaza/Riesgo: ", r->riesgo, MAX_TEXTO);
    pedirTexto("Nuevo Dano: ", r->dano, MAX_TEXTO);

    printf("\nNuevos Criterios (1-5):\n");
    r->F = pedirValor("Criterio F", 1, 5);
    r->S = pedirValor("Criterio S", 1, 5);
    r->P = pedirValor("Criterio P", 1, 5);
    r->E = pedirValor("Criterio E", 1, 5);
    r->A = pedirValor("Criterio A", 1, 5);
    r->B = pedirValor("Criterio B", 1, 5);

    calcularRiesgo(r);
    printf("\n[SUCCESS] Riesgo ID #%d actualizado correctamente.\n", r->id);
}

void borrarRiesgo(RiesgoMosler **lista, int *cantidad) {
    if (*cantidad == 0) {
        printf("\n[INFO] No hay riesgos guardados para borrar.\n");
        return;
    }

    int id = pedirValor("\nIngrese el ID a borrar", 1, *cantidad);
    int indice = id - 1;

    for (int i = indice; i < (*cantidad) - 1; i++) {
        (*lista)[i] = (*lista)[i + 1];
        (*lista)[i].id = i + 1;
    }

    (*cantidad)--;

    if (*cantidad > 0) {
        RiesgoMosler *temp = realloc(*lista, (*cantidad) * sizeof(RiesgoMosler));
        if (temp != NULL) *lista = temp;
    } else {
        free(*lista);
        *lista = NULL;
    }

    printf("\n[SUCCESS] Riesgo ID #%d eliminado con exito.\n", id);
}

void imprimirRiesgos(const RiesgoMosler *lista, int cantidad) {
    if (cantidad == 0) {
        printf("\n[INFO] No existen registros cargados en memoria.\n");
        return;
    }

    printf("\n=========================================================================================\n");
    printf("                                REPORTES DE RIESGOS - MOSLER                             \n");
    printf("=========================================================================================\n");
    printf("ID | Bien/Activo       | Riesgo          | F S P E A B |  C  | Pb |   ER  | Clasificacion \n");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < cantidad; i++) {
        const RiesgoMosler *r = &lista[i];
        printf("%-2d | %-17.17s | %-15.15s | %d %d %d %d %d %d | %3d | %2d | %5d | %-12s\n",
               r->id, r->bien, r->riesgo, r->F, r->S, r->P, r->E, r->A, r->B, r->C, r->Pb, r->ER, r->clasificacion);
    }
    printf("=========================================================================================\n");
}

void liberarMemoria(RiesgoMosler **lista, int *cantidad) {
    if (*lista != NULL) {
        free(*lista);
        *lista = NULL;
    }
    *cantidad = 0;
}

void guardarEnBinario(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL) {
        printf("\n[ERROR] No se pudo abrir el archivo binario para escritura.\n");
        return;
    }

    fwrite(&cantidad, sizeof(int), 1, archivo);
    if (cantidad > 0) {
        fwrite(lista, sizeof(RiesgoMosler), cantidad, archivo);
    }

    fclose(archivo);
    printf("\n[SUCCESS] %d registros guardados exitosamente en archivo binario '%s'\n", cantidad, nombreArchivo);
}

void cargarDesdeBinario(RiesgoMosler **lista, int *cantidad, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        printf("\n[ERROR] No se pudo abrir el archivo binario '%s'.\n", nombreArchivo);
        return;
    }

    liberarMemoria(lista, cantidad);

    int numRegistros = 0;
    if (fread(&numRegistros, sizeof(int), 1, archivo) != 1) {
        printf("\n[ERROR] Error leyendo encabezado del archivo binario.\n");
        fclose(archivo);
        return;
    }

    if (numRegistros > 0) {
        *lista = (RiesgoMosler *)malloc(numRegistros * sizeof(RiesgoMosler));
        if (*lista == NULL) {
            printf("\n[ERROR] Error asignando memoria durante la carga.\n");
            fclose(archivo);
            return;
        }

        fread(*lista, sizeof(RiesgoMosler), numRegistros, archivo);
        *cantidad = numRegistros;

        for (int i = 0; i < *cantidad; i++) {
            calcularRiesgo(&((*lista)[i]));
        }
    }

    fclose(archivo);
    printf("\n[SUCCESS] %d registros cargados desde el binario '%s' en memoria dinamica.\n", *cantidad, nombreArchivo);
}

void guardarEnJSON(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("\n[ERROR] No se pudo crear el archivo JSON.\n");
        return;
    }

    fprintf(archivo, "[\n");
    for (int i = 0; i < cantidad; i++) {
        const RiesgoMosler *r = &lista[i];
        fprintf(archivo, "  {\n");
        fprintf(archivo, "    \"id\": %d,\n", r->id);
        fprintf(archivo, "    \"bien\": \"%s\",\n", r->bien);
        fprintf(archivo, "    \"riesgo\": \"%s\",\n", r->riesgo);
        fprintf(archivo, "    \"dano\": \"%s\",\n", r->dano);
        fprintf(archivo, "    \"criterios\": {\"F\": %d, \"S\": %d, \"P\": %d, \"E\": %d, \"A\": %d, \"B\": %d},\n",
                r->F, r->S, r->P, r->E, r->A, r->B);
        fprintf(archivo, "    \"evaluacion\": {\"C\": %d, \"Pb\": %d, \"ER\": %d},\n", r->C, r->Pb, r->ER);
        fprintf(archivo, "    \"clasificacion\": \"%s\"\n", r->clasificacion);
        fprintf(archivo, "  }%s\n", (i == cantidad - 1) ? "" : ",");
    }
    fprintf(archivo, "]\n");

    fclose(archivo);
    printf("\n[SUCCESS] Datos exportados correctamente a JSON: '%s'\n", nombreArchivo);
}

void guardarEnXML(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("\n[ERROR] No se pudo crear el archivo XML.\n");
        return;
    }

    fprintf(archivo, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(archivo, "<riesgos_mosler>\n");
    for (int i = 0; i < cantidad; i++) {
        const RiesgoMosler *r = &lista[i];
        fprintf(archivo, "  <riesgo id=\"%d\">\n", r->id);
        fprintf(archivo, "    <bien>%s</bien>\n", r->bien);
        fprintf(archivo, "    <amenaza>%s</amenaza>\n", r->riesgo);
        fprintf(archivo, "    <dano>%s</dano>\n", r->dano);
        fprintf(archivo, "    <criterios F=\"%d\" S=\"%d\" P=\"%d\" E=\"%d\" A=\"%d\" B=\"%d\"/>\n",
                r->F, r->S, r->P, r->E, r->A, r->B);
        fprintf(archivo, "    <caracter>%d</caracter>\n", r->C);
        fprintf(archivo, "    <probabilidad>%d</probabilidad>\n", r->Pb);
        fprintf(archivo, "    <cuantificacion>%d</cuantificacion>\n", r->ER);
        fprintf(archivo, "    <clasificacion>%s</clasificacion>\n", r->clasificacion);
        fprintf(archivo, "  </riesgo>\n");
    }
    fprintf(archivo, "</riesgos_mosler>\n");

    fclose(archivo);
    printf("\n[SUCCESS] Datos exportados correctamente a XML: '%s'\n", nombreArchivo);
}
