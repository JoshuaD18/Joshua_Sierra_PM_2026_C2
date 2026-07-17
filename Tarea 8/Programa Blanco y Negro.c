#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_ARCHIVOS 100
#define MAX_NOMBRE 256

typedef struct {
    char nombre[MAX_NOMBRE];
} ArchivoImagen;

int tiene_extension_valida(const char *nombre) {
    const char *ext = strrchr(nombre, '.');
    if (!ext) return 0;
    if (strcasecmp(ext, ".png") == 0 ||
        strcasecmp(ext, ".jpg") == 0 ||
        strcasecmp(ext, ".jpeg") == 0 ||
        strcasecmp(ext, ".bmp") == 0) {
        return 1;
    }
    return 0;
}

int listar_imagenes(ArchivoImagen lista[]) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile("*", &findData);
    int contador = 0;

    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    }

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (tiene_extension_valida(findData.cFileName)) {
                strncpy(lista[contador].nombre, findData.cFileName, MAX_NOMBRE);
                contador++;
                if (contador >= MAX_ARCHIVOS) break;
            }
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
    return contador;
}

void convertir_a_blanco_y_negro(const char *nombre_origen) {
    char nombre_salida[MAX_NOMBRE];
    char *ext = strrchr(nombre_origen, '.');
    int longitud_base = ext - nombre_origen;
    strncpy(nombre_salida, nombre_origen, longitud_base);
    nombre_salida[longitud_base] = '\0';
    strcat(nombre_salida, "_bw");
    strcat(nombre_salida, ext);

    char comando[1024];
    snprintf(comando, sizeof(comando),
        "powershell -Command \""
        "[void][Reflection.Assembly]::LoadWithPartialName('System.Drawing');"
        "$img = [System.Drawing.Image]::FromFile('%s');"
        "$bmp = New-Object System.Drawing.Bitmap($img.Width, $img.Height);"
        "$g = [System.Drawing.Graphics]::FromImage($bmp);"
        "$cm = New-Object System.Drawing.Imaging.ColorMatrix(,"
        " @( @(0.299, 0.299, 0.299, 0, 0),"
        "    @(0.587, 0.587, 0.587, 0, 0),"
        "    @(0.114, 0.114, 0.114, 0, 0),"
        "    @(0,     0,     0,     1, 0),"
        "    @(0,     0,     0,     0, 1) )"
        ");"
        "$ia = New-Object System.Drawing.Imaging.ImageAttributes;"
        "$ia.SetColorMatrix($cm);"
        "$g.DrawImage($img, (New-Object System.Drawing.Rectangle(0,0,$img.Width,$img.Height)), 0, 0, $img.Width, $img.Height, [System.Drawing.GraphicsUnit]::Pixel, $ia);"
        "$g.Dispose();"
        "$img.Dispose();"
        "$bmp.Save('%s');"
        "$bmp.Dispose();\"",
        nombre_origen, nombre_salida);

    int resultado = system(comando);

    if (resultado == 0) {
        printf("Imagen convertida con exito: %s\n", nombre_salida);
    } else {
        printf("Error al procesar la conversion de la imagen.\n");
    }
}

int main() {
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);

    ArchivoImagen lista[MAX_ARCHIVOS];

    while (1) {
        system("cls");
        printf("CONVERSOR DE IMAGENES A BLANCO Y NEGRO\n");
        int total_imagenes = listar_imagenes(lista);

        if (total_imagenes == 0) {
            printf("No se encontraron archivos PNG, JPG o BMP en esta carpeta.\n\n");
            system("pause");
            break;
        }

        printf("Seleccione la imagen a convertir:\n");
        for (int i = 0; i < total_imagenes; i++) {
            printf(" [%d] %s\n", i + 1, lista[i].nombre);
        }
        printf(" [%d] Salir\n", total_imagenes + 1);

        int opcion;
        printf("\nSeleccion: ");
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (opcion == total_imagenes + 1) {
            break;
        }

        if (opcion < 1 || opcion > total_imagenes) {
            printf("\nOpcion no valida.\n");
            system("pause");
            continue;
        }

        char *archivo_seleccionado = lista[opcion - 1].nombre;
        convertir_a_blanco_y_negro(archivo_seleccionado);

        printf("\n");
        system("pause");
    }

    return 0;
}
