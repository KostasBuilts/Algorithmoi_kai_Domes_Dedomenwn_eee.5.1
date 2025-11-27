/*
analysh aritmnou se ginomeno prwtwn arithmwn
*/

#include <stdio.h>

void calcPrimes(int p[50], int *np);
int checkprwtos(int a);

int main(void)
{
	int a, np, i, y;
	int p[50];

	calcPrimes(p, &np);
	
	printf("Eishgage arithmo metaksi 1 kai 100\n");
	scanf("%d", &a);
	
	i= 0;
	do
	{ 	do
	    {   y = a%p[i];
			if (y==0)		
			{   printf("%d ",p[i]);
				a= a/p[i];
			}
		} while (y==0);
		
		i= i + 1;		
	} while (a!=1);
	
}

void calcPrimes(int p[50], int *np)
{
    int isPrime[101];
    int i, j;

    for (i = 2; i <= 100; i++)
        isPrime[i] = 1;

    for (i = 2; i * i <= 100; i++) {
        if (isPrime[i]) {
            for (j = i * i; j <= 100; j += i)
                isPrime[j] = 0;
        }
    }

    *np = 0;
    for (i = 2; i <= 100; i++) {
        if (isPrime[i]) {
            p[*np] = i;
            (*np)++;
        }
    }
}
