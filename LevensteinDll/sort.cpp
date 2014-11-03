#include "stdafx.h"
#include "sort.h"

int SORT_CMP(struct words sortwordsI, struct words sortwordsJ) //int SORT_CMP(struct words sortwords[], int i, int j);
{
	return strcmp(sortwordsI.entry, sortwordsJ.entry);
}

int SORT_CMP_X(struct words x, struct words sortwordsJ)
{
	return strcmp(x.entry, sortwordsJ.entry);
}

void TIM_SORT(struct words sortwords[], const size_t size) //original
{
	/* don't bother sorting an array of size 1 */
	if (SIZE == 1) {
		return;
	}

	uint64_t minrun;
	TEMP_STORAGE_T _store, *store;
	TIM_SORT_RUN_T run_stack[128];
	uint64_t stack_curr = 0;
	uint64_t curr = 0;

	if (SIZE < 64) {
		BINARY_INSERTION_SORT(sortwords, size);
		return;
	}

	/* compute the minimum run length */
	minrun = compute_minrun(size);
	/* temporary storage for merges */
	store = &_store;
	store->alloc = 0;
	store->storage = NULL;

	if (!PUSH_NEXT(sortwords, size, store, minrun, run_stack, &stack_curr, &curr)) {
		return;
	}

	if (!PUSH_NEXT(sortwords, size, store, minrun, run_stack, &stack_curr, &curr)) {
		return;
	}

	if (!PUSH_NEXT(sortwords, size, store, minrun, run_stack, &stack_curr, &curr)) {
		return;
	}

	while (1) {
		if (!CHECK_INVARIANT(run_stack, stack_curr)) {
			stack_curr = TIM_SORT_COLLAPSE(sortwords, run_stack, stack_curr, store, size);
			continue;
		}

		if (!PUSH_NEXT(sortwords, size, store, minrun, run_stack, &stack_curr, &curr)) {
			return;
		}
	}
}

void TIM_SORT_RESIZE(TEMP_STORAGE_T *store, size_t new_size)
{
	if (store->alloc < new_size) {
		struct words *tempstore = (struct words *)realloc(store->storage, new_size * sizeof(words));
		if (tempstore == NULL) {
			fprintf(stderr, "Error allocating temporary storage for tim sort: need %lu bytes", sizeof(words)* new_size);
			exit(1);
		}
		store->storage = tempstore;
		store->alloc = new_size;
	}
}

void TIM_SORT_MERGE(struct words sortwords[], const TIM_SORT_RUN_T *stack, const int stack_curr, TEMP_STORAGE_T *store)
{
	const int64_t A = stack[stack_curr - 2].length;
	const int64_t B = stack[stack_curr - 1].length;
	const int64_t curr = stack[stack_curr - 2].start;
	struct words *storage;
	int64_t i, j, k;
	TIM_SORT_RESIZE(store, MIN(A, B));
	storage = store->storage;
	/* left merge */
	if (A < B) { 
		memcpy(storage, &sortwords[curr], A * sizeof(words)); 
		i = 0;
		j = curr + A;

		for (k = curr; k < curr + A + B; k++) {
			if ((i < A) && (j < curr + A + B)) {
				if (SORT_CMP(storage[i], sortwords[j]) <= 0)
				{
					sortwords[k] = storage[i++];
				}
				else {
					sortwords[k] = sortwords[j++];
				}
			}
			else if (i < A) {
				sortwords[k] = storage[i++];
			}
			else {
				sortwords[k] = sortwords[j++];
			}
		}
	}
	else {
		/* right merge */
		memcpy(storage, &sortwords[curr + A], B * sizeof(words)); //memcpy(storage, &sortwords[curr + A], B * sizeof(sortwords)); also tested //memcpy(storage, &sortwords[curr + A], B * sizeof(words));
		i = B - 1;
		j = curr + A - 1;

		for (k = curr + A + B - 1; k >= curr; k--) {
			if ((i >= 0) && (j >= curr)) {
				if (SORT_CMP(sortwords[j], storage[i])>0) {
					sortwords[k] = sortwords[j--];
				}
				else {
					sortwords[k] = storage[i--];
				}
			}
			else if (i >= 0) {
				sortwords[k] = storage[i--];
			}
			else {
				sortwords[k] = sortwords[j--];
			}
		}
	}
}

int TIM_SORT_COLLAPSE(struct words sortwords[], TIM_SORT_RUN_T *stack, int stack_curr, TEMP_STORAGE_T *store, const size_t size)
{
	while (1) {
		int64_t A, B, C;

		/* if the stack only has one thing on it, we are done with the collapse */
		if (stack_curr <= 1) {
			break;
		}

		/* if this is the last merge, just do it */
		if ((stack_curr == 2) && (stack[0].length + stack[1].length == size)) {
			TIM_SORT_MERGE(sortwords, stack, stack_curr, store);
			stack[0].length += stack[1].length;
			stack_curr--;
			break;
		}
		/* check if the invariant is off for a stack of 2 elements */
		else if ((stack_curr == 2) && (stack[0].length <= stack[1].length)) {
			TIM_SORT_MERGE(sortwords, stack, stack_curr, store);
			stack[0].length += stack[1].length;
			stack_curr--;
			break;
		}
		else if (stack_curr == 2) {
			break;
		}

		A = stack[stack_curr - 3].length;
		B = stack[stack_curr - 2].length;
		C = stack[stack_curr - 1].length;

		/* check first invariant */
		if (A <= B + C) {
			if (A < C) {
				TIM_SORT_MERGE(sortwords, stack, stack_curr - 1, store);
				stack[stack_curr - 3].length += stack[stack_curr - 2].length;
				stack[stack_curr - 2] = stack[stack_curr - 1];
				stack_curr--;
			}
			else {
				TIM_SORT_MERGE(sortwords, stack, stack_curr, store);
				stack[stack_curr - 2].length += stack[stack_curr - 1].length;
				stack_curr--;
			}
		}
		/* check second invariant */
		else if (B <= C) {
			TIM_SORT_MERGE(sortwords, stack, stack_curr, store);
			stack[stack_curr - 2].length += stack[stack_curr - 1].length;
			stack_curr--;
		}
		else {
			break;
		}
	}
	return stack_curr;
}

