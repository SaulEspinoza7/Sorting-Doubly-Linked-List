/*
	Name: Saul Espinoza Nalvarte
	Panther ID: 6271143
	Affirmation of Originality: I affirm that I have written this program by myself
	without any help from any other people or sources from the internet.
	Description: This program will primarily read strings, sort them, and put them into a linked
	list accordingly. Making the use of getopt, the program gets flags and depending on them, 
	they will produce the following effects: 
	- firstly, entering an argument without a specific flag will make the program read all the 
	input from the file with the exact name of the argument.
	- If the -o flag is present with it's proper name, the program will create and give the
       	output to a text file with that name, if no -o flag is present the output will just be 
	displayed to the console.
	- If the -c flag is present, the strings will be compared case sensitive, otherwise, case
	sensitivity won't matter as all strings will become capitalized.
	- If the -d flag is present, the output will be displayed in a descending manner from higher
	to lower (using the ascii characters as a reference), otherwise the output displays in an
	ascending manner.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Implementation of the Node in a doubly linked list, with next and previous pointers each, and
// a head and last values.
struct Node {
	char value[20];
	struct Node* next;
	struct Node* previous;
};

struct Node* head = NULL;
struct Node* last = NULL;

// Function that returns the value of a string as an integer for comparison.
int stringValue (char myString[]) {
	int myValue = 0;
	
	for (int i = 0; i < strlen(myString); i++)
		myValue += myString[i];

	return myValue;
}

// Function that deals with both the insertion and sorting of the linked list.
void insertSorting (char myString[]) {
	struct Node* myNode = (struct Node*)malloc(sizeof(struct Node));
	struct Node* current = head; // Current pointer to traverse through the entire list.
	struct Node* alteration = NULL;

	// If head is null (no elements) just create a new node with no defined previous or next pointers
	// also assign the head variable with this pointer so that we keep track of the start of the list
	if (head == NULL) {
		head = myNode;
		last = myNode;
		strcpy((*head).value, myString);
		(*head).previous = NULL;
		(*head).next = NULL;	
	} else { // Else insert a new node into the list.
		while (current != NULL) {
			// If the value of the user String is less than that of the current node, then
			// insert the new node just before the current one.
			if (stringValue(myString) < stringValue((*current).value)) {
				if ((*current).previous != NULL) {
					alteration = (*current).previous;
					(*alteration).next = myNode;
					(*myNode).previous = (*current).previous;	
				}

				strcpy((*myNode).value, myString);
				(*current).previous = myNode;
				(*myNode).next = current;

				if (stringValue(myString) < stringValue((*head).value))
					head = myNode;

				return;
			} else if ((*current).next == NULL){ // If String is larger, insert after current.
				strcpy((*myNode).value, myString);
				(*current).next = myNode;
				(*myNode).previous = current;
				
				if (stringValue(myString) > stringValue((*last).value))
					last = myNode;

				return;
			}

			current = (*current).next;
		}
	}
}

// Function that determines if two strings are equal. 
bool isEqual (char firstString[], char secondString[]) {
	if (strlen(firstString) != strlen(secondString))
		return false;
	
	for (int i = 0; i < strlen(firstString); i++) {
		if (firstString[i] != secondString[i])
			return false;
	}
	
	return true;
}

// Function that determines if the given string is in the linked List.
bool isStringInList (char myString[]) {
	struct Node* current = head;

	if (current == NULL)
		return false;
	else {
		while (current != NULL) {
			if (isEqual((*current).value, myString)) 
				return true;

			current = (*current).next;
		}

		return false;
	}
}

// Function that capitalizes an entire string.
void capitalizeString (char myString[]) {
	for (int i = 0; myString[i] != '\0'; i++)
		myString[i] = toupper(myString[i]);
}

// Function that prints all the strings in the linked list in an ascending order.
void printList (FILE* myOutput) {
	struct Node* current = head;

	while (current != NULL) {
		fprintf(myOutput, "%s\n", (*current).value);
		current = (*current).next;
	}
}

// Function that prints all the strings in the linked list in a descending order.
void printReverseList (FILE* myOutput) {
	struct Node* current = last;

	while (current != NULL) {
		fprintf(myOutput, "%s\n", (*current).value);
		current = (*current).previous;
	}
}

// Function that deallocates the nodes from memory at the end of the program.
void freeAllNodes() {
	struct Node* current = head;

	while (current != NULL) {
		free(current);
		current = (*current).next;
	}
}

// MAIN function
int main (int argc, char **argv) {
	extern char *optarg;
	extern int optind;
	int c; 
	int cflag = 0, dflag = 0, oflag = 0;
	char *oname = "default_oname";
	static char usage[] = "usage: %s [-cd] [-o output_file_name] [input_file_name]\n";
	char word[20];
	FILE* myStream;
	FILE* myOutput;

	while ((c = getopt(argc, argv, "cdo:")) != -1)
		switch (c) {
		case 'c':
			cflag = 1;
			break;
		case 'd':
			dflag = 1;
			break;
		case 'o':
			oflag = 1;
			oname = optarg;
			break;
		}

	// If there is no argument, the input will be read from user input (keyboard).
	if ((optind + 1) > argc) {
		myStream = stdin;
	} else { // Otherwise, the program will attempt to read from the file.
		myStream = fopen(argv[optind], "r");

		if (!myStream) {
			printf("ERROR: Could not open the file\n");
			exit(101);
		}
	}
	
	// If the user chose the o flag, a file will be created to put the final result.
	if (oflag == 1)
		myOutput = fopen(oname, "w");
	else // Otherwise the result will just be printed out on the console.
		myOutput = stdout;
	
	// If the user chose the c flag, the program will compare the strings case sensitive.
	if (cflag == 1) {
		if ((optind + 1) > argc) {
			do {
				printf("Please enter a word (input \"quit\" to finish)\n");
				fscanf(myStream, "%s", word);

				if (!isEqual(word, "quit") && !isStringInList(word))
					insertSorting(word);

			} while (!isEqual(word, "quit"));

		} else {
			while ((fscanf(myStream, "%s", &word)) == 1) {
				if (!isStringInList(word))
					insertSorting(word);
			
			}	
		}
	} else { // If no c flag is present, the program will make the letters of the strings capitalized.
		if ((optind + 1) > argc) {
			do {
				printf("Please enter a word (input \"quit\" to finish)\n");
				fscanf(myStream, "%s", word);

				capitalizeString(word);

				if (!isEqual(word, "QUIT") && !isStringInList(word))
					insertSorting(word);
				
			} while (!isEqual(word, "QUIT"));

		} else {
			while ((fscanf(myStream, "%s", &word)) == 1) {
				capitalizeString(word);

				if (!isStringInList(word))
					insertSorting(word);
			}
		}
	}

	// If the user chose a d flag the output will be printed in descending order.
	if (dflag == 1)
		printReverseList(myOutput);
	else // Otherwise, the output will be printed in ascending order.
		printList(myOutput);

	freeAllNodes();	

	exit(0);
}
