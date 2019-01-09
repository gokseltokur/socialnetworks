/*

	Göksel Tokur 150116049

*/

#include <stdio.h> 
#include <stdlib.h>
#include<locale.h>
#include <string.h>
#include <math.h>
int V;


typedef struct AdjListNode AdjListNode;
typedef struct AdjList AdjList;
typedef struct Graph Graph;
typedef struct Queue Queue;
typedef struct NodeArr NodeArr;
Graph* createGraph(int V);
void createMatrix(int arr[][V]);
void addEdgeMatrix(int arr[][V],int s, int data);
void printAdjMatrix(int arr[][V], char nameArr[][50]);
void addEdge(Graph* graph, int s, int data);
void printGraph(Graph* graph, char nameArr[][50]);
void readNames(char fileName[], char nameArr[][50]);
Queue* createQueue(unsigned capacity);
int isFull(Queue* queue);
int isEmpty(Queue* queue);
void enqueue(Queue* queue, int item);
int dequeue(Queue* queue);
int front(Queue* queue);
int rear(Queue* queue);
int findCloseness(Graph* graph, int s);
int closeness (int distance[]);
void printClosenessCentrality(Graph* graph, char nameArr[][50], double arr[]);
int findDegree(Graph* graph, int s);
void printDegreeCentrality(Graph* graph, char nameArr[][50], double arr[]);
void findAllShortestPaths(Graph* graph,int s, int d, NodeArr** headptr);
void findAllShortestPathsRe(Graph* graph, int u, int d, int visited[], int path[], int path_index, NodeArr** headptr);
NodeArr* createNodeArr(int V);
NodeArr* add(NodeArr** headptr, int data[]);
void printPaths(NodeArr* node);
NodeArr* findOneBeforeFromTheLast(NodeArr** headptr);
int getPathSize(int arr[V]);
NodeArr* findLast(NodeArr** headptr);
void findShortestPaths(Graph* graph , NodeArr** headptr);
void calculateBetweenness(NodeArr** headptr, char nameArr[][50], double arr[]);
void findShortestPathSize(Graph* graph, int s, int d, NodeArr** headptr);
void findShortestPathSizeRe(Graph* graph, int u, int d, int visited[], int path[], int path_index, NodeArr** headptr);
void printIntermediaandPaths(NodeArr** headptr, char nameArr[][50]);
void printAllCentralities(char nameArr[][50], double dcent[V], double ccent[V], double bcent[V]);



// Structure of Queue
struct Queue { 
    int front, rear, size; 
    unsigned capacity; 
    int* array; 
}; 

// Adjacency Nodes
struct AdjListNode{ 
    int data; 
    AdjListNode * next; 
}; 
  
// Adjacency linked list 
struct AdjList{ 
    AdjListNode *head;  
};
  
// Graph structure 
struct Graph{ 
    int V; 
    AdjList* array; 
};

struct NodeArr{ 
    int V; 
    int* array;
    NodeArr* next;
};

int main() { 
    setlocale(LC_ALL,"turkish");
    V = sizeOfGraph("input.txt");
    char nameArr[V][50];
	readNames("input.txt", nameArr);
	
	//This block of code to create Matrix and Graph List Structure
	int adjMatrix[V][V];
	createMatrix(adjMatrix);
	
	// Create a graph represented as Adjacency List.
	Graph* graph = createGraph(V);
	
	FILE *file;
	if((file = fopen("input.txt", "r")) == NULL){
		printf("ERROR, file does not exist.");
	}
	int i = 0;
	int j = 0;
	char srcName[50];
	char dataName[50];
	char c;
	char line[100000];
	
	// this loop to read all names and adjacencies from text file.
	while(fgets(line, sizeof(line), file) != NULL){
		c = 'x';
		while (c != '\0'){
			c = line[i];
			if(c == ';')break;
			srcName[j] = c ;
			srcName[j+1] = '\0' ;
			i++;
			j++;
		}
		i++;
		j=0;
		while(c != '\0'){
			while (c != '\0'){
				c = line[i];
				if(c == ','){
					j = 0;
					break;
				}
				//printf("%c", c);
				if(isalpha((int)c)){
					dataName[j] = c ;
					dataName[j+1] = '\0';
					j++;
				}
				i++;
			}
			i++;
			addEdge(graph, searchInNameArr(srcName, nameArr), searchInNameArr(dataName, nameArr));
			addEdgeMatrix(adjMatrix, searchInNameArr(srcName, nameArr), searchInNameArr(dataName, nameArr));
		}
		i = 0;
		j = 0;
	}
	printGraph(graph, nameArr);
	
	printf("\n");
	
	// Arrays will keep standardized centrality values
	double dcent[V];
	double ccent[V];
	double bcent[V];
	NodeArr* headptr = NULL;
	
	printf("\n\n");
	printf("Degree Centrality:\n");
	printDegreeCentrality(graph, nameArr, dcent);
	printf("\n\n");
	printf("Closeness Centrality:\n");
	printClosenessCentrality(graph, nameArr, ccent);
	printf("\n\n");
	printf("Betweenness Centrality:\n");
	findShortestPaths(graph, &headptr);
	calculateBetweenness(&headptr, nameArr, bcent);
	
	printf("\n\n");
	printAdjMatrix(adjMatrix, nameArr);
	printf("\n\n");
	printAllCentralities(nameArr, dcent, ccent, bcent);
	printf("\n\n");
	printIntermediaandPaths(&headptr, nameArr);
	
	
	
	printf("\n");
	
	
  
    return 0; 
}

