/*
Efarmmogh fysikhs: kinhsh mazas se barytiko pedio
Genikeymeno programma me 3 statherous planhtes (terasties mazes)
kai N eisagomenous doruforous (max 10)
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OBJECTS 13  // 3 planhtes + 10 doruforoi
#define NUM_PLANETS 3   // Statheroi planhtes

typedef struct{ 
    double x;
    double y;
} V2D;

typedef struct{
    double m;
    V2D av;
    V2D uv;
    V2D rv;
    V2D nextrv;
    int movable;
    char name[20];
} TObj;

const double G = 1.0;

/*------------------------------------*/
V2D vadd (V2D a, V2D b)
{
    V2D c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

/*------------------------------------*/
V2D vkmult (double k , V2D a)
{
    V2D c;
    c.x = k * a.x;
    c.y = k * a.y;
    return c;
}

/*------------------------------------*/
double vmetro(V2D a)
{
    double d;
    d = sqrt(a.x*a.x + a.y*a.y);
    return d;
}

/*------------------------------------*/
void printV2D (V2D a)
{
    printf("%.6f %.6f\n", a.x, a.y);
}

/*------------------------------------*/
V2D vpp(V2D r1, V2D r2)
{ 
    V2D c;
    c.x = r2.x - r1.x;
    c.y = r2.y - r1.y;
    return c;
}

/*------------------------------------*/
V2D vnorm( V2D a)
{ 
    V2D c;
    double metro = vmetro(a);
    if (metro > 1e-12) 
    {
        c = vkmult(1.0/metro, a);
    } 
    else 
    {
        c.x = 0;
        c.y = 0;
    }
    return c;
}

/*------------------------------------*/
V2D gravforce(double m1, V2D r1v, double m2, V2D r2v)
{
    V2D r3v, fv, r3nv;
    double r3, f;
    
    r3v = vpp(r1v, r2v);
    r3 = vmetro(r3v);
    
    if (r3 < 1e-6) r3 = 1e-6; // Avoid division by zero
    
    f = (G * m1 * m2) / (r3 * r3);
    r3nv = vnorm(r3v);
    fv = vkmult(f, r3nv);
    
    return fv;
}

/*------------------------------------*/
V2D vset(double x, double y)
{
    V2D a;
    a.x = x;
    a.y = y;
    return a;
}

/*------------------------------------*/
double dobjs(TObj obj1, TObj obj2)
{
    return vmetro(vpp(obj1.rv, obj2.rv));
}

/*------------------------------------*/
void objmove(TObj *pobj, V2D fv, double dt)
{
    if ((*pobj).movable) 
    {
        (*pobj).av = vkmult(1.0/(*pobj).m, fv);
        V2D du = vkmult(dt, (*pobj).av);
        (*pobj).uv = vadd((*pobj).uv, du);
        V2D dr = vkmult(dt, (*pobj).uv);
        (*pobj).nextrv = vadd((*pobj).rv, dr);
    }
}

/*------------------------------------*/
void initializePlanets(TObj planets[])
{
    planets[0].m = 5000;
    planets[0].rv = vset(5, 5);
    planets[0].uv = vset(0, 0);
    planets[0].movable = 0;
    strcpy(planets[0].name, "Central");
    
    planets[1].m = 5000;
    planets[1].rv = vset(-5, 5);
    planets[1].uv = vset(0, 0);
    planets[1].movable = 0;
    strcpy(planets[1].name, "Shepherd1");
    
    planets[2].m = 5000;
    planets[2].rv = vset(-300, -300);
    planets[2].uv = vset(0, 0);
    planets[2].movable = 0;
    strcpy(planets[2].name, "Shepherd2");
}

/*------------------------------------*/
void inputSatellites(TObj satellites[], int *n)
{
    printf("===============================================\n");
    printf("PROGRAMMA PROSOMMOIWSHS DORUFORWN\n");
    printf("Yparxoun 3 statheroi planhtes me terasties mazes:\n");
    printf("1. Planet_A (m=10000) sthn thesh (0,0)\n");
    printf("2. Planet_B (m=5000) sthn thesh (40,40)\n");
    printf("3. Planet_C (m=8000) sthn thesh (-30,30)\n");
    printf("===============================================\n\n");
    
    do {
        printf("Doste arithmo doruforwn (3-10): ");
        scanf("%d", n);
        if (*n < 1 || *n > 10) 
        {
            printf("Lathos! Prepei 3-10 doruforoi.\n");
        }
    } while (*n < 3 || *n > 10);
    
    printf("\nEisagwgh stoixeiwn doruforwn:\n");
    for (int i = 0; i < *n; i++) 
    {
        printf("\n--- Doruforos %d ---\n", i+1);
        printf("Onoma (max 19 xaraktires): ");
        scanf("%s", satellites[i].name);
        
        printf("Maza (mikrh se sxesh me tous planhtes): ");
        scanf("%lf", &satellites[i].m);
        
        printf("Thesh arxikhs (x y): ");
        scanf("%lf %lf", &satellites[i].rv.x, &satellites[i].rv.y);
        
        printf("Taxythta arxikh (ux uy): ");
        scanf("%lf %lf", &satellites[i].uv.x, &satellites[i].uv.y);
        
        satellites[i].movable = 1;
        satellites[i].nextrv = satellites[i].rv;
        satellites[i].av = vset(0, 0);
        
        // Ypologismos taxytitas gia kyklikh troxia gyrw apo Planet_A
        printf("\nSymeiwsh: Gia kyklikh troxia gyrw apo Planet_A (0,0):\n");
        double r = vmetro(satellites[i].rv); // Apostash apo Planet_A
        double u_circular = sqrt(G * 10000 / r); // G*M/r gia Planet_A
        printf("  H taxythta gia kyklikh troxia einai: %.3f\n", u_circular);
        printf("  H taxythta sas einai: %.3f\n", vmetro(satellites[i].uv));
    }
}

void internal_Satellites(TObj planets[])
{

    planets[3].m = 10;
    planets[3].rv = vset(0.1, 0);
    planets[3].uv = vset(10, 10);
    planets[3].movable = 1;
    strcpy(planets[3].name, "A");
    
    planets[4].m = 10;
    planets[4].rv = vset(-0.1, 0);
    planets[4].uv = vset(-10, 10);
    planets[4].movable = 1;
    strcpy(planets[4].name, "B");
    
    planets[5].m = 10;
    planets[5].rv = vset(0, -5);
    planets[5].uv = vset(20, 5);
    planets[5].movable = 1;
    strcpy(planets[5].name, "C");
}

/*------------------------------------*/
void readSatellitesFromFile(TObj satellites[], int *n)
{
    FILE *fp = fopen("satellite_data.txt", "r");
    if (fp == NULL) 
    {
        return;
    }
    
    printf("Reading satellites from satellite_data.txt...\n");
    
    *n = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), fp) != NULL && *n < 10) 
    {
        // Skip empty lines and comments
        if (line[0] == '\n' || line[0] == '#' || line[0] == '/') 
        {
            continue;
        }
        
        char name[20];
        double m, x, y, vx, vy;
        
        // Try to read the data
        if (sscanf(line, "%s %lf %lf %lf %lf %lf", name, &m, &x, &y, &vx, &vy) == 6) 
        {
            
            strcpy(satellites[*n].name, name);
            satellites[*n].m = m;
            satellites[*n].rv = vset(x, y);
            satellites[*n].uv = vset(vx, vy);
            satellites[*n].movable = 1;
            satellites[*n].nextrv = satellites[*n].rv;
            satellites[*n].av = vset(0, 0);
            
            printf("  Loaded: %s (m=%.2f, pos=(%.1f,%.1f), vel=(%.2f,%.2f))\n", name, m, x, y, vx, vy);
            
            (*n)++;
        }
    }
    
    fclose(fp);
    
    if (*n == 0) {
        printf("WARNING: No satellites found in file. Using default system.\n");
        readSatellitesFromFile(satellites, n); // Recursive fallback
    } else {
        printf("Successfully loaded %d satellites.\n", *n);
    }
}

