#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFFER_SIZE 10
#define THREAD_COUNT 2
int buffer[BUFFER_SIZE];
int in = -1; 
int out = -1;
int num = 0; 
sem_t buffer_slot;
sem_t items;
sem_t mutex; 


void *use(void *arg); 
void *make(void *arg); 
int main(void)
{
	sem_init(&buffer_slot, 0, BUFFER_SIZE);
	sem_init(&items, 0, 0);
	sem_init(&mutex, 0, 1);
	pthread_t threads[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		if (i % 2 == 0) 
		pthread_create(&threads[i], NULL, use, NULL);
		else
		pthread_create(&threads[i], NULL, make, NULL);
	}
	for (int i = 0; i < THREAD_COUNT; i++) pthread_join(threads[i], NULL); 
	printf("\n result : %d \n \n", num);
	pthread_exit(NULL);
	sem_destroy(&buffer_slot);
	sem_destroy(&items);
	return 0;
}
void *use(void *arg)
{
	int data;
	for (int i = 0; i < BUFFER_SIZE * 10; i++)
	{
		sem_wait(&items); 
		sem_wait(&mutex); 
		out++;
		num--;
		out %= BUFFER_SIZE;
		data = buffer[out];
		sem_post(&mutex); 
		sem_post(&buffer_slot);
		printf("use : %d \n", data); 
	}
}
void *make(void *arg)
{
	int i, data;
	for (int i = 0; i < BUFFER_SIZE * 10; i++)
	{
		printf("make : %d \n", i); 
		sem_wait(&buffer_slot); 
		sem_wait(&mutex); 
		num++;
		in++;
		in %= BUFFER_SIZE;
		buffer[in] = i;
		sem_post(&mutex); 
		sem_post(&items);
	}
}