// Print all tables in a table.
void printAllCentralities(char nameArr[][50], double dcent[V], double ccent[V], double bcent[V]){
	int i;
	printf("%-15s%-20s%-20s%-20s\n", "Source", "Degree Centrality", "Closeness Centrality", "Betweenness Centrality");
	for(i=0 ; i<V ; i++){
		printf("%-15s%-20lf%-20lf%-20lf\n", nameArr[i], dcent[i], ccent[i], bcent[i]);
	}
}

//Function to print all intermedia nodes and shortest paths.
void printIntermediaandPaths(NodeArr** headptr, char nameArr[][50]){
	NodeArr* ptr = *headptr;
	int source, target, i;
	printf("\n%-18s%-18s%-50s%s", "Source", "Target", "Intermedia Nodes", "Path"  );
	while(ptr != NULL){
		//To find source and target nodes.
		source = ptr->array[0];
		for(i=0;i<V;i++){
			if(ptr->array[i+1] == -1){
				target = ptr->array[i];
				break;
			}
		}
		
		char intermedia[10000], path[10000];
		char *inter = intermedia;
		char *pa = path;
		for(i=0;i<V;i++){
			if(ptr->array[i] != source && ptr->array[i] != target){
				char x[50];
				strcpy(x, nameArr[ptr->array[i]]);
				if(ptr->array[i] >= 0){
					strcat(x, ",");
				}
				
				strcat(inter , x);
			}
			
			char y[50];
			strcpy(y, nameArr[ptr->array[i]]);
			if(ptr->array[i] >= 0){
					strcat(y, "-");
			}
			strcat(pa , y);
			
			
		}
		
		printf("\n%-18s%-18s%-50s%s", nameArr[source], nameArr[target], inter, pa  );
		memset(inter, 0, sizeof(inter));
		memset(pa, 0, sizeof(pa));
			
			
		
		ptr=ptr->next;
	}
	printf("\n");
}

int pathercontroller;
// To find shortest path size.
void findShortestPathSize(Graph* graph, int s, int d, NodeArr** headptr){
	int visited[V];
	int path[V];
	int path_index=0;
	int i;
	for (i = 0; i < V; i++) 
        visited[i] = 0;
    for (i = 0; i < V; i++) 
        path[i] = -1;
    
    pathercontroller=0;
    findShortestPathSizeRe(graph, s, d, visited, path, path_index, headptr);
}

// findShortestPathSize function continue from here with recurrence.
void findShortestPathSizeRe(Graph* graph, int u, int d, int visited[], int path[], int path_index, NodeArr** headptr){
	visited[u] = 1; 
    path[path_index] = u;
    path_index++;

	if (u == d ){ 
		int i;
		int patharr[V];
		for (i = 0; i<V ; i++) patharr[i] = -1;
    	for (i = 0; i<path_index; i++){
    		patharr[i] = path[i];
		}
		if(pathercontroller == 0){
			add(headptr, patharr);
		}
		else if(pathercontroller>0){
			NodeArr* obftl = findOneBeforeFromTheLast(headptr);
			if(obftl != NULL){
				if(getPathSize((obftl)->next->array) > getPathSize(patharr) ){
					(obftl)->next = NULL;
					add(headptr, patharr);
				}else{
					// do nothing
				}
			}else{// if obftl is null calculate last node;
				NodeArr* last = findLast(headptr);
				if(getPathSize(last->array) > getPathSize(patharr) ){
					//last = NULL;
					*headptr = NULL;
					add(headptr, patharr);
				}else{
					// do nothing
				}
				
			}
		}
		pathercontroller++;							
    }
	else{
		AdjListNode* i = graph->array[u].head;
		while( i != NULL){
			if(visited[i->data] == 0 ){
				findShortestPathSizeRe(graph, i->data, d, visited, path, path_index, headptr);
			}
			i = i->next;
		}
	}
	
	path_index--;
    visited[u] = 0;
}


