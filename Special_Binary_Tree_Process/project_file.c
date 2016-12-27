#include <stdio.h>
#include <stdlib.h>
#define INITIAL_DEPTH 0
/* This program was implemented by Yusuf Kamil AK as Project #2 of Data Structures lesson in Marmara University. 
   It reads an input file named "input.txt" and create a tree based on those entries. You are able to insert new entries to the tree
   or delete an existing one. Remember that input file will not be changed and changes will be applied on created tree. */

// Declaration of a node which we will use to create tree.
struct node {
	int firstKey,secondKey;//we could hold keys in one variable but that makes our process cheaper and easier.
	struct node *left,*right;
};
typedef struct node node;
// Declarations of functions defined below main function.
node *createNewNode(int firstKeyOfTarget,int secondKeyOfTarget);
int power(int base, unsigned int exponent);
int convertIntArrayToValue(int array[]);
node *createTreeByReadingFromFile(char inputFileName[]);
void printNodesInDepthLevel(node* root, int depthLevel);
void printTree(node* root);
void printInOrderTraversal(node* root);
int heightOfTree(node *root);
node *minimumNodeInTree(node *root);
node *insertNodeToTree(node *root,int firstKeyOfTarget,int secondKeyOfTarget,int depthLevel);
node *deleteNodeFromTree(node *root,int firstKeyOfTarget,int secondKeyOfTarget,int depthLevel);

int main(void) {
	int keepAsking = 1,choice = 0;
	node *root = NULL;
	root = createTreeByReadingFromFile("input.txt");
	printf("\nHello! Your 2-D tree has been created using entries in the file.\n");
	while(keepAsking) {
		printf("\nWhich process would you like to do now?\n"
			"1 ->> Print the recent tree by level-order.\n"
			"2 ->> Print the recent tree by in-order.\n"
			"3 ->> Insert an entry as a node to the tree.\n"
			"4 ->> Delete an existing node.\n"
			"5 ->> Shut down the program.\n");
		scanf("%d",&choice);
		switch(choice) {
			case 1: {
				printTree(root);
				break;
			}
			case 2: {
				printf("\n\nIn-Order Traversal of the recent tree: \n\n");
				printInOrderTraversal(root);
				printf("\n\nDid you know that you can print this tree in level-order traversal?\n");
				break;
			}
			case 3: {
				int firstKey,secondKey;
				printf("Enter keys of the node you'd like to add into tree in following form;  key1,key2  : ");
				scanf("%d,%d",&firstKey,&secondKey);
				insertNodeToTree(root,firstKey,secondKey,INITIAL_DEPTH);
				printTree(root);
				break;
			}
			case 4: {
				int firstKey,secondKey;
				printf("Enter keys of the node you'd like to remove from tree in following form;  key1,key2  : ");
				scanf("%d,%d",&firstKey,&secondKey);
				deleteNodeFromTree(root,firstKey,secondKey,INITIAL_DEPTH);
				printf("\n\nNode has been successfully deleted. \n\n");
				printTree(root);
				break;
			}
			case 5: {
				printf("Hope to see you in another program later.\n");
				keepAsking = 0;
				break;
			}
		}
	}
	return 0;
}

node *createTreeByReadingFromFile(char inputFileName[]) {
	char ch;
	int firstKey,secondKey,key[5],i = 0;
	FILE *file;
	node *root = NULL;
	file = fopen(inputFileName,"r");
	if (file == NULL) {
		printf("Reading file process has failed! File may not be existing.\n");
		exit(0);
	}
	while((ch = fgetc(file)) != EOF) {
		switch (ch) {
			case ',': {
				key[i] = -1;
				firstKey = convertIntArrayToValue(key);
				i = 0;
				break;
			}
			case '\n': {
				key[i] = -1;
				secondKey = convertIntArrayToValue(key);
				i = 0;
				if (root == NULL) 
					root = insertNodeToTree(root,firstKey,secondKey,INITIAL_DEPTH);
				else 
					insertNodeToTree(root,firstKey,secondKey,INITIAL_DEPTH);
				break;
			}
			default: {
				if (ch == ' ') 
					ch = fgetc(file);
				else if (ch < '0' || ch > '9') {
					printf("\nInvalid input!\nPlease check input file and run the program again.\n");
					exit(0);
				}
				else {
					key[i] = ch - '0';
					i++;
				}
				break;
			}
		}
	}
	if (ch == EOF) {
		key[i] = -1;
		secondKey = convertIntArrayToValue(key);
		insertNodeToTree(root,firstKey,secondKey,INITIAL_DEPTH);
	}
	return root;
}

