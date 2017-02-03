#include "declarations.h" //includes all definitions of constants and declarations.

/* That is the 3rd project of Data Structures class, which is the last one. 
 * This project was created by Yusuf Kamil AK in 27.12.2016.
 * It is an optimisation experiment about quantum size value of process. 
 * For details, you may check out project 3 documentation. */

  // Definitions of structures which will be used as core.
 struct node {
    double priority;
    node leftChild;
    node sibling;
 };

 struct queue {
    int currentSize;
    node tree[MAX_TREE_SIZE];
 };

struct process { //I preferred to read whole input file at the beginning.
    char processID[4];
    double executionTime;
    int arrivalTime;
    int timeOfChange;
    double changeAmountOfExecutionTime;
};

struct processList {
    int processCount;
    process list[MAX_PROCESS_SIZE];
};

// Main body of the whole program.
 int main(void) {
    processList processes;
    double awtValues[10];
    processes = readFile(); //reads and stores whole input file.
    findAWTValues(awtValues,processes);
    return 0;
 }

// Evaluations of AWT per each q values, storing all time data and displaying the optimum one.
void findAWTValues(double awtValues[],processList processes) {
    printf("AWT Values for: \n");
    for (int q = 1; q <= Q_VALUE_LIMIT; ++q) //Calculating awt values for all q values.
    {
        awtValues[q-1] = averageWaitingTimeFor(processes,q);
        printf("q:%d = %.2lf  ",q,awtValues[q-1]);
    }
    printf("Optimum one: ");
    double optimum = awtValues[0];
    for (int i = 0; i < Q_VALUE_LIMIT; ++i)
    {
        if (awtValues[i] < optimum) 
            optimum = awtValues[i];
    }
    printf("%.2lf\n", optimum);
}

double averageWaitingTimeFor(processList processes,int q) {
    queue Queue = initialize();
    int t = 0,remainingPriority = 0;
    double eMax = maximumExecutionTime(processes);
    int timeMax = maximumTimeArrival(processes);
    double priority = 0,totalWaitingTime = 0;
    while(t <= timeMax || !isEmpty(Queue)) {
        for (int i = 0; i < processes.processCount; ++i)//In case that there exist more than one process with same time arrival.
        {
            if(processes.list[i].arrivalTime == t) {
                priority = processes.list[i].executionTime;
                int isSame = checkIfSamePriority(processes,i);
                if(isSame) {
                    priority += 0.001; //which will change its position in queue but not affect in calculation.  
                }
                insert(priority,Queue);
            }
        }
        double change = checkForExecutionTimeChange(processes, t);//handles changing in execution time.
        if (!isEmpty(Queue) && (t % q == 0)) {
            int qTemp = q;//to keep q saved.
            do {
                int min = deleteMin(Queue);
                totalWaitingTime += Queue->currentSize;//processes wait in queue except of one of them which is being processed.
                if (change != -1) {
                    remainingPriority = change;
                    change = -1;
                } else {
                    remainingPriority = min - qTemp;
                }
                if (qTemp > min) {
                    qTemp = qTemp - min;
                } else {
                    qTemp = 0;
                }
                if (remainingPriority > 0) {
                    priority = remainingPriority / exp((-1) * pow((2 * remainingPriority) / (3 * eMax),3));//calculation formula.
                    insert(priority,Queue);
                }
            } while(qTemp > 0 && !isEmpty(Queue));
        }
        t++;
    }
    double averageWaitingTime = totalWaitingTime / (double)processes.processCount;
    return averageWaitingTime;
}

int checkIfSamePriority(processList processes,int i) {
    for (int j = 0; j < i; ++j) 
        if (processes.list[j].executionTime == processes.list[i].executionTime) 
            return 1; 
    return 0;
}

double checkForExecutionTimeChange(processList processes, int t) {
    double newExecutionTime = -1;
    for (int i = 0; i < processes.processCount; ++i)
    {
        if (processes.list[i].timeOfChange == t) {
            newExecutionTime = processes.list[i].changeAmountOfExecutionTime;
        }
    }
    return newExecutionTime;
}