// Function to calculate betweenness scores and print them.
void calculateBetweenness(NodeArr** headptr, char nameArr[][50], double arr[]){
	NodeArr* temp = *headptr;
	int i;
	int betweenness[V];
	for(i=0 ; i<V ; i++){
		betweenness[i] = 0;
	}
	int betweennesspath[V];
	for(i=0 ; i<V ; i++){
		betweennesspath[i] = 0;
	}
	
	while(temp != NULL){
		int score = 0;
		for(i=1 ; i<V ; i++){
			if(temp->array[i+1] >= 0){
				if(temp->array[i] >= 0){
					betweenness[temp->array[i]] = betweenness[temp->array[i]] + 1;
				}
			}
		}
		for(i=0 ; i<V ; i++){	
			if(temp->array[i] >= 0){
				betweennesspath[temp->array[i]] = betweennesspath[temp->array[i]] + 1;
			}
		}
		temp = temp->next;
	}
	
	double denominator = ((V-1)*(V-2))/2;
	printf("%-15s%-15s%-15s\n", "Node", "Score", "Standardized Score");
	for(i=0 ; i<V ; i++){
		printf("%-15s %d/%-15d%-15lf\n", nameArr[i], betweenness[i],betweennesspath[i], (double)((double)(betweenness[i])/(double)(betweennesspath[i]))/denominator);
		arr[i] =  (double)((double)(betweenness[i])/(double)(betweennesspath[i]))/denominator;
	}
}

//This function to find all shortest paths with findAllShortestPaths function.
void findShortestPaths(Graph* graph, NodeArr** headptr){
	int i, j;
	for(i = 0; i<V; i++ ){
		for(j = i+1; j<V; j++){
			findAllShortestPaths(graph, i, j, headptr);
		}
	}
}

int pather;
//This function to find all shortest paths.
void findAllShortestPaths(Graph* graph, int s, int d, NodeArr** headptr){
	int visited[V];
	int path[V];
	int path_index=0;
	int i;
	for (i = 0; i < V; i++) 
        visited[i] = 0;
    for (i = 0; i < V; i++) 
        path[i] = -1;
    
    pather=0;
    findAllShortestPathsRe(graph, s, d, visited, path, path_index, headptr);
}


void findAllShortestPathsRe(Graph* graph, int u, int d, int visited[], int path[], int path_index, NodeArr** headptr){
	visited[u] = 1; 
    path[path_index] = u;
    path_index++;

 
	if (u == d ){
		int i;
		int patharr[V];
		for (i = 0; i<V ; i++) patharr[i] = -1;
    	for (i = 0; i<path_index; i++){
    		patharr[i] = path[i];
		}
		NodeArr* sizeptr = NULL;

		for(i=0 ; i<V ; i++){
			if(path[i] == -1 ){
				i--;
				break;
			}
		}
		
		findShortestPathSize(graph, path[0], path[path_index-1], &sizeptr);
		if(sizeptr != NULL){ 
			if(getPathSize(sizeptr->array) == getPathSize(patharr)){
				add(headptr, patharr);			
			}
		}	
		pather++; 						
    }
	else{
		AdjListNode* i = graph->array[u].head;
		while( i != NULL){
			if(visited[i->data] == 0 ){
				findAllShortestPathsRe(graph, i->data, d, visited, path, path_index, headptr);
			}
			i = i->next;
		}
	}
	
	path_index--; 
    visited[u] = 0;
}

// To find last point in a node array structure
NodeArr* findLast(NodeArr** headptr){
	NodeArr* last = *headptr;
	
	if (last == NULL)
		return NULL;
	
	while(last != NULL){
		if(last->next == NULL)
			return last;		
	}
}

