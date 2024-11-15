#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <time.h>

#define NUM_FRAMES 4   // Número de frames (páginas físicas en memoria)
#define NUM_PAGES 10   // Número total de páginas virtuales

// Estructura para un frame de página en memoria física
typedef struct Frame {
    int page; // Número de página almacenada en el frame (valor -1 si está vacío)
    bool valid; // Indica si el frame está ocupado (true) o vacío (false)
    int bitOportunidad; // indica el bit de segunda oportunidad que interviene en el momento de eliminar un frame
    struct Frame *prev; // Puntero al frame previo (para lista doblemente enlazada)
    struct Frame *next; // Puntero al frame siguiente (para lista doblemente enlazada)
} Frame;

// Estructura para la lista de frames en memoria física
typedef struct FrameList {
    int numFrames; // Número de frames actualmente ocupados
    Frame *head; // Puntero al primer frame de la lista
    Frame *tail; // Puntero al último frame de la lista
} FrameList;

// Función para crear un nuevo frame
Frame *createFrame() {
    Frame *frame = (Frame *) malloc(sizeof(Frame));
    if (frame != NULL) {
        frame->page = -1; // Inicialmente no hay página asignada
        frame->valid = false;
        frame->bitOportunidad = rand() % 2;
        frame->prev = NULL;
        frame->next = NULL;
    }
    return frame;
}

// Función para inicializar la lista de frames en memoria física
FrameList *createFrameList() {
    FrameList *frameList = (FrameList *) malloc(sizeof(FrameList));
    if (frameList != NULL) {
        frameList->numFrames = 0;
        frameList->head = NULL;
        frameList->tail = NULL;
    }
    return frameList;
}

// Función para insertar un frame al frente de la lista (más recientemente usado)
void insertFrame(FrameList *frameList, Frame *frame) {
    if (frameList->head == NULL) {
        // Lista vacía
        frameList->head = frame;
        frameList->tail = frame;
    } else {
        // Insertar al frente de la lista
        frame->next = frameList->head;
        frameList->head->prev = frame;
        frameList->head = frame;
    }
    frameList->numFrames++;
}

// Función para eliminar un frame de la lista (menos recientemente usado)
void removeFrame(FrameList *frameList, Frame *frame) {
    if (frame->prev != NULL) {
        frame->prev->next = frame->next;
    } else {
        frameList->head = frame->next;
    }
    if (frame->next != NULL) {
        frame->next->prev = frame->prev;
    } else {
        frameList->tail = frame->prev;
    }
    frameList->numFrames--;
    free(frame);
}

// Función para buscar un frame específico por número de página
Frame *findFrame(FrameList *frameList, int page) {
    Frame *current = frameList->head;
    while (current != NULL) {
        if (current->page == page) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Anterior funcionalidad: Recorre la lista y devuelve el nodo con menor frecuencia
 * 
 * Nueva funcionalidad: Recorre la lista y devuelve el primer nodo que no disponga de un bit de segunda oportunidad (aquel que su valor sea 0)
 */
Frame *getFrameLeast(FrameList *frameList) {
    Frame *nav = frameList->head;

    Frame *leastFrame = nav;
    while (nav != NULL) {
        // si es 0 el bit de segunda oportunidad sera reemplazado
        if(nav->bitOportunidad == 0) {
            leastFrame = nav;
        }
        nav = nav->next;
    }
    return leastFrame;
}

void updateSecondChance(FrameList *frameList) {
    if (frameList->head == NULL) return;  // Lista vacía

    Frame *current = frameList->tail;  // Comenzamos desde el último frame (que sería el 1)
    
    while (current != NULL) {
        if (current->bitOportunidad == 1) {
            current->bitOportunidad = 0;
            // Avanzamos al siguiente frame (que en realidad es el anterior en la lista)
            current = current->prev;
        } else {
            // Si encontramos un 0, nos detenemos inmediatamente
            break;
        }
    }
}


// Función para imprimir el estado actual de la lista de frames (solo para fines de depuración)
void printFrameList(FrameList *frameList) {
    printf("Estado actual de la lista de frames:\n");
    Frame *current = frameList->head;
    
    while (current != NULL) {
        printf("Página: %d, Bit de segunda oportunidad: %d - \t", current->page,current->bitOportunidad);
        if (current->valid) {
            printf("Estado: Ocupado\n");
        } else {
            printf("Estado: Vacío\n");
        }
        current = current->next;
    }
    printf("\n");
}

// Función para simular la carga de una página a memoria física
void loadPage(FrameList *frameList, int page) {
    Frame *frame = createFrame();
    frame->page = page;
    frame->valid = true;

    // Si la lista de frames ya está llena, reemplazar la página menos recientemente usada (LRU)
    if (frameList->numFrames == NUM_FRAMES) {
        //Aquí se elimina los nodos por el orden de entrada, si el nodo en cuestion tiene un bit de segunda oportunidad = 1, no se elimina, pero cambia su valor a 0, pero si su valor inicial es 0, se elimina y se reemplaza
        Frame *lruFrame = getFrameLeast(frameList);
        removeFrame(frameList, lruFrame);
        
        updateSecondChance (frameList);
    }

    insertFrame(frameList, frame);
}

int main() {
	srand(time(NULL));
    FrameList *frameList = createFrameList();

    // Simular la carga de varias páginas a memoria física
    loadPage(frameList, 1);
    loadPage(frameList, 2);
    loadPage(frameList, 3);
    loadPage(frameList, 4);
    printFrameList(frameList); // Debería imprimir el estado actual de los frames

    // Intentar cargar otra página cuando todos los frames están ocupados
    loadPage(frameList, 5);
    loadPage(frameList, 6);
    printFrameList(frameList); // Debería imprimir el estado actual después de la sustitución

    // Liberar la memoria utilizada por la lista de frames
    free(frameList);

    return 0;
}
        