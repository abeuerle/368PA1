#include <stdio.h>
#include <stdlib.h>
#include "shell_list.h"
#include "sequence.h"

void printList(Node *head);
int length(Node *head);
void freeList(Node *head);
Node *createNode(int value);
void push(Node **tail, int newVal);








Node *List_Load_From_File(char *filename, int *status) {
    //You should return an empty list if you could not open the file or fail to allocate sufficient memory.
    //store in *status a value of −1 if you could not open the file or fail to allocate sufficient memory

    //If you could open the file and allocate sufficient memory to create the linked list, you should store in
    //*status a value of 0. If the input file is empty, you should return an empty list

    //You should not expect the caller function to initialize *status !!!!!!!! 
    
    FILE* fptr;
    fptr = fopen(filename, "r"); // rb indicated binary read mode
    
    if (fptr == NULL) { //could not open the file so return empty list with *status = -1
        *status = -1;
        return NULL;
    }

    Node *head = NULL; //initialize node head for later
    Node *curr = NULL; // initialize node curr for iterating through LL

    fseek(fptr, 0, SEEK_END); // seek to the end of file
    long file_size = ftell(fptr); // get current file pointer
    rewind(fptr);

    if (file_size == 0) {//file is opened successfully but is empty so retrun empty list
        fclose(fptr);
        return NULL;
    }

    //int size = file_size / sizeof(long); //size of binary file is a multiple of sizeof(long), this is the number of long_ints

    long value;
    //cannot allocate memory to an array like we did for shell_array.c so we have go by node
    while (fread(&value, sizeof(long), 1, fptr)) { //while fread returns a value
        Node *new_node = (Node *)malloc(sizeof(Node)); //allocate space for a new node

        if (new_node == NULL) { //this occurs if malloc fails and we cannot allocate sufficient memory so *status = -1 and return NULL
            *status = -1;
            fclose(fptr);

            while (head != NULL) { // also must free already allocated space or will cause seg fault
                Node *temp = head;
                head = head->next;
                free(temp);
            }
            return NULL;
        }

        //initialize new_node 
        new_node->value = value;
        new_node->next = NULL;

        if (head == NULL) { //if head is empty let new_node be head if not then let curr-> next = new_node
            head = new_node; //now the initial new node is accessed by 2 pointer nodes, head and curr
            curr = new_node;
        } else {
            curr->next = new_node;
            curr = new_node;
        }
    }

    fclose(fptr);
    *status = 0;
    return head;
}


int List_Save_To_File(char *filename, Node *list) {
    //The save function should write all (long) integers in a linked list into the output file in the order in which
    //they are stored in the linked list. This function returns the number of integers successfully written into the file.

    //If the output file cannot be opened for writing, the function should return a value of −1.
    //Note that this function asks you to save a list to a file. It does not ask you to free the list

    FILE* fptr;
    int num_long_ints;
    fptr = fopen(filename, "w");

    if (list->next == NULL || fptr == NULL) {//if file cannot be opened return -1
        return -1; // Invalid input, return -1
    }

    //list should be head of the list
    while (list != NULL) {
        fwrite(&(list->value), sizeof(long), 1, fptr); //first input must be pointer to value
        list = list->next;
        num_long_ints++; //check how many elements in list are added to file
    }
    
    fclose(fptr);


    return num_long_ints;
}


//----------------------------------------------------------------


// For creating a new node for a number.
Node *createNode(int value) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->value = value;
  newNode->next = NULL;
  return newNode;
}

// Function to push nodes into the list.
void push(Node **tail, int newVal) {
  Node *newNode = createNode(newVal);

  if (*tail == NULL) {
    // If the list is empty, make the new node the head of the list
    *tail = newNode;
  } else {
    // Otherwise, append the new node to the end of the list
    (*tail)->next = newNode;
    *tail = newNode;
  }
}

void freeList(Node *head) {
  Node *current = head;
  Node *next;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}

// Function to print the Linked List
void print(Node *head) {
  Node *temp = head;
  while (temp != NULL) {
    printf("%ld -> ", temp->value);
    temp = temp->next;
  }
  printf("NULL\n");
}

// Function to calculate the length of the Linked List
int length(Node *head) {
  Node *temp = head;
  int len = 0;
  while (temp != NULL) {
    len++;
    temp = temp->next;
  }
  return len;
}

