#include "stdafx.h"
#include <stdio.h>
#include "stack.h"

//stack variables
#define MAXSTACK 100
#define EMPTYSTACK -1

int top, status;
char items[MAXSTACK];

#define MAX 1000


//stack functions need to modify for sets in DFA
/*PUSH FUNCTION*/
void push(int stack[], int item)
{
	if (top == (MAX - 1))
		status = 0;
	else
	{
		status = 1;
		++top;
		stack[top] = item;
	}
}

/*POP FUNCTION*/
int pop(int stack[])
{
	int ret;
	if (top == -1)
	{
		ret = 0;
		status = 0;
	}
	else
	{
		status = 1;
		ret = stack[top];
		--top;
	}
	return ret;
}

/*FUNCTION TO DISPLAY STACK*/
void display(int stack[])
{
	int i;
	printf("\nThe Stack is: ");
	if (top == -1)
		printf("empty");
	else
	{
		for (i = top; i >= 0; --i)
			printf("\n--------\n|%3d   |\n--------", stack[i]);
	}
	printf("\n");
}