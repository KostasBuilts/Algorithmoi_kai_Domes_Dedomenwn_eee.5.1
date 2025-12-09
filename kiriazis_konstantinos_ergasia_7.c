/* 
Find path in graph 

Test data 

# branches: 7 
list of branches: 
  0: 0 1 
  1: 1 2 
  2: 2 3 
  3: 3 4 
  4: 0 3 
  5: 0 4 
  6: 2 4 
# nodes: 5 

find path: 1 0 3 2 4 
find path: 1 0 3 4 
find path: 1 0 4 
find path: 1 2 3 0 4 
find path: 1 2 3 4 
find path: 1 2 4 
*/ 
#include <stdio.h> 
#include <math.h> 




typedef struct 
{ 
    int node1; 
    int node2; 
    int branch_weight;
} TBranch; 

typedef struct 
{ 
    int path_nodes[40]; 
    int pk; 
    int path_cost;
} TPath; 


void copyArray(int src[], int dst[], int len)
{
    int i;
    for (i=0; i<len; i++)
    {
        dst[i]=src[i];
    }
}


///////////////////////////////////////////////////////////////////////////////////////////
int findnode( int a[], int currentNode, int x) 
{   
    int i; 
    for(i=0; i<currentNode; i++) 
    { 
        if(a[i]==x) 
        {            
            return i; 
        } 
    }    
    return -1; 
}

