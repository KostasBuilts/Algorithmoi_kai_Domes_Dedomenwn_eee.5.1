/*
Circuit solver with V, I and R
Attention! If we have 2 branches with same nodes we cannot distinguish them,
so we insert virtual node and corresponding R 0
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
    int a;
    int b;
    char type;
    float value;
} TBranch;

typedef struct
{
    int p[40];
    int pk;
} TLoop;

int findnode(int a[], int n, int x);
void findpath(int n0, float a[][40], int n, int p[], int *pk, TLoop iloops[], int *ilk);

int findbranchinpath(int a, int b, TLoop path);
int findbranchinlistofpaths(int a, int b, TLoop iloops[], int ilk);
int findindyloop(TLoop path, TLoop iloops[], int ilk);

void printloop(TLoop a);
void printbranches(TBranch a[], int nb);
void printbranchesvalues(TBranch a[], int nb, float ii[]);

void printarr(int a[], int n);

int findbranchidx(int a, int b, TBranch net[], int nb, char *pt, float *pv);
void printmatrix(float a[][40], int n);

void solvesystem(float a[40][40], int n, float c[], float x[]);
float determinant(float matrix[40][40], int size);
void addcurrentsourcestoRHS(TBranch branches[], int nb, float vv[], int ilk, int indylk);

int main(void)
{
    TBranch a[40];
    int nb;
    int nn;
    
    float netw[40][40] = {0};
    int p[40];
    int pk;
    
    int i, maxn;
    int j;
        
    TLoop iloops[100];
    int ilk;

    TLoop indyloops[100];
    int indylk;
        
    int k, n1, n2;
    char t;
    float v;
    int tcode;

    float rr[40][40] = {0};
    float vv[40] = {0};
    float ii[40];
    
    // read circuit
    printf("Enter number of branches: ");
    scanf("%d", &nb);
    
    for(i = 0; i < nb; i++)
    {   
        printf("Enter branch %d:\n", i);
        printf("Node A: "); 
        scanf("%d", &a[i].a);
        printf("Node B: ");
        scanf("%d", &a[i].b);
        printf("Type V, I or R (0=V, 1=R, 2=I): ");
        scanf("%d", &tcode);
        
        if(tcode == 0)
        {
            a[i].type = 'V';
        }
        else if(tcode == 1)
        {
            a[i].type = 'R';
        }
        else if(tcode == 2)
        {
            a[i].type = 'I';
        }
        else
        {
            a[i].type = 'x';    
        }
        
        printf("Value: ");
        scanf("%f", &a[i].value);        
    }
    
    /* test circuit with current source
    nb = 5;
    
    a[0].a = 0;
    a[0].b = 1;
    a[0].type = 'V';
    a[0].value = 10;
    
    a[1].a = 1;
    a[1].b = 2;
    a[1].type = 'R';
    a[1].value = 5;
    
    a[2].a = 2;
    a[2].b = 0;
    a[2].type = 'R';
    a[2].value = 10;
    
    a[3].a = 1;
    a[3].b = 3;
    a[3].type = 'I';
    a[3].value = 2;
    
    a[4].a = 3;
    a[4].b = 0;
    a[4].type = 'R';
    a[4].value = 8;
    */
    
    printf("\n");
    printf("Number of branches: %d\n", nb);
    printf("List of branches:\n");
    printbranches(a, nb);
    
    maxn = 0;
    for(i = 0; i < nb; i++)
    {   
        if(a[i].type != 'I')  // current sources don't create connections for loop finding
        {
            netw[a[i].a][a[i].b] = 1;
            netw[a[i].b][a[i].a] = 1;
        }
        
        if(a[i].a > maxn)
            maxn = a[i].a;
        if(a[i].b > maxn)
            maxn = a[i].b;
    }
    
    nn = maxn + 1;
    printf("Number of nodes: %d\n", nn);
    
    /* find all loops/paths from node 0 */
    ilk = 0;
    pk = 0; 
    findpath(0, netw, nn, p, &pk, iloops, &ilk);    
    
    /* filter iloops and find the independent loops */
    indylk = 0;
    for(i = 0; i < ilk; i++)
    {
        if(findindyloop(iloops[i], indyloops, indylk) == 1)
        {
            indyloops[indylk] = iloops[i];
            indylk = indylk + 1;
        }
    }
    
    /* debug
    printf("Independent loops from node 0:\n");
    for(i = 0; i < indylk; i++)
    {   
        printf("%3d: ", i);
        printloop(indyloops[i]);
    }
    */
    
    // Apply KVL for each independent loop
    for(i = 0; i < indylk; i++)
    {
        for(j = 0; j < indyloops[i].pk - 1; j++)
        {
            n1 = indyloops[i].p[j];
            n2 = indyloops[i].p[j + 1];
            k = findbranchidx(n1, n2, a, nb, &t, &v);
            
            if(k != -1)
            {
                if(t == 'V')
                {
                    vv[i] = vv[i] - v;  // KVL: sum of voltage sources
                }
                else if(t == 'R')
                {
                    rr[i][k] = rr[i][k] + v;  // R*i term
                }
                // Current sources don't appear in KVL equations directly
            }
        }
    }
    
    // Apply KCL for each node (except reference node 0)
    int current_equation_idx = indylk;  // Start after KVL equations
    
    for(i = 0; i < nn - 1; i++)  // For nodes 1 to nn-1 (node 0 is ground)
    {
        for(j = 0; j < nb; j++)
        {
            if(a[j].a == i + 1)  // Current leaving node i+1
            {
                if(a[j].type == 'I')
                {
                    // Current source contributes to RHS
                    vv[current_equation_idx] -= a[j].value;
                }
                else
                {
                    rr[current_equation_idx][j] = 1;
                }
            }
            if(a[j].b == i + 1)  // Current entering node i+1
            {
                if(a[j].type == 'I')
                {
                    // Current source contributes to RHS
                    vv[current_equation_idx] += a[j].value;
                }
                else
                {
                    rr[current_equation_idx][j] = -1;
                }
            }
        }
        current_equation_idx++;
    }
    
    // Handle current sources in KVL equations (they contribute to RHS)
    addcurrentsourcestoRHS(a, nb, vv, indylk, indylk + nn - 1);
    
    /* debug
    printf("KVL+KCL matrix:\n");
    printmatrix(rr, nb);
    
    printf("RHS vector:\n");
    for(i = 0; i < nb; i++)
        printf("%6.1f ", vv[i]);
    printf("\n");
    */
    
    solvesystem(rr, nb, vv, ii);
    
    printf("\nBranch currents:\n");
    printbranchesvalues(a, nb, ii);
    
    return 0;
}

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
    printf("\n");
}

