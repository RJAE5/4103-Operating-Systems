/*****************************************************************************\
*
*  Description:
*         This program was used for troubleshooting when first developing the
*         algorithm. It contains a lot of console output for debugging purposes
*         and still is not 100% accurate (see note below). This is solely 
*         preserved to showcase trial and error of this project.
*         
*  Usage:
*         To interact with this program, you may compile and run, and view
*         the console statements to see how the threads are actually processing
*         their work.
*         
\******************************************************************************/

// NOTE: Very rarely, this code computes the LCS to be slightly shorter (by no more than 4)
// than what it actually meant to be, for example, it may compute 6533 rather than 6536
// in the non-randomized version.
#include <iostream>
#include <chrono>
#include <map>
#include <iomanip>
#include <pthread.h>
#include <string>

using namespace std;

// Global variables
const int NUM_THREADS = 4;
const int SEQUENCE_LENGTH = 10000;
const int MATRIX_DIM = SEQUENCE_LENGTH + 1;
const int ROW_OFFSET = 100;
const int COL_OFFSET = 2500;

int threadBase = 0;

char s1[SEQUENCE_LENGTH];
char s2[SEQUENCE_LENGTH];

// Initialize array to all zeros
int seqArr[MATRIX_DIM][MATRIX_DIM] = {0};

// Map for int to char conversion
map<int, char> bases = {{0, 'A'}, {1, 'G'}, {2, 'C'}, {3, 'T'}};

pthread_mutex_t mutex;
pthread_cond_t condOne, condTwo, condThree;

bool zeroFin = false;
bool oneFin = false;
bool twoFin = false;

// Temp prototype, delete later
void printArrSection(int iOffset, int iStop, int jOffset, int jStop);

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
    *      Initializes 2 character arrays (string) with
    *      random letters representing the nucleotides
    *      within the sequence
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
// void *tempFindLCS(void * tid)
// {
//         // Cast the thread id to an integer: 0-3
//     int *idPtr = (int *)tid;

//     cout << "findLCS called by thread: " << *idPtr << endl;

//     int iOffset = (*idPtr + threadBase) * ROW_OFFSET;
//     int iStop = (*idPtr + threadBase + 1) * ROW_OFFSET;
//     int jOffset = *idPtr * COL_OFFSET;
//     int jStop = (*idPtr + 1) * COL_OFFSET;

//     // Clamp to matrix dimensions
//     if(iStop > MATRIX_DIM) iStop = MATRIX_DIM;
//     if(jStop > MATRIX_DIM) jStop = MATRIX_DIM;

//     cout << "Values for thread " << *idPtr << ": " << iOffset << " " << iStop << " " << jOffset << " " << jStop << endl;

//     // Wait for previous thread's column to be ready
//     if(*idPtr > 0)
//     {
//         cout << "Thread " << *idPtr << " is waiting" << endl;
//         pthread_mutex_lock(&mutex);
        
//         // Wait until previous thread has computed its column section
//         while(threadBase < *idPtr)
//         {
//             switch(*idPtr)
//             {
//                 case 1:
//                     pthread_cond_wait(&condOne, &mutex);
//                     break;
//                 case 2:
//                     pthread_cond_wait(&condTwo, &mutex);
//                     break;
//                 case 3:
//                     pthread_cond_wait(&condThree, &mutex);
//                     break;
//             }
//         }
//         pthread_mutex_unlock(&mutex);
//         cout << "Thread " << *idPtr << " received signal" << endl;
//     }

//     cout << "For loop entered by thread: " << *idPtr << endl;

//     for(int i = 1 + iOffset; i < iStop && i < 10000; i++)
//     {
//         for(int j = 1 + jOffset; j < jStop && j < 10000; j++)
//         {
//             // NOTE: This cout statement seems to cause an infinite loop saying thread
//             // 0 is in the for loop, but not having it there causes it to act like normal.
//             // cout << "Thread " << *idPtr << " is currently in the for loop" << endl;
//             // s1 uses j since it is labeled as the column
//             // Conversely, s2 uses i since it represents the row
//             if(s1[j - 1] == s2[i - 1])
//                 seqArr[i][j] = 1 + seqArr[i-1][j-1];
//             else
//                 seqArr[i][j] = getMax2(seqArr[i-1][j], seqArr[i][j-1]);
//         }
//     }

