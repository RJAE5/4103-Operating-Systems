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
*         meant to resemble the room numbers the regarding Bolin Science Hall
*         remodel.
*         
*  Usage:
*         To use this program, use some standard C++ compiler such a g++,
*         and simply execut the program.
*         Ex: `g++ main.cpp -o main` and `./main`.
*         The output will be room numbers from 00-20 with the floor number
*         1-3 coming first. The order of these numbers is subject to change
*         depending on exact execution time, exhibiting a drawback of children.
*         The program ends with the parent printing `Goodbye!`
*         
*  Files: 
*         main.cpp
\******************************************************************************/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    // Initial child creation
    int pid0 = fork();
    
    if(pid0 != 0)
    {
        // Original parent code (Containing 1st floor & Goodbye)

        // Parent creates an additional child using pid1
        int pid1 = fork(); 

        if(pid1 != 0)
        {
            // Parent code, waiting for all children to finish before saying goodby
            waitpid(-1, NULL, 0);
            cout << "Goodbye!" << endl;
        }
        else
        {
            // pid1 Child code for printing the 1st floor numbers
            for(int i = 0; i <= 20; i++)
            {cout << 100 + i << " ";}
            cout << endl;
        }
    } // End parent
    else
    {
        // Initial child code (pid0)
        int pid2 = fork(); // Create 3rd and final child (pid2)
        if(pid2 != 0)
        {
            // Initial child printing 2nd floor (pid0)
            for(int i = 0; i <= 20; i++)
            {cout << 200 + i << " ";}
            cout << endl;
        }
        else
        {
            // 3rd child code printing 3rd floor numbers
            for(int i = 0; i <= 20; i++)
            {cout << 300 + i << " ";}
            cout << endl;
        }
    } // End initial child
} // End main