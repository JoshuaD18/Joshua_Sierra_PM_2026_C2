#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO "vehiculos.dat"
#define MAX_NOMBRE 50

typedef struct {
    char nombre[MAX_NOMBRE];
    float costo_compra;
    float vida_util_anios;
    float seguro_anual;
    float mantenimiento_anual;
    float costo_neumaticos;
    float km_vida_neumaticos;
    float consumo_ciudad;
    float consumo_autopista;
    float km_anuales_estimados;
} Vehiculo;

float amortizacion_anual(Vehiculo v) {
    if (v.vida_util_anios <= 0) return 0.0f;
    return v.costo_compra / v.vida_util_anios;
}

float mantenimiento_total_vida(Vehiculo v) {
    return v.mantenimiento_anual * v.vida_util_anios;
}

float costo_fijo_por_km(Vehiculo v) {
    if (v.km_anuales_estimados <= 0 || v.km_vida_neumaticos <= 0) return 0.0f;

    float costo_amort_km = amortizacion_anual(v) / v.km_anuales_estimados;
    float costo_seguro_km = v.seguro_anual / v.km_anuales_estimados;
    float costo_mant_km = v.mantenimiento_anual / v.km_anuales_estimados;
    float costo_neum_km = v.costo_neumaticos / v.km_vida_neumaticos;

    return costo_amort_km + costo_seguro_km + costo_mant_km + costo_neum_km;
}

void crear_vehiculo() {
    FILE *f = fopen(ARCHIVO, "ab");
    if (!f) return;

    Vehiculo v;
    printf("\nNombre: ");
    fgets(v.nombre, MAX_NOMBRE, stdin);
    v.nombre[strcspn(v.nombre, "\n")] = 0;

    printf("Costo compra (RD$): "); scanf("%f", &v.costo_compra);
    printf("Vida util (anios): "); scanf("%f", &v.vida_util_anios);
    printf("Seguro anual (RD$): "); scanf("%f", &v.seguro_anual);
    printf("Mantenimiento anual (RD$): "); scanf("%f", &v.mantenimiento_anual);
    printf("Costo neumaticos (RD$): "); scanf("%f", &v.costo_neumaticos);
    printf("Vida neumaticos (km): "); scanf("%f", &v.km_vida_neumaticos);
    printf("Rendimiento ciudad (km/gal): "); scanf("%f", &v.consumo_ciudad);
    printf("Rendimiento autopista (km/gal): "); scanf("%f", &v.consumo_autopista);
    printf("Km anuales estimados: "); scanf("%f", &v.km_anuales_estimados);

    fwrite(&v, sizeof(Vehiculo), 1, f);
    fclose(f);
}

void listar_vehiculos() {
    FILE *f = fopen(ARCHIVO, "rb");
    if (!f) return;

    Vehiculo v;
    while (fread(&v, sizeof(Vehiculo), 1, f)) {
        printf("\n[%s]\n", v.nombre);
        printf("Costo compra: RD$%.2f | Vida util: %.1f anios\n", v.costo_compra, v.vida_util_anios);
        printf("Amortizacion/anio: RD$%.2f\n", amortizacion_anual(v));
        printf("Mantenimiento total vida: RD$%.2f\n", mantenimiento_total_vida(v));
        printf("Costo desgaste/km: RD$%.4f\n", costo_fijo_por_km(v));
    }
    fclose(f);
}

