//
// Created by angel on 16/10/24.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t puente;
pthread_mutex_t mutex;
int auto_puente = 0;
int sentido_actual=0;
int auto_sentido1, auto_sentido2;

void *autoenpuente(void *arg) {
  int sentido=*(int *)arg;
  int total_vehiculos;
  if (sentido == 1) {
    total_vehiculos = auto_sentido1;
  } else {
    total_vehiculos = auto_sentido2;
  }

  for (int i = 0; i < total_vehiculos; i++) {
    usleep(rand()%20000);
    while(1){
      pthread_mutex_lock(&mutex);
      if (auto_puente==0 || sentido_actual==sentido){
        if(auto_puente==0){
          sentido_actual=sentido;
        }
        auto_puente++;
        printf("Auto %d entrando al puente en sentido %d. Total en puente: %d\n", i+1, sentido, auto_puente);
        pthread_mutex_unlock(&mutex);
        break;
      }
      pthread_mutex_unlock(&mutex);
      usleep(1000);
    }
    sleep(1);
    pthread_mutex_lock(&mutex);
    auto_puente--;
    printf("Auto %d saliendo del puente en sentido %d. Autos restantes en el puente: %d\n",i+1, sentido,auto_puente);
    if(auto_puente==0){
      sentido_actual=0;
    }
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main() {
  pthread_mutex_init(&mutex, NULL);
  srand(time(NULL));
  pthread_t hilos[2];
  printf("Ingrese el numero de vehiculos para el sentido 1: \n");
  scanf("%d", &auto_sentido1);
  printf("Ingrese el numero de autos en el sentido 2: \n");
  scanf("%d", &auto_sentido2);

  int *sentido1 = (int *)malloc(sizeof(int));
  *sentido1=1;
  pthread_create(&hilos[0], NULL, autoenpuente, sentido1);
  int *sentido2 = (int *)malloc(sizeof(int));
  *sentido2=2;
  pthread_create(&hilos[1],NULL,autoenpuente,sentido2);
  pthread_join(hilos[0],NULL);
  pthread_join(hilos[1],NULL);
  free(sentido1);
  free(sentido2);
  return 0;

  pthread_mutex_destroy(&mutex);
}
//
