/*****************************************************************************\
*
*  Description:
*         This program was used for testing to see the error rates for hundreds
*         if not thousands of runs automatically. It tracks error and execution
*         time and outputs end results of each run to the console for verifying
*         the success of the run. This version specifically runs the counting
*         semaphore solution to the problem. See also: multirun1.cpp
*         
*  Usage:
*         To interact with this program, you may change the number of times the
*         encompassing for loop in main() runs, then use any standard compiler
*         to compile and execute the code. A run count and the results of each
*         run will be output to the console for verification.
*         
\******************************************************************************/

#include <iostream>
#include <chrono>
#include <map>
#include <iomanip>
#include <pthread.h>
#include <semaphore.h>
#include <thread>
#include <string>
#include <stdlib.h>

using namespace std;

// Global variables
const int NUM_THREADS = 4;
const int SEQUENCE_LENGTH = 10000;
const int MATRIX_DIM = SEQUENCE_LENGTH + 1;
const int ROW_OFFSET = 100;
const int COL_OFFSET = 2500;

char s1[SEQUENCE_LENGTH];
char s2[SEQUENCE_LENGTH];

// Initialize array to all zeros
int seqArr[MATRIX_DIM][MATRIX_DIM] = {0};

// Map for int to char conversion
map<int, char> bases = {{0, 'A'}, {1, 'G'}, {2, 'C'}, {3, 'T'}};

// Counting semaphores
sem_t zeroColSem, oneColSem, twoColSem;

// Column finished variables
bool zeroFin = false;
bool oneFin = false;
bool twoFin = false;

int errCount = 0;

// counting_semaphore<4> sem(4);

/*
    * Public : getMax2()
    *
    * Description:
    *      Returns the larger of two integers using
    *      a ternary operator
    *
    * Params:
    *     int x - an arbitrary integer
    *     int y - an arbitrary integer
    *
    * Returns:
    *     int - Larger of the two
*/
int getMax2(int x, int y)
{
    // Return x if greater than y, otherwise return y
    return x > y ? x : y;
}

/*
    * Public : generateSeq()
    *
    * Description:
    *      Initializes 2 arrays of chars (strings) with
    *      random letters representing the nucleotides
    *      within the DNA sequence
    *
    * Params:
    *     None
    *
    * Returns:
    *     None
*/
void generateSeq()
{
    // Timing to seed random
    // srand(std::chrono::system_clock::now().time_since_epoch().count());

    // Initialize sequences randomly
    for(int i = 0; i < SEQUENCE_LENGTH; i++)
    {
        // Use map to simplify random int to char
        s1[i] = bases[rand() % 4];
        s2[i] = bases[rand() % 4];
    }
}

/*
    * Public : *findLCS
    *
    * Description:
    *      implements a multithreaded solution to the
    *      dynamic programming required to compute the
    *      longest common subsequence of any DNA sequence
    *
    * Params:
    *     void * tid - The id of the thread, in address form
    *
    * Returns:
    *     None
*/
void *findLCS(void * tid)
{
    // Cast the thread id to an integer: 0-3
    int *idPtr = (int *)tid;

    // Variables for splitting respectice array-thread sections
    int iOffset, iStop, jOffset, jStop;
    bool colFinished = false;

    // Repeat until each thread's respective colum is finished.
    for(int sectionCount = 0; sectionCount < 100; sectionCount++)
    {
        // Variables for each respective thread's section
        iOffset = (sectionCount) * ROW_OFFSET;
        iStop = (sectionCount + 1) * ROW_OFFSET;
        jOffset = *idPtr * COL_OFFSET;
        jStop = (*idPtr + 1) * COL_OFFSET;

        // Failsafe to avoid OOB indexing
        if(iOffset == 0) iOffset++;
        if(iStop > MATRIX_DIM) iStop = MATRIX_DIM;
        if(jStop > MATRIX_DIM) jStop = MATRIX_DIM;
        
        // All threads other than thread0 need to wait
        if(*idPtr > 0)
        {            
            // Wait until previous thread has computed its entire section    
            {
                switch(*idPtr)
                {
                    case 1:
                        sem_wait(&zeroColSem);
                        break;
                    case 2:
                        sem_wait(&oneColSem);
                        break;
                    case 3:
                        sem_wait(&twoColSem);;
                        break;
                }
            }
        }

        ////////////////////////////
        //   Main LCS Algorithm   //
        ////////////////////////////
        for(int i = iOffset; i < iStop + 1; i++)
        {
            for(int j = jOffset; j < jStop + 1; j++)
            {
                // s1 uses j since it is labeled as the column
                // Conversely, s2 uses i since it represents the row
                if(s1[j - 1] == s2[i - 1])
                    seqArr[i][j] = 1 + seqArr[i-1][j-1];
                else
                    seqArr[i][j] = getMax2(seqArr[i-1][j], seqArr[i][j-1]);
            }
        }

        // Increment column semaphore to signal next thread
        switch(*idPtr)
        {
            case 0:
                sem_post(&zeroColSem);
                break;
            case 1:
                sem_post(&oneColSem);
                break;
            case 2:
                sem_post(&twoColSem);
                break;
            default:
                break;
        }
    }

    if(*idPtr == 3)
    {
        cout << seqArr[10000][10000] << endl;
        if (seqArr[10000][10000] != 6537) errCount++; 
    }
    return NULL;
}

