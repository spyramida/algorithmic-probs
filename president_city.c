#include <stdio.h> 
#include <stdlib.h> 


/* 
 *   To lepto T, fevgw apo thn A kai paw sthn B
 *   o proedros pernaei apo tis K, C[K] poleis. prepei na vrw poia xronikh stigmh einai se poia polh
 *   pros to paron, ftiaxnw ton pinaka E[K], p exei pote vrisketai se kapoia polh
 *   3ekinaw gia to shortest path
 *   oi epiloges einai: shortest path-> perimenw ligo 'h paw apo to amesws epomeno shortester?
 *   to dillhma afto einai na vrw to min{amesws epomeno shortest , na ginei to yparxwn shortest: shortest+anamonh}
*/  
       
#define INFINITY 1000002
#define MAX1 60002
#define MAX2 300002




// A structure to represent a node in adjacency list
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency liat
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    struct AdjList* array;
};
 
// Structure to represent a min heap node
struct MinHeapNode
{
    int  v;
    int dist;
};
 
// Structure to represent a min heap
struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};
 
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);
void minHeapify(struct MinHeap* minHeap, int idx);
int isEmpty(struct MinHeap* minHeap);
struct MinHeapNode* extractMin(struct MinHeap* minHeap);
void decreaseKey(struct MinHeap* minHeap, int v, int dist);
int isInMinHeap(struct MinHeap *minHeap, int v);
void printArr(int dist[], int n);
int dijkstra(struct Graph* graph, int src);
struct AdjListNode* newAdjListNode(int dest, int weight);
struct Graph* createGraph(int V);
void addEdge(struct Graph* graph, int src, int dest, int weight);
struct MinHeapNode* newMinHeapNode(int v, int dist);
struct MinHeap* createMinHeap(int capacity);
	
static int N,M,A,B,T,K;  
static int C[MAX2];
static int E[MAX2];

//================================================================================================================
//================================================================================================================
//================================= E N D   O F   D E F I N I T I O N S ==========================================
//================================================================================================================
//================================================================================================================

    /* 
     *  ======			======			======
     *  ======   E N D  O F  M A I N    ======
     *  ======			======			====== 
    */  
int main() {  
          
	int i,j,z,x,t;
	struct Graph* ma_graph;
	struct AdjListNode* temp;
	
	
    scanf("%d %d",&N,&M);  
	scanf("%d %d %d %d",&A,&B,&T,&K);
	A--;
	B--;
	ma_graph=createGraph(N);
	for (i=1; i<=K; i++){
        scanf("%d",&x);
        C[i]=x-1;
    }
	for (i=1; i<=M; i++){
        scanf("%d %d %d",&j,&z,&x);
        j--;
        z--;
        addEdge(ma_graph,j,z,x);
    }

	E[1]=0;
	for (i=2; i<=K; i++){
		temp=ma_graph->array[C[i-1]].head;
		while (temp->dest!=C[i]){
			temp=temp->next;
			if (temp==NULL)
				break;
		}
		
		E[i]=E[i-1]+temp->weight;
        
    }
 
    
    
    x=dijkstra(ma_graph,A);


	printf("%d\n",x);
    }  
      
    /* 
     *  ======			======			======
     *  ======   E N D  O F  M A I N    ======
     *  ======			======			====== 
    */  



//================================================================================================================
//================================================================================================================
//================================================================================================================
//=============================== M I N   H E A P  I M P L E M E N T A T I O N ====================================
//================================================================================================================
//================================================================================================================
//================================================================================================================

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}
 
// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;
 
    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];
 
        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
 
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
 
        minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}
 
// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
 
    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
 
    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
 
    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
 
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return root;
}
 
// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
 
    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;
 
    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
 
        // move to parent index
        i = (i - 1) / 2;
    }
}
 
// A utility function to check if a given vertex
// 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return 1;
   return 0;
}
 
// A utility function used to print the solution
void printArr(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}
 
// The main function that calulates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
int dijkstra(struct Graph* graph, int src)
{
	int t,president,point_to_E=0;//dikes mou
    int V = graph->V;// Get the number of vertices in graph
    int dist[V];      // dist values used to pick minimum weight edge in cut
 
    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);
 
    // Initialize min heap with all vertices. dist value of all vertices 
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INFINITY;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
 
    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
 
    // Initially size of min heap is equal to V
    minHeap->size = V;
 
    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number
 
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        struct AdjListNode* pCrawl = graph->array[u].head;
        
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
 			//egw
 			//an eimai sto u kai 8elw na paw sto v. 
 			//vriskomai thn xronikh stigmh t=T+dist[u], dhladh T_arxiko otan 3ekinhsa+poso ekana na ftasw sto u
 			//oso proxwraei to t, vriskw to E[i]<t<E[i+1]. 
 			//gi afto to i, otan paw sthn C[i]=u kai meta C[i+1]=v

            t=T+dist[u];

            president=0;//president einai to offset pou pros8etw. an den yparxei logos na valw einai 0
            //logw tou Dijkstra, to dist[u], einai af3ousa.pada pairnw apo ta mikrotera sta megalytera
            //ara seiriaka, prospelavnw ton E, kai vriskw to t mou pou anhkei. 
            //an gi afto to t, C[],C[+1] == (u,v) tote 8a exw extra anamonh an parw afto to monopati
            while (E[point_to_E+1]<=t && point_to_E < K){
            	//printf("while: %d\n",point_to_E);
            	point_to_E++;
            }
            if ( (C[point_to_E]==u && C[point_to_E+1]==v) || (C[point_to_E]==v && C[point_to_E+1]==u ) ){
            	president=E[point_to_E+1]-t;
            	//printf("president: %d\n",president);
            }

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && dist[u] != INFINITY  
                                        &&  pCrawl->weight + president + dist[u] < dist[v])
            {

                dist[v] = dist[u] + president + pCrawl->weight;
 
                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
 
    // print the calculated shortest distances
    //printArr(dist, V);
    return dist[B];
}