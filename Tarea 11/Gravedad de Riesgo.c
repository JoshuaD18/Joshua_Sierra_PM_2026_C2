#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de constantes para límites
#define MAX_TEXTO 100
#define MAX_FILENAME 150

// Estructura que representa un riesgo bajo el Método Mosler
typedef struct {
    int id;
    char bien[MAX_TEXTO];
    char riesgo[MAX_TEXTO];
    char dano[MAX_TEXTO];
    
    // Criterios de la Fase 2 (Escala de 1 a 5)
    int F; // Función
    int S; // Sustitución
    int P; // Profundidad
    int E; // Extensión
    int A; // Agresión
    int B; // Vulnerabilidad
    
    // Resultados de la Fase 3 y 4
    int I;  // Importancia (F * S)
    int D;  // Daños (P * E)
    int C;  // Carácter (I + D)
    int Pb; // Probabilidad (A * B)
    int ER; // Cuantificación del riesgo (C * Pb)
    char clasificacion[MAX_TEXTO]; // Clase de riesgo
} RiesgoMosler;

// Prototipos de Funciones
void calcularRiesgo(RiesgoMosler *r);
void pedirTexto(const char *mensaje, char *buffer, int maxLen);
int pedirValor(const char *mensaje, int min, int max);

void crearRiesgo(RiesgoMosler **lista, int *cantidad);
void modificarRiesgo(RiesgoMosler *lista, int cantidad);
void borrarRiesgo(RiesgoMosler **lista, int *cantidad);
void imprimirRiesgos(const RiesgoMosler *lista, int cantidad);

void guardarEnCSV(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo);
void guardarEnJSON(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo);
void cargarDesdeCSV(RiesgoMosler **lista, int *cantidad, const char *nombreArchivo);

void liberarMemoria(RiesgoMosler **lista, int *cantidad);

