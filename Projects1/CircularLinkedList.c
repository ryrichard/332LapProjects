/*
The operations on Circular Linked list:
Traverse: Traverse the link list to give the count of number of elements in the link list and Print
link list.
Insert: Insert a new Item in the link list. There are three position that you can insert an item into
a link list:
1. Insert at the start of the link list.
2. Insert at a given position in the link list.
3. Insert at the end of the link list.
Delete: Delete an item from the list.
Search: Search an item in the link list.
Sort: Sort in ascending or descending order.
At the beginning of your program, you should give a selectable list of actions to the user- and
according to the action the user chooses, it should be able to perform that task.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node 
{
    int data;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

int getLength()
{
    if(head == NULL) return 0;

    struct node *ptr = head->next;
    int count = 1;

    while(ptr != head)
    {
        count++;
        ptr = ptr->next;
    }

    return count;
}

//print the order of the linked list
void printList()
{
    if(getLength() == 0)
    {
        printf("List is Empty\n");
        return;
    }

    printf("List : %d ", head->data);
    struct node *ptr = head->next;
    while(ptr != head)
    {
        printf("%d ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

bool searchFor(int data)
{
    if(head->data == data) return true;

    struct node *iter = head->next;
    while(iter != head)
    {
        if(iter->data == data) return true;
        iter = iter->next;
    }

    return false;
}

//insert first
void insertFirst(int data)
{
    struct node *newNode = (struct node*) malloc (sizeof(struct node)); 
    // struct node *newNode;
    newNode->data = data;

    if(head != NULL)
    {
        newNode->next = head;
        struct node *current = head;

        while(current->next != head) 
            current = current->next;

        current->next = newNode;
        head = newNode;
    }
    else
    {
        head = newNode;
        head->next = head;
    }
}

//insert last
void insertLast(int data)
{
    if(head == NULL) insertFirst(data);

    struct node *newNode = (struct node*) malloc (sizeof(struct node)); 
    newNode->data = data;
    newNode->next = head;
    current = head;

    while(current->next != head)
        current = current->next;
    
    current->next = newNode;
}

//insert based on pos
void insert(int pos, int data)
{
    if(getLength() == 0) 
        {
            insertFirst(data);
            return;        
        }

    struct node *newNode = (struct node*) malloc (sizeof(struct node));
    newNode->data = data;

    struct node *ptr = head;

    for(int i = 0; i < pos-1; i++)
    {
        ptr = ptr->next;
    }

    newNode->next = ptr->next;
    ptr->next = newNode;
}

void delete(int data)
{
    // //empty case
    if(head == NULL) return;

    struct node *previous = NULL;
    struct node *current;

    while(searchFor(data))
    {
        current = head;
        while(current->data != data)
        {
            previous = current;
            current = current->next;
        }

        if(current == head)
        {
            while(current->next != head)
                current = current->next;
            current->next = head->next;
            head = head->next; //deleting head
        }
        else previous->next = current->next; //delete node
    }
}

void sortSmallestFirst()
{
    if(head == NULL) return;

    int j, k, l, tempdata;
    struct node *current;
    struct node *next;

    int LENGTH = getLength();
    l = LENGTH;

    for(j = 0; j < LENGTH - 1; j++, l--)
    {
        current = head;
        next = head->next;

        for(k = 1; k < l; k++)
        {
            if(current->data > next->data)
            {
                tempdata = current->data;
                current->data = next->data;
                next->data = tempdata;
            }

            current = current->next;
            next = next->next;
        }
    }
}

void sortLargestFirst()
{
    if(head == NULL) return;

    int j, k, l, tempdata;
    struct node *current;
    struct node *next;

    int LENGTH = getLength();
    l = LENGTH;

    for(j = 0; j < LENGTH - 1; j++, l--)
    {
        current = head;
        next = head->next;

        for(k = 1; k < l; k++)
        {
            if(current->data < next->data)
            {
                tempdata = current->data;
                current->data = next->data;
                next->data = tempdata;
            }

            current = current->next;
            next = next->next;
        }
    }
}

void UserInterface()
{
    bool cont = true;
    int input;
    int num;
    while(cont)
    {
        printf("\nPlease type numbers from 1 - 8\n1 - add to head\n2 - add to pos\n3 - add to end\n4 - get length of list\n5 - get list\n6 - delete number\n7 - sort from smallest to largest\n8 - sort from largest to smallest\n9 - exit\n");

        printf("Type input: ");
        scanf("%d", &input);

        switch(input)
        {
        case 1: //add to head
            printf("Type number: ");
            scanf("%d", &num);
            insertFirst(num);
            break;
        case 2: //add to pos
            printf("Type number: ");
            scanf("%d", &num);
            printf("Type position number: ");
            int pos;
            scanf("%d", &pos);
            insert(pos, num);
            break;
        case 3: //add to end
            printf("Type number: ");
            scanf("%d", &num);
            insertLast(num);
            break;
        case 4: //get length
            printf("Length: %d", getLength());
            break;
        case 5: //get list
            printList();
            break;
        case 6: //delete
            printf("Type number: ");
            scanf("%d", &num);
            delete(num);
            break;
        case 7: //sort lowest to highest
            sortSmallestFirst();
            break;
        case 8: //sort largest to smallest
            sortLargestFirst();
            break;
        case 9:
            cont = false;
            break;
        };
    }
}

int main()
{
    printf("Simple Linked List\n");
    UserInterface();
}