// Implementations of file process.
processList readFile() {
    processList temp;
    FILE *fp;
    int length = 0,spaceCount = 0,isNegative = 0;
    char c,id[MAX_ID_SIZE],number[MAX_INT_SIZE];
    temp.processCount = 0;
    fp = fopen("input.txt","r");
    if (fp == NULL)
        error();
    do {
        c = getc(fp);
        if (c == ' ') {
            if (spaceCount == 0) {
                temp.list[temp.processCount].processID[length] = '\0';
            } else {
                number[length] = '\0';
                double value = convertCharArrayToDouble(number);
                if (isNegative == 1) {
                    value = value * -1;
                    isNegative = 0;
                }
                if (spaceCount == 1) {
                    temp.list[temp.processCount].executionTime = value;
                } else if (spaceCount == 2) {
                    temp.list[temp.processCount].arrivalTime = value;
                } else if (spaceCount == 3) {
                    temp.list[temp.processCount].timeOfChange = value;
                } else if (spaceCount == 4) {
                    temp.list[temp.processCount].changeAmountOfExecutionTime = value;
                }
                fillCharArrayWith('?',number);
                
            }
            length = 0;
            spaceCount++;
            c = getc(fp);
        }
        if (c == '\n') {
            spaceCount = 0;
            (temp.processCount)++;
        }
        if (c == '-') {
            isNegative = 1;
            c = getc(fp);
        }
        if (spaceCount == 0) {
            temp.list[temp.processCount].processID[length] = c;
            length++;
        }
        if (spaceCount >= 1) {
            number[length] = c;
            length++;
        }
    } while(c != EOF); 
    temp.processCount++; //to make it show real number of processes.
    return temp;
}

// Implementations of priority queue with min-heap methods.
queue initialize() {
    queue Q;
    int i;
    Q = malloc(sizeof(struct queue));
    if(Q == NULL) {
        error();
    }
    Q->currentSize = 0;
    for(i = 0; i < MAX_TREE_SIZE; i++)
        Q->tree[i] = NULL;
    return Q;
}

queue insert(double priority, queue Q) {
    node newNode;
    queue newItem;
    newNode = malloc(sizeof(struct node));
    if(newNode == NULL)
        error();
    newNode->leftChild = newNode->sibling = NULL;
    newNode->priority = priority;
    newItem = initialize();
    newItem->currentSize = 1;
    newItem->tree[0] = newNode;
    return merge(Q,newItem);
}

queue merge(queue Q1,queue Q2) {
    node firstNode, secondNode, carry = NULL;
    int i, j;
    if(Q1->currentSize + Q2->currentSize > CAPACITY)
        error();
    Q1->currentSize += Q2->currentSize;
    for(i = 0, j = 1; j <= Q1->currentSize; i++, j *= 2) {
        firstNode = Q1->tree[i]; 
        secondNode = Q2->tree[i];
        switch(!!firstNode + 2 * !!secondNode + 4 * !!carry) {
            case 0: /* No trees */
            case 1: /* Only Q1 */
                break;
            case 2: /* Only Q2 */
                Q1->tree[i] = secondNode;
                Q2->tree[i] = NULL;
                break;
            case 4: /* Only carry */
                Q1->tree[i] = carry;
                carry = NULL;
                break;
            case 3: /* Q1 and Q2 */
                carry = combineTrees(firstNode,secondNode);
                Q1->tree[i] = Q2->tree[i] = NULL;
                break;
            case 5: /* Q1 and carry */
                carry = combineTrees(firstNode,carry);
                Q1->tree[i] = NULL;
                break;
            case 6: /* Q2 and Carry */
                carry = combineTrees(secondNode,carry);
                Q2->tree[i] = NULL;
                break;
            case 7: /* All three */
                Q1->tree[i] = carry;
                carry = combineTrees(firstNode,secondNode);
                Q2->tree[i] = NULL;
                break;
        }
    }
    return Q1;
}

node combineTrees(node firstNode,node secondNode) {
    if(firstNode->priority > secondNode->priority)
        return combineTrees(secondNode,firstNode);
    secondNode->sibling = firstNode->leftChild;
    firstNode->leftChild = secondNode;
    return firstNode;
}

