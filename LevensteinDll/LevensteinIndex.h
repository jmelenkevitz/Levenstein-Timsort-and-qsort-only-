#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <windows.h>

enum strategy { None = 0, Insert, Substitute, Delete };

struct source {
	int x_element_source;
	int y_element_source;
};

struct destination {
	int x_element_destination;
	int y_element_destination;
};

struct nfa {  //note with automata n^e where n no. states consumed so far and e is no. errors allowed. Related position in nfa (source and destination valuesaaaa0 
	int termLength; //term is input 
	source* input; //this gives position in automata in terms of 
	destination* dest;
	char charX; //Null (no character as input. A stategy/operation instead).  Horizontal, pairs with None for strategy.  
	strategy* nfa_element_strats; //operation to perform on input.  If None corresponds to horizontal  Char value.
};

extern "C" __declspec(dllexport) void setVariables(int distance, int diagonalChanges);
extern "C" __declspec(dllexport) nfa*** LevensteinAutomata(char* term);
extern "C" __declspec(dllexport) void Convert_Nfa_to_Dfa(nfa* NFA);  
extern "C" __declspec(dllexport) void sortTest(void);