/*------------------------------------*/
void saveTrajectories(FILE *fp, TObj objects[], int total_objects, double t)
{
    fprintf(fp, "%.6f", t);
    for (int i = 0; i < total_objects; i++) 
    {
        fprintf(fp, ";%.6f;%.6f", objects[i].rv.x, objects[i].rv.y);
    }
    fprintf(fp, "\n");
}

/*------------------------------------*/
void printSystemInfo(TObj objects[], int total_objects, int num_satellites)
{
    printf("\n===============================================\n");
    printf("SYSTHMA PROSOMMOIWSHS\n");
    printf("===============================================\n");
    printf("PLANHTES (STATHEROI):\n");

    for (int i = 0; i < NUM_PLANETS; i++) 
    {
        printf("  %s: m=%.0f, thesh=(%.1f, %.1f)\n", objects[i].name, objects[i].m, objects[i].rv.x, objects[i].rv.y);
    }
    
    printf("\nDORUFOROI (KINOUMENOI):\n");
    for (int i = NUM_PLANETS; i < total_objects; i++) 
    {
        printf("  %s: m=%.2f, thesh=(%.2f, %.2f), taxythta=(%.2f, %.2f)\n", objects[i].name, objects[i].m, objects[i].rv.x, objects[i].rv.y, objects[i].uv.x, objects[i].uv.y);
    }
    printf("===============================================\n\n");
}

