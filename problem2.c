#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *join_thread(void *arg)
{
	pthread_exit(arg); 
}
int main(int argc, char *argv[])
{
	pthread_t tid;
	int arg, status;
	int sum = 0;
	void *result;		
	arg = atoi(argv[1]);
	for(int i=0; i<arg; i++) {
		status = pthread_create(&tid, NULL, join_thread, (void *)i);
		status = pthread_join(tid, &result);
		sum += (int)result;
		printf("sum = %d\n",sum);
	}
	printf("Total : %d \n",sum);
	return sum;
}
