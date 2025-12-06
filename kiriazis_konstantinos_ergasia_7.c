/*
 * kostas_ergasia_1.c
 * Program to find the minimum cost path between node X and node Y
 * in a graph with costs on each branch.
 * Based on F-8 program with modifications for cost calculation.
 * Author: Your Name
 */

#include <stdio.h>

typedef struct
{
    int a;
    int b;
    int cost;  /* New field for branch cost */
} TBranch;

typedef struct
{
    int p[40];      /* Path nodes */
    int totalCost;  /* Total cost of this path */
    int pk;         /* Path length */
} TPath;

int findnode(int a[], int n, int x)
{
    int i;
    for(i = 0; i < n; i++)
    {
        if(a[i] == x)
        {
            return i;
        }
    }
    return -1;
}

void printarr(int a[], int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        printf("%3d", a[i]);
    }
}

void printPath(TPath path)
{
    int i;
    printf("Path: ");
    for(i = 0; i < path.pk; i++)
    {
        printf("%3d", path.p[i]);
    }
    printf("  | Cost: %d\n", path.totalCost);
}

void printbranches(TBranch a[], int nb)
{
    int i;
    for(i = 0; i < nb; i++)
    {
        printf("%3d: %3d %3d (cost: %d)\n", i, a[i].a, a[i].b, a[i].cost);
    }
}

/* Global variables to track the best path found */
TPath bestPath;
int bestCost = 999999; /* Initialize with a very large number */

void findMinPath(int current, int target, 
                 int netw[][40], int costs[][40], 
                 int n, int path[], int *pk, int currentCost)
{
    int i;
    
    /* Add current node to path */
    path[*pk] = current;
    *pk = *pk + 1;
    
    /* If we reached the target */
    if(current == target)
    {
        /* Found a complete path */
        TPath foundPath;
        int j;
        
        /* Copy path to foundPath */
        for(j = 0; j < *pk; j++)
        {
            foundPath.p[j] = path[j];
        }
        foundPath.pk = *pk;
        foundPath.totalCost = currentCost;
        
        /* Print the found path */
        printPath(foundPath);
        
        /* Check if this is better than the best so far */
        if(currentCost < bestCost)
        {
            bestCost = currentCost;
            bestPath = foundPath;
        }
        
        /* Backtrack */
        *pk = *pk - 1;
        return;
    }
    
    /* Try all adjacent nodes */
    for(i = 0; i < n; i++)
    {
        if(netw[current][i] != 0)
        {
            /* Check if node i is not already in the path */
            if(findnode(path, *pk, i) == -1)
            {
                /* Add the cost of this branch */
                int newCost = currentCost + costs[current][i];
                
                /* Only continue if this path could be better than the best so far */
                if(newCost < bestCost)
                {
                    findMinPath(i, target, netw, costs, n, path, pk, newCost);
                }
            }
        }
    }
    
    /* Backtrack */
    *pk = *pk - 1;
}

int main(void)
{
    TBranch a[40];
    int netw[40][40] = {0};
    int costs[40][40] = {0}; /* New matrix for costs */
    int path[40];
    int pk;
    int i, nb, nn, maxn;
    int startNode, endNode;
    
    /* Test data with costs */
    nb = 9; /* 9 branches for better testing */
    a[0].a = 0; a[0].b = 1; a[0].cost = 2;
    a[1].a = 1; a[1].b = 2; a[1].cost = 3;
    a[2].a = 2; a[2].b = 3; a[2].cost = 1;
    a[3].a = 3; a[3].b = 4; a[3].cost = 4;
    a[4].a = 0; a[4].b = 3; a[4].cost = 7;
    a[5].a = 0; a[5].b = 4; a[5].cost = 8;
    a[6].a = 2; a[6].b = 4; a[6].cost = 2;
    a[7].a = 1; a[7].b = 3; a[7].cost = 5; /* Additional branch */
    a[8].a = 1; a[8].b = 4; a[8].cost = 6; /* Additional branch */
    
    printf("\n");
    printf("===========================================\n");
    printf("MINIMUM COST PATH FINDER\n");
    printf("Based on F-8 program with cost extensions\n");
    printf("===========================================\n\n");
    
    printf("Processing graph data...\n");
    printf("# branches: %d\n", nb);
    printf("List of branches with costs:\n");
    printbranches(a, nb);
    
    /* Build adjacency matrix and cost matrix */
    maxn = 0;
    for(i = 0; i < nb; i++)
    {
        netw[a[i].a][a[i].b] = 1;
        netw[a[i].b][a[i].a] = 1;
        costs[a[i].a][a[i].b] = a[i].cost;
        costs[a[i].b][a[i].a] = a[i].cost;
        
        if(a[i].a > maxn) maxn = a[i].a;
        if(a[i].b > maxn) maxn = a[i].b;
    }
    
    nn = maxn + 1;
    printf("# nodes: %d\n", nn);
    
    /* Reset best path variables */
    bestCost = 999999;
    bestPath.pk = 0;
    
    /* Find minimum cost path from node 1 to node 4 */
    startNode = 1;
    endNode = 4;
    
    printf("\n");
    printf("===========================================\n");
    printf("Finding all paths from node %d to node %d:\n", startNode, endNode);
    printf("===========================================\n");
    
    pk = 0;
    findMinPath(startNode, endNode, netw, costs, nn, path, &pk, 0);
    
    printf("\n");
    printf("===========================================\n");
    printf("RESULT:\n");
    printf("===========================================\n");
    
    if(bestPath.pk > 0)
    {
        printf("Minimum cost path found!\n");
        printf("Path: ");
        for(i = 0; i < bestPath.pk; i++)
        {
            printf("%d", bestPath.p[i]);
            if(i < bestPath.pk - 1)
            {
                printf(" -> ");
            }
        }
        printf("\nTotal cost: %d\n", bestCost);
    }
    else
    {
        printf("No path found between node %d and node %d\n", startNode, endNode);
    }
    
    printf("\n");
    return 0;
}
