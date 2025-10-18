/*****************************************************************************\
*
*  Author:           Rykir Evans
*  Email:            rjevans0408@my.msutexas.edu | rykirjoe@yahoo.com
*  Title:            POSIX Fork
*  Course:           CMPS 4103 Intro to Operating Systems
*  Professor:        Dr. Nelson Passos
*  Semester:         Fall 2025
*
*  Description:
*         This program introduces the POSIX thread, allowing us to run
*         the same function multiple times concurrently. This specific
*         program generates a random 100x100x20 3D array with each index
*         containing a random 1-digit number 0-9, and keeps track of the
*         true sum of all of these indicies. The main portion creates
*         5 threads which iterate through a certain portion of the array
*         adding to the global variable `sum` and outputs this value.
*         
*  Usage:
*         To use this program, use some standard C++ compiler such a g++,
*         and simply execute the program.
*         Ex: `g++ main.cpp -o main` and `./main`.
*         The output will be the true sum of each index in the array
*         followed by the output of the `sum` variable, which will vary
*         wildly from run to run because of multithreading. With the
*         implementation of mutex semaphores, the code works as intended.
*         
*  Files: 
*         main.cpp
\******************************************************************************/

#include <pthread.h> // Needed for thread creation
#include <iostream>
#include <random>

#define NUM_THREADS 5

using namespace std;

// Global variables
int trueSum = 0;
int sum = 0;
int A[100][100][20];
pthread_mutex_t mutex;

/*
    * Public : generate_array()
    *
    * Description:
    *      Initializes a 3D array of dimensions 100x100x20
    *      with random numbers 0-9 for each index and
    *      also updates the trueSum variable
    *
    * Params:
    *     None
    *
    * Returns:
    *     None
*/
void generate_array()
{
    // 3D foor loop
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            for(int k = 0; k < 20; k++)
            {
                // Generate random number
                int temp = rand() % 10;

                // Store it in current index
                A[i][j][k] = temp;

                // Update trueSum
                trueSum += temp;
            }
        }
    }
}

/*
    * Public : *sum_array
    *
    * Description:
    *      Initializes a new round by resetting letter checks, bad guesses,
    *      and generating a new category and word.
    *
    * Params:
    *     void * tid - The id of the thread, in address form
    *
    * Returns:
    *     None
*/
void *sum_array(void * tid)
{
    // Cast the thread id to an integer: 0-4
    int *id_ptr = (int *)tid;

    // Iterate through each section using tids 0-4
    for(int i = *id_ptr * 20; i < (*id_ptr + 1) * 20; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            for(int k = 0; k < 20; k++)
            {
                // Lock the mutex to ensure exclusive access
                // Commenting this and the unlock function result
                // In the sum varying from run to run
                pthread_mutex_lock(&mutex);
                sum += A[i][j][k];
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    return NULL;
}

int main()
{
    // Create the array containing the threads
    pthread_t threads[NUM_THREADS];
    int status;

    // Generate the 3D array
    generate_array();

    // Show the true tracked sum of the array
    cout << "True Sum = " << trueSum << endl;

    // Create the threads
    for(int i = 0; i < NUM_THREADS; i++)
    {
        // Store i as a pointer to pass to the threaded function
        // since it cannot take pure integers
        int *arg = new int;
        *arg = i;
        
        // Generate the threads, store in array, and assign their func
        status = pthread_create(&threads[i], NULL, sum_array, arg);

        if(status != 0)
        {
            cout << "Error from pthread_create" << endl;
            exit(-1);
        }
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        // Use join function to ensure threads wait on each other.
        // Otherwise sum would output before they finish running.
        // It is also crucial that it has its own for loop, otherwise
        // the various threads run sequentially, and the mutex becomes
        // useless
        pthread_join(threads[i], NULL);
    }

    // Show the sum added by all the threads
    cout << "Sum = " << sum << endl;
    return 0;
}

// There definitely was a difference between using semaphores and not.
// Without the mutex locking, the value of `sum` varies exceptionally
// between runs, resulting in false values. I tried to condensethe code
// by leaving the pthread_join() function in the main for loop, but then
// through troubleshooting, I learned that the mutex became useless afer
// this, so I figured it's own for loop was necessary, as the threads
// would run sequentially otherwise.

