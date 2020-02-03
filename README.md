# Measure the cost of SystemCall and Context Switch

Measure the cost of a System Call and the cost of Context Switch

## Getting Started

In this project, we test and measure the cost of system calls and the cost of context switch by the processor. For system call, we repeatedly call a system call and use timer function like gettimeofday() to measure the time cost of the loop of calls, then divide by the number of iteration for average cost. For context switch, we need to make sure to run process in the same CPU and use pipe and fork to perform context switch, then proceed to measure it.

### Prerequisites

You need to run the program on a Linux system, because there are library that wasn't supported on Windowns or Mac.

## Running the tests

-   Run *make* to compile the programs
-   Run *make runsc* to test the System Call Test
-   Run *make runcs* to test the Context Switch Test
-   Run *make clean* to remove generated files.

## Authors

* **Gavin Le** - *Initial work*
* **Aung Khaing** - *Initial work*
