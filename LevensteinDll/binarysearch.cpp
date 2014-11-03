#include "stdafx.h"
#include "binarysearch.h"

//binary search to use with Array bisection (left) 
int binary_search(int A[], int key, int imin, int imax)
{
	// test if array is empty
	if (imax < imin)
		// set is empty, so return value showing not found
		return -1; 
	else
	{
		// calculate midpoint to cut set in half
		int imid = (imin + imax) / 2.0;

		// three-way comparison
		if (A[imid] > key)
			// key is in lower subset
			return binary_search(A, key, imin, imid - 1);
		else if (A[imid] < key)
			// key is in upper subset
			return binary_search(A, key, imid + 1, imax);
		else
			// key has been found
			return imid;
	}
}