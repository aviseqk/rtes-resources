/* 
 * This program is a minimal example of how two processes
 * do talk in a synchronized environment
 * */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define TRUE (1)
#define FALSE (0)


int main()
{
	int chPID; //child PID
	int stat;  //used by parent wait
	int rc;    //return code
	int i=0;
	pid_t thisChPID;
	sem_t *syncSemC, *syncSemP;
	char syncSemCName[]="/twoprocCsync";
	char syncSemPName[]="/twoprocPsync";
	
	printf("two procs\n");

	// setup synchronizing before fork
	syncSemC = sem_open(syncSemCName, O_CREAT, 0700, 0);
	printf("two procs syncSemC done\n");
	syncSemP = sem_open(syncSemPName, O_CREAT, 0700, 0);
	printf("two procs syncSemP done\n");
	printf("two procs semaphores setup, calling the fork\n");

	if((chPID = fork()) == 0) // this is the child
	{
		while(i<3)
		{
			printf("Child: taking syncC semaphore\n");
			if(sem_wait(syncSemC) < 0) perror("sem_wait syncSemC Child");
			printf("Child: got syncC semaphore\n");

			printf("Child: posting syncP semaphore\n");
			if(sem_post(syncSemP) < 0) perror("sem_post syncSemP Child");
			printf("Child: posted syncP Parent semaphore\n");

			i++;										}
		printf("Child is closing down\n");
		if(sem_close(syncSemP) < 0) perror("sem_close syncSemP Child");
		if(sem_close(syncSemC) < 0) perror("sem_close syuncSemC Child");
		exit(0);

	}
	else // this is the parent
	{
		while(i<3)
		{
			printf("Parent: posting syncC semaphore\n");
			if(sem_post(syncSemC) < 0) perror("sem_post syncSemC Parent");
			printf("Parent: posted syncC semaphore\n");

			printf("Parent: taking syncP semaphore\n");
			if(sem_wait(syncSemP) < 0) perror("sem_wait syncSemP Parent");
			printf("Child: got syncP semaphore\n");

			i++;		
		}
		// Now wait for the child to terminate
		printf("Parent waiting for the child to terminate\n");

		thisChPID = wait(&stat);

		printf("Parent is closing down\n");
		if(sem_close(syncSemP) < 0) perror("sem_close syncSemP Parent");
		if(sem_close(syncSemC) < 0) perror("sem_close syuncSemC Parent");
		
		if(sem_unlink(syncSemCName) < 0) perror("sem_unlink syncSemCName");
		if(sem_unlink(syncSemPName) < 0) perror("sem_unlink syncSemPName");


		exit(0);

	
	}
}







