#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sort_common.h"

#define SIZE 1000        
#define MAXLENGTH 40   

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

typedef struct {   
	size_t alloc;
	struct words *storage;
} TEMP_STORAGE_T;

extern struct words {
	char entry[MAXLENGTH];
};

extern struct words sortwords[SIZE];

__inline void SORT_SWAP(struct words sortwords[], int64_t i, int64_t j) // swapping entry a[i] string with b[j] string jm
{
	char temp[MAXLENGTH];
	strcpy(temp, sortwords[i].entry);
	strcpy(sortwords[i].entry, sortwords[j].entry);
	strcpy(sortwords[j].entry, temp);
}

__inline void REVERSE_ELEMENTS(struct words sortwords[], int64_t start, int64_t end) { //original
	while (1) {
		if (start >= end) {
			return;
		}

		SORT_SWAP(sortwords, start, end);
		start++;
		end--;
	}
}

int SORT_CMP(struct words sortwordsI, struct words sortwordsJ);
int SORT_CMP_X(struct words x, struct words sortwordsJ);

void TIM_SORT(struct words sortwords[], const size_t size);
void TIM_SORT_RESIZE(TEMP_STORAGE_T *store, const size_t new_size);
void TIM_SORT_MERGE(struct words sortwords[], const TIM_SORT_RUN_T *stack, const int stack_curr, TEMP_STORAGE_T *store);
int TIM_SORT_COLLAPSE(struct words sortwords[], TIM_SORT_RUN_T *stack, int stack_curr, TEMP_STORAGE_T *store, const size_t size);
int PUSH_NEXT(struct words sortwords[], const size_t size, TEMP_STORAGE_T *store, const uint64_t minrun, TIM_SORT_RUN_T *run_stack, uint64_t *stack_curr, uint64_t *curr);
int64_t COUNT_RUN(struct words sortwords[], const uint64_t start, const size_t size);
int CHECK_INVARIANT(TIM_SORT_RUN_T *stack, const int stack_curr);
void BINARY_INSERTION_SORT(struct words sortwords[], const size_t size);
int64_t BINARY_INSERTION_FIND(struct words sortwords[], const struct words  x, const size_t size);
void BINARY_INSERTION_SORT_START(struct words sortwords[], const size_t start, const size_t size);