// To find one before from the last node in a node array structure.
NodeArr* findOneBeforeFromTheLast(NodeArr** headptr){
	NodeArr* obftl = *headptr;
	
	if ( obftl == NULL || obftl->next == NULL)
		return NULL;
	
	while(obftl != NULL){
		if(obftl->next->next == NULL)
			return obftl;
		obftl = obftl->next;		
	}
}

// To get path size in an array
int getPathSize(int arr[V]){
	int size = 0;
	int i;
	for(i=0; i<V; i++){
		if(arr[i] >= 0)
			size++;
	}
	return size;	
}

// To print panths in a node array structure
void printPaths(NodeArr* node){
	while(node != NULL){
		printf("\n");
		int i;
		for(i = 0; i<V ; i++){
			printf("%d/", node->array[i]);
		}
		node = node->next;	
	}
}

// Function to add data to an node array structure
NodeArr* add(NodeArr** headptr, int data[]){
    NodeArr* newNode = createNodeArr(V);
    NodeArr* last = *headptr;
  
    int i;
	for(i = 0 ; i<V; i++)
	    newNode->array[i] = data[i];
  
    newNode->next = NULL;
    
    if (*headptr == NULL) { 
       *headptr = newNode; 
       return; 
    }  

    while (last->next != NULL) 
        last = last->next; 
  
    last->next = newNode; 
    return;
}

// Function to create node array and set all of array's values to -2.
NodeArr* createNodeArr(int V) { 
    NodeArr* nodearr = (NodeArr*) malloc(sizeof(NodeArr)); 
    nodearr->V = V; 
 
    nodearr->array = (int*) malloc(V * sizeof(int)); 
    
    int i; 
    for (i = 0; i < V; ++i) 
        nodearr->array[i] = -2; 
  
    return nodearr; 
}

// Function to find Degree of the graph's nodes
int findDegree(Graph* graph, int s){
	int score = 0;
	AdjListNode* i = graph->array[s].head;
	while( i != NULL){
		score++;
		i = i->next;
	}
	return score;
}

// Function to print degree centralities
void printDegreeCentrality(Graph* graph, char nameArr[][50], double arr[]){
	double denominator = (double)V - 1.0;
	int i;
	
	printf("%-15s %-15s %-15s\n", "Node", "Score", "Standardized Score");
	for(i=0 ; i<V ; i++){
		double standardized = ((double)findDegree(graph, i))/denominator;
		arr[i] = standardized;
		printf("%-15s %-15d %-15lf\n", nameArr[i], findDegree(graph, i),  standardized);
	}
}

void printClosenessCentrality(Graph* graph, char nameArr[][50], double arr[]){
	double denominator = (double)V - 1.0;
	int i;
	
	printf("%-15s %-15s %-15s\n", "Node", "Score", "Standardized Score");
	for(i=0 ; i<V ; i++){
		double standardized = denominator/((double)findCloseness(graph, i));
		arr[i] = standardized;
		printf("%-15s 1/%-15d %-15lf\n", nameArr[i], findCloseness(graph, i),  standardized);
	}
}

// Function to find closeness centrality values of all nodes.
int findCloseness(Graph* graph, int s){
	int distance[V+1];
	int x;
	for (x = 0; x < V+1; x++) {
		distance[x] = -1;
	}
	
	int visited[V];
	int i;
	for(i = 0 ; i<V ; i++){
		visited[i]=0;
	}
	
	Queue* queue = createQueue(V);
	visited[s]=1;
	distance[s]=0;
	enqueue(queue, s);
	
	while(!isEmpty(queue)){
		s = dequeue(queue);
		
		AdjListNode* j = graph->array[s].head;
		int controller = 1;
		while(j != NULL){
			if(visited[j->data] == 0){
				visited[j->data] = 1;
				enqueue(queue, j->data);
				distance[j->data] = distance[s]+1;
			}
			j = j->next;
		}
	}
	
	int closenesx;
	closenesx = closeness(distance);
	return closenesx;
}

int closeness (int distance[]){
	int closeness = 0;
	int i;
	for (i = 0; i < V+1; i++) {
		if(distance[i] != -1)
			closeness = closeness + distance[i];
	}
	return closeness;
}

int degreeCentrality(Graph* graph, int s){
	AdjListNode* j = graph->array[s].head;
	int score = 0;
	while(j != NULL){
		score++;
		j = j->next;
	}
	printf("SCORE:%d", score);
	return score;
	
}