void printloop(TLoop a)
{   
    int i;
    for(i = 0; i < a.pk; i++)
    {   
        printf("%3d", a.p[i]);
    }
    printf("\n");
}

void printbranches(TBranch a[], int nb)
{   
    int i;
    for(i = 0; i < nb; i++)
    {   
        printf("%3d: %3d %3d  %c %6.1f\n", i, a[i].a, a[i].b, a[i].type, a[i].value);
    }
}

int findbranchinpath(int a, int b, TLoop path)
{   
    int i;
    for(i = 0; i < path.pk - 1; i++)
    {
        if((a == path.p[i] && b == path.p[i + 1]) ||
           (b == path.p[i] && a == path.p[i + 1]))
        {
            return 1;
        }
    }
    return -1;
}

int findbranchinlistofpaths(int a, int b, TLoop iloops[], int ilk)
{   
    int i;
    for(i = 0; i < ilk; i++)
    {
        if(findbranchinpath(a, b, iloops[i]) == 1)
        {
            return 1;
        }
    }
    return -1;
}

int findindyloop(TLoop path, TLoop iloops[], int ilk)
{   
    int i, a, b;
    for(i = 0; i < path.pk - 1; i++)
    {
        a = path.p[i];
        b = path.p[i + 1];
        
        if(findbranchinlistofpaths(a, b, iloops, ilk) == -1)
        {
            return 1;
        }
    }
    return -1;
}

