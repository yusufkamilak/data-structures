#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 // Typedefs
typedef struct node *node;
typedef struct queue *queue;
typedef struct process process;
typedef struct processList processList;
 // Definitions
#define MAX_TREE_SIZE 12
#define CAPACITY 4095
#define INFINITE (6000000000L)
#define MAX_INT_SIZE 8
#define MAX_ID_SIZE 3
#define MAX_PROCESS_SIZE 100
#define Q_VALUE_LIMIT 10
#define E 2.71828
 // Declarations
void error();
int isFull(queue aQueue);
int isEmpty(queue aQueue);
double convertCharArrayToDouble(char array[]);
queue initialize();
queue insert(double item, queue Q);
node combineTrees(node firstNode,node secondNode);
queue merge(queue Q1,queue Q2);
node printTree(node tree,node *r,int i);
double findMin(queue aQueue);
static void destroyTree(node tree);
void destroy(queue aQueue);
queue makeEmpty(queue aQueue);
void findAWTValues(double awtValues[],processList processes);
double averageWaitingTimeFor(processList processes,int q);
processList readFile();
void fillCharArrayWith(char charToFill,char array[]);
double deleteMin(queue H);
double maximumExecutionTime(processList processes);
int maximumTimeArrival(processList processes);
double checkForExecutionTimeChange(processList processes, int t);
int checkIfSamePriority(processList processes,int i);