#include <time.h>
#include <stdio.h>
#include <sys/time.h>

int main()
{
    //Declare variable
    unsigned int NUM_TRIAL = 10000000;
    unsigned int i, timeInterval;
    struct timeval temp;
    struct timespec begin, end;

    //Get the present time and store as begin time
    clock_gettime(CLOCK_MONOTONIC, &begin);

    //Run the programe for 10,000,000 times
    for (i = 0; i < NUM_TRIAL; ++i) {
        gettimeofday(&temp, NULL);
    }

    //Get the present time and store as begin time
    clock_gettime(CLOCK_MONOTONIC, &end);

    //Subtract the end time with the begin to get the runtime of the for loop.
    timeInterval = (end.tv_sec - begin.tv_sec)*1e9 + (end.tv_nsec - begin.tv_nsec);

    //Print out the result
    printf("Number of Calls = %u\n", NUM_TRIAL);
    printf("Time Start = %.2f ns\n", begin.tv_sec*1e9 + begin.tv_nsec);
    printf("time End = %.2f ns\n", end.tv_sec*1e9 + end.tv_nsec);
    printf("Time Interval = %.2u ns\n", timeInterval);

    //Divide Time Interval by Number of Trials to get the cost of gettimeofday() System Call
    printf("The cost of System Call: %.3lf ns/trial \n", (double)timeInterval/(double)NUM_TRIAL);

    return 0;
}