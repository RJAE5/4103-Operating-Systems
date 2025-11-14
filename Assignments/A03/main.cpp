/*****************************************************************************\
*
*  Author:           Rykir Evans & Victoria Heredia
*  Email:            rjevans0408@my.msutexas.edu | vdheredia1128@my.msutexas.edu
*  Title:            POSIX Thread
*  Course:           CMPS 4103 Intro to Operating Systems
*  Professor:        Dr. Nelson Passos
*  Semester:         Fall 2025
*
*  Description:
*         This program 
*         
*  Usage:
*         To use this program, 
*         
*  Files: 
*         main.cpp
\******************************************************************************/

#include <iostream>
#include <random>
#include <chrono>
#include <bitset>

using namespace std;

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

class Page_Frames_List
{
    protected: 
    Node * head;
    Node * tail;
    int size;
    int pageFaults;

public:

    int getPageFaults()
    {
        return pageFaults;
    }

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

    void removeHead()
    {
        if (head == NULL) return;

        Node* temp = head;
        head = head->next;

        if (head != NULL)
            head->prev = NULL;
        else
            tail = NULL;   // List is now empty

        delete temp;
        size--;
    }

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
    // List constructor
    Second_Chance_List()
    {
        head = NULL;
        tail = NULL;
        size = 0;
        pageFaults = 0;
    }

    // Back-sert method
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
                cout << "No action taken, page already in memory" << endl;
            else if(size >= 8)
            {
                cout << "Page needed is not in memory, a page needs to be replaced" << endl;
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

    void SC_Page_Replace(int pgNum)
    {
        bool replaced = false;
        // Iterate through size of list
        for(int i = 0; i < size; i++)
        {        
            // Head is out of chances, terminate it
            if(head->ref == 0)
            {
                cout << "Removed head on iteration: " << i << endl;
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

        // cout << "Calling insert page with pg: " << pgNum << endl;

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

        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
                LRUTable[i][j] = 0;
        }
    }

    // Get position within linked list and relate it to LRUTable
    int getPgNumLLIndex(int pgNum)
    {
        int index = 0;
        Node * traverse = head;
        while(traverse)
        {
            if(traverse->page == pgNum)
            {
                return index;
            }
            index++;
        }

        return -1;

    }

    // Update specific page number index within table since it was used
    void updateTable(int pgNum)
    {
        int pgIndex = getPgNumLLIndex(pgNum);

        // Update row to 1s
        for(int i = 0; i < 8; i++)
        {
            LRUTable[i][pgIndex] = 1;
        }
        
        // Update columns to 0s
        for(int i = 0; i < 8; i++)
        {
            LRUTable[pgIndex][i] = 0;
        }
    }

    // Back-sert method
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
                cout << "Page already in memory moved to the end of LL" << endl;
                // Move item to back of list as per instructions.
                // Node * traverse = head;
                // while(traverse)
                //     {
                //         if(traverse->page == pgNum)
                //         {
                //             Node * temp = new Node(pgNum, NULL, tail);
                //             tail->next = temp;
                //             tail = temp;
                //         }
                //         traverse = traverse->next;
                //     }


            }
            else if(size >= 8)
            {
                cout << "Page needed is not in memory, a page needs to be replaced" << endl;
                pageFaults++;
                LRU_Page_Replace(pgNum);
            
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

    int findLRUPageIndex()
    {
        int minRow = 0;
        int minCount = 9; // Bigger than the possible 8

        // Iterate through every row
        for (int i = 0; i < 8; i++) {
            int count = 0;

            // Find the largest row
            for (int j = 0; j < 8; j++)
                count += LRUTable[i][j];

            if (count < minCount) {
                minCount = count;
                minRow = i;
            }
        }
        return minRow;
    }

    void LRU_Page_Replace(int replacement)
    {
        int LRUPageIndex = findLRUPageIndex();
        bool replaced = false;
        // int index = getPgNumIndex(LRUPage); // Redundant

        Node * traverse = head;
        while(traverse)
        {
            if(traverse->page == LRUPage)
            {
                
            }
        }

        // Iterate twice, guaranteed to remove one
        // for(int i = 0; i < size; i++)
        // {        
        //     // Head is out of chances, terminate it
        //     if(head->ref == 0)
        //     {
        //         cout << "Removed head on iteration: " << i << endl;
        //         removeHead();
        //         replaced = true;
        //     }

        //     // Traverse through the list to find page with ref 0
        //     else
        //     {
        //         // Give head a second chance
        //         head->ref = 0;

        //         Node* oldHead = head;

        //         // unlink head
        //         head = head->next;
        //         head->prev = NULL;

        //         // move old head to tail
        //         tail->next = oldHead;
        //         oldHead->prev = tail;
        //         oldHead->next = NULL;

        //         tail = oldHead;
        //     }
            
        //     // Exit algorithm if replaced
        //     if(replaced)
        //         break;
                
        // }
        // insertAtEnd(pgNum);
        
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
    srand(chrono::system_clock::now().time_since_epoch().count());


    Second_Chance_List List_Second_Chance;
    LRU_List List_LRU;

    for(int i = 0; i < 1000; i++)
    {
        int r = rand();
        int address = r & 32767; // 2^15 - 1 to extract 15 rightmost bits for address 
        int pageNumber = address & 0x7C00; // 5 leftmost bits
        cout << "Inserting page: " << pageNumber << endl;
        List_Second_Chance.insertAtEnd(pageNumber);
        List_LRU.insertAtEnd(pageNumber);
    }


    List_Second_Chance.displayList();

    cout << "Page faults: " << List_LRU.getPageFaults() << endl;

}