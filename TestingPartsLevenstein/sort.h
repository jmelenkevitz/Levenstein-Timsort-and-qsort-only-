#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sort_common.h"

#define SIZE 10000
#define MAXLENGTH 40
/* see below for strings
#ifndef SORT_CMP
#define SORT_CMP(x, y)  ((x) < (y) ? -1 : ((x) == (y) ? 0 : 1))
#endif
*/
/* faster for integers is #define SORT_CMP(x, y) (x - y) */

//#define SORT_TYPE char*
//#define SORT_SWAP(x,y) {SORT_TYPE __SORT_SWAP_t = (x); (x) = (y); (y) = __SORT_SWAP_t;} //not compatible with arrays used

#ifndef MAX
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#endif
#ifndef MIN
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#endif

typedef struct {
  uint64_t start;
  uint64_t length;
} TIM_SORT_RUN_T;

//typedef struct {
//  size_t alloc;
//  SORT_TYPE *storage;
//} TEMP_STORAGE_T;

typedef struct {
	size_t alloc;
	char **storage; //also tried char (*storage)[MAXLENGTH];
} TEMP_STORAGE_T;

__inline void SORT_SWAP(char a[][MAXLENGTH], int64_t i, int64_t j) // swapping entry a[i] string with b[j] string
{
	char temp[MAXLENGTH];
	strcpy(temp, a[i]);
	strcpy(a[i], a[j]);
	strcpy(a[i], temp); 
}

//__inline void REVERSE_ELEMENTS(SORT_TYPE *dst, int64_t start, int64_t end) {
//  while (1) {
//    if (start >= end) {
//      return;
//    }
//
//    SORT_SWAP(dst[start], dst[end]);
//    start++;
//    end--;
//  }
//}

__inline void REVERSE_ELEMENTS(char dst[][MAXLENGTH], int64_t start, int64_t end) {
  while (1) {
    if (start >= end) {
      return;
    }

	SORT_SWAP(dst, start, end);
    start++;
    end--;
  }
}

int SORT_CMP(char* a, char* b);
//void TIM_SORT(SORT_TYPE *dst, const size_t size);
void TIM_SORT(char dst[][MAXLENGTH], size_t size);

void TIM_SORT_RESIZE(TEMP_STORAGE_T *store, const size_t new_size);
//void TIM_SORT_MERGE(SORT_TYPE *dst, const TIM_SORT_RUN_T *stack, const int stack_curr,TEMP_STORAGE_T *store);
void TIM_SORT_MERGE(char dst[][MAXLENGTH], const TIM_SORT_RUN_T *stack, const int stack_curr, TEMP_STORAGE_T *store);

//int TIM_SORT_COLLAPSE(SORT_TYPE *dst, TIM_SORT_RUN_T *stack, int stack_curr,TEMP_STORAGE_T *store, const size_t size);
int TIM_SORT_COLLAPSE(char dst[][MAXLENGTH], TIM_SORT_RUN_T *stack, int stack_curr, TEMP_STORAGE_T *store, const size_t size);

//int PUSH_NEXT(SORT_TYPE *dst, const size_t size, TEMP_STORAGE_T *store, const uint64_t minrun, TIM_SORT_RUN_T *run_stack, uint64_t *stack_curr,uint64_t *curr); 
int PUSH_NEXT(char dst[][MAXLENGTH], const size_t size, TEMP_STORAGE_T *store, const uint64_t minrun, TIM_SORT_RUN_T *run_stack, uint64_t *stack_curr, uint64_t *curr);

//int64_t COUNT_RUN(SORT_TYPE *dst, const uint64_t start, const size_t size);
int64_t COUNT_RUN(char dst[][MAXLENGTH], const uint64_t start, const size_t size);

int64_t CHECK_INVARIANT(TIM_SORT_RUN_T *stack, int64_t stack_curr);

//void BINARY_INSERTION_SORT(SORT_TYPE *dst, const size_t size); /*for short arrays */
void BINARY_INSERTION_SORT(char dst[][MAXLENGTH], const size_t size); /*for short arrays */

//int64_t BINARY_INSERTION_FIND(SORT_TYPE *dst, const SORT_TYPE x,const size_t size);
int64_t BINARY_INSERTION_FIND(char dst[][MAXLENGTH], const char** x, const size_t size);

//void BINARY_INSERTION_SORT_START(SORT_TYPE *dst, const size_t start, const size_t size);
void BINARY_INSERTION_SORT_START(char dst[][MAXLENGTH], const size_t start, const size_t size);











