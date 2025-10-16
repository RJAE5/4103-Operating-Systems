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
*         This program introduces the POSIX fork() function and utilizing
*         children to speed up the execution of a program. The output is
*         meant to resemble the room numbers regarding the Bolin Science Hall
*         remodel.
*         
*  Usage:
*         To use this program, use some standard C++ compiler such a g++,
*         and simply execute the program.
*         Ex: `g++ main.cpp -o main` and `./main`.
*         The output will be room numbers from 00-20 with the floor number
*         1-3 coming first. The order of these numbers is subject to change
*         depending on exact execution time, exhibiting a drawback of children.
*         The program ends with the parent printing `Goodbye!`
*         
*  Files: 
*         main.cpp
\******************************************************************************/

#include <pthread.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>



#define NUM_THREADS 5

using namespace std;


// void *print_hw(void * tid)
// {
//     int *id_ptr = (int *)tid;
//     cout << "Hello World. Greetings from thread " << *id_ptr << endl;
// }

int trueSum = 0;
int sum = 0;
int A[100][100][20];

void generate_array()
{
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            for(int k = 0; k < 20; k++)
            {
                A[i][j][k] = rand() % 10;
                trueSum += A[i][j][k];
            }
        }

    }
}


void *sum_array(void * tid)
{
    int *id_ptr = (int *)tid;
    for(int i = *id_ptr * 20; i < (*id_ptr + 1) * 20 - 1; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            for(int k = 0; k < 20; k++)
            {
                sum += A[i][j][k];
            }
        }
    }

}

int main()
{
    pthread_t threads[NUM_THREADS];
    int status;

    generate_array();




    cout << "True Sum = " << trueSum << endl;

        for(int i = 0; i < NUM_THREADS; i++)
    {
        int *arg = new int;
        *arg = i;

        cout << "Main here, creating thread " << i << endl;
        status = pthread_create(&threads[i], NULL, sum_array, arg);

        if(status != 0)
        {
            cout << "Error from pthread_create" << endl;
            exit(-1);

        }
        
    }

    cout << "Sum = " << sum << endl;


    // for(int i = 0; i < NUM_THREADS; i++)
    // {
    //     cout << "Main here, creating thread " << i << endl;
    //     status = pthread_create(&threads[i], NULL, print_hw, (void *)i);

    //     if(status != 0)
    //     {
    //         cout << "Error from pthread_create" << endl;
    //         exit(-1);

    //     }
        
    // }
    // return 0;

}