/*------------------------------------*/
int main(void)
{
    FILE *fp;
    double t, dt;
    int num_satellites, steps, i, j, k;
    int total_objects;
    TObj objects[MAX_OBJECTS];
    V2D totalForces[MAX_OBJECTS];
    
    // Arxikopoiisi twn 3 planhtwn
    initializePlanets(objects);
    
    // Eidagwgh doruforwn apo xrhsth
    //inputSatellites(&objects[NUM_PLANETS], &num_satellites);
    
    //readSatellitesFromFile(&objects[NUM_PLANETS], &num_satellites);

    internal_Satellites(objects);
    num_satellites = 3;
    
    total_objects = NUM_PLANETS + num_satellites;
    
    // printf("\nDoste xrono bhmatos dt (mikro, p.x. 0.001): ");
    // scanf("%lf", &dt);
    
    // printf("Doste arithmo bhmatwn prosomoiwshs (p.x. 10000-50000): ");
    // scanf("%d", &steps);
    
    dt = 0.001;
    steps = 10000;

    // Emfanish plhroforiwn systhmatos
    printSystemInfo(objects, total_objects, num_satellites);
    
    // Anoigma arxeiou gia apothikeysh
    fp = fopen("orbits_data.csv", "w");
    if (fp == NULL) 
    {
        printf("Adynato anoigma arxeiou!\n");
        return 1;
    }
    
    // Grammh epikefalidas gia to Excel
    fprintf(fp, "Time");
    for (i = 0; i < total_objects; i++) 
    {
        fprintf(fp, ";%s_x;%s_y", objects[i].name, objects[i].name);
    }
    fprintf(fp, "\n");
    
    // Arxikh apothikeysh
    //saveTrajectories(fp, objects, total_objects, 0);
    
    printf("\n===============================================\n");
    printf("ENARXH PROSOMMOIWSHS\n");
    printf("===============================================\n\n");
    
    // Kyrios kyklos prosomoiwshs
    for (i = 1; i <= steps; i++) 
    {
        t = i * dt;
        
        // Apothikeysh theshs (kathe 10 bhmata)
        // if (i % 10 == 0) 
        // {
            saveTrajectories(fp, objects, total_objects, t);
        //}
        
        // Emfanish kathe 500 bhmata
        // if (i % 500 == 0) {
        //     printf("t = %.2f: ", t);
        //     for (j = NUM_PLANETS; j < total_objects; j++) {
        //         double dist_to_A = dobjs(objects[j], objects[0]); // Apostash apo Planet_A
        //         printf("%s(dist=%.1f) ", objects[j].name, dist_to_A);
        //     }
        //     printf("\n");
        // }
        
        // Ypologismos dynamewn gia kathe doruforo
        for (j = 0; j < total_objects; j++) 
        {
            totalForces[j] = vset(0, 0);
        }
        
        // Gia kathe doruforo
        for (j = NUM_PLANETS; j < total_objects; j++) 
        {
            // Dynameis apo olous tous planhtes
            for (k = 0; k < NUM_PLANETS; k++) 
            {
                V2D f = gravforce(objects[j].m, objects[j].rv, objects[k].m, objects[k].rv);
                totalForces[j] = vadd(totalForces[j], f);
            }
            
            //Dinameis apo tous allous doruforous (SATELLITE-TO-SATELLITE FORCES ADDED)
            for (k = NUM_PLANETS; k < total_objects; k++) 
            {
                if (j != k) 
                {
                    V2D f = gravforce(objects[j].m, objects[j].rv, objects[k].m, objects[k].rv);
                    totalForces[j] = vadd(totalForces[j], f);
                }
            }
        }
        
        // Kinisi MONO twn doruforwn
        for (j = NUM_PLANETS; j < total_objects; j++) 
        {
            objmove(&objects[j], totalForces[j], dt);
        }
        
        // Enimerosi thesewn gia epomeno vhma (MONO gia doruforous)
        for (j = NUM_PLANETS; j < total_objects; j++) 
        {
            if (objects[j].movable) 
            {
                objects[j].rv = objects[j].nextrv;
            }
        }
    }
    
    fclose(fp);
    
    printf("\n===============================================\n");
    printf("PROSOMMOIWSH OLOKLHRWTHIKH\n");
    printf("Dedomena apothikeytikan sto arxeio: orbits_data.txt\n");
    printf("===============================================\n");
    
    return 0;
}