node printTree(node tree,node *r,int i) {
    node t[20]={NULL}, q; 
    int j;
    for ( j=0; j<i; j++ ) 
        t[j]= r[j];
    i=0;
    if (tree != NULL) { 
        printf("& %2.1lf ",tree->priority);
        q=tree->sibling;
        j=0;
        do {
            while (q != NULL) { 
                printf("%2.1lf ",q->priority); 
                if (q->leftChild != NULL) {
                    t[i]=q->leftChild; 
                    i++;
                }
                q=q->sibling;
            }
            q=t[j++];
            } while (q != NULL);
        }
    else 
        return NULL;
    printf("\n");
    printTree(tree->leftChild, r, i);
    return NULL;
}

double findMin(queue aQueue) {
    int i;
    double minItem;
    if(isEmpty(aQueue)) {
                printf("Empty Queue\n");
                minItem=0;
                return minItem;
    }
    minItem = INFINITE;
    for(i = 0; i < MAX_TREE_SIZE; i++) {
        if(aQueue->tree[i] && aQueue->tree[i]->priority < minItem) {
            minItem = aQueue->tree[i]->priority;
        }   
    }
    return minItem;
}

double deleteMin(queue H) {
    int i, j;
    int minTree; //The tree with the minimum item
    queue deletedQueue;
    node deletedTree, oldRoot;
    double minItem;
    if(isEmpty(H)) {
        error();
        minItem=-INFINITE;
        return minItem;
    }
    minItem = INFINITE;
    for(i = 0; i < MAX_TREE_SIZE; i++)
    {
        if( H->tree[i] && H->tree[i]->priority < minItem) {
            minItem = H->tree[i]->priority;
            minTree = i;
        }
    }
    deletedTree = H->tree[minTree];
    oldRoot = deletedTree;
    deletedTree = deletedTree->leftChild;
    free(oldRoot);
    deletedQueue = initialize();
    deletedQueue->currentSize = (1 << minTree ) - 1;
    for( j = minTree - 1; j >= 0; j-- ) {
        deletedQueue->tree[j] = deletedTree;
        deletedTree = deletedTree->sibling;
        deletedQueue->tree[j]->sibling = NULL;
    }
    H->tree[minTree] = NULL;
    H->currentSize -= deletedQueue->currentSize + 1;
    merge(H,deletedQueue);
    return minItem;
}

static void destroyTree(node tree) {
    if(tree != NULL) {
        destroyTree(tree->leftChild);
        destroyTree(tree->sibling);
        free(tree);
    }
}

void destroy(queue aQueue) {
    int i;
    for( i = 0; i < MAX_TREE_SIZE; i++ )
        destroyTree(aQueue->tree[i]);
}

queue makeEmpty(queue aQueue) {
    int i;
    destroy(aQueue);
    for( i = 0; i < MAX_TREE_SIZE; i++ )
        aQueue->tree[i] = NULL;
    aQueue->currentSize = 0;
    return aQueue;
}

// Utility functions.
void error() {
    printf("A problem has occured.\n");
    exit(0);
}

int isEmpty(queue aQueue) { return aQueue->currentSize == 0; }

int isFull(queue aQueue) { return aQueue->currentSize == CAPACITY; }

void fillCharArrayWith(char charToFill,char array[]) {
    int i = 0;
    while(array[i] != '\0') {
        array[i] = charToFill;
        i++;
    }
    array[i] = charToFill;
}

double convertCharArrayToDouble(char array[]) {
    double value = 0;
    int i = 0,j = 0;
    while(array[i] != '\0') { i++; } //finds element count in array.
    int intArray[i];
    for (int j = 0; j < i; ++j)
        intArray[j] = array[j] - 48;
    i--;
    while(i != -1) {
        value += intArray[i] * pow(10,j);
        i--; j++;
    }
    return value;
}

double maximumExecutionTime(processList processes) {
    double eMax = 0;
    for (int i = 0; i < processes.processCount; ++i)
    {
        if(processes.list[i].executionTime > eMax) {
            eMax = processes.list[i].executionTime;
        }
    }
    return eMax;
}

int maximumTimeArrival(processList processes) {
    int timeArrival = 0;
    for (int i = 0; i < processes.processCount; ++i)
    {
        if(processes.list[i].executionTime > timeArrival) {
            timeArrival = processes.list[i].arrivalTime;
        }
    }
    return timeArrival;
}