void findpath(int n0, float a[][40], int n, int p[], int *pk, TLoop iloops[], int *ilk)
{   
    int i, jj;
    
    p[*pk] = n0;
    *pk = *pk + 1;
    
    for(i = 0; i < n; i++)
    {
        if(a[n0][i] != 0)
        {
            if(i == p[0])
            {
                if(*pk > 2)
                {                    
                    for(jj = 0; jj < *pk; jj++)
                        iloops[*ilk].p[jj] = p[jj];
                    iloops[*ilk].p[*pk] = p[0];
                      
                    iloops[*ilk].pk = *pk + 1;  
                    *ilk = *ilk + 1;
                }
            }
            else
            {
                if(findnode(p, *pk, i) == -1)
                {
                    findpath(i, a, n, p, pk, iloops, ilk);
                    *pk = *pk - 1;
                }
            }
        }
    }
}

int findbranchidx(int a, int b, TBranch net[], int nb, char *pt, float *pv)
{   
    int i;
    for(i = 0; i < nb; i++)
    {
        if((net[i].a == a) && (net[i].b == b))
        {
            *pt = net[i].type;
            *pv = net[i].value;
            return i;
        }

        if((net[i].a == b) && (net[i].b == a))
        {
            *pt = net[i].type;
            *pv = -net[i].value;
            return i;
        }
    }
    return -1;            
}

void printmatrix(float a[][40], int n)
{   
    int i, j;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {   
            printf("%6.1f", a[i][j]);
        }
        printf("\n");
    }
}

float determinant(float matrix[40][40], int size)
{
    float s = 1, det = 0, m_minor[40][40];
    int i, j, m, n, c;
    
    if(size == 1)
    {
        return matrix[0][0];
    }
    else
    {
        det = 0;
        for(c = 0; c < size; c++)
        {
            m = 0;
            for(i = 0; i < size; i++)
            {   
                if(i != 0)
                {   
                    n = 0;
                    for(j = 0; j < size; j++)
                    {   
                        if(j != c)
                        {   
                            m_minor[m][n] = matrix[i][j];
                            n = n + 1;
                        }                        
                    }                
                    m = m + 1;
                }
            }
            det = det + s * (matrix[0][c] * determinant(m_minor, size - 1));
            s = -1 * s;
        }
    }
    return det;
}

void solvesystem(float a[40][40], int n, float c[], float x[])
{
    float d, dx;
    int i, j, z;
    float b[40][40];
    
    d = determinant(a, n);
    
    if(fabs(d) < 1e-10)
    {
        printf("Error: Singular matrix! Circuit may have problems.\n");
        for(z = 0; z < n; z++)
            x[z] = 0;
        return;
    }
    
    for(z = 0; z < n; z++)
    {
        for(i = 0; i < n; i++)
        {   
            for(j = 0; j < n; j++)
            {   
                if(j == z)
                {   
                    b[i][j] = c[i];
                }
                else
                {   
                    b[i][j] = a[i][j];
                }
            }
        }
        
        dx = determinant(b, n);
        x[z] = dx / d;
    }
}

void printbranchesvalues(TBranch a[], int nb, float ii[])
{   
    int i;
    for(i = 0; i < nb; i++)
    {   
        printf("%3d: %3d %3d  %c %6.2f  I = %6.2f\n", i, a[i].a, a[i].b, a[i].type, a[i].value, ii[i]);
    }
}

void addcurrentsourcestoRHS(TBranch branches[], int nb, float vv[], int ilk, int indylk)
{
    int i, j, k;
    char t;
    float v;
    
    // For each current source, find which loops it belongs to and adjust RHS
    for(i = 0; i < nb; i++)
    {
        if(branches[i].type == 'I')
        {
            // Check each independent loop
            for(j = 0; j < ilk; j++)
            {
                // We need to know if current source is in this loop
                // This requires loop information which isn't fully implemented here
                // In a complete implementation, we would track which branches are in each loop
            }
        }
    }
}
