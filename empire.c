#include <stdio.h>


/*
 * In a Galaxy far far away, the Empire decides to build a new space station, for commerce business. 
 * For this reason, she creates transport channels between planets. 
 * The creation cost, between planets i and j costs A[i,j]. 
 * There are also K-planets capable of hosting teleporters! 
 * The cost of a teleporter at the i-planet is B[i]. 
 * The only way to transfer the essentials between two planets, is for them to be 
 * connected through a combination of transport channels and/or teleporters. 
 * Input  : N number of planets
 *	       K number of planets capable of building a teleporter
 *	       M number of possible transport channels
 *	       B[i] cost of building a teleporter in i-planet
 *	       i,j,A[i,j] cost of building a transport channel between planets i and j
 * Output : Minimum cost (if there is one), to make possible the transfer between any two planets
 * 
 * A Minimum Spanning Tree exercise. I use the Kruskal algorithm.
 * 
 * I implement the teleporters, by adding 1 more edge in the graph.
 * Every edge-planet capable of a teleporter is connected with this new edge. The "concept" of the 
 * teleporter connencts each teleporter-capable planet.
*/
 
typedef struct edge
{
    int u,v,weight,flag;
	//u 	:
	//v		:
	//weight	:
	//f		:
}edge;


typedef struct edgelist
{
    edge data[600001];
    int n;
	//edgelist.n, signifies the size of the edgelist
}edgelist;
 
edgelist elist;
/* 
 * elist, is  a list having each combination of a possible trasnport line
 * between planet edge.u and edge.v with a cost of edge.weight.
 */
edgelist spanlist;
 
void kruskal();
int find(int belongs[],int vertexno);
void union1(int belongs[],int c1,int c2);
void quicksort();
int cost;

static int n,K,M;
static int B[10000]; 
int flags;


/*
 *
 *  ------   M A I N   ------
*/
int main(){
	
	int i,j,z,x,zero,first,second,TEMP;
    
	//FILE *fp;

    //fp = fopen("spacemst/input9.txt","r");  
	//fscanf(fp,"%d %d %d",&n,&K,&M);
	scanf("%d %d %d",&n,&K,&M);
	
	for (j=1; j<=K; j++){
			//fscanf(fp,"%d %d",&i,&x);
			scanf("%d %d",&i,&x);
			B[i-1]=x;
		}

	elist.n=0;
	for (z=1; z<=M; z++){
		//fscanf(fp,"%d %d",&i,&j);
		//fscanf(fp,"%d",&x);
		scanf("%d %d",&i,&j);
		scanf("%d",&x);
		i--; //since, the 1st planet-edge is the 0, every input of i,j needs to be decremented by 1
		j--; //also, i make sure that the path between 2 edges is recorded from the smaller (i) edge 
			 // to the bigger (j) edge.
		if (j<i){
			TEMP=i;
			i=j;
			j=TEMP;
		}

		elist.data[elist.n].u=i;
        elist.data[elist.n].v=j;
        elist.data[elist.n].weight=x;
        elist.n++;
	}
	
	
	/*
	 * Adding the extra edge for the teleporter, connected with every planet capable of building a teleporter.
	 * the edge.flag==2 points that this vertice is a teleporting one.
	*/
	for (i=0; i<n; i++){
		if (B[i]!=0){
			elist.data[elist.n].u=n;
        	elist.data[elist.n].v=i;
        	elist.data[elist.n].weight=B[i];
        	elist.data[elist.n].flag=2;
        	elist.n++;
		}
		
	}
    quicksort(0,elist.n-1); //sorting the vertices to implement kruskal
    flags=1;
    cost=0;
	/*
	 * First execution of kruskal is without taking into consideration the teleporters.
	 *
	 */
    kruskal();
    first=cost;

	// flags=2, and n++ means that now, we examine if its worth building at least the first 2 teleporters
	// and then if i should keep building.
	flags=2;
	n++;
	cost=0;	
	kruskal();
	second=cost;

    if (first<second)
			printf("%d\n",first);
		else
			printf("%d\n",second);
	
}

/*
 *
 *  ------   E N D  O F  M A I N   ------
*/

void kruskal()
{
	/*
	 * I dont actually need the spanning tree, just its cost
	*/
    int belongs[10001],i,j,cno1,cno2,min;
    
    for(i=0;i<n;i++)
        belongs[i]=i;
    
    spanlist.n=0;
    for(i=0;i<elist.n;i++)
       {
       	if (flags==1 && elist.data[i].flag==2)
       		continue;
        cno1=find(belongs,elist.data[i].u);
        cno2=find(belongs,elist.data[i].v);
        // if cno1==cno2, then this edge, already exists in the mst, so adding 
		// this vertice will create a cycle.
        if(cno1!=cno2)
        {
        	spanlist.data[spanlist.n]=elist.data[i];
        	cost+=spanlist.data[spanlist.n].weight;
            spanlist.n=spanlist.n+1;
            union1(belongs,cno1,cno2);
        }
     
    }
}

 
int find(int belongs[],int vertexno)
{
    return(belongs[vertexno]);
}
 
void union1(int belongs[],int c1,int c2)
{
    int i;
    
    for(i=0;i<n;i++)
        if(belongs[i]==c2)
            belongs[i]=c1;
}
 
void quicksort(int first , int last)
{
	int pivot,j,i;
	edge temp;
     if(first<last){
         pivot=first;
         i=first;
         j=last;

         while(i<j){
             while(elist.data[i].weight<=elist.data[pivot].weight && i<last)
                 i++;
             while(elist.data[j].weight>elist.data[pivot].weight)
                 j--;
             if(i<j){
                 temp=elist.data[i];
                  elist.data[i]=elist.data[j];
                  elist.data[j]=temp;
             }
         }

         temp=elist.data[pivot];
         elist.data[pivot]=elist.data[j];
         elist.data[j]=temp;
         quicksort(first,j-1);
         quicksort(j+1,last);

	}
}