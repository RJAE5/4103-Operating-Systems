/*****************************************************************************\
*
*  Author:           Rykir Evans & Victoria Heredia
*  Email:            rjevans0408@my.msutexas.edu | vdheredia1128@my.msutexas.edu
*  Title:            Page Replacement Algorithm Simulation
*  Course:           CMPS 4103 Intro to Operating Systems
*  Professor:        Dr. Nelson Passos
*  Semester:         Fall 2025
*
*  Description:
*       This program simulates two page replacement algorithms which are the
*       Second Chance and Least Recently Used (LRU). It uses a linked list to 
*       represent the frame queues. Each page in memory contains a reference
*       bit (the R-bit) that determines whether the page receives a "second
*       chance" before being replaced. If a page fault occurs and the front
*       page has R = 1, the algorithm clears the bit, moves the page to the end 
*       of the queue, and continues searching for a page with R = 0 to replace.
*         
*  Usage:
*       To use this program, use a standard C++ compiler and execute the
*       program. It will automatically run a 1000 page simulation for both
*       Second Chance and LRU algorithms and output the number of page faults
*       associated with each. Each class has several methods which are not
*       utilized for this program, but rather for further testing and 
*       troubleshooting.
*         
*  Files: 
*         main.cpp
\******************************************************************************/

#include <iostream>
#include <random>
#include <chrono>

using namespace std;

/**
 * Public : Node
 *
 * Description:
 *      This is the struct constructor for a page-frame node used in the 
 *      linked list. Each node represents a page currently loaded into memory
 *      and contains the page number,the reference (R) bit, and pointers to
 *      the previous and next nodes in the list.
 *
 * Params:
 *      int d   : The page number stored in this node
 *      Node * n: Pointer to the next node in the list (default NULL)
 *      Node * p: Pointer to the previous node in the list (default NULL)
 *
 * Returns:
 *      None
 */
struct Node
{
  int page;
  bool ref;
  Node * prev;
  Node * next;
  

// Constructor for Node
  Node(int d, Node * n = NULL, Node * p = NULL)
    {
      page = d;
      next = n;
      prev = p;
      ref = false;
    }
};

// General class to eliminate repetitive functions
class Page_Frames_List
{
    protected: 
    Node * head;
    Node * tail;
    int size;
    int pageFaults;
    int pageHits;

public:

    // Varying setters and getters

    //Retrieves the total number of page faults counted by the
    //page replacement algorithm and returns the # of page faults 
    // that have ocurred. Used for analysis
    int getPageFaults()
    {return pageFaults;}

    // Used for testing
    void setPageFaults(int pf = 0)
    {pageFaults = pf;}

    // Used for curiosity
    int getPageHits()
    {return pageHits;}

    /**
    * Public: isInMemory
    *
    * Description:
    *      Checks whether a given page number exists in the current
    *      frame list. If the page is found its reference bit is set
    *      to 1 (meaning that it was recently used).
    *
    * Params:
    *      int pgNum: The page number being searched for
    *
    * Returns:
    *      bool: True if the page is in memory, false otherwise
    */
    bool isInMemory(int pgNum)
    {
        Node * traverse = head;
        while(traverse)
        {
            // cout << "Traverse->next exists" << endl;
            if(traverse->page == pgNum)
            {
                traverse->ref = true;
                return true;
            } 
            else
                traverse = traverse->next;
        }
        return false;
    }

    /**
    * Public : removeHead
    *
    * Description:
    *      Removes the page at the front (head) of the queue. This is the page that
    *      is evaluated first for replacement under the Second Chance algorithm. 
    *      If this page has R = 0, it is removed and replaced. If R = 1 the bit 
    *      is cleared and the page is reinserted at the end of the queue instead.
    *
    * Params:
    *      None
    *
    * Returns:
    *      Node*: A pointer to the removed node so the caller can inspect its data.
    */
    void removeHead()
    {
        if (head == NULL) return;

        Node* temp = head;
        head = head->next;

        // Non-empty list
        if (head != NULL)
            head->prev = NULL;
        else
            tail = NULL;   // List is now empty

        delete temp;
        size--;
    }

    /**
    * Public : displayList
    *
    * Description:
    *      Prints the page numbers of all frames currently stored in
    *      the linked list, from head to tail. 
    *
    * Params:
    *      None
    *
    * Returns:
    *      void
    */
    void displayList()
    {
        // Traverse pointer
        Node * traverse = head;

        // Move traverse throughout the list
        while(traverse)
        {
            // Print out each item in the list and move traverse down
            cout << traverse->page << " ";
            traverse = traverse->next;
        }
        cout << endl;
    }// End of displayList
};

class Second_Chance_List: public Page_Frames_List
{
    public:
    // List constructor: Initializes an empty Second Chance frame list by setting
    //                   head, tail, size, and pageFaults to their starting values.
    Second_Chance_List()
    {
        head = NULL;
        tail = NULL;
        size = 0;
        pageFaults = 0;
        pageHits = 0;
    }

