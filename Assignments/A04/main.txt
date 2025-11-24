/*****************************************************************************\
*
*  Author:           Rykir Evans
*  Email:            rjevans0408@my.msutexas.edu | rykirjoe@yahoo.com
*  Title:            C/C++ File System Calls
*  Course:           CMPS 4103 Intro to Operating Systems
*  Professor:        Dr. Nelson Passos
*  Semester:         Fall 2025
*
*  Description:
*         This program may prompt the user for a file name and a starting byte
*         to inspect and retrieves 3 consecutive 1-byte characters and outputs
*         them to the console, signifying the initials of a "criminal" being
*         investigated. By default, it will use this file as per assignment
*         instructions
*         
*  Usage:
*         To use this program, use some standard C++ compiler such a g++,
*         and simply execute the program. 
*         Ex: `g++ main.cpp -o main` and `./main`.
*         Follow the prompts, ensuring the file you wish to inspect is in the
*         current directory, and provide a starting byte. Beware, if a lot of
*         consecutive spaces exist in the file, it is possible that the output
*         will appear blank, i.e. using this file, there exists a lot of spaces
*         purely for aesthetic format.
*         
*  Files: 
*         main.cpp
\******************************************************************************/
#include <iostream>

using namespace std;

int main()
{

    int startByte;

    // Below is some commented code to allow for custom input of file names.
    // The default is txt version of this code.

    // Needs to be character pointer for fopen() as string doesn't convert
    // char* fileName = "main.txt";
    
    // cout << "Please enter the name of the file you wish to inspect, including suffix: " << endl;
    // cin >> fileName;
    // FILE* fp = fopen(fileName, "r");

    FILE* fp = fopen("main.txt", "r");

    // Input validation, get a new file name that works if previous one doesn't exist
    // while(fp == NULL)
    // {
    //     cout << "Error, the file was not found. Please enter another file name:" << endl;
    //     cin >> fileName;
    //     fp = fopen(fileName, "r");
    // }

    cout << "Please enter a the starting byte of the information: " << endl;
    cin >> startByte;

    // Buffer for storage of criminal initials 
    char initials[3] = {""};
    
    // Navigate to the specified start byte
    fseek(fp, startByte, SEEK_SET);

    // Read in the following 3 characters
    fread(initials, 1, 3, fp);

    // Close file
    fclose(fp);

    // Output
    cout << "Rykir Evans\nCMPS 4103 - Operating Systems\nFall 2025\n\n"
         << "The initials of the criminal are: "
         << initials[0] << initials[1] << initials[2] << endl;

    return 0;
}