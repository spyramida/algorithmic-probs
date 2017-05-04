#include <stdio.h>


/*
 *  
 * 
 * 
*/
 
typedef struct edge
{
    int u,v,w,f;
}edge;
 
typedef struct edgelist
{
    edge data[600001];
    int n;
}edgelist;
 
edgelist elist;
 
//static int A[10001][10001];
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
		i--;
		j--;    
		if (j<i){
			TEMP=i;
			i=j;
			j=TEMP;
		}
		//A[i][j]=x;
        //A[j][i]=x;
		elist.data[elist.n].u=i;
        elist.data[elist.n].v=j;
        elist.data[elist.n].w=x;
        elist.n++;
	}
	
	
	
	for (i=0; i<n; i++){
		if (B[i]!=0){
    		//A[n][i]=B[i];
			elist.data[elist.n].u=n;
        	elist.data[elist.n].v=i;
        	elist.data[elist.n].w=B[i];
        	elist.data[elist.n].f=2;
        	elist.n++;
		}
		
	}
    quicksort(0,elist.n-1);
    flags=1;
    cost=0;
    kruskal();
    first=cost;

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
    int belongs[10001],i,j,cno1,cno2,min;
    
    for(i=0;i<n;i++)
        belongs[i]=i;
    
    spanlist.n=0;
    for(i=0;i<elist.n;i++)
       {
       	if (flags==1 && elist.data[i].f==2)
       		continue;
        cno1=find(belongs,elist.data[i].u);
        cno2=find(belongs,elist.data[i].v);
        
        if(cno1!=cno2)
        {
        	spanlist.data[spanlist.n]=elist.data[i];
        	cost+=spanlist.data[spanlist.n].w;
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
             while(elist.data[i].w<=elist.data[pivot].w && i<last)
                 i++;
             while(elist.data[j].w>elist.data[pivot].w)
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