//     cout << "For loop completed by thread: " << *idPtr << endl;

//     pthread_mutex_lock(&mutex);
//     threadBase += 1;
    
//     switch(*idPtr)
//     {
//         case 0:
//             cout << "Signaling thread 1" << endl;
//             pthread_cond_signal(&condOne);
//             break;
//         case 1:
//             cout << "Signaling thread 2" << endl;
//             pthread_cond_signal(&condTwo);
//             break;
//         case 2:
//             cout << "Signaling thread 3" << endl;
//             pthread_cond_signal(&condThree);
//             break;
//         case 3:
//             break;
//     }
//     pthread_mutex_unlock(&mutex);

// }
void *findLCS(void * tid)
{
    int *idPtr = (int *)tid;
    int iOffset, iStop, jOffset, jStop;
    int rowCount = 0;
    bool breakFlag = false;

    while(seqArr[MATRIX_DIM - 1][MATRIX_DIM - 1] == 0 && !breakFlag)
    {
    
        // Cast the thread id to an integer: 0-3
        

        // cout << "findLCS called by thread: " << *idPtr << endl;

        iOffset = (rowCount) * ROW_OFFSET;
        iStop = (rowCount + 1) * ROW_OFFSET;
        jOffset = *idPtr * COL_OFFSET;
        jStop = (*idPtr + 1) * COL_OFFSET;

        if(iOffset == 0) iOffset++;
        if(iStop > MATRIX_DIM) iStop = MATRIX_DIM;
        if(jStop > MATRIX_DIM) jStop = MATRIX_DIM;
        

        cout << "Values for thread " << *idPtr << ": " << iOffset << " " << iStop << " " << jOffset << " " << jStop << endl;

        if(*idPtr > 0)
        {
            cout << "Thread " << *idPtr << " is waiting" << endl;
            pthread_mutex_lock(&mutex);
            
            // Wait until previous thread has computed its column section
            
            //////////////////////////////////////////////////////////////////////////////
            // TODO: This conditional is what I think is breaking the boundaries, it
            // needs to be refined.
            //////////////////////////////////////////////////////////////////////////////
            
            {
                switch(*idPtr)
                {
                    case 1:
                        while(!zeroFin && seqArr[iOffset][jOffset - 1] == 0)
                            pthread_cond_wait(&condOne, &mutex);
                        // cout << "Value of arr from thread: " << *idPtr << " :" << seqArr[1][jStop - 1] << endl;
                        break;
                    case 2:
                        while(!oneFin && seqArr[iOffset][jOffset - 1] == 0)
                            pthread_cond_wait(&condTwo, &mutex);
                        // cout << "Value of arr from thread: " << *idPtr << " :" << seqArr[1][jStop - 1] << endl;
                        break;
                    case 3:
                        while(!twoFin && seqArr[iOffset][jOffset - 1] == 0)
                            pthread_cond_wait(&condThree, &mutex);
                        // cout << "Value of arr from thread: " << *idPtr << " :" << seqArr[1][jStop - 1] << endl;
                        break;
                }
            }
        }
        // else
        // {
        //     iOffset += 1;
        // }


        ///////////////////////////////////////////////////////////////////////////////////
        // TODO: This switch case currently causes an infinite loop somehow, and need
        //       to be fixed. The commented out version above seems to work just fine,
        //       in terms of not looping, but the only real difference is the conditionals,
        //       needs more research into why this is happening.
        ///////////////////////////////////////////////////////////////////////////////////

        // pthread_mutex_lock(&mutex);
        // switch(*idPtr)
        // {
        //     case 0:
                
        //         break;
        //     case 1:
        //         cout << "Thread 1 is waiting" << endl;
        //         // pthread_mutex_lock(&mutex);
        //         while(seqArr[1][jStop - 1] == 0)
        //             pthread_cond_wait(&condOne, &mutex);
        //         cout << "Thread 1 received signal" << endl;
        //         break;
        //     case 2:
        //         cout << "Thread 2 is waiting" << endl;  
        //         // pthread_mutex_lock(&mutex);
        //         while(seqArr[1][jStop - 1] == 0)
        //             pthread_cond_wait(&condTwo, &mutex);
        //         cout << "Thread 2 received signal" << endl;
        //         break;
        //     case 3:
        //         cout << "Thread 3 is waiting" << endl;
        //         // pthread_mutex_lock(&mutex);
        //         while(seqArr[1][jStop - 1] == 0)
        //             pthread_cond_wait(&condThree, &mutex);
        //         cout << "Thread 3 received signal" << endl;
        //         break;
        //     default:
        //         cout << "Error with thread id conversion." << endl;
        // }

        pthread_mutex_unlock(&mutex);

        cout << "For loop entered by thread: " << *idPtr << endl;

        for(int i = iOffset; i < iStop; i++)
        {
            // cout << "Thread " << *idPtr << " is currently in the for loop" << endl;

            for(int j = jOffset; j < jStop; j++)
            {
                // s1 uses j since it is labeled as the column
                // Conversely, s2 uses i since it represents the row
                if(s1[j - 1] == s2[i - 1])
                    seqArr[i][j] = 1 + seqArr[i-1][j-1];
                else
                    seqArr[i][j] = getMax2(seqArr[i-1][j], seqArr[i][j-1]);
            }
        }

        cout << "For loop completed by thread: " << *idPtr << endl;

        pthread_mutex_lock(&mutex);
        threadBase += 1;
        
        switch(*idPtr)
        {
            case 0:
                cout << "Signaling thread 1" << endl;
                // cout << "Value of arr from thread: " << *idPtr << " :" << seqArr[1][jOffset - 1] << endl;
                pthread_cond_signal(&condOne);
                break;
            case 1:
                cout << "Signaling thread 2" << endl;
                // cout << "Value of arr from thread: " << *idPtr << " :" << seqArr[101][jStop - 1] << endl;
                pthread_cond_signal(&condTwo);
                break;
            case 2:
                cout << "Signaling thread 3" << endl;
                // cout << "Value of arr from thread: " << *idPtr << " :" << seqArr[1][jStop - 2] << endl;

                pthread_cond_signal(&condThree);
                break;
            case 3:
                break;
        }
        pthread_mutex_unlock(&mutex);

        rowCount++;


        cout << "Final switch case completed by thread: " << *idPtr << endl;

        cout << "rowCount from thread " << *idPtr << ": " << rowCount << endl;
        
        cout << "Bottom corner: " << seqArr[10000][10000] << endl;

        // int starting = (rowCount) * 100 + 95;
        // int stopping = starting + 10;

        if(rowCount > 99) 
        {
            cout << "Thread " << *idPtr << " is ready to break!" << endl;
            breakFlag = true;
        }

        // printArrSection(starting, stopping, 2495, 2505);


}

    cout << "While loop broken by thread: " << *idPtr << "!!!!" << endl;
    switch(*idPtr)
    {
        case 0:
            zeroFin = true;
            cout << "zeroFin changed!" << endl;
            break;
        case 1:
            oneFin = true;
            break;
        case 2:
            twoFin = true;
            break;
        case 3:
            break;
    }
    return NULL;
}