int PUSH_NEXT(struct words sortwords[], const size_t size, TEMP_STORAGE_T *store, const uint64_t minrun, TIM_SORT_RUN_T *run_stack, uint64_t *stack_curr, uint64_t *curr)
{
	uint64_t len = COUNT_RUN(sortwords, *curr, size);
	uint64_t run = minrun;

	if (run < minrun) {
		run = minrun;
	}

	if (run > size - *curr) {
		run = size - *curr;
	}

	if (run > len) {
		BINARY_INSERTION_SORT_START(&sortwords[*curr], len, run);
		len = run;
	}

	run_stack[*stack_curr].start = *curr;
	run_stack[*stack_curr].length = len;
	(*stack_curr)++;
	*curr += len;

	if (*curr == size) {
		/* finish up */
		while (*stack_curr > 1) {
			TIM_SORT_MERGE(sortwords, run_stack, *stack_curr, store);
			run_stack[*stack_curr - 2].length += run_stack[*stack_curr - 1].length;
			(*stack_curr)--;
		}

		if (store->storage != NULL) {
			free(store->storage);
			//store->storage = NULL;
			store->storage;
		}
		return 0;
	}
	return 1;
}

int64_t COUNT_RUN(struct words sortwords[], const uint64_t start, const size_t size)
{
	uint64_t curr;

	if (size - start == 1) {
		return 1;
	}

	if (start >= size - 2) {
		if (SORT_CMP(sortwords[size - 2], sortwords[size - 1]) > 0)
		{
			SORT_SWAP(sortwords, size - 2, size - 1);
		}

		return 2;
	}

	curr = start + 2;

	if (SORT_CMP(sortwords[start], sortwords[start + 1]) <= 0) {
		/* increasing run */
		while (1) {
			if (curr == size - 1) {
				break;
			}

			if (SORT_CMP(sortwords[curr - 1], sortwords[curr]) > 0) {
				break;
			}

			curr++;
		}

		return curr - start;
	}
	else {
		/* decreasing run */
		while (1) {
			if (curr == size - 1) {
				break;
			}
			if (SORT_CMP(sortwords[curr - 1], sortwords[curr]) <= 0) {
				break;
			}

			curr++;
		}

		/* reverse in-place */
		REVERSE_ELEMENTS(sortwords, start, curr - 1);
		return curr - start;
	}
}

int CHECK_INVARIANT(TIM_SORT_RUN_T *stack, const int stack_curr) {
	int64_t A, B, C;

	if (stack_curr < 2) {
		return 1;
	}

	if (stack_curr == 2) {
		const int64_t A1 = stack[stack_curr - 2].length;
		const int64_t B1 = stack[stack_curr - 1].length;

		if (A1 <= B1) {
			return 0;
		}
		return 1;
	}

	A = stack[stack_curr - 3].length;
	B = stack[stack_curr - 2].length;
	C = stack[stack_curr - 1].length;

	if ((A <= B + C) || (B <= C)) {
		return 0;
	}

	return 1;
}

/*Binary Insertion Sort Code */
int64_t BINARY_INSERTION_FIND(struct words sortwords[], const struct words  x, const size_t size)
{
	int64_t l, c, r;
	struct words  cx;
	l = 0;
	r = size - 1;
	c = r >> 1;

	/* check for out of bounds at the beginning. */
	if (SORT_CMP_X(x, sortwords[0]) < 0) {
		return 0;
	}
	else if (SORT_CMP_X(x, sortwords[r]) > 0) {
		return r;
	}

	cx = sortwords[c];

	while (1) {
		const int val = SORT_CMP_X(x, cx); //was SORT_CMP_Y(x, cx);

		if (val < 0) {
			if (c - l <= 1) {
				return c;
			}

			r = c;
		}
		else { /* allow = for stability. The binary search favors the right. */
			if (r - c <= 1) {
				return c + 1;
			}

			l = c;
		}

		c = l + ((r - l) >> 1);
		cx = sortwords[c];
	}
}

/* Binary insertion sort, but knowing that the first "start" entries are sorted.  Used in timsort. */
void BINARY_INSERTION_SORT_START(struct words sortwords[], const size_t start, const size_t size) {
	uint64_t i;

	for (i = start; i < size; i++) {
		int64_t j;
		struct words x;
		int64_t location;

		/* If this entry is already correct, just move along */
		if (SORT_CMP(sortwords[i - 1], sortwords[i]) <= 0) {
			continue;
		}

		/* Else we need to find the right place, shift everything over, and squeeze in */
		x = sortwords[i];
		location = BINARY_INSERTION_FIND(sortwords, x, i);

		for (j = i - 1; j >= location; j--) {
			sortwords[j + 1] = sortwords[j];
		}

		sortwords[location] = x;
	}
}

/* Binary insertion sort */
void BINARY_INSERTION_SORT(struct words sortwords[], const size_t size)
{
	/* don't bother sorting an array of size 0 or 1 */
	if (size <= 1)
	{
		return;
	}
	BINARY_INSERTION_SORT_START(sortwords, 1, size);
}


