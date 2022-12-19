#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define BUFFER_SIZE 50
#define NUMITEMS 30

int value = 0;
pthread_mutex_t insert_mutex;
pthread_mutex_t client_mutex;
pthread_mutex_t server_mutex;
pthread_cond_t client_broad;
pthread_cond_t server_sig;
receive_broadcast(int arg){
	pthread_cond_wait(&client_broad, &client_mutex);
	printf("Child %d is created  : %s\n", arg,"new child");
}
void *server(void *arg)
{
	while (1) {
		pthread_mutex_lock(&server_mutex);
		pthread_cond_wait(&server_sig, &server_mutex);
		pthread_cond_broadcast(&client_broad);
		value = 0;
		pthread_mutex_unlock(&server_mutex);
	}
}
void *client(void *arg){
	int status;
	while (1) {
		if(value == 0){
			value = 1;
			sleep(1);
			pthread_cond_signal(&server_sig);
		}
		else
			receive_broadcast(arg);
			pthread_mutex_unlock(&client_mutex);
	}
}
int main(int argc, char *argv[]){
	int status;
	int cycle;
	void *result;
	pthread_t server_tid, client_tid[8];
	pthread_mutex_init(&client_mutex, NULL);
	pthread_mutex_init(&server_mutex, NULL);
	pthread_cond_init(&server_sig, NULL);
	pthread_cond_init(&client_broad, NULL);
	cycle = 5;
	for(int i = 0; i < cycle; i++)status = pthread_create (&client_tid, NULL, client, i+1);
	status = pthread_create (&server_tid, NULL, server, NULL);
	status = pthread_join (server_tid, NULL);
	for(int i = 0; i < cycle; i++){
		status = pthread_join (client_tid, NULL);
	}
	return 0;
}