// ==========================================
// PROGRAMA PRINCIPAL
// ==========================================
int main() {
    RiesgoMosler *listaRiesgos = NULL;
    int cantidadRiesgos = 0;
    int opcion;
    char archivoActual[MAX_FILENAME] = "riesgos_mosler.csv";

    do {
        printf("\n============================================\n");
        printf("   SISTEMA DE GESTION DE RIESGOS - MOSLER   \n");
        printf("============================================\n");
        printf("Archivo Seleccionado: [%s]\n", archivoActual);
        printf("Riesgos Cargados en Memoria: %d\n", cantidadRiesgos);
        printf("--------------------------------------------\n");
        printf("1. Crear nuevo riesgo\n");
        printf("2. Modificar parametros de un riesgo\n");
        printf("3. Recalcular todos los riesgos\n");
        printf("4. Imprimir riesgos en pantalla\n");
        printf("5. Borrar un riesgo\n");
        printf("6. Seleccionar/Cambiar nombre de archivo\n");
        printf("7. Cargar riesgos desde archivo (Memoria Dinamica)\n");
        printf("8. Guardar en archivo Plano / CSV\n");
        printf("9. Guardar en archivo JSON\n");
        printf("10. Salir\n");
        printf("--------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar el salto de línea sobrante

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
                pedirTexto("Ingrese el nombre del archivo objetivo (ej: datos.csv): ", archivoActual, MAX_FILENAME);
                printf("[INFO] Archivo de trabajo actualizado a: %s\n", archivoActual);
                break;
            case 7:
                cargarDesdeCSV(&listaRiesgos, &cantidadRiesgos, archivoActual);
                break;
            case 8:
                guardarEnCSV(listaRiesgos, cantidadRiesgos, archivoActual);
                break;
            case 9: {
                char archivoJSON[MAX_FILENAME];
                pedirTexto("Ingrese nombre del archivo JSON a exportar (ej: riesgos.json): ", archivoJSON, MAX_FILENAME);
                guardarEnJSON(listaRiesgos, cantidadRiesgos, archivoJSON);
                break;
            }
            case 10:
                printf("\nLiberando recursos y saliendo del sistema...\n");
                liberarMemoria(&listaRiesgos, &cantidadRiesgos);
                break;
            default:
                printf("\n[ERROR] Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 10);

    return 0;
}

// ==========================================
// FUNCIONES AUXILIARES
// ==========================================

// Limpia el salto de línea sobrante de fgets
void pedirTexto(const char *mensaje, char *buffer, int maxLen) {
    printf("%s", mensaje);
    fgets(buffer, maxLen, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

// Solicita un número asegurando que esté dentro del rango de 1 a 5
int pedirValor(const char *mensaje, int min, int max) {
    int valor;
    do {
        printf("%s (%d-%d): ", mensaje, min, max);
        if (scanf("%d", &valor) != 1) {
            while (getchar() != '\n'); // Limpiar entrada invalida
            valor = -1;
        }
    } while (valor < min || valor > max);
    getchar(); // Limpiar el buffer de entrada
    return valor;
}

// Lógica de cálculo estricta del Método Mosler
void calcularRiesgo(RiesgoMosler *r) {
    r->I = r->F * r->S;
    r->D = r->P * r->E;
    r->C = r->I + r->D;
    r->Pb = r->A * r->B;
    r->ER = r->C * r->Pb;

    // Clasificación según la escala oficial del Método Mosler
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

// ==========================================
// GESTIÓN DE MEMORIA DINÁMICA Y RIESGOS
// ==========================================

void crearRiesgo(RiesgoMosler **lista, int *cantidad) {
    // Reasignación dinámica de memoria para agregar un elemento más
    RiesgoMosler *temp = realloc(*lista, (*cantidad + 1) * sizeof(RiesgoMosler));
    if (temp == NULL) {
        printf("\n[ERROR] No se pudo asignar memoria dinamicamente.\n");
        return;
    }
    *lista = temp;

    RiesgoMosler *nuevo = &((*lista)[*cantidad]);
    nuevo->id = *cantidad + 1;

    printf("\n--- REGISTRO DE NUEVO RIESGO (FASE 1) ---\n");
    pedirTexto("Ingrese el Activo/Bien objeto de proteccion: ", nuevo->bien, MAX_TEXTO);
    pedirTexto("Ingrese el Riesgo/Amenaza: ", nuevo->riesgo, MAX_TEXTO);
    pedirTexto("Ingrese la descripcion del Dano posible: ", nuevo->dano, MAX_TEXTO);

    printf("\n--- VALORACION DE CRITERIOS FASE 2 (Escala 1 a 5) ---\n");
    nuevo->F = pedirValor("1. Criterio de Funcion (F)", 1, 5);
    nuevo->S = pedirValor("2. Criterio de Sustitucion (S)", 1, 5);
    nuevo->P = pedirValor("3. Criterio de Profundidad (P)", 1, 5);
    nuevo->E = pedirValor("4. Criterio de Extension (E)", 1, 5);
    nuevo->A = pedirValor("5. Criterio de Agresion (A)", 1, 5);
    nuevo->B = pedirValor("6. Criterio de Vulnerabilidad (B)", 1, 5);

    // Ejecuta las Fases 3 y 4 de forma inmediata
    calcularRiesgo(nuevo);

    (*cantidad)++;
    printf("\n[SUCCESS] Riesgo ID #%d registrado y calculado exitosamente.\n", nuevo->id);
}

void modificarRiesgo(RiesgoMosler *lista, int cantidad) {
    if (cantidad == 0) {
        printf("\n[INFO] No hay riesgos para modificar.\n");
        return;
    }

    int id = pedirValor("\nIngrese el ID del riesgo que desea modificar", 1, cantidad);
    RiesgoMosler *r = &lista[id - 1];

    printf("\nModificando Datos del Riesgo ID #%d:\n", r->id);
    pedirTexto("Nuevo Activo/Bien: ", r->bien, MAX_TEXTO);
    pedirTexto("Nuevo Riesgo: ", r->riesgo, MAX_TEXTO);
    pedirTexto("Nuevo Dano: ", r->dano, MAX_TEXTO);

    printf("\nNuevos Criterios (1-5):\n");
    r->F = pedirValor("Criterio F", 1, 5);
    r->S = pedirValor("Criterio S", 1, 5);
    r->P = pedirValor("Criterio P", 1, 5);
    r->E = pedirValor("Criterio E", 1, 5);
    r->A = pedirValor("Criterio A", 1, 5);
    r->B = pedirValor("Criterio B", 1, 5);

    calcularRiesgo(r);
    printf("\n[SUCCESS] Riesgo ID #%d modificado y recalculado con exito.\n", r->id);
}

void borrarRiesgo(RiesgoMosler **lista, int *cantidad) {
    if (*cantidad == 0) {
        printf("\n[INFO] No hay riesgos para borrar.\n");
        return;
    }

    int id = pedirValor("\nIngrese el ID del riesgo que desea borrar", 1, *cantidad);
    int indice = id - 1;

    // Desplazar elementos en la memoria dinámica
    for (int i = indice; i < (*cantidad) - 1; i++) {
        (*lista)[i] = (*lista)[i + 1];
        (*lista)[i].id = i + 1; // Ajustar IDs consecutivos
    }

    (*cantidad)--;

    if (*cantidad > 0) {
        RiesgoMosler *temp = realloc(*lista, (*cantidad) * sizeof(RiesgoMosler));
        if (temp != NULL) *lista = temp;
    } else {
        free(*lista);
        *lista = NULL;
    }

    printf("\n[SUCCESS] El riesgo ID #%d ha sido eliminado exitosamente.\n", id);
}

void imprimirRiesgos(const RiesgoMosler *lista, int cantidad) {
    if (cantidad == 0) {
        printf("\n[INFO] No existen riesgos registrados en el sistema.\n");
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

// ==========================================
// PERSISTENCIA Y ARCHIVOS (CSV / JSON)
// ==========================================

void guardarEnCSV(const RiesgoMosler *lista, int cantidad, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("\n[ERROR] No se pudo abrir el archivo para escritura.\n");
        return;
    }

    // Cabecera CSV
    fprintf(archivo, "ID,Bien,Riesgo,Dano,F,S,P,E,A,B,C,Pb,ER,Clasificacion\n");

    for (int i = 0; i < cantidad; i++) {
        const RiesgoMosler *r = &lista[i];
        fprintf(archivo, "%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                r->id, r->bien, r->riesgo, r->dano,
                r->F, r->S, r->P, r->E, r->A, r->B,
                r->C, r->Pb, r->ER, r->clasificacion);
    }

    fclose(archivo);
    printf("\n[SUCCESS] Datos guardados exitosamente en el archivo CSV: '%s'\n", nombreArchivo);
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
    printf("\n[SUCCESS] Estructura exportada exitosamente en formato JSON: '%s'\n", nombreArchivo);
}

void cargarDesdeCSV(RiesgoMosler **lista, int *cantidad, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("\n[ERROR] No se pudo abrir el archivo '%s'. Asegurese de que exista.\n", nombreArchivo);
        return;
    }

    // Liberar la memoria actual antes de cargar nuevos datos
    liberarMemoria(lista, cantidad);

    char linea[512];
    // Ignorar la primera línea si contiene los encabezados
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo)) {
        RiesgoMosler r;
        // Parsear formato separado por comas
        int leidos = sscanf(linea, "%d,%99[^,],%99[^,],%99[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%99[^\n]",
                            &r.id, r.bien, r.riesgo, r.dano,
                            &r.F, &r.S, &r.P, &r.E, &r.A, &r.B,
                            &r.C, &r.Pb, &r.ER, r.clasificacion);

        if (leidos >= 10) {
            RiesgoMosler *temp = realloc(*lista, (*cantidad + 1) * sizeof(RiesgoMosler));
            if (temp == NULL) {
                printf("\n[ERROR] Memoria insuficiente durante la carga de datos.\n");
                fclose(archivo);
                return;
            }
            *lista = temp;
            
            // Recalcular para asegurar coherencia de datos cargados
            calcularRiesgo(&r);
            (*lista)[*cantidad] = r;
            (*cantidad)++;
        }
    }

    fclose(archivo);
    printf("\n[SUCCESS] Carga de datos completada exitosamente desde '%s'. (%d registros cargados)\n", 
           nombreArchivo, *cantidad);
}