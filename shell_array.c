#include <stdio.h>
#include <stdlib.h>
#include "sequence.h"
#include "shell_array.h"


long *Array_Load_From_File(char *filename, int *size) {
    //opens file and assignes it to file name
    FILE* fptr;
    fptr = fopen(filename, "r"); // rb indicated binary read mode

    if (fptr == NULL) { //if file cannot be opened set *size to zero and allocate zero memory
        *size = 0;
        return NULL;
    }

    fseek(fptr, 0, SEEK_END); // seek to the end of file
    long file_size = ftell(fptr); // get current file pointer
    // fseek(fptr, 0, SEEK_SET); // seek back to beginning of the file
    rewind(fptr);
    if (file_size == 0) {//file is opened successfully but is empty
        *size = 0;
        fclose(fptr);
        return (long *)malloc(0);
    }
    
    *size = file_size / sizeof(long); //size of binary file is a multiple of sizeof(long), this is the number of long_ints
    long* array_long_int = (long*)malloc(*size * sizeof(*array_long_int));//allocates memory 
    if (array_long_int == NULL) {
        *size = 0;
        return NULL;
    }
    
    size_t elements_read = fread(array_long_int, sizeof(long), *size, fptr);//dont have to iterate through file ?? can just be read all at once hopefully
    if (elements_read != *size) {
        //printf("Unable to free elements correctly");
    }

    fclose(fptr);
    return array_long_int;

}

int Array_Save_To_File(char *filename, long *array, int size) {
    if (array == NULL || size <= 0) {
        return -1; // Invalid input, return -1
    }
    FILE* fptr;
    int num_long_ints;
    fptr = fopen(filename, "w");

    if (fptr == NULL) { //if file cannot be opened set *size to zero and allocate zero memory
        size = -1;// should this be -1 or 0??????
        return (-1);
    }
    //printf("This is the size %d", size);
    num_long_ints = fwrite(array,sizeof(long), size, fptr);
    
    fclose(fptr);


    return num_long_ints;
}

void Array_Shellsort(long *array, int size, long *n_comp) {
    //size is num of ints to sort
    //*n_comp is number of comaprisons used to sort
    //call Generate 2p3q Seq to use as the sequence for sorting
    //use insertion or bubble sort for each subarrray
    //if gen2p3q fails to allocate memory for the sequence use regualr insertin or bubble sort


    //use size as n for gen2p3q
    int seq_size;
    int comp = 0;
    *n_comp = comp;
    long *pratt_seq = Generate_2p3q_Seq(size, &seq_size);

    if (pratt_seq == NULL) { // if generate2p3q fails then generate pratt_seq with just a value of 1 and allocate memory for it
        pratt_seq = (long *)malloc(sizeof(long));
        pratt_seq[0] = 1;
        seq_size = 1;
    }
    int k_val;
    long temp;
    int j;
    int i;
    

    for(int k_itr = (seq_size-1); k_itr >= 0; k_itr--) {    //iterate over all elements in pratt_seq
        k_val = pratt_seq[k_itr]; //k_val for subroutine
        for (j = k_val; j <= size - 1; j++) { //sort over each interval of k_val //seq_size or size???????
            temp = array[j];
            i = j;
            while ((i >= k_val) && (array[i-k_val] > temp)) {
                array[i] = array[i - k_val];
                i = i - k_val;
                comp++;
            }
            array[i] = temp;
        }
    }

    *n_comp = comp;
    free(pratt_seq);

    return;
}
