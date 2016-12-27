#import <stdio.h>
#import <stdlib.h>
/*
This program was created by Yusuf Kamil AK (150116827) as the 1st project of Data Structures (CSE2025) in 20/25.10.2016.

It allows user to insert two unlimited bases of decimal numbers and multiplies them using linked lists for this purpose.

*/
//Definition of linked list which we will use for input and output purposes.
struct linkedList  {
	int digit;
	struct linkedList *next;
};

// Declarations of functions.
struct linkedList *multiplyTwoLists(struct linkedList *firstList,struct linkedList *secondList,struct linkedList **outputList);
void storeInputIntoLinkedList(char *ptrInput,struct linkedList **inputList);
char *getInputFromUser(char *ptrInput);
void printLinkedList(struct linkedList *listToPrint);
int convertCharToDigit(char ch);
struct linkedList *createNodeWith(int digit);
struct linkedList *reverseList(struct linkedList *list);
int sizeOfList(struct linkedList *list);


int main(void){
	char *ptrFirstInput,*ptrSecondInput;
	struct linkedList *firstInputList = NULL,*secondInputList = NULL,*outputList = NULL;
	//Firstly,we will get input in terms of character.
	printf("Please enter the first arbitrary-length number: ");
	ptrFirstInput = getInputFromUser(ptrFirstInput);
	printf("Please enter the second arbitrary-length number: ");
	ptrSecondInput = getInputFromUser(ptrSecondInput);
	//Then, we replace our characters into integers in linked lists.
	storeInputIntoLinkedList(ptrFirstInput,&firstInputList);
	storeInputIntoLinkedList(ptrSecondInput,&secondInputList);
	//Multiplication process will be proceed in function below.
	outputList = multiplyTwoLists(firstInputList,secondInputList,&outputList);
	printf("Your result is: ");
	printLinkedList(outputList);
	return 0;
}

struct linkedList *multiplyTwoLists(struct linkedList *firstList,struct linkedList *secondList,struct linkedList **outputList){
	int sizeOfTempOutput,tour = 1,carryNumber = 0;
	struct linkedList *temp,*tempOutput = NULL,*headOfFirst = firstList,*headOfSecond = secondList,*headOfOutput;
	while(secondList != NULL) {
		temp = NULL;
		tempOutput = NULL;
		firstList = headOfFirst; //To be able to start over firstList in each iteration of upper loop.
		while(firstList != NULL) {
			temp = createNodeWith(((secondList->digit * firstList->digit) + carryNumber) % 10);
			carryNumber = (int)(((secondList->digit * firstList->digit) + carryNumber) / 10);
			temp->next = tempOutput;
			tempOutput = temp;
			if(firstList->next == NULL && carryNumber != 0) {
				temp = createNodeWith(carryNumber);
				carryNumber = 0;
				temp->next = tempOutput;
				tempOutput = temp;
			}
			firstList = firstList->next;
		}
		
		sizeOfTempOutput = sizeOfList(tempOutput);
		tempOutput = reverseList(tempOutput);
		if ((*outputList) == NULL) {
			(*outputList) = tempOutput;
			headOfOutput = (*outputList);
		} else {
			headOfOutput = (*outputList);
			for (int i = 1; i < tour; ++i)
			{
				(*outputList) = (*outputList)->next;
			}
			for (int i = 0; i < sizeOfTempOutput - 1; ++i)
			{
				int value = ((*outputList)->digit + tempOutput->digit + carryNumber);
				(*outputList)->digit = value % 10;
				carryNumber = (int)(value / 10);
				if ((*outputList)->next != NULL) {
					(*outputList) = (*outputList)->next;
				}
				tempOutput = tempOutput->next;
			}
			temp = createNodeWith(tempOutput->digit + carryNumber);
			carryNumber = 0;
			(*outputList)->next = temp;
			(*outputList) = headOfOutput;
		}
		secondList = secondList->next;
		tour++;
	} 
	firstList = headOfFirst;
	(*outputList) = reverseList(*outputList);
	return *outputList;
}

void storeInputIntoLinkedList(char *ptrInput,struct linkedList **inputList){
	int i = 0,digit;
	while((*(ptrInput + i)) != '\0') {
		struct linkedList *temp = (struct linkedList *)malloc(sizeof(struct linkedList));
		digit = convertCharToDigit(*(ptrInput + i));
		if (digit == -1){
			printf("\nInvalid Input. Please be sure that you've inserted decimal numbers.\n");
			exit(0);
		} else {
			temp->digit = digit;
		}
		temp->next = (*inputList);
		(*inputList) = temp;
		i++;
	}
}

char *getInputFromUser(char *ptrInput) {
	ptrInput = (char*)malloc(sizeof(char)*10); // Beginning space. If input is large than 10 bases, we'll realloc more spaces.
	int i = 0,j,capacity = 10;
	char a;
	while((a = getchar()) != '\n'){
		(*(ptrInput + i)) = a;
		i++;
		if (i % 10 == 0) {
			capacity += 10;
			ptrInput = (char*)realloc(ptrInput,sizeof(char)* capacity);
		}
	}
	(*(ptrInput + i)) = '\0';
	printf("You've inserted: ");
	for (j = 0; j < i; ++j)
	{
		printf("%c", (*(ptrInput + j)));
	}
	printf("\n");
	return ptrInput;
}

struct linkedList *createNodeWith(int digit){
	struct linkedList *node = (struct linkedList *)malloc(sizeof(struct linkedList));
	node->digit = digit;
	node->next = NULL;
	return node;
}

struct linkedList *reverseList(struct linkedList *list){
	struct linkedList *head,*temp = NULL,*newList = NULL;
	head = list;
	while(list != NULL) {
		temp = createNodeWith(list->digit);
		temp->next = newList;
		newList = temp;
		list = list->next;
	}
	list = head;
	return newList;
}

void printLinkedList(struct linkedList *listToPrint){
	while(listToPrint != NULL){
		printf("%d",listToPrint->digit);
		listToPrint = listToPrint->next;
	}
	printf("\n");
}

int convertCharToDigit(char ch){
	switch (ch) {
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;	
	}
	return -1;
}

int sizeOfList(struct linkedList *list) {
	int i = 0;
	while(list != NULL){
		i++;
		list = list->next;
	}
	return i;
}