///////////////////////////////////////////////////////////////////////////////////////////
void findSortestPath(TPath allPaths[], int numOfPaths, int minPaths[]) 
{   
    int i;
	int j=0;
    int minPathCost = allPaths[0].path_cost;
    for(i=1; i<numOfPaths; i++) 
    { 
        if(allPaths[i].path_cost < minPathCost) 
        {            
            minPathCost = allPaths[i].path_cost;
        } 
    }
    for(i=0; i<numOfPaths; i++)
    {
		if(minPathCost == allPaths[i].path_cost)
		{
			minPaths[j] = i;
			j++;
		}
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void printarr( int a[], int n) 
{
    int i; 
    for( i=0; i<n; i++) 
    {   
        printf("%3d", a[i]);
    } 
    printf("\n"); 
}


///////////////////////////////////////////////////////////////////////////////////////////
void printPath( TPath a) 
{   
    int i; 
    for( i=0; i<a.pk; i++) 
    {   
        printf("%3d", a.path_nodes[i]);
    } 
    printf("\n"); 
} 


///////////////////////////////////////////////////////////////////////////////////////////
void printbranches( TBranch branches[], int nb) 
{   
    int i; 
    for( i=0; i<nb; i++) 
    {   
        printf("%3d: %3d %3d  weight= %d\n", i, branches[i].node1, branches[i].node2, branches[i].branch_weight); 
    } 
} 


///////////////////////////////////////////////////////////////////////////////////////////
void findpath( int startNode, int endNode, int con_arr[][40], int numberOfNodes, int p_arr[], int *pk, int *p_idx, TPath path[]) 
{    
    int i; 
    p_arr[*pk]=startNode; 
    *pk=*pk+1; 
    if (startNode==endNode) 
    { 
        copyArray(p_arr, path[*p_idx].path_nodes, *pk); // apothikeuo ta nodes tou path stin structure
        path[*p_idx].pk=*pk;                            // apothikeuo to plithos ton nodes

        printf("find path: ");  printarr( p_arr, *pk); 
        *p_idx = *p_idx + 1;
        return; 
    } 

    for(i=0; i<numberOfNodes; i++) 
    { 
        if (con_arr[startNode][i] > 0) 
        { 
            if (findnode( p_arr, *pk, i) == -1 ) 
            { 
                findpath( i, endNode, con_arr, numberOfNodes, p_arr, pk, p_idx, path); 
                *pk= *pk - 1; 
            } 
        } 
    }
     
} 




///////////////////////////////////////////////////////////////////////////////////////////
int main(void) 
{ 
    TBranch myBranches[40]; 
    int connection_array[40][40]={0}; 
    int path_array[40]; 
    int equalShortPaths[40];
    int pk; 
    int i, num_of_branches, num_of_nodes, maxn, path_idx;
    TPath myPaths[40];

    /* 
    //read graph 
    printf("enter # branches: "); 
    scanf("%d",&num_of_branches); 

    for(i=0; i<num_of_branches; i++) 
    {   
        printf("enter branch  %d:\n",i); 
        scanf("%d",&myBranches[i].node1); 
        scanf("%d",&myBranches[i].node2); 
        scanf("%d",&myBranches[i].branch_weight); 
    } 
    */ 
    /**/  
    //test graph 
    num_of_branches=12; 
	myBranches[0].node1=4; 
    myBranches[0].node2=6; 
    myBranches[0].branch_weight=6;
	myBranches[1].node1=2; 
    myBranches[1].node2=6; 
    myBranches[1].branch_weight=9;
    myBranches[2].node1=0; 
    myBranches[2].node2=1; 
    myBranches[2].branch_weight=4;
    myBranches[3].node1=1; 
    myBranches[3].node2=2; 
    myBranches[3].branch_weight=7;
    myBranches[4].node1=2; 
    myBranches[4].node2=3; 
    myBranches[4].branch_weight=0;
    myBranches[5].node1=3; 
    myBranches[5].node2=4; 
    myBranches[5].branch_weight=9;
    myBranches[6].node1=0; 
    myBranches[6].node2=3;
    myBranches[6].branch_weight=6;   
    myBranches[7].node1=0; 
    myBranches[7].node2=4;
    myBranches[7].branch_weight=3;   
    myBranches[8].node1=2; 
    myBranches[8].node2=4; 
    myBranches[8].branch_weight=8;
    myBranches[9].node1=4; 
    myBranches[9].node2=5; 
    myBranches[9].branch_weight=2;
    myBranches[10].node1=0; 
    myBranches[10].node2=5; 
    myBranches[10].branch_weight=1;
    myBranches[11].node1=2; 
    myBranches[11].node2=5; 
    myBranches[11].branch_weight=1;
    /**/ 
    printf("\n\n"); 
    printf("Processing ..\n"); 
    printf("# branches: %d\n", num_of_branches); 
    printf("list of branches:\n"); 
    printbranches( myBranches, num_of_branches); 
    maxn=0; 

    // dimioyrgia Network Array
    for(i=0; i<num_of_branches; i++) 
    {   
        if (myBranches[i].branch_weight<=0) // check for illegal weights
        {
            printf("\n*********************** WARNING ************************\n");
            printf("The branch %d<->%d has weight=%d (the branch is ingnored)\n", myBranches[i].node1, myBranches[i].node2, myBranches[i].branch_weight);
            printf("********************************************************\n");
        }

        //tranform input branches into 2 dimension connection network array netw[][]
        connection_array[myBranches[i].node1][myBranches[i].node2]= myBranches[i].branch_weight; 
        connection_array[myBranches[i].node2][myBranches[i].node1]= myBranches[i].branch_weight; 
        if (myBranches[i].node1 > maxn) 
            maxn=myBranches[i].node1;
        if (myBranches[i].node2 > maxn) 
            maxn=myBranches[i].node2; 
    } 
    num_of_nodes = maxn+1; 
    printf("# nodes: %d\n\n", num_of_nodes); 
    
    //find path from node 1 to node 4 
    pk=0; path_idx=0;
    findpath( 1, 4, connection_array, num_of_nodes, path_array, &pk, &path_idx, myPaths);  

    // calculate total_costs per path
    for(i=0; i<path_idx; i++)
    {
        for(int j=0; j<myPaths[i].pk-1; j++)  //for each path iterate all nodes
        {
            myPaths[i].path_cost =  + myPaths[i].path_cost + connection_array[myPaths[i].path_nodes[j]][myPaths[i].path_nodes[j+1]];
        }
    }


    printf("\n---------------------------- RESULTS -----------------------------\n");
    for(i=0; i<path_idx; i++)
    {
        printf("pathNo=%2d cost=%3d  nodeCount=%2d  nodeArray=", i, myPaths[i].path_cost, myPaths[i].pk );
        printPath(myPaths[i]);
    }


    findSortestPath(myPaths, path_idx, equalShortPaths);
    printf("\n================================================================================");
	printf("\nThe sortest path is No=%2d cost=%3d  nodeCount=%2d  path=", equalShortPaths[0], myPaths[equalShortPaths[0]].path_cost, myPaths[0].pk );
	printPath(myPaths[equalShortPaths[0]]);
	for(i=1; i<path_idx; i++)
	{
		if(equalShortPaths[i] != 0)
		{
    		printf("Same cost for path  No=%2d cost=%3d  nodeCount=%2d  path=", equalShortPaths[i], myPaths[equalShortPaths[i]].path_cost, myPaths[equalShortPaths[i]].pk );
    		printPath(myPaths[equalShortPaths[i]]);
		}
	}
    printf("================================================================================\n\n");

    return 0; 
}