/*
    * Public : printArrSection()
    *
    * Description:
    *      Prints a designated section of the global
    *      array. Bounds passed in should be used
    *      starting at index 1
    *
    * Params:
    *     int startRow - the starting value for row iteration
    *     int stopRow  - the stopping value for row iteration
    *     int startcol - the starting value for col iteration
    *     int stopCol  - the stopping value for col iteration
    *
    * Returns:
    *     None
    *     Outputs to the console
*/
void printArrSection(int startRow, int stopRow, int startCol, int stopCol)
{
    // Displaying based on length of each number
    int colWidth = to_string(seqArr[startRow][startCol]).length();

    // Column Headers & Print formatting
    cout << "  ";
    for(int i = startCol; i < stopCol; i++)
    {
        // Set width depending on how long the number is
        cout << setw(colWidth) << s1[i - 1] << " ";
    }
    cout << endl;

    // Main table body
    for(int i = startRow; i < stopRow; i++)
    {
        // Row headers
        cout << s2[i - 1] << " ";
        for(int j = startCol; j < stopCol; j++)
        {
            // Array at index  
            cout << setw(colWidth) << seqArr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    // Randomly generate 2 DNA sequences
    generateSeq();
    

    // Start timer after array is generated so the time taken to do that
    // is not included
    
    pthread_t threads[NUM_THREADS];
    int status;

    for(int temp = 0; temp < 250; temp++)
    {
    
    cout << endl << "Run: " << temp << endl;
    // pthread_mutex_init(&mutex, 0);
    // pthread_cond_init(&condOne, 0);
    // pthread_cond_init(&condTwo, 0);
    // pthread_cond_init(&condThree, 0);

    //TEMP
    zeroFin = oneFin = twoFin = false;

    auto startTime = chrono::high_resolution_clock::now();

    for(int i = 0; i < MATRIX_DIM; i++)
    {
        for(int j = 0; j < MATRIX_DIM; j++)
        {
                seqArr[i][j] = 0;
        }
    }
    
    // Create the threads
    for(int i = 0; i < NUM_THREADS; i++)
    {
        // Store i as a pointer to pass to the threaded function
        // since it cannot take pure integers
        int *arg = new int;
        *arg = i;
        
        // Generate the threads, store in array, and assign their func
        status = pthread_create(&threads[i], NULL, findLCS, arg);

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

    // Timer Calculation and output
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = endTime - startTime;

    auto milliseconds = chrono::duration_cast<std::chrono::milliseconds>(duration);
    cout << "Execution time: " << milliseconds.count() << " milliseconds" << std::endl;


    }

    printArrSection(9990, 10001, 9990, 10001);

    // pthread_cond_destroy(&condOne);
    // pthread_cond_destroy(&condTwo);
    // pthread_cond_destroy(&condThree);
    // pthread_mutex_destroy(&mutex);

    // TEMP
    cout << "S1 final 10: ";
    for(int i = 9990; i < 10000; i++)
    {
        cout << s1[i] << " ";
    }
    cout << endl << "S2 final 10: ";
    for(int i = 9990; i < 10000; i++)
    {
        cout << s2[i] << " ";
    }
    cout << endl;

    // Execution Time
    
    cout << "Bottom right: " << seqArr[10000][10000] << endl;
    cout << "Error count: " << errCount << endl;

}