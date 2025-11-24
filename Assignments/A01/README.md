## A01 - POSIX Fork
### Rykir Evans
### Description:

This program introduces us to the POSIX `fork()` function in UNIX. The goal of this program is to output room numbers regarding the recent Bolin Science Hall Renovation. The floors 1-3 and the room numbers 00-20 are printing using different children of the program. The program ends by displaying `Goodbye!`

The program ends after one iteration and must be executed again to perform another display. Results may vary from run to run because the children are not required to wait on one another. 

### Usage
For WSL, the following instructions may be used to run the program:
1. Navigate to the directory of the program
2. Use the g++ compiler or any standard C++ compiler `g++ main.cpp -o main`
3. Run the program using `./main`

#### Optional
In the `printFloorNums()` function, you may uncomment the `<< flush;` and observe that this slight change adds a lot more uncertainty to the order of the output, especially varying from one execution to another.