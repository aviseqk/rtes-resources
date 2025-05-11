#include <syslog.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define COUNT 20

void main(void)
{
	struct timespec ts;
	double fnow;
	int i=0;
	
	for (i; i < COUNT; i++)
	{
		if(clock_gettime(CLOCK_REALTIME, &ts) == -1) {
			perror("clock_gettime");
			exit(-1);
		}
		printf("PID: %d, seconds: %ld , microseconds: %ld\n", getpid(), ts.tv_sec, ts.tv_nsec);
		syslog(LOG_CRIT, "TAG: PID %d @ %ld sec %ld nanoseconds \n", getpid(), ts.tv_sec, ts.tv_nsec);
	}


}
