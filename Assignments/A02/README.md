## A02 - POSIX Thread
### Rykir Evans
### Description:

This program introduces the POSIX thread, allowing us to run the same function multiple times concurrently. This specific program generates a random 100x100x20 3D array with each index containing a random 1-digit number 0-9, and keeps track of the true sum of all of these indicies. The main portion creates 5 threads which iterate through a certain portion of the array adding to the global variable `sum` and outputs this value.

The program ends after one iteration and must be executed again to perform another display. Results may vary from run to run because the children are not required to wait on one another. 

### Usage
For WSL, the following instructions may be used to run the program:
1. Navigate to the directory of the program
2. Use the g++ compiler or any standard C++ compiler `g++ main.cpp -o main`
3. Run the program using `./main`

#### Note
The output will be the true sum of each index in the array followed by the output of the `sum` variable, which may vary wildly from run to run because of multithreading. However, With  the implementation of mutex semaphores, the code works as intended. You may comment the mutex lock and unlock functions in the `sum_array()` function to see this behavior.