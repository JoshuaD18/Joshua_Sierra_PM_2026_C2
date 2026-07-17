#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_ARCHIVOS 100
#define MAX_NOMBRE 256

typedef struct {
    char nombre[MAX_NOMBRE];
} Archivo;

const unsigned char firma_png[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
const unsigned char firma_jpg[] = {0xFF, 0xD8, 0xFF};
const unsigned char firma_bmp[] = {0x42, 0x4D};

int tiene_extension_imagen(const char *nombre) {
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

int listar_archivos(Archivo lista[]) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile("*", &findData);
    int contador = 0;

    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    }

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (tiene_extension_imagen(findData.cFileName)) {
                strncpy(lista[contador].nombre, findData.cFileName, MAX_NOMBRE);
                contador++;
                if (contador >= MAX_ARCHIVOS) break;
            }
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
    return contador;
}

void verificar_archivo(const char *ruta) {
    const char *ext = strrchr(ruta, '.');
    if (!ext) return;

    FILE *archivo = fopen(ruta, "rb");
    if (!archivo) {
        printf("%s -> Error: No se pudo abrir.\n", ruta);
        return;
    }

    unsigned char cabecera[8];
    size_t bytes_leidos = fread(cabecera, 1, 8, archivo);
    fclose(archivo);

    if (strcasecmp(ext, ".bmp") == 0) {
        if (bytes_leidos >= 2 && cabecera[0] == firma_bmp[0] && cabecera[1] == firma_bmp[1]) {
            printf("%s -> OK: Contenido BMP valido.\n", ruta);
        } else {
            printf("%s -> ERROR: El contenido NO es un BMP real.\n", ruta);
        }
    }
    else if (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0) {
        if (bytes_leidos >= 3 && cabecera[0] == firma_jpg[0] && cabecera[1] == firma_jpg[1] && cabecera[2] == firma_jpg[2]) {
            printf("%s -> OK: Contenido JPG valido.\n", ruta);
        } else {
            printf("%s -> ERROR: El contenido NO es un JPG real.\n", ruta);
        }
    }
    else if (strcasecmp(ext, ".png") == 0) {
        if (bytes_leidos >= 8 && memcmp(cabecera, firma_png, 8) == 0) {
            printf("%s -> OK: Contenido PNG valido.\n", ruta);
        } else {
            printf("%s -> ERROR: El contenido NO es un PNG real.\n", ruta);
        }
    }
}

int main() {
    Archivo lista[MAX_ARCHIVOS];
    int total = listar_archivos(lista);

    printf("VERIFICADOR DE CONTENIDO REAL\n");

    if (total == 0) {
        printf("No se encontraron archivos PNG, JPG o BMP en la carpeta.\n");
    } else {
        for (int i = 0; i < total; i++) {
            verificar_archivo(lista[i].nombre);
        }
    }

    printf("\n");
    system("pause");
    return 0;
}