void modificar_vehiculo() {
    FILE *f = fopen(ARCHIVO, "rb+");
    if (!f) return;

    char nombre[MAX_NOMBRE];
    printf("\nVehiculo a modificar: ");
    fgets(nombre, MAX_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    Vehiculo v;
    while (fread(&v, sizeof(Vehiculo), 1, f)) {
        if (strcmp(v.nombre, nombre) == 0) {
            printf("Nuevo costo compra (RD$): "); scanf("%f", &v.costo_compra);
            printf("Nueva vida util (anios): "); scanf("%f", &v.vida_util_anios);
            printf("Nuevo seguro anual (RD$): "); scanf("%f", &v.seguro_anual);
            printf("Nuevo mantenimiento anual (RD$): "); scanf("%f", &v.mantenimiento_anual);
            printf("Nuevo costo neumaticos (RD$): "); scanf("%f", &v.costo_neumaticos);
            printf("Nueva vida neumaticos (km): "); scanf("%f", &v.km_vida_neumaticos);
            printf("Nuevo rendimiento ciudad (km/gal): "); scanf("%f", &v.consumo_ciudad);
            printf("Nuevo rendimiento autopista (km/gal): "); scanf("%f", &v.consumo_autopista);
            printf("Nuevos km anuales: "); scanf("%f", &v.km_anuales_estimados);

            fseek(f, -((long)sizeof(Vehiculo)), SEEK_CUR);
            fwrite(&v, sizeof(Vehiculo), 1, f);
            break;
        }
    }
    fclose(f);
}

void borrar_vehiculo() {
    FILE *f = fopen(ARCHIVO, "rb");
    if (!f) return;

    char nombre[MAX_NOMBRE];
    printf("\nVehiculo a borrar: ");
    fgets(nombre, MAX_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    FILE *temp = fopen("temp.dat", "wb");
    Vehiculo v;

    while (fread(&v, sizeof(Vehiculo), 1, f)) {
        if (strcmp(v.nombre, nombre) != 0) {
            fwrite(&v, sizeof(Vehiculo), 1, temp);
        }
    }

    fclose(f);
    fclose(temp);

    remove(ARCHIVO);
    rename("temp.dat", ARCHIVO);
}

void calcular_viaje(float precio_combustible) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (!f) return;

    char nombre[MAX_NOMBRE];
    printf("\nVehiculo: ");
    fgets(nombre, MAX_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    Vehiculo v;
    int encontrado = 0;
    while (fread(&v, sizeof(Vehiculo), 1, f)) {
        if (strcmp(v.nombre, nombre) == 0) {
            encontrado = 1;
            break;
        }
    }
    fclose(f);

    if (!encontrado) return;

    float km_c, km_a;
    printf("Km ciudad: "); scanf("%f", &km_c);
    printf("Km autopista: "); scanf("%f", &km_a);

    float galones_ciudad = (v.consumo_ciudad > 0) ? (km_c / v.consumo_ciudad) : 0.0f;
    float galones_autopista = (v.consumo_autopista > 0) ? (km_a / v.consumo_autopista) : 0.0f;
    float total_galones = galones_ciudad + galones_autopista;

    float costo_combustible = total_galones * precio_combustible;
    float km_totales = km_c + km_a;
    float costo_fijo = costo_fijo_por_km(v) * km_totales;
    float costo_total = costo_combustible + costo_fijo;
    float costo_real_km = (km_totales > 0) ? (costo_total / km_totales) : 0.0f;

    printf("\n--- RESULTADOS --- \n");
    printf("Km totales: %.2f km\n", km_totales);
    printf("Galones consumidos: %.2f gal\n", total_galones);
    printf("Costo combustible: RD$%.2f\n", costo_combustible);
    printf("Costo amortizacion/mantenimiento/neumaticos/seguro: RD$%.2f\n", costo_fijo);
    printf("COSTO TOTAL: RD$%.2f\n", costo_total);
    printf("COSTO REAL POR KM: RD$%.4f/km\n", costo_real_km);
}

int main() {
    int op;
    float precio_combustible = 290.00f;

    do {
        printf("\n--- MENU ---\n");
        printf("Precio combustible por galon: RD$%.2f\n", precio_combustible);
        printf("1. Crear vehiculo\n");
        printf("2. Listar vehiculos\n");
        printf("3. Modificar vehiculo\n");
        printf("4. Borrar vehiculo\n");
        printf("5. Calcular viaje\n");
        printf("6. Modificar precio combustible\n");
        printf("7. Salir\n");
        printf("Opcion: ");
        scanf("%d", &op);
        getchar();

        switch (op) {
            case 1: crear_vehiculo(); break;
            case 2: listar_vehiculos(); break;
            case 3: modificar_vehiculo(); break;
            case 4: borrar_vehiculo(); break;
            case 5: calcular_viaje(precio_combustible); break;
            case 6:
                printf("Nuevo precio por galon (RD$): ");
                scanf("%f", &precio_combustible);
                break;
        }
    } while (op != 7);

    return 0;
}
