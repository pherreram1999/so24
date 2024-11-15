#include <stdio.h>

int main() {
    // Ruta al archivo en la tercera carpeta
    const char *rutaArchivo = "./Investigacion 3/Controladores de Dispositivos de Entrada y Salida.txt";
    
    // Abrir el archivo en modo lectura
    FILE *archivo = fopen(rutaArchivo, "r");
    
    // Verificar si el archivo se abrió correctamente
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return 1; // Retorna un código de error
    }
    
    // Leer y mostrar el contenido del archivo (opcional)
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        printf("%s", linea);
    }
    
    // Cerrar el archivo
    fclose(archivo);
    printf("\n\n\n----Archivo leido correctamente.----\n");

    return 0; // Retorna éxito
}