Node *List_Shellsort(Node *list, long *n_comp) {
    //Node *i = list;
    long list_size = length(list);

    // int seq_size;
    // long *pratt_seq = Generate_2p3q_Seq(list_size, &seq_size);
    
    // if (pratt_seq == NULL) {
    //   return;
    // }

    printf("\nlist_size = %ld\n", list_size);
    int pratt_seq[6] = {1,2,3,4,6,8};

    //int pratt_size = 4;

    Node *prev = list;
    Node *j = list;
    Node *post_temp;
    Node *temp;
    Node *start_j;
    Node *start_prev;
    Node *pre_temp;

    
    int atleast1swap = 1;
    int k_val;

    for (int pratt_itr = 4-1; pratt_itr >= 0; pratt_itr--) {
      k_val = pratt_seq[pratt_itr];

      int check_loops = 0;
      start_j = list;
      //begin at start_j = ;ist then start_pos should move start_j down the list as the loop continues
      //within the start_pos loop set j = start_j before the loop and when it finishes move it down the list
      //within the while(atleast1swap) loop start_j should remain at the same position in the list
      for (int start_pos = 1; start_pos <= k_val; start_pos++) {
        printf("---------start_pos = %d\n", start_pos);

        printf("Intitial list->value = %ld\n", list->value);

        j = start_j;
        printf("Intitial start_j->value = %ld\n", start_j->value);
        start_prev = list;
        for (int prev_itr = 1; prev_itr < start_pos-1; prev_itr++) {
          start_prev = start_prev->next;
        }
        printf("Intitial start_prev->value = %ld\n", start_prev->value);

        atleast1swap = 1;


        //within while(atleast1swap) start_j should be reset to its initial start position if there is a swap with it
        //the loop should begin with j = start_j  each time the loop begins
        // the loop should begin with prev = start_prev each time
        //the loop should continue as long as there is at least 1 swap 
        //atlest1swap should =1 going into the loop so it compares at least once
        while(atleast1swap) {
          j = start_j;
          prev = start_prev;
          atleast1swap = 0;
          //this loop will run the number of times needed to complete bubble sort once for each subarray
          //makes sure the correct number of comarrisons are made
          // if start_pos = 1 or 2, prev should be at least 1, if start_pos = 3 then prev should be 2 and so on
          for (int itr = start_pos; (itr + k_val) <= list_size; itr += k_val) {
            if (k_val == 1) { //this is for the k_val = 1 group
              temp = j->next;
              if ((j->value) > (temp->value)) {
                atleast1swap = 1;
                if (j == list) {
                  j->next=temp->next;
                  temp->next=j;
                  prev=temp;
                  list=prev;
                  start_j=prev;
                }
                else {
                  j->next=temp->next;
                  temp->next=j;
                  prev->next=temp;
                  prev=temp;
                }
              }
              else {
                prev = j;
                j = j->next;
              }
            }
            else { // This is for the j!=1 group
              pre_temp = j;
              for (int next_itr = 0; next_itr < (k_val -1); next_itr++) {
                pre_temp = pre_temp->next;
              }
              temp = pre_temp->next;
              printf("comparisson of j: %ld and temp: %ld\n", j->value, temp->value);
              check_loops++;

              //this if block swaps j and temp if needed and then reassigns j and prev to their new spots
              if ((j->value) > (temp->value)) {
                atleast1swap = 1;
                if (j == list) {
                  post_temp = temp->next;
                  temp->next = j->next;
                  pre_temp->next = j;
                  j->next = post_temp;
                  prev = temp;
                  for (int next_itr = 0; next_itr < (k_val - 1); next_itr++) {
                    prev = prev->next;
                  }
                  list = temp;
                  start_j = temp;
                }
                else {
                  post_temp = temp->next;
                  temp->next = j->next;
                  pre_temp->next = j;
                  j->next = post_temp;
                  prev->next = temp;
                  for (int next_itr = 0; next_itr < (k_val); next_itr++) {
                    prev = prev->next;
                  }
                  if (j == start_j) {
                    start_j = temp;
                    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                  }
                  printf("Linked list after iteration (start_pos: %d\n): ", start_pos);
                  print(list);
                  printf("start_j = %ld\n", start_j->value);
                  printf("temp = %ld\n", temp->value);
                  printf("j = %ld\n", j->value);



                }
              }
              else { //if no swap is needed it still puts j and prev in their new spots for the next loop
                if (j == list) {
                  prev = j;
                  for (int next_itr = 0; next_itr < (k_val - 1); next_itr++) {
                    prev = prev->next;
                  }
                  for (int next_itr = 0; next_itr < (k_val); next_itr++) {
                    j = j->next;
                  }
                } else {
                  prev = j;
                  for (int next_itr = 0; next_itr < (k_val-1); next_itr++) {
                    prev = prev->next;
                  }
                  for (int next_itr = 0; next_itr < (k_val); next_itr++) {
                    j = j->next;
                  }
                }
              }
            }
          } //closing - for (int itr = start_pos; (itr + k_val) <= list_size; itr += k_val)
        } //close while(atleast1swap)
        start_j = start_j->next;

        //PRINT FULL LINKED LIST HERE AFTER EACH ITERATION
        //printf("start_pos: %d, k_val: %d, check_loops: %d\n", start_pos, k_val, check_loops);
      } //close for (int start_pos = 1; start_pos <= k_val; start_pos++)
      printf("before new k_val start_j = %ld\n", start_j->value);
    }
    return list;
}

// Driver function
int main() {
  Node *refHead = createNode(0);
  Node *tail = refHead;

  // Linked list elements
  push(&tail, 90);
  push(&tail, 1);
  push(&tail, 11);
  push(&tail, 12);
  push(&tail, 56);
  push(&tail, 2);
  push(&tail, 18);
  push(&tail, 100);
  push(&tail, 42);

  // Print unsorted list
  printf("Linked list before sorting: ");
  print(refHead->next);

  // Resultant linked list after sorting
  long n_comp;
  Node *res = NULL;
  res = List_Shellsort(refHead->next, &n_comp);

  // Print sorted list
  printf("Linked list after sorting: ");
  print(res);

  freeList(res);
  free(refHead);


  return 0;
}