    /*
    * Public : insertAtEnd
    *
    * Description:
    *      Inserts a page into the frame list. If the page is already in
    *      memory, nothing is done. If memory is full the Second Chance 
    *      algorithm is triggered. Otherwise the new page is put at the
    *      end of the queue.
    *
    * Params:
    *      int pageNum: The page number to insert.
    *
    * Returns:
    *      void
    */
    void insertAtEnd(int pgNum)
    {
        // New Node
        Node * temp = new Node(pgNum);
        temp->ref = 1;

        // Checks if list is empty, if it is, new node becomes head
        if(head == NULL)
        {
            head = temp;
            tail = temp;
            size++;
        }
        else
        {
            if(isInMemory(pgNum))
                pageHits++;
            else if(size >= 8)
            {
                pageFaults++;
                SC_Page_Replace(pgNum);
            }
            // Otherwise it goes to the end of list and sets appropriate pointers
            else
            {
                tail->next = temp;
                tail = temp;
                size++;
            }
  
        }

    }// End of insertAtEnd

    ///////////////////////////////////////////
    //            Main Algorithm             //
    ///////////////////////////////////////////
    /**
    * Public : SC_Page_Replace
    *
    * Description:
    *      Executes the Second Chance page replacement algorithm. Pages are examined in FIFO order.
    *      - If a page has R = 0, it is selected as the victim and removed.
    *      - If a page has R = 1, the bit is cleared and the page is moved 
    *        to the end of the queue (given a "second chance").
    *
    *      The process repeats until a victim page is found, after which
    *      the new page is inserted into the list.
    *
    * Params:
    *      int pgNum: The new page number that must be inserted
    *
    * Returns:
    *      void
    */
    void SC_Page_Replace(int pgNum)
    {
        bool replaced = false;
        // Iterate through size of list
        for(int i = 0; i < size; i++)
        {        
            // Head is out of chances, terminate it
            if(head->ref == 0)
            {
                // cout << "Removed head on iteration: " << i << endl;
                removeHead();
                replaced = true;
            }

            // Traverse through the list to find page with ref 0
            else
            {
                // Give head a second chance
                head->ref = 0;

                Node* oldHead = head;

                // unlink head
                head = head->next;
                head->prev = NULL;

                // move old head to tail
                tail->next = oldHead;
                oldHead->prev = tail;
                oldHead->next = NULL;

                tail = oldHead;
            }
            
            // Exit algorithm if replaced
            if(replaced)
                break;
        }
        insertAtEnd(pgNum);
    }// End of remove

    //Destructor method- delete the list and free memory back up
    ~Second_Chance_List()
    {
        while(head)
        {
            Node * temp = head->next;
            delete head;
            head = temp;
        }
    }
};

// Least Recently Used List Algorithm
class LRU_List: public Page_Frames_List
{
    private:
    int LRUTable[8][8];
        