// void *findLCS(void * tid)
// {
//     // Cast the thread id to an integer: 0-3
//     int *idPtr = (int *)tid;

//     cout << "findLCS called by thread: " << *idPtr << endl;

//     int iOffset = (*idPtr + threadBase) * ROW_OFFSET;
//     int iStop = (*idPtr + threadBase + 1) * ROW_OFFSET;
//     int jOffset = *idPtr * COL_OFFSET;
//     int jStop = (*idPtr + 1) * COL_OFFSET;

//     // Clamp to matrix dimensions
//     if(iStop > MATRIX_DIM) iStop = MATRIX_DIM;
//     if(jStop > MATRIX_DIM) jStop = MATRIX_DIM;

//     cout << "Values for thread " << *idPtr << ": " << iOffset << " " << iStop << " " << jOffset << " " << jStop << endl;

//     // Wait for previous thread's column to be ready
//     if(*idPtr > 0)
//     {
//         cout << "Thread " << *idPtr << " is waiting" << endl;
//         pthread_mutex_lock(&mutex);
        
//         // Wait until previous thread has computed its column section
//         while(threadBase < *idPtr)
//         {
//             switch(*idPtr)
//             {
//                 case 1:
//                     pthread_cond_wait(&condOne, &mutex);
//                     break;
//                 case 2:
//                     pthread_cond_wait(&condTwo, &mutex);
//                     break;
//                 case 3:
//                     pthread_cond_wait(&condThree, &mutex);
//                     break;
//             }
//         }
//         pthread_mutex_unlock(&mutex);
//         cout << "Thread " << *idPtr << " received signal" << endl;
//     }

