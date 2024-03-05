#include <stdio.h>
#include <stdlib.h>
#include "sequence.h"
#include "shell_array.h"
#include "shell_list.h"
#include <time.h>
#include <string.h>





int main(int argc, char* argv[]) {
    clock_t begin = clock();
    //------------------------
    
    if (argc != 4) {
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "-a") == 0) {
        int size = 0;
        long* array = Array_Load_From_File(argv[2], &size);
        if (array == NULL || size <= 0) {
            return EXIT_FAILURE;
        }
    
        //printf("\n");
        //check space if array return null

        long n_comp;
        Array_Shellsort(array, size, &n_comp);//changed from &n_comp
        
        printf("%ld\n", n_comp);
        Array_Save_To_File(argv[3], array, size);
        if (array == NULL || size <= 0) {
            return EXIT_FAILURE;
        }
        free(array);
    }
    else if (strcmp(argv[1], "-l") == 0) {
        int status = 0;
        Node* list = List_Load_From_File(argv[2], &status);

        long n_comp;
        list = List_Shellsort(list, &n_comp);


        //printf("\n");
        printf("%ld\n", n_comp);
        List_Save_To_File(argv[3], list);
        //free(list); //fix
        Node* temp = list;
    }

    //----------------------
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nrun time = %f", time_spent);

    return 0;
}