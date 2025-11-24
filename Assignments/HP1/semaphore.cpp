/*****************************************************************************\
*
*  Author:           Rykir Evans
*  Email:            rjevans0408@my.msutexas.edu | rykirjoe@yahoo.com
*  Title:            Honors Project: DNA Sequencing (Counting Semaphore Solution)
*  Course:           CMPS 4103 Intro to Operating Systems
*  Professor:        Dr. Nelson Passos
*  Semester:         Fall 2025
*
*  Description:
*         This program implements a multithreaded solution to computing the
*         longest common subsequence (LCS) of two random DNA sequences. By
*         default, the sequences are 10,000 characters long, and 4 threads
*         are used for the computation. This solution utilized counting
*         semaphores to complete the algorithm. See also: mutex.cpp and 
*         Testing_Code folder.
*         
*  Usage:
*         To use this program, use some standard c++ compiler such as g++ and
*         run the program. No input is required from the user, all values are
*         coded, and random generation is handled automatically.
*         
*  Files: 
*         semaphore.cpp
\******************************************************************************/

#include <iostream>
#include <chrono>
#include <map>
#include <iomanip>
#include <pthread.h>
#include <semaphore.h>
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
    srand(std::chrono::system_clock::now().time_since_epoch().count());

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
    // Heading output
    cout << "Rykir Evans" << endl
         << "DNA LCS Project" << endl
         << "CMPS 4103 - Operating Systems" << endl
         << "Fall 2025" << endl << endl;

    cout << "The resultant array is: " << endl << endl;

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
    auto startTime = chrono::high_resolution_clock::now();
    
    pthread_t threads[NUM_THREADS];
    int status;
    
    // Counting semaphores
    sem_init(&zeroColSem, 0, 0);
    sem_init(&oneColSem, 0, 0);
    sem_init(&twoColSem, 0, 0);

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
        pthread_join(threads[i], NULL);
    }

    // Print final section of the array.
    printArrSection(MATRIX_DIM - 10, MATRIX_DIM, MATRIX_DIM - 10, MATRIX_DIM);

    // Execution Time
    // Timer Calculation and output
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = endTime - startTime;

    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(duration);
    if(milliseconds.count() > 0)
        cout << "Execution time: " << milliseconds.count() 
             << " milliseconds" << endl;
    else
        // Sometimes the time is negative, so I put in error display
        cout << "Error calculating execution time: " 
             << "Result is negative, please try again" << endl;
}