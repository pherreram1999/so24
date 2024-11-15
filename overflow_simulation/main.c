#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));

	int memory_size,enableFreeMemory, rangeMemory;
	int *memory;
	printf("ingresa el tamaño de la memoria: ");
	scanf("%d",&memory_size);

	if(memory_size == 0){
		perror("memoria vacia, no valida\n");
		return 1;
	}

	memory = (int*)malloc(memory_size);

	// simulamos un random donde borra o no la memoria
	enableFreeMemory = rand() % 2;

	if(enableFreeMemory == 1) {
		free(memory);
	}
	// preguntamos si la memoria ha sido liberada
	if(memory == NULL) { // un error detectado
		perror("memoria vacia, no valida, se intento usar memoria liberada\n");
	}
	// agregamos valores a memoria,
	// simulamos un n cantidad de datos a ingresar para disparar un desbordamiento de memoria
	rangeMemory = (rand() % memory_size) + (memory_size / 2);

	// solo si rangeMemorua es mayor a memory_size

	if(rangeMemory > memory_size) {
		perror("desbordamiento de memoria\n");
		return 1;
	}

	// simulamos el llenado

	for(int i = 0; i < memory_size; i++) {
		memory[i] = rand() % rangeMemory;
	}

	free(memory);
	rangeMemory = 0;
	enableFreeMemory = 0;
	memory_size =  0;

	printf("todo OK\n");

	return 0;
}

