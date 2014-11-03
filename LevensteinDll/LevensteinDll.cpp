// LevenshteinDLL.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <windows.h>
#include <math.h>
#include <time.h>
#include "sort.h" 
#include "QuickSort.h"


struct words sortwords[SIZE]; //try an array of struct holding the words

extern "C" __declspec(dllexport) void sortTest(void)
{	
	///*Tester code for Tim Sort */ /*and QuickerSort*/
	char line[MAXLENGTH];  //max length of single array
	int count = 0;

	FILE *fp = fopen("c:\\wordlist.txt", "r");
	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	while (count < SIZE)
	{
		fgets(line, sizeof(line), fp); 
		strtok(line, "\n");
		strcat(line, "\0");
		strcpy(sortwords[count].entry, line); //put word into struct array
		count++;
	} // puts char strings in dic array

	//scramble entries in dic array to test sorting code 
	int ifirst, isecond, iswitch; //jm
	char temp[MAXLENGTH];
	count = 0;
	while (count < SIZE - 1)
	{
		iswitch = rand(); //RAND_MAX maximum allowed at least 32767 with Visual Win and 2147483647 with GNU C compiler
		                  //see rand.c for rand().
		if (iswitch < SIZE)
		{
			strcpy(temp, sortwords[count].entry);
			strcpy(sortwords[count].entry, sortwords[iswitch].entry);
			strcpy(sortwords[iswitch].entry, temp);
			count++;
		}
	}

	for (int k = 0; k < SIZE; k++)  //before sort
	{
		printf("%d,%s \n", k, sortwords[k].entry);
	}
	printf("======================================================================\n");
	
	//qsort() beginning
	//char **strings;
	//strings = (char **)malloc(SIZE * sizeof(char*));  		
	//for (int jj = 0; jj < SIZE; jj++)
	//{
	//	strings[jj] = (char *)malloc(MAXLENGTH);
	//	strcpy(strings[jj], sortwords[jj].entry);
	//}
	////if unknown can use sizeof(strings) 					
	//qsort(strings, SIZE, sizeof(char *), cstring_cmp);
	//for (int i = 0; i < SIZE; i++)  //print for qsort sorted
	//{
	//	printf("%s   \n", strings[i]);
	//}
	//printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	//for (int jj = SIZE; jj < 200; jj++) //qsort works for multiple sorts
	//{
	//	strcpy(strings[jj], sortwords[jj].entry);
	//}
	//qsort(strings, SIZE, sizeof(char *), cstring_cmp);
	//for (int i = 0; i < SIZE; i++)  //print for qsort sorted
	//{
	//	printf("%s   \n", strings[i]);
	//}
	//free(strings); //always 
	//end qsort

	TIM_SORT(sortwords, SIZE);
	//TIM_SORT(sortwords, SIZE); //works ok with multiple calls
	for (int k = 0; k < SIZE; k++)  //print for Tim Sort sorted
	{
		printf("%d,%s \n", k, sortwords[k].entry);
	}
}





