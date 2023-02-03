/*
The operations on Linked list:
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
    int key;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//print the order of the linked list
void printList()
{
    printf("Printing List\n");
    if(head == NULL) printf("Empty List\n");

    struct node *ptr = head;
    int count = 0;

    while(ptr != NULL)
    {
        printf("%d ", ptr->data);
        count++;
        ptr = ptr->next;
    }

    printf("; %d\n", count);
}

int getLength()
{
    if(head == NULL) return 0;

    struct node *ptr = head;
    int count = 0;

    while(ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }

    return count;
}

//insert first
void insertFirst(int key, int data)
{
    printf("Creating node\n");
    struct node *newNode = (struct node*) malloc (sizeof(struct node)); 
    // struct node *newNode;
    newNode->key = key;
    newNode->data = data;

    newNode->next = head;

    head = newNode;
}

//insert last
void insertLast(int key, int data)
{
    if(head == NULL) insertFirst(key, data);

    struct node *newNode = (struct node*) malloc (sizeof(struct node)); 
    newNode->key = key;
    newNode->data = data;

    current = head;

    while(current->next != NULL)
        current = current->next;
    
    current->next = newNode;
}

//insert based on key
void insert(int key, int data)
{
    struct node *newNode = (struct node*) malloc (sizeof(struct node));
    newNode->key = key;
    newNode->data = data;

    struct node *ptr = head;

    for(int i = 0; i < key-1; i++)
    {
        ptr = ptr->next;
    }

    if(ptr->next != NULL)
        newNode->next = ptr->next;
    ptr->next = newNode;
}

struct node* delete(int data)
{
    printf("Deleting %d", data);

    //empty case
    if(head == NULL) return NULL;

    struct node *previous = NULL;
    struct node *current = head;

    //search for node
    while(current->data != data)
    {
        if(current->next == NULL) return NULL;
        else
        {
            previous = current;
            current = current->next;
        }
    }

    if(current == head) head = head->next; //deleting head
    else previous->next = current->next; //rerouting links

    return current;
}

bool search(int data)
{
    struct node *iter = head;
    while(iter != NULL)
    {
        if(iter->data == data) return true;
        iter = iter->next;
    }

    return false;
}

void sort()
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
            next = next->data;
        }
    }
    


}

int main()
{
    insertFirst(0, 10);
    insert(1, 35);
    insertLast(2, 55);
    printList();
    delete(35);
    printf("%d", getLength());
}