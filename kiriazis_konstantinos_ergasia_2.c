/*
eyresh rizas ejiswshs f(x)=0
*/
#include <stdio.h>
#include <math.h>

#define step	0.1

double f(double x);
double dichotomia(double a, double b);

int main(void)
{
    double x1, x2, a, b;
    double riza;
    
    scanf("%lf", &x1);
    scanf("%lf", &x2);

    a = x1;
    while (a + step <= x2)
    {
        b = a + step;
        if (f(a) * f(b) <= 0)
        {
            riza = dichotomia(a, b);
            printf("Riza x = %.12lf\n", x1, b, riza);
            a = riza + 0.1;
        }
        else
            a += step;
    }

    return 0;
}

/*
Algorithmos dixotomias
*/
double dichotomia(double a, double b)
{
    double m;
    while (fabs(b - a) > 1e-12)
    {
        m = (a + b) / 2.0;
        if (f(a) * f(m) <= 0)
            b = m;
        else
            a = m;
    }
    return (a + b) / 2.0;
}

/*
Synarthsh f(x)
*/
double f(double x)
{
    return cos(x)*x;
}
