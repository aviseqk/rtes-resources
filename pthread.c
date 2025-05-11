#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 12

typedef struct {
	int threadIdx;
} threadParams_t;

pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

void *counterThread(void *threadp){

	int sum = 0, i;
	
	threadParams_t *threadParams = (threadParams_t *)threadp;

	for (i=0; i < (threadParams->threadIdx)+1; i++){
		sum = sum + i;
	}
	
	printf("Thread idx=%d, sum[0...%d]=%d\n", 
			threadParams->threadIdx,
			threadParams->threadIdx, sum);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	int rc, i;

	for (i=0; i < NUM_THREADS; i++) {
		threadParams[i].threadIdx = i;

		pthread_create(&threads[i],
				(void *)0,
				counterThread,
				(void *)&threadParams[i]);
	}

	for (i=0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("test complete\n");
	pthread_exit(NULL);
}
