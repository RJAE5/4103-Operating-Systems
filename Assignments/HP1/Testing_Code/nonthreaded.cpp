/*****************************************************************************\
*
*  Description:
*         This program was used to showcase the LCS algorithm running on a
*         singular thread. It is much simpler than the multithreaded versions
*         but this comes with sacrificing speeds.
*         
*  Usage:
*         To use this program, use some standard c++ compiler such as g++ and
*         run the program. No input is required from the user, all values are
*         coded, there is no random generation enabled on this by default, but
*         you may uncomment the srand line in the `generateSeq()` function.
*         
\******************************************************************************/

#include <iostream>
#include <chrono>
#include <map>
#include <iomanip>

using namespace std;

const int SEQUENCE_LENGTH = 10000;
const int MATRIX_DIM = SEQUENCE_LENGTH + 1;

char s1[SEQUENCE_LENGTH];
char s2[SEQUENCE_LENGTH];
int seqArr[MATRIX_DIM][MATRIX_DIM] = {0};

// Map for int to char conversion
map<int, char> bases = {{0, 'A'}, {1, 'G'}, {2, 'C'}, {3, 'T'}};


int getMax2(int x, int y)
{
    // Return x if greater than y, otherwise return y
    return x > y ? x : y;
}

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
    generateSeq();

    auto startTime = chrono::high_resolution_clock::now();

    //Trying to implement the algorithm without threads
    for(int i = 1; i < MATRIX_DIM; i++)
    {
        for(int j = 1; j < MATRIX_DIM; j++)
        {
            // s1 uses j since it is labeled as the column
            // Conversely, s2 uses i since it represents the row
            if(s1[j - 1] == s2[i - 1])
                seqArr[i][j] = 1 + seqArr[i-1][j-1];
            else
                seqArr[i][j] = getMax2(seqArr[i-1][j], seqArr[i][j-1]);
        }
    }

    // Timer Calculation and output
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = endTime - startTime;

    auto milliseconds = chrono::duration_cast<std::chrono::milliseconds>(duration);


    printArrSection(9990, 10001, 9990, 10001);

    cout << "Execution time: " << milliseconds.count() << " milliseconds" << std::endl;

    return 0;
}