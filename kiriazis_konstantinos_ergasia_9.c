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
    int is_planet;  // 1 = planhtas, 0 = doruforos
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
    if (metro > 1e-12) {
        c = vkmult(1.0/metro, a);
    } else {
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
    if ((*pobj).movable && !(*pobj).is_planet) {
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
    // Planhtas 1: Kentriko me megalh maza
    planets[0].m = 100;
    planets[0].rv = vset(0, 0);
    planets[0].uv = vset(0, 0);
    planets[0].movable = 0;
    planets[0].is_planet = 1;
    strcpy(planets[0].name, "SUN");
    
    // Planhtas 2: Se tetragwno thesi
    planets[1].m = 0.00000000001;
    planets[1].rv = vset(0, 0);
    planets[1].uv = vset(0, 0);
    planets[1].movable = 0;
    planets[1].is_planet = 1;
    strcpy(planets[1].name, "Planet_B");
    
    // Planhtas 3: Se allh thesi
    planets[2].m = 0.00000000001;
    planets[2].rv = vset(0, 0);
    planets[2].uv = vset(0, 0);
    planets[2].movable = 0;
    planets[2].is_planet = 1;
    strcpy(planets[2].name, "Planet_C");
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
        if (*n < 1 || *n > 10) {
            printf("Lathos! Prepei 3-10 doruforoi.\n");
        }
    } while (*n < 3 || *n > 10);
    
    printf("\nEisagwgh stoixeiwn doruforwn:\n");
    for (int i = 0; i < *n; i++) {
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
        satellites[i].is_planet = 0;
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
    // OPTIONAL: Test moon-moon system (comment out when using user input)
    // EARTH-like body
    planets[3].m = 1;
    planets[3].rv = vset(10, 0);
    planets[3].uv = vset(0, 3.16227766);  // sqrt(G*10000/100) = 10
    planets[3].movable = 1;
    planets[3].is_planet = 0;
    strcpy(planets[3].name, "EARTH");
    
    // MOON orbiting Earth
    planets[4].m = 1;
    planets[4].rv = vset(12, 0);  // 10 units from Earth
    planets[4].uv = vset(0, 3.86938444);  // Earth's velocity (10) + sqrt(G*100/10)=0.316
    planets[4].movable = 1;
    planets[4].is_planet = 0;
    strcpy(planets[4].name, "MOON");
    
    // MOON's MOON
    planets[5].m = 0.01;
    planets[5].rv = vset(12.5, 0);  // 2 units from Moon
    planets[5].uv = vset(0, 4.31659804);  // Moon's velocity + sqrt(G*1/2)=0.075
    planets[5].movable = 1;
    planets[5].is_planet = 0;
    strcpy(planets[5].name, "MOON-MOON");
}

/*------------------------------------*/
void saveTrajectories(FILE *fp, TObj objects[], int total_objects, double t)
{
    //fprintf(fp, "%.6f", t);
    for (int i = 0; i < total_objects; i++) {
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
    for (int i = 0; i < NUM_PLANETS; i++) {
        printf("  %s: m=%.0f, thesh=(%.1f, %.1f)\n", 
               objects[i].name, objects[i].m, 
               objects[i].rv.x, objects[i].rv.y);
    }
    
    printf("\nDORUFOROI (KINOUMENOI):\n");
    for (int i = NUM_PLANETS; i < total_objects; i++) {
        printf("  %s: m=%.2f, thesh=(%.2f, %.2f), taxythta=(%.2f, %.2f)\n",
               objects[i].name, objects[i].m,
               objects[i].rv.x, objects[i].rv.y,
               objects[i].uv.x, objects[i].uv.y);
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
    
    internal_Satellites(objects);
    num_satellites = 3;

    
    total_objects = NUM_PLANETS + num_satellites;
    
    printf("\nDoste xrono bhmatos dt (mikro, p.x. 0.001): ");
    scanf("%lf", &dt);
    
    printf("Doste arithmo bhmatwn prosomoiwshs (p.x. 10000-50000): ");
    scanf("%d", &steps);
    
    // Emfanish plhroforiwn systhmatos
    printSystemInfo(objects, total_objects, num_satellites);
    
    // Anoigma arxeiou gia apothikeysh
    fp = fopen("orbits_data.txt", "w");
    if (fp == NULL) {
        printf("Adynato anoigma arxeiou!\n");
        return 1;
    }
    
    // Grammh epikefalidas gia to Excel
    fprintf(fp, "Time");
    for (i = 0; i < total_objects; i++) {
        fprintf(fp, ";%s_x;%s_y", objects[i].name, objects[i].name);
    }
    fprintf(fp, "\n");
    
    // Arxikh apothikeysh
    saveTrajectories(fp, objects, total_objects, 0);
    
    printf("\n===============================================\n");
    printf("ENARXH PROSOMMOIWSHS\n");
    printf("===============================================\n\n");
    
    // Kyrios kyklos prosomoiwshs
    for (i = 1; i <= steps; i++) {
        t = i * dt;
        
        // Apothikeysh theshs (kathe 10 bhmata)
        if (i % 10 == 0) {
            saveTrajectories(fp, objects, total_objects, t);
        }
        
        // Emfanish kathe 500 bhmata
        if (i % 500 == 0) {
            printf("t = %.2f: ", t);
            for (j = NUM_PLANETS; j < total_objects; j++) {
                double dist_to_A = dobjs(objects[j], objects[0]); // Apostash apo Planet_A
                printf("%s(dist=%.1f) ", objects[j].name, dist_to_A);
            }
            printf("\n");
        }
        
        // Ypologismos dynamewn gia kathe doruforo
        for (j = 0; j < total_objects; j++) {
            totalForces[j] = vset(0, 0);
        }
        
        // Gia kathe doruforo
        for (j = NUM_PLANETS; j < total_objects; j++) {
            // Dinameis apo olous tous planhtes
            for (k = 0; k < NUM_PLANETS; k++) {
                V2D f = gravforce(objects[j].m, objects[j].rv, 
                                objects[k].m, objects[k].rv);
                totalForces[j] = vadd(totalForces[j], f);
            }
            
            // Dinameis apo tous allous doruforous (SATELLITE-TO-SATELLITE FORCES ADDED)
            for (k = NUM_PLANETS; k < total_objects; k++) {
                if (j != k) {
                    V2D f = gravforce(objects[j].m, objects[j].rv, 
                                    objects[k].m, objects[k].rv);
                    totalForces[j] = vadd(totalForces[j], f);
                }
            }
        }
        
        // Kinisi MONO twn doruforwn
        for (j = NUM_PLANETS; j < total_objects; j++) {
            objmove(&objects[j], totalForces[j], dt);
        }
        
        // Enimerosi thesewn gia epomeno vhma (MONO gia doruforous)
        for (j = NUM_PLANETS; j < total_objects; j++) {
            if (objects[j].movable) {
                objects[j].rv = objects[j].nextrv;
            }
        }
        
        // Elegxos gia "xtysh" se planhti
        for (j = NUM_PLANETS; j < total_objects; j++) {
            for (k = 0; k < NUM_PLANETS; k++) {
                double distance = dobjs(objects[j], objects[k]);
                double planet_radius = 3.0; // Efektikh aktina planhti
                
                if (distance < planet_radius) {
                    printf("\n! SYMBASH: %s xtypise ton %s (dist=%.3f)\n", 
                           objects[j].name, objects[k].name, distance);
                    // Mhdenismos taxytitas
                    objects[j].uv = vset(0, 0);
                    objects[j].movable = 0;
                }
            }
        }
    }
    
    fclose(fp);
    
    printf("\n===============================================\n");
    printf("PROSOMMOIWSH OLOKLHRWTHIKH\n");
    printf("Dedomena apothikeytikan sto arxeio: orbits_data.txt\n");
    printf("===============================================\n");
    
    // Odhgies gia to Excel
    printf("\nODHGIES GIA TO EXCEL:\n");
    printf("1. Anoigete to arxeio orbits_data.txt sto Excel\n");
    printf("2. Epilexte Text import, delimiter = ';'\n");
    printf("3. Xrhsimopoihste scatter plot gia na deite tis troxies\n");
    printf("4. PLANHTES: xrhsimopoihste megaloys kyklous\n");
    printf("5. DORUFOROI: xrhsimopoihste mikres teies\n");
    printf("6. Gia kaliterh oratopoihsh epilexte:\n");
    printf("   - Kokkino gia Planet_A\n");
    printf("   - Mple gia Planet_B\n");
    printf("   - Prasino gia Planet_C\n");
    printf("   - Diaforetika xrwmata gia kathe doruforo\n");
    
    // Epipleon arxeio me plhrofories
    FILE *info_fp = fopen("system_info.txt", "w");
    fprintf(info_fp, "SYSTHMA PLANHTWN KAI DORUFORWN\n");
    fprintf(info_fp, "===============================\n\n");
    fprintf(info_fp, "PLANHTES (STATHEROI):\n");
    for (i = 0; i < NUM_PLANETS; i++) {
        fprintf(info_fp, "%s: m=%.0f, pos=(%.1f,%.1f)\n",
                objects[i].name, objects[i].m, objects[i].rv.x, objects[i].rv.y);
    }
    fprintf(info_fp, "\nDORUFOROI:\n");
    for (i = NUM_PLANETS; i < total_objects; i++) {
        fprintf(info_fp, "%s: m=%.2f, pos=(%.2f,%.2f), vel=(%.2f,%.2f)\n",
                objects[i].name, objects[i].m,
                objects[i].rv.x, objects[i].rv.y,
                objects[i].uv.x, objects[i].uv.y);
    }
    fclose(info_fp);
    
    printf("\nPlhrofories systhmatos ston fakelo: system_info.txt\n");
    
    return 0;
}