//     cout << "For loop entered by thread: " << *idPtr << endl;

//     for(int i = 1 + iOffset; i < iStop; i++)
//     {
//         for(int j = 1 + jOffset; j < jStop; j++)
//         {
//             // s1 uses j since it is labeled as the column
//             // Conversely, s2 uses i since it represents the row
//             if(s1[j - 1] == s2[i - 1])
//                 seqArr[i][j] = 1 + seqArr[i-1][j-1];
//             else
//                 seqArr[i][j] = getMax2(seqArr[i-1][j], seqArr[i][j-1]);
//         }
//     }

//     cout << "For loop completed by thread: " << *idPtr << endl;

//     // Signal next thread and update threadBase
//     pthread_mutex_lock(&mutex);
//     threadBase += 1;
    
//     switch(*idPtr)
//     {
//         case 0:
//             cout << "Signaling thread 1" << endl;
//             pthread_cond_signal(&condOne);
//             break;
//         case 1:
//             cout << "Signaling thread 2" << endl;
//             pthread_cond_signal(&condTwo);
//             break;
//         case 2:
//             cout << "Signaling thread 3" << endl;
//             pthread_cond_signal(&condThree);
//             break;
//         case 3:
//             break;
//     }
//     pthread_mutex_unlock(&mutex);

//     cout << "Final switch case completed by thread: " << *idPtr << endl;
//     cout << "ThreadBase: " << threadBase << endl;

//     return NULL;
// }


void printArrSection(int startRow, int stopRow, int startCol, int stopCol)
{
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
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&condOne, 0);
    pthread_cond_init(&condTwo, 0);
    pthread_cond_init(&condThree, 0);


    // Randomly generate 2 DNA sequences
    generateSeq();

    // Start timer after array is generated so the time taken to do that
    // is not included
    auto startTime = chrono::high_resolution_clock::now();
    pthread_t threads[NUM_THREADS];
    int status;

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

    printArrSection(9990, 10000, 9990, 10000);

    pthread_cond_destroy(&condOne);
    pthread_cond_destroy(&condTwo);
    pthread_cond_destroy(&condThree);
    pthread_mutex_destroy(&mutex);
}


    // Trying to implement the algorithm without threads
    // for(int i = 1; i < MATRIX_DIM; i++)
    // {
    //     for(int j = 1; j < MATRIX_DIM; j++)
    //     {
    //         // s1 uses j since it is labeled as the column
    //         // Conversely, s2 uses i since it represents the row
    //         if(s1[j - 1] == s2[i - 1])
    //             seqArr[i][j] = 1 + seqArr[i-1][j-1];
    //         else
    //             seqArr[i][j] = getMax2(seqArr[i-1][j], seqArr[i][j-1]);
    //     }
    // }