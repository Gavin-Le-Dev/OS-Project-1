#include <time.h>
#include <stdio.h>
#include <sys/time.h>

int main()
{
    unsigned int NUM_TRIAL = 10000000;
    unsigned int i, timeInterval;
    struct timeval temp;
    struct timespec begin, end;

    clock_gettime(CLOCK_MONOTONIC, &begin);
    for (i = 0; i < NUM_TRIAL; ++i) {
        gettimeofday(&temp, NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    timeInterval = (end.tv_sec - begin.tv_sec)*1e9 + (end.tv_nsec - begin.tv_nsec);
    printf("Number of Calls = %u\n", NUM_TRIAL);
    printf("Time Start = %.2f ns\n", begin.tv_sec*1e9 + begin.tv_nsec);
    printf("time End = %.2f ns\n", end.tv_sec*1e9 + end.tv_nsec);
    printf("Time Interval = %.2u ns\n", timeInterval);
    printf("The cost of System Call: %.3lf ns/trial \n", (double)timeInterval/(double)NUM_TRIAL);

    return 0;
}