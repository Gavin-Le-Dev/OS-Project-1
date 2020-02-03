#define _GNU_SOURCE
#include <stdio.h>	
#include <stdlib.h>	
#include <time.h>	
#include <string.h>
#include <sched.h>

#define STRING_SIZE 100
#define NUM_TRIAL 10000

int main() 
{ 
	//Create variables
	struct timespec begin, end;
	unsigned int i, timeInterval;

	//This variable is used to store parent and child ID for testing purpose
	int childID, parentID;

	//Create string to passing between parent and child	
	char stringParent[STRING_SIZE] = "This message from parent to child";
	char stringChild[STRING_SIZE] = "This message from child to parent";

	//Create parent and child pipe with two ends, READ and WRITE
	//Where 0 is READ end and 1 is WRITE end.
	int parentPid[2];
	int childPid[2];
	//Connect two pipe together.
	pipe(parentPid);
	pipe(childPid);
	//Start the processor
	pid_t pid = fork();

	//If pid < 0 pipe is fail
	if (pid < 0) {
		fprintf(stderr, "PIPE FAILED\n");
        exit(1);
	}

	//Create cpu_set name mask
	cpu_set_t mask;
	//Clear cpu
	CPU_ZERO (&mask);
	//Set cpu at 0
	CPU_SET (0, &mask);
	//Get a process's CPU affinity mask
	sched_setaffinity(pid, sizeof(cpu_set_t), &mask);
	
	//If pid < 0, fork() is fail
	if(pid < 0){
		fprintf(stderr,"***ERROR: forking child process failed.\n");
		exit(1);
	}
	else {
		//Start timing by store current time as beginning.
		clock_gettime(CLOCK_MONOTONIC, &begin);
		//Start the loop, run for 10,000 times
		for (i = 0; i < NUM_TRIAL; i++){
			//if pid = 0, this is Child process
			if (pid == 0) {		
				//Get child ID
				childID = getpid();
				//Create string variable to read string from parent
				char receivedString[STRING_SIZE];
				//Close child Write end to start READING process
				close(childPid[1]);
				//Read message from Parent
				read(childPid[0], receivedString, STRING_SIZE);
				//Close Read end
				close(childPid[0]);
				
				//Close parent reading end to start WRITING process
				close(parentPid[0]);
				//Write message to parent
				write(parentPid[1], stringParent, strlen(stringParent)+1);
				//Close parent writing end
				close(parentPid[1]);

				//Test output:
				//printf("Child ID is %d, I receive this message from Parent = %s\n", childID, receivedString);
			}
			//Else, this is Parent process
			else {
				//Get parent ID
				parentID = getpid();
				//Create string variable to read string from child
				char receivedString[STRING_SIZE];

				//Close child reading end to start WRITING process
				close(childPid[0]);
				//Write message to child
				write(childPid[1], stringChild, strlen(stringChild)+1);
				//Close child writing end
				close(childPid[1]);

				//Close parent writing end to start READING process
				close(parentPid[1]);
				//Read message from child
				read(parentPid[0], receivedString, STRING_SIZE);
				//Close Parent reading end
				close(parentPid[0]);

				//Test output
				//printf("Parent ID is %d, I receive this message from Child = %s\n",parentID, receivedString);
			}
			//Stop the clock by recording the ending time
			clock_gettime(CLOCK_MONOTONIC, &end);
		}
	} 
	//Subtract ending time to beginning time to get time Interval
	timeInterval = (end.tv_sec - begin.tv_sec)*1e9 + (end.tv_nsec - begin.tv_nsec);
	//Print out result
    printf("Time Interval = %.2u ns\n", timeInterval);
	//Divide time interval by number of trials to find the cost fo Context Switch
	printf("The cost of Context Switch is %.3lf ns/trial\n", (double)timeInterval/(double)NUM_TRIAL);
    return 0;
} 