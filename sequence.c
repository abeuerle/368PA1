#include <stdio.h>
#include <stdlib.h>
#include "sequence.h" // need this for 2p3q sequence

static int compare(const void *a, const void *b);


//basic qsort helper function
static int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

long *Generate_2p3q_Seq(int n, int *seq_size) {

    //if n is 1,0, or an invalid length seq_size should be set to 0 and return NULL
    if (n <= 1) {
        *seq_size = 0;
        return NULL;
    }

    //initialize variables used for math
    int i_exponent;
    int j_exponent;
    int new_add;
    int stop_row_check = 1;
    int row = 0;
    int array_length = 0;
    int pow2toi;
    int pow3toj;
    long *sequence = NULL;     //initialize point to later dynamically allocate space

    int q = 0;
    int maximum_three = 1;
    while(maximum_three < n) {
        maximum_three *= 3;
        q++;
    }
    int max_area = (q+1) * (q+2);
    sequence = (long*)malloc(max_area * sizeof(long));
    if (sequence == NULL) {
        *seq_size = 0;
        return NULL;
    }

    while (stop_row_check < n) {//move down rows until at a column where max 2^i variable is greater or equal to n
        stop_row_check = stop_row_check * 2;
        //create new (2^i)*(3^j) element and add it to the list if viable
        for (j_exponent = 0; j_exponent <= row; j_exponent++) {//move across column
            i_exponent = row - j_exponent;
            pow3toj = 1;
            for (int k = 0; k < j_exponent; k++) {
                pow3toj *= 3;
            }
            pow2toi = 1;
            for (int t = 0; t < i_exponent; t++) {
                pow2toi *= 2;
            }
            new_add = pow2toi * pow3toj;
            if (new_add < n) {
                //increment array_length to change seq_size at the end
                array_length++;
                
                //instert new_add into array
                sequence[array_length - 1] = new_add;
            }
        }
        row++;
    }

    //sort sequence array using qsort
    qsort(sequence, array_length, sizeof(long), compare);

    //check order of sequence

    //set seq_size to be the array_length
    *seq_size = array_length;



    return sequence;
}
