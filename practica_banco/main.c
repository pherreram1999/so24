#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;                                                          //Declaracion del mutex

int saldo = 0;                                                                  //Declaracion e inicializacion de la variable saldo en 0

void* realizarRetiro(void* arg) {
	int monto = *(int*)arg;                                                     //Declaracion de la variable monto, la cual obtiene el valor obtenida en el main por la variable "montoRetiro"
	pthread_mutex_lock(&mutex) ;                                                //Se obtiene un lock al mutex, lo que significa que si el mutex esta abierto, el hilo podrá acceder a la instruccion, de lo contrario esperará a la llamada "pthread_mutex_unlock"
	printf("Realizando retiro de %d\n", monto);
	saldo -= monto;
	printf("Saldo restante: %d\n", saldo);
	pthread_mutex_unlock(&mutex);                                               //Desbloquea a mutex, permitiendo que este se re-abra
	pthread_exit (NULL) ;                                                       //Termina el hilo
}

void* realizarDeposito(void* arg) {
	int monto = *(int*)arg;                                                     //Declaracion de la variable monto, la cual obtiene el valor obtenida en el main por la variable "montoDeposito"
	printf("Realizando deposito de %d\n", monto);
	saldo += monto;
	printf("Saldo actual: %d\n", saldo); 
	pthread_mutex_unlock (&mutex);                                              //Desbloquea a mutex, permitiendo que este se re-abra
	pthread_exit(NULL);                                                         //Termina el hilo
}

int main (){
	pthread_t hiloRetiro, hiloDeposito;                                         //Declaracion de los hilos "hiloRetiro" e "hiloDeposito"
	int montoRetiro = 100;                                                      //Declaracion e inicializacion de la variable montoRetiro en 100
	int montoDeposito = 200;                                                    //Declaracion e inicializacion de la variable montoDeposito en 200
	
	pthread_mutex_init (&mutex, NULL);                                          //Inicializacion del mutex
	pthread_create(&hiloRetiro, NULL, realizarRetiro, &montoRetiro);            //Inicializacion del hilo "hiloRetiro"
	pthread_create(&hiloDeposito, NULL, realizarDeposito, &montoDeposito);      //Inicializacion del hilo "hiloDeposito"
	
	pthread_join(hiloRetiro, NULL);                                             //Espera a que termine el hilo "hiloRetiro"
	pthread_join(hiloDeposito, NULL);                                           //Espera a que termine el hilo "hiloDeposito"
	pthread_mutex_destroy (&mutex);                                             //Destruye el mutex
	return 0;
}
