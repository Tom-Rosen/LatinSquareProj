/******************************************************************************
 * @file: latin_square_functions.c
 *
 * trrosen
 * Tom Rosen
 * 908 190 4204
 *
 * @creator: Tom Rosen (trrosen@wisc.edu)
 * @modified: 11/13/21
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "latin_square_functions.h"

#define DEBUG               0 
#define dbgprintf(...)      if (DEBUG) { printf(__VA_ARGS__); }
#define dbgprint_latin_square(n, LS) if (DEBUG) { Print_Latin_Square(n, LS); }

extern void Print_Latin_Square(const size_t n, char **latin_square);

/* 
 * This function takes the name of the file containing the latin square
 * and reads in the data to the the latin_square parameter.  
 *
 * @param filename The name of the file to read in
 * @param latin_square_in A pointer to the latin square variable in main
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 */
void Read_Latin_Square_File(const char *filename, 
                            char ***latin_square_in, 
                            size_t *n) {
    /* BEGIN MODIFYING CODE HERE */
    FILE *fptr = fopen(filename, "r");
    if(fptr == NULL)
    {
        **latin_square_in = NULL;
        *n = 0;
        return;
    }
    //use fgets to find the length of first row and set value of n
    char *firstLine = (char*)malloc(1001 * sizeof(char));
    if(fgets(firstLine, 1001, fptr) == NULL)
    {
        *latin_square_in = NULL;
        *n = 0;
        free(firstLine);
        return;
    }
    
    int count = 0;
    for(int i = 0; *(firstLine + i) != '\n'; i++)
    {
        count++;
    }
    *n = count;

    if(*n >= 1000)
    {
        *n = strlen(firstLine);
    }

    *latin_square_in = (char**)malloc(*n * sizeof(char*)); //check here

    for(int i = 0; i < *n; i++)
    {
        *(*latin_square_in + i) = (char*)malloc(*n * sizeof(char));
    }
    
    fclose(fptr);
    fptr = fopen(filename, "r");
    fgets(firstLine, 1001, fptr);

    for(int i = 0; i < *n; i++)
    {
        for(int j = 0; j < *n; j++)
        {
           *(*(*latin_square_in + i) + j) = *(firstLine + j); 
        }
        fgets(firstLine, 1001, fptr);
    }
    
    fclose(fptr);
    free(firstLine);
    return;
    /* END MODIFYING CODE HERE */
}

/* 
 * This function checks to see that exactly n symbols are used and that 
 * each symbol is used exactly n times.
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
int Verify_Alphabet(const size_t n, char **latin_square) {
    if (latin_square == NULL) {
        printf("Verify_Alphabet - latin_square is NULL\n"); 
        return 0;
    }
    /* BEGIN MODIFYING CODE HERE */
    int *dupAmount = (int*)malloc(94 * sizeof(int));
    for(int i = 0; i < 94; i++)
    {   
        *(dupAmount + i) = 0;
    }
    
    int index;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            index = (int)(*(*(latin_square + i) + j)) - 33;
              
            if ((*(*(latin_square + i) + j)) >= 33 && (*(*(latin_square + i) + j)) <= 126)
            {
                *(dupAmount + index) += 1;
                continue;
            }
            free(dupAmount);
            return 0;
         }
     }
     for(int z = 0; z < 94; z++)
     {
        if(*(dupAmount + z) != n && *(dupAmount + z) != 0)
        {
            free(dupAmount);
            return 0;
        }
     }
    
    free(dupAmount);
    /* END MODIFYING CODE HERE */
    return 1;
}

/* 
 * This function verifies that no symbol is used twice in a row or column.
 * It prints an error message alerting the user which rows or columns have 
 * duplicate symbols. This means that you will have at most n row error prints
 * and n column error prints.
 * 
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
 int Verify_Rows_and_Columns(const size_t n, char **latin_square){
    if (latin_square == NULL) {
        printf("Verify_Rows_and_Columns - latin_square is NULL\n"); 
        return 0;
    }
    /* BEGIN MODIFYING CODE HERE */
    int *dupAmount = (int*)malloc(94 * sizeof(int));
    for(int z = 0; z < 94; z++)
    {
       *(dupAmount + z) = 0;
    }
    int local;
    int error = 0;    
    int passOrFail = 0;    
    for(int i = 0; i < n; i++)
    {
        error = 0;
        for(int z = 0; z < 94; z++)
        {
            *(dupAmount + z) = 0;
        }
        for(int j = 0; j < n; j++)
        {
            local = (int)(*(*(latin_square + i) + j)) - 33;

            if(*(dupAmount + local) != 0 && error == 0)
            {
                printf("Error in row %d\n", i);
                error = 1;
                passOrFail = 1;
            }
            *(dupAmount + local) = *(dupAmount + local) + 1;
        }
    }

    for(int i = 0; i < 94; i++)
    {
        *(dupAmount + i) = 0;
    }    
    
    for(int i = 0; i < n; i++)
    {
        for(int z = 0; z < 94; z++)
        {
            *(dupAmount + z) = 0;
        }
        for(int j = 0; j < n; j++)
        {
            local = (int)(*(*(latin_square + j) + i)) - 33;
            
            if(*(dupAmount + local) != 0)
            {
                printf("Error in column %d\n", i);
                //free(dupAmount);
                passOrFail = 1;
                //return 0;
            }
            *(dupAmount + local) = *(dupAmount + local) + 1;
        }
    }

    if(passOrFail == 1)
    {
        free(dupAmount);
        return 0;
    }
    
    free(dupAmount);
    // printf("Error in column %d\n", i);
    /* END MODIFYING CODE HERE */
    return 1;
}

/* 
 * This function calls free to allow all memory used by the latin_square 
 * verification program to be reclaimed.
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 */ 
void Free_Memory(const size_t n, char **latin_square) {
    /* BEGIN MODIFYING CODE HERE */
    for(int i = 0; i < n; i++)
    {
        free(*(latin_square + i));
    }
    free(latin_square);
    return;
    /* END MODIFYING CODE HERE */
}



