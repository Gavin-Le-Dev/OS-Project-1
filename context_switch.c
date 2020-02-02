#define _GNU_SOURCE
#include <stdio.h>	
#include <stdint.h>	
#include <stdlib.h>	
#include <time.h>	
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sched.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>


#define STRING_SIZE 100
#define NUM_TRIAL 10000

int main() 
{ 
	struct timespec begin, end;
	unsigned int i, timeInterval;

	char stringParent[STRING_SIZE] = "This message from parent to child";
	char stringChild[STRING_SIZE] = "This message from child to parent";

	int parentPid[2];
	int childPid[2];
	int childID, parentID;
	pipe(parentPid);
	pipe(childPid);

	pid_t pid = fork();

	if (pid < 0) {
		fprintf(stderr, "FORK FAILED\n");
        exit(1);
	}

	cpu_set_t mask;
	CPU_ZERO (&mask); 
	CPU_SET (0, &mask);
	sched_setaffinity(pid, sizeof(cpu_set_t), &mask);
	
	clock_gettime(CLOCK_MONOTONIC, &begin);
	if(pid < 0){
		fprintf(stderr,"***ERROR: forking child process failed.\n");
		exit(1);
	}
	else {
		for (i = 0; i < NUM_TRIAL; i++){
			if (pid == 0) {		
				childID = getpid();
				char receivedString[STRING_SIZE];

				close(childPid[1]);
				read(childPid[0], receivedString, STRING_SIZE);
				close(childPid[0]);

				close(parentPid[0]);
				write(parentPid[1], stringParent, strlen(stringParent)+1);
				close(parentPid[1]);

				//printf("Child ID is %d, I receive this message from Parent = %s\n", childID, receivedString);
			}
			else {
				parentID = getpid();
				char receivedString[STRING_SIZE];

				close(childPid[0]);
				write(childPid[1], stringChild, strlen(stringChild)+1);
				close(childPid[1]);

				close(parentPid[1]);
				read(parentPid[0], receivedString, STRING_SIZE);
				close(parentPid[0]);

				//printf("Parent ID is %d, I receive this message from Child = %s\n",parentID, receivedString);
			}
			clock_gettime(CLOCK_MONOTONIC, &end);
		}
	} 
	
	timeInterval = (end.tv_sec - begin.tv_sec)*1e9 + (end.tv_nsec - begin.tv_nsec);
    printf("Time Interval = %.2u ns\n", timeInterval);
	printf("The cost of Context Switch is %.3lf ns/trial\n", (double)timeInterval/(double)NUM_TRIAL);
    return 0;
} 