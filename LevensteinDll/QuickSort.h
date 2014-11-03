#include <stdio.h>
#define MAXLENGTH 60 

void quickSort(char a[][MAXLENGTH], int, int);
void swap_str_ptrs(char const **arg1, char const **arg2); 
void quicksort_strs(char const *args[], unsigned int l);
int cstring_cmp(const void *a, const void *b);
