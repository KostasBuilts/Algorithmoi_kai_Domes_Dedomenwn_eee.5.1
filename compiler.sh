#include <stdio.h>

#define TOTAL_NUMBERS 12
#define COMBINATION_SIZE 6
#define USER_COLUMNS 33

int user_columns[USER_COLUMNS][COMBINATION_SIZE] = {
    {1, 2, 3, 4, 5, 6}, 
    {1, 2, 3, 7, 8, 9},
    {1, 2, 3, 10, 11, 12},
    {1, 2, 4, 7, 8, 10},
    {1, 2, 4, 9, 11, 12},
    {1, 2, 5, 7, 8, 11},
    {1, 2, 5, 9, 10, 12},
    {1, 2, 6, 7, 8, 12},
    {1, 2, 6, 9, 10, 11},
    {1, 3, 4, 7, 9, 10},
    {1, 3, 4, 8, 11, 12},
    {1, 3, 5, 7, 9, 11},
    {1, 3, 5, 8, 10, 12},
    {1, 3, 6, 7, 9, 12},
    {1, 3, 6, 8, 10, 11},
    {1, 4, 5, 7, 10, 11},
    {1, 4, 5, 8, 9, 12},
    {1, 4, 6, 7, 10, 12},
    {1, 4, 6, 8, 9, 11},
    {1, 5, 6, 7, 11, 12},
    {1, 5, 6, 8, 9, 10},
    {2, 3, 4, 7, 11, 12},
    {2, 3, 4, 8, 9, 10},
    {2, 3, 5, 7, 10, 12},
    {2, 3, 5, 8, 9, 11},
    {2, 3, 6, 7, 10, 11},
    {2, 3, 6, 8, 9, 12},
    {2, 4, 5, 7, 9, 12},
    {2, 4, 5, 8, 10, 11},
    {2, 4, 6, 7, 9, 11},
    {2, 4, 6, 8, 10, 12},
    {2, 5, 6, 7, 9, 10},
    {2, 5, 6, 8, 11, 12}
};

int combination[COMBINATION_SIZE];
int results[100][4];
int results_count;

int ncr(int n, int k);
void combine(int na, int n, int b[], int t, int L);
void printcomb(int a[], int n);
int count_matches(int comb1[], int comb2[]);
void add_result(int sixes, int fives, int fours);
void print_results();

int main(void)
{
    int i, j;
   
    printf("612D0K system code\n");
    printf("6 12 33\n");
   
    for(i = 0; i < USER_COLUMNS; i++)
    {
        for(j = 0; j < COMBINATION_SIZE; j++)
        {
            printf("%d", user_columns[i][j]);
            if(j < COMBINATION_SIZE - 1) printf(" ");
        }
        printf("\n");
    }
   
    results_count = 0;
    for(i = 0; i < 100; i++)
    {
        results[i][0] = 0;
        results[i][1] = 0;
        results[i][2] = 0;
        results[i][3] = 0;
    }
   
    printf("GOALS\n");
    printf("      %d combinations in total %d choose %d\n",ncr(TOTAL_NUMBERS,COMBINATION_SIZE), TOTAL_NUMBERS, COMBINATION_SIZE);
   
    combine(TOTAL_NUMBERS, COMBINATION_SIZE, combination, 1, 0);
   
    print_results();
   
    return 0;
}

int ncr(int n, int k)
{
    int p, pk, i;
    p = 1;
   
    for(i = n - k + 1; i <= n; i++)
    {
        p = p * i;
    }
   
    pk = 1;
    for(i = 1; i <= k; i++)
    {
        pk = pk * i;
    }
    return p / pk;
}

void combine(int na, int n, int b[], int t, int L)
{
    int i;
   
    for(i = t; i <= na - n + L + 1; i++)
    {
        b[L] = i;
        if(L == n - 1)
        {
            int sixes = 0, fives = 0, fours = 0;
            int j;
           
            for(j = 0; j < USER_COLUMNS; j++)
            {
                int matches = count_matches(b, user_columns[j]);
               
                if(matches == 6) sixes++;
                else if(matches == 5) fives++;
                else if(matches == 4) fours++;
            }
            add_result(sixes, fives, fours);
        }
        else
        {
            combine(na, n, b, i + 1, L + 1);
        }
    }
}

void printcomb(int a[], int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        printf("%3d", a[i]);
    }
    printf("\n");
}


int count_matches(int comb1[], int comb2[])
{
    int i, j, count = 0;
   
    for(i = 0; i < COMBINATION_SIZE; i++)
    {
        for(j = 0; j < COMBINATION_SIZE; j++)
        {
            if(comb1[i] == comb2[j])
            {
                count++;
                break;
            }
        }
    }
   
    return count;
}



void add_result(int sixes, int fives, int fours)
{
    int i;
    int found = 0;
   
    for(i = 0; i < results_count; i++)
    {
        if(results[i][1] == sixes && results[i][2] == fives && results[i][3] == fours)
        {
            results[i][0]++;
            found = 1;
            break;
        }
    }
   
    if(!found)
    {
        results[results_count][0] = 1;
        results[results_count][1] = sixes;
        results[results_count][2] = fives;
        results[results_count][3] = fours;
        results_count++;
    }
}

void print_results()
{
    int i;
   
    printf("      %d winning patterns\n", results_count);
   
    for(i = 0; i < results_count; i++)
    {
        if(results[i][0] > 0)
        {
            printf("%6d %3d %3d %3d\n",
                   results[i][0], results[i][1], results[i][2], results[i][3]);
        }
    }
}
