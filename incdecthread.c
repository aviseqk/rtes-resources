#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sched.h>
#include <stdatomic.h>

#define COUNT 1000

typedef struct
{
	int threadIdx;	
} threadParams_t;


/* POSIX Threads Declarations and Scheduling Attributes*/
pthread_t threads[2];
threadParams_t threadParams[2];

int gsum = 0; // unsafe global variable
atomic_int agsum = 0; // safer atomic global variable

void *incThread(void *threadp){
	int i;
	threadParams_t *threadParams = (threadParams_t *)threadp;

	for(i=0; i <COUNT;i++){
		gsum += i;
		agsum += i;
		printf("Increment Thread idx =%d, gsum=%d, agsum=%d\n", threadParams->threadIdx,gsum, agsum);
	}
	return (void *)0;
}

void *decThread(void *threadp){
	int i;
	threadParams_t *threadParams = (threadParams_t *)threadp;

	for(i=0;i<COUNT;i++){
		gsum -=i;
		agsum -=i;
		printf("Decrement Thread idx=%d, gsum=%d, agsum=%d\n", threadParams->threadIdx,gsum, agsum);
	}
	return (void *)0;
}

int main(int argc, char *argv[]){
	int i=0;

	threadParams[i].threadIdx = i;
	pthread_create(&threads[i], (void *)0, incThread, (void *)(&threadParams[i]));
	i++;
	pthread_create(&threads[i], (void *)0, decThread, (void *)(&threadParams[i]));;

	for(i=0;i<2;i++){
		pthread_join(threads[i], NULL);
	}
	printf("Test Complete\n");
}
