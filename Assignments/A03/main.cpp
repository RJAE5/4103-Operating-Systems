#include <iostream>
#include <random>
#include <chrono>
#include <bitset>

using namespace std;

struct Node
{
  int page;
  Node * prev;
  Node * next;
  bool ref;

// Constructor for Node
  Node(int d, Node * n = NULL, Node * p = NULL)
    {
      page = d;
      next = n;
      prev = p;
      ref = 0;
    }
};

class Second_Chance_List
{
    // Head pointer
    private:
    Node * head;
    Node * tail;
    int size;

    public:
    // List constructor
    Second_Chance_List()
    {
        head = NULL;
        tail = NULL;
        size = 0;
    }

    bool isInMemory(int pgNum)
    {
        Node * traverse = head;
        while(traverse)
        {
            // cout << "Traverse->next exists" << endl;
            if(traverse->page == pgNum)
            {
                traverse->ref = 1;
                return true;
            }
                
            else
                traverse = traverse->next;
        }

        return false;
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
            cout << "Size: " << size << endl;
            displayList();
            if(size > 31)
            {
                cout << "List size exceeded, a page needs to be replaced" << endl;
                SC_Page_Replace();

            }
            // Otherwise it goes to the end of list and sets appropriate pointers
            else if(!isInMemory(pgNum))
            {
                tail->next = temp;
                tail = temp;
                size++;

            }
            else
                cout << "No action taken, page already in memory" << endl;
        }

    }// End of insertAtEnd

    void removeHead()
    {
        Node * temp = head;
        head = head->next;
        delete temp;
        temp = NULL;
        head->prev = NULL;

    }

    void SC_Page_Replace()
    {

        // Iterate twice, guaranteed to remove one
        for(int i = 0; i < 2; i++)
        {
            if(head->ref == 0)
            {
                Node * temp = head;
                head = head->next;
                delete temp;
                temp = NULL;
                head->prev = NULL;
            }

            // Traverse through the list to find matching item
            else
            {
                head->ref = 0;

                Node * traverse = head;

                // Move traverse to item just before matching criteria
                while(traverse->next && traverse->next->ref)
                    traverse = traverse->next;

                // Temp pointer for deleteing
                Node * temp = traverse->next;

                // Traverse skips item pgNume need to remove
                traverse->next = temp->next;

                // Test for properly linking in an item that is not at the end of the list
                if(temp->next != NULL)
                    temp->next->prev = traverse;

                // Remove item
                temp->prev = NULL;
                temp->next = NULL;
                delete temp;
                temp = NULL;
            }
                
        }
        
    }// End of remove

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

int main()
{
    srand(chrono::system_clock::now().time_since_epoch().count());
    int r = rand();
    int address = r & 32767; // 2^15 - 1 to extract 15 rightmost bits for address 
    int pageNumber = address & 0x7C00; // 5 leftmost bits

    Second_Chance_List myList;
    // myList.insertAtEnd(10);
    // myList.insertAtEnd(11);
    // myList.insertAtEnd(12);
    // myList.insertAtEnd(12);
    // myList.insertAtEnd(100);

    for(int i = 0; i < 100; i++)
    {
        myList.insertAtEnd(i % 35);
    }


    myList.displayList();


    // std::bitset<32> bits(r); 
    
    // // Print the bitset to standard output
    // std::cout << "The bits of r are:    " << bits << std::endl;
    

    // std::bitset<32> bits2(address); 
    // std::cout << "The bits of addr are: " << bits2 << std::endl;

    // cout << "r: " << r << endl << "15: " << address << endl;

    // std::bitset<32> bits3(pgNum); 
    // std::cout << "The bits of addr are: " << bits3 << std::endl;

}