    public:
    // List constructor
    LRU_List()
    {
        head = NULL;
        tail = NULL;
        size = 0;
        pageFaults = 0;
        pageHits = 0;

        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
                LRUTable[i][j] = 0;
        }
    }

    /**
    * Public : getPgNumLLIndex
    *
    * Description:
    *      Iterates through the linked list and finds an "index" to use
    *      for the algorithm table
    *
    * Params:
    *      int pgNum: The new page number being searched for
    *
    * Returns:
    *      int      : The index in the list
    */
    int getPgNumLLIndex(int pgNum)
    {
        int index = 0;
        Node * traverse = head;
        // Find position in list similar to array
        while(traverse)
        {
            if(traverse->page == pgNum)
            {return index;}

            index++;
            traverse = traverse->next;
        }
        return -1;
    }

    /**
    * Public : updateTable
    *
    * Description:
    *      Iterates through the algorithm matrix to update all
    *      cols for a page 1, and all rows to 0
    *
    * Params:
    *      int pgNum: The new page number being referenced
    *
    * Returns:
    *      void
    */
    void updateTable(int pgNum)
    {
        int pgIndex = getPgNumLLIndex(pgNum);

        // Update row to 1s
        for(int i = 0; i < 8; i++)
        {LRUTable[pgIndex][i] = 1;}
        
        // Update columns to 0s
        for(int i = 0; i < 8; i++)
        {LRUTable[i][pgIndex] = 0;}
    }

    /*
    * Public : insertAtEnd
    *
    * Description:
    *      Inserts a page into the frame list. If the page is already in
    *      memory, nothing is done. If memory is full the Least Recently 
    *      Used algorithm is triggered. Otherwise the new page is put at the
    *      end of the queue.
    *
    * Params:
    *      int pageNum: The page number to insert.
    *
    * Returns:
    *      void
    */
    void insertAtEnd(int pgNum)
    {
        // New Node
        Node * temp = new Node(pgNum);
        temp->ref = 1;

        // Checks if list is empty, if it is, new node becomes head
        if(head == NULL)
        {
            head = temp;
            tail = temp;
            size++;
        }
        else
        {
            if(isInMemory(pgNum))
            {
                // Track pageHits if page already in memory
                pageHits++;
                
                // Reference the page in table
                updateTable(pgNum);
            }
            else if(size >= 8)
            {
                // Page not in memory, and frames full, replacement needed
                pageFaults++;
                LRU_Page_Replace(pgNum);
            }
            else
            {
                // Append node to list
                tail->next = temp;
                tail = temp;
                size++;
            }  
        }
    }// End of insertAtEnd

    /**
    * Public : findLRUPageTableIndex
    *
    * Description:
    *      Goes through the algorithm table to find the
    *      page that has been least recently used.
    *
    * Params:
    *      none
    *
    * Returns:
    *      int      : The index in the table
    */
    int findLRUPageTableIndex()
    {
        int minRow = 0;
        int minCount = 9; // Bigger than the possible 8

        // Iterate through every row
        for (int i = 0; i < 8; i++) 
        {
            int count = 0;

            // Find the largest row
            for (int j = 0; j < 8; j++)
                count += LRUTable[i][j];

            // Set the lowest value row
            if (count < minCount) 
            {
                minCount = count;
                minRow = i;
            }
        }
        return minRow;
    }

    ///////////////////////////////////////////
    //            Main Algorithm             //
    ///////////////////////////////////////////
    /**
    * Public : LRU_Page_Replace
    *
    * Description:
    *      Executes the Least Recently Used Page Replacement Algorithm
    *      - Tracks a matrix where the LRU Page will have a row with a low binary value
    *      - Removes the page with the lowest value
    *
    * Params:
    *      int replacement: The new page number that will replace the LRU page
    *
    * Returns:
    *      void
    */
    void LRU_Page_Replace(int replacement)
    {
        // Setup
        int LRUPageIndex = findLRUPageTableIndex();
        Node * replaceMe = head;

        // Arrive at the node to be replaced
        for(int i = 0; i < LRUPageIndex; i++)
        {replaceMe = replaceMe->next;}

        // Perform replacement
        replaceMe->page = replacement;
        replaceMe->ref = 1;
        updateTable(replacement);
    }// End of remove

    //Destructor method- delete the list and free memory back up
    ~LRU_List()
    {
        while(head)
        {
            Node * temp = head->next;
            delete head;
            head = temp;
        }
    }
};

int main()
{
    // Setup
    srand(chrono::system_clock::now().time_since_epoch().count());
    Second_Chance_List List_Second_Chance;
    LRU_List List_LRU;

    /////////////////////////////////////////////////////////////////////////
    // Below is some commented code for mass testing purposes to determine //
    // the average amount of page faults for each algorithm, running       //
    // thousands of times                                                  //
    /////////////////////////////////////////////////////////////////////////

    // Mass testing variables
    // float total_SC_Errors = 0;
    // float total_LRU_Errors = 0;
    // int testTimes = 10000;

    // for(int k = 0; k < testTimes; k++)
    // {
        for(int i = 0; i < 1000; i++)
        {
            // Generate a random number and extract the page number from the "address"
            int r = rand();
            int address = r & 32767; // 2^15 - 1 to extract 15 rightmost bits for address 
            int pageNumber = address >> 10; // Shift address to get 5 leftmost bits

            // Add same page to both algorithms to accurately compare
            List_Second_Chance.insertAtEnd(pageNumber);
            List_LRU.insertAtEnd(pageNumber);
        }

    //     total_SC_Errors += List_Second_Chance.getPageFaults();
    //     List_Second_Chance.setPageFaults();

    //     total_LRU_Errors += List_LRU.getPageFaults();
    //     List_LRU.setPageFaults();

    // }

    // cout << "SC Page faults average: " << total_SC_Errors / testTimes << endl;
    // // cout << "SC Page Hits: " << List_Second_Chance.getPageHits() << endl;

    // cout << "LRU Page faults average: " << total_LRU_Errors / testTimes << endl;
    // // cout << "LRU Page Hits: " << List_LRU.getPageHits() << endl;

    // End mass testing code

    // Used for troubleshooting
    // List_Second_Chance.displayList();
    
    cout << "Second Chance Page Fault Count: " << List_Second_Chance.getPageFaults() << endl;
    cout << "Least Recently Used Page Fault Count: " << List_LRU.getPageFaults() << endl;
}

// Report:
// Between the two algorithms, they are very close in terms of page faults,
// however, in our opinon, LRU has more overhead since it has to do a lot of
// function calls, and keep track of a 2D array which requires a bunch of
// iterations. Second Chance was a little confusing at first to understand,
// but ultimately, it does look simpler in retrospect. 

// Below is the average page fault between the two algorithm. We ran
// a 1,000 page simulation 10,000 times and on average, they were
// extremely close to each other, but consistently, Second Chance had
// about 9-10 page faults more than LRU, which in our opinion is a worthwhile
// tradeoff for significantly less overhead associatated with Second Chance.

// SC Page faults average: 759.392 / 1000 pages  (10,000 run average)
// LRU Page faults average: 750.059 / 1000 pages (10,000 run average)