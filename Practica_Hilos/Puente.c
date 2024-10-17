//
// Created by angel on 16/10/24.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

int auto_puente = 0;
int sentido_actual=0;

typedef struct {
  int id;
  int sentido;
}Auto;

void *Autofunc(void *arg) {
  Auto *a = (Auto *)arg;
  usleep(rand()%20000);
  pthread_mutex_lock(&mutex);
  while(auto_puente>0 && sentido_actual!=a->sentido) {
    pthread_cond_wait(&cond,&mutex);
  }
  if(auto_puente>0) {
    sentido_actual=a->sentido;
  }
  auto_puente++;
  printf("Auto %d entrando al puente en sentido %d. Total en puente: %d\n", a->id, a->sentido, auto_puente);
  pthread_mutex_unlock(&mutex);
  sleep(1);
  pthread_mutex_lock(&mutex);
  auto_puente--;
  printf("Auto %d saliendo del puente en sentido %d. Autos restantes en el puente: %d\n",a->id, a->sentido,auto_puente);
    if(auto_puente==0){
      sentido_actual=0;
      pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);
  free(a);
  return NULL;
}

int main() {
  srand(time(NULL));

  int auto_sentido1,auto_sentido2;

  printf("Ingrese el numero de vehiculos para el sentido 1: \n");
  scanf("%d", &auto_sentido1);
  printf("Ingrese el numero de autos en el sentido 2: \n");
  scanf("%d", &auto_sentido2);
  int total_autos=auto_sentido1+auto_sentido2;

  pthread_t *hilos=malloc(total_autos*sizeof(pthread_t));

  for(int i=0;i<auto_sentido1;i++) {
    Auto *a = (Auto *)malloc(sizeof(Auto));
    a->id=i+1;
    a->sentido=1;
    pthread_create(&hilos[auto_sentido1], NULL, Autofunc, a);
  }
  for(int i=0;i<auto_sentido2;i++) {
    Auto *a = (Auto *)malloc(sizeof(Auto));
    a->id=i+1;
    a->sentido=2;
    pthread_create(&hilos[auto_sentido2], NULL, Autofunc, a);
  }
  for(int i=0;i<total_autos;i++) {
    pthread_join(hilos[i],NULL);
  }

  free(hilos);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  return 0;
}
//
