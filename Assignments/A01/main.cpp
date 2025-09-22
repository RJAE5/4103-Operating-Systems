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

#include <iostream>
#include <unistd.h> // Needed for fork()
#include <sys/wait.h> // Needed for waitpid()

using namespace std;

void printFloorNums(int floor)
{
    // Note: If flush were added, it would make the floor number output jarringly disorganized.
    // I learned this from ChatGPT with the prompt asking why the same floor was reliably
    // on the same line instead of mixed around. It indicated that the lack of occasionally
    // flushing caused each cout buffer to wait until the endl, which would mean the output
    // buffer was completed with the desired output by the time it was flushed.

    // I decided to comment it out because it was easier for testing to verify the ordered version
    for(int i = 0; i <= 20; i++)
        cout << 100 * floor + i << " "; // << flush; 
    cout << endl;
}

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
            printFloorNums(1);
        }
    } // End parent
    else
    {
        // Initial child code (pid0)
        int pid2 = fork(); // Create 3rd and final child (pid2)
        if(pid2 != 0)
        {
            // Initial child printing 2nd floor (pid0)
            printFloorNums(2);
        }
        else
        {
            // 3rd child code printing 3rd floor numbers
            printFloorNums(3);
        }
    } // End initial child
} // End main