void printTree(node* root) {
	int height = heightOfTree(root);
    for (int i=1; i<=height; i++) {
    	printf("Depth Level %d:",i-1);
        printNodesInDepthLevel(root, i);
        printf("\n");
    }
    printf("\n!!! NOTE THAT empty children and their empty subtrees are represented as a single space. !!!\n");
}

void printInOrderTraversal(node* root) {
	if (root != NULL) {
		printInOrderTraversal(root->left);
		printf(" %d,%d ",root->firstKey,root->secondKey);
		printInOrderTraversal(root->right);
	}
}

void printNodesInDepthLevel(node* root, int depthLevel) {
    if (root == NULL) { // NOTE THAT empty children and its empty subtree is represented as a single space.
    	for (int i = 0; i < 5; ++i) {
    		printf("_");
    	}
    	printf(" ");
        return;
    }
    if (depthLevel == 1)
        printf(" %d,%d ", root->firstKey,root->secondKey);
    else if (depthLevel > 1) {
        printNodesInDepthLevel(root->left, depthLevel-1);
        printNodesInDepthLevel(root->right, depthLevel-1);
    }
}

int heightOfTree(node *root) {
    if (root == NULL)
        return 0; //Since height of a leaf is 0.
    else {
    	//Finding heights of each nodes recursively. 
        int leftHeightOfNode = heightOfTree(root->left);
        int rightHeightOfNode = heightOfTree(root->right);
        return leftHeightOfNode > rightHeightOfNode ? leftHeightOfNode + 1 : rightHeightOfNode + 1; 
        //Taking bigger one since height is the longest path from node to the leaf.
    }
}

node *insertNodeToTree(node *root,int firstKeyOfTarget,int secondKeyOfTarget,int depthLevel) {
	if (root == NULL) 
		return createNewNode(firstKeyOfTarget,secondKeyOfTarget);
	if (depthLevel % 2) 
		if (secondKeyOfTarget < root->secondKey) 
			root->left = insertNodeToTree(root->left,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
		else
			root->right = insertNodeToTree(root->right,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
	else 
		if (firstKeyOfTarget < root->firstKey) 
			root->left = insertNodeToTree(root->left,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
		else
			root->right = insertNodeToTree(root->right,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
	return root;
}

node *deleteNodeFromTree(node *root,int firstKeyOfTarget,int secondKeyOfTarget,int depthLevel){
	node *temp;
	enum boolean { false,true } shouldDelete = false; //To be able to create a boolean in C and improve readability.
	if (root == NULL) 
		printf("This tree doesn't have any nodes.\n");
	else if (depthLevel % 2) {
		if (secondKeyOfTarget < root->secondKey) {
			root->left = deleteNodeFromTree(root->left,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
		} else if (secondKeyOfTarget > root->secondKey) {
			root->right = deleteNodeFromTree(root->right,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
		} else { // When key was found...
			if (firstKeyOfTarget == root->firstKey) 
				shouldDelete = true;
			else
				root->right = deleteNodeFromTree(root->right,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
		}
	} else {
		if (firstKeyOfTarget < root->firstKey) 
			root->left = deleteNodeFromTree(root->left,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
		else if (firstKeyOfTarget > root->firstKey)
			root->right = deleteNodeFromTree(root->right,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
		else
			if (secondKeyOfTarget == root->secondKey) 
				shouldDelete = true;
			else
				root->right = deleteNodeFromTree(root->right,firstKeyOfTarget,secondKeyOfTarget,depthLevel+1);
	}
	if (shouldDelete) {
		if (root->left && root->right) {
			temp = minimumNodeInTree(root->right);
			root->firstKey = temp->firstKey;
			root->secondKey = temp ->secondKey;
			root->right = deleteNodeFromTree(root->right,root->firstKey,root->secondKey,depthLevel+1);
		} else {
			temp = root;
			if (root->left == NULL) 
				root = root->right;
			else if (root->right == NULL)
				root = root->left;
			free(temp);
		}
		shouldDelete = false;
	}
	return root;
}

node *minimumNodeInTree(node *root) {
	if (root != NULL)
		while(root->left != NULL) root = root->left; 
	return root;
}

node *createNewNode(int firstKeyOfTarget,int secondKeyOfTarget) {
	node *newNode = (node *)malloc(sizeof(node));
	newNode->firstKey = firstKeyOfTarget;
	newNode->secondKey = secondKeyOfTarget;
	newNode->left = newNode->right = NULL;
	return newNode;
}

int convertIntArrayToValue(int array[]){
	int i = 0,k = 0,number = 0;
	while (array[i+1] != -1) { i++; }
	while (i != -1) {
		number += array[i] * power(10,k);
		k++;
		i--;
	}
	return number;
}

int power(int base, unsigned int exponent) {
    int i, result = 1;
    for (i = 0; i < exponent; i++)
        result *= base;
    return result;
 }
