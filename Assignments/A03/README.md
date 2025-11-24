## A03 - POSIX Thread
### Rykir Evans
### Description:

This program simulates two page replacement algorithms which are the Second Chance and Least Recently Used (LRU). It uses a linked list to represent the frame queues. Each page in memory contains a reference bit (the R-bit) that determines whether the page receives a "second chance" before being replaced. If a page fault occurs and the front page has R = 1, the algorithm clears the bit, moves the page to the end of the queue, and continues searching for a page with R = 0 to replace.

### Usage
For WSL, the following instructions may be used to run the program:
1. Navigate to the directory of the program
2. Use the g++ compiler or any standard C++ compiler `g++ main.cpp -o main`
3. Run the program using `./main`

It will automatically run a 1000 page simulation for both Second Chance and LRU algorithms and output the number of page faults associated with each. Each class has several methods which are not utilized for this program, but rather for further testing and  troubleshooting.