int searchInNameArr(char name[], char nameArr[][50]){
	int i;
	for(i=0 ; i<V; i++){
		if(strcmp(name, nameArr[i]) == 0)
			return i;
	}
}

void readNames(char fileName[], char nameArr[][50]){
	FILE *file;
	if((file = fopen(fileName, "r")) == NULL){
		printf("ERROR, file does not exist.");
	}
	char name[50];
	char c;
	char line[100000];
	int i,j = 0;
	while ((c = getc(file)) != ';' && c != EOF){
		nameArr[i][j]= c ;
		nameArr[i][j+1]= '\0';
		j++;
	}
	j=0;
	i++;
	   		//putchar(c);
	while(fgets(line, sizeof(line), file) != NULL){
		while ((c = getc(file)) != ';' && c != EOF){
			nameArr[i][j]= c;
			nameArr[i][j+1]= '\0';
			j++;
		}
	   	i++;
	   	j=0;
	}
   		
   	fclose(file);
}

// Function to get size of Graph
int sizeOfGraph(char fileName[]){
	FILE *file;
	if((file = fopen(fileName, "r")) == NULL){
		printf("ERROR, file does not exist.");
	}
	int numberOfLine = 0;
	
	char line[1000000];
	while (fgets(line, sizeof(line), file) != NULL){
		numberOfLine++;
    }
    fclose(file);
    return numberOfLine;
}

// Create a new adjacency list node.
AdjListNode* newAdjListNode(int data) { 
    AdjListNode* newNode = (AdjListNode*) malloc(sizeof(struct AdjListNode)); 
    newNode->data = data; 
    newNode->next = NULL; 
    return newNode; 
}
  
// Function to create graph and adjust it as null
Graph* createGraph(int V) { 
    Graph* graph = (Graph*) malloc(sizeof(Graph)); 
    graph->V = V; 
  
    graph->array = (AdjList*) malloc(V * sizeof(AdjList)); 
    
    int i; 
    for (i = 0; i < V; ++i) 
        graph->array[i].head = NULL; 
  
    return graph; 
}

void addEdge(Graph* graph, int s, int data) {
    AdjListNode* newNode = newAdjListNode(data); 
    newNode->next = graph->array[s].head; 
    graph->array[s].head = newNode; 
} 
  
// Function to print Adjacency list
void printGraph(Graph* graph, char nameArr[][50]) { 
    int i; 
    for (i = 0; i < graph->V; ++i) { 
        AdjListNode* x = graph->array[i].head; 
        printf("\n%s's friends\n", nameArr[i]); 
        while (x){ 
            printf(" -> %s", nameArr[x->data]); 
            x = x->next; 
        } 
        printf("\n"); 
    } 
}

// Init queue
Queue* createQueue(unsigned capacity) { 
    Queue* queue = (Queue*) malloc(sizeof(Queue)); 
    queue->capacity = capacity;
    queue->front = queue->size = 0; 
    queue->rear = capacity - 1;
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(Queue* queue) {  
	return (queue->size == queue->capacity);  
} 
  
// Queue is empty when size is 0 
int isEmpty(Queue* queue) {
	return (queue->size == 0); 
} 
  
// Function to add an item to the queue.
void enqueue(Queue* queue, int item) { 
    if (isFull(queue))
        return; 
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
} 
  
// Function to remove an item from queue.  
int dequeue(Queue* queue) { 
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
} 
  
// Function to get front of queue 
int front(Queue* queue) {
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
} 
  
// Function to get rear of queue
int rear(Queue* queue) {
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}


/////// MATRIX FUNCTIONS ///////////
//init matrix to 0
void createMatrix(int arr[][V]){
    int i,j;
    for(i = 0; i < V; i++)
        for(j = 0; j < V; j++)
            arr[i][j] = 0;
}

void addEdgeMatrix(int arr[][V],int s, int data){
     arr[s][data] = 1;
}

void printAdjMatrix(int arr[][V], char nameArr[][50]){
    int i, j;
    printf("      ");
	for(i = 0; i<V; i++)
		printf("%-16s", nameArr[i]);
	printf("\n");
    for(i = 0; i < V; i++){
    	printf("%-6s", nameArr[i]);
        for(j = 0; j < V; j++){
            printf("%-15d ", arr[i][j]);
        }
        printf("\n");
	}
}

/*

	Göksel Tokur 150116049

*/


