/******************************************************************************
 * @file: latin_square_functions.c
 *
 * trrosen
 * Tom Rosen
 * 908 190 4204
 * OTHER COMMENTS FOR THE GRADER (OPTIONAL)
 *
 * @creator: Tom Rosen (trrosen@wisc.edu)
 * @modified: 11/13/21
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "latin_square_functions.h"

// Set this to 1 to enable dbgprintf statements, make sure to set it back to 0 
// before submitting!
#define DEBUG               0 
#define dbgprintf(...)      if (DEBUG) { printf(__VA_ARGS__); }
#define dbgprint_latin_square(n, LS) if (DEBUG) { Print_Latin_Square(n, LS); }

/******************************************************************************
 * Data Types and Structures
 *****************************************************************************/

// ADD ANY ADDITIONAL DATA TYPES OR STRUCTSvHERE 

/******************************************************************************
 * Globals
 *****************************************************************************/
extern void Print_Latin_Square(const size_t n, char **latin_square);

/******************************************************************************
 * Helper functions
 *****************************************************************************/

// ADD ANY HELPER FUNCTIONS YOU MIGHT WRITE HERE 

/******************************************************************************
 * Verificaiton functions
 *****************************************************************************/

/* 
 * This function takes the name of the file containing the latin square
 * and reads in the data to the the latin_square parameter.  
 *
 * There are many approaches that will work to read in the latin square data.
 * In any approach you choose, you will need to do at least the following:
 *     1) open the file 
 *     2) read in the text from the file
 *     3) figure out the dimensions of the latin square (n)
 *     4) reserve memory for the latin_square. This requires 2 steps
 *         4a) reserve an array of pointers to the rows
 *         4b) reserve an array of characters for each row
 *     5) fill in the latin_square data structure 
 *     6) close the file
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
 * Note: Test all rows first then test all columns.
 * 
 * Error messages have been included for you. Do not change the format out the 
 * print statements, as our grading scripts will use exact string matching. You
 * change the variable passed as the formatting argument to printf.
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
 * Note: you will have n+1 calls to free
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



