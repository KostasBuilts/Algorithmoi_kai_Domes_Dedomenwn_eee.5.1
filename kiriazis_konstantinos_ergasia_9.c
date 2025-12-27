/*
Efarmmogh fysikhs: kinhsh mazas se barytiko pedio
Genikeymeno programma me 3 statherous planhtes
kai N eisagomenous doruforous
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OBJECTS     13  // 3 planhtes + 10 doruforoi
#define NUM_PLANETS     3   // Statheroi planhtes
#define SAMPLES         1000 // Posa tha einai ta telika samples sto arxeio
#define DISPLAY_RATE    50
#define C_RESET         0
#define C_RED           1
#define C_GREEN         2
#define C_YELLOW        3
#define C_BLUE          4
#define C_MAGENTA       5
#define C_CYAN          6
#define C_WHITE         7

#ifdef _WIN32
    #include <windows.h>

    void print_color(const char *msg, int color)
    {
        static HANDLE hConsole = NULL;
        static WORD defaultAttr;

        if (!hConsole) {
            CONSOLE_SCREEN_BUFFER_INFO info;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            GetConsoleScreenBufferInfo(hConsole, &info);
            defaultAttr = info.wAttributes;
        }

        WORD attr = FOREGROUND_INTENSITY;

        switch (color) {
            case C_RED:     attr |= FOREGROUND_RED; break;
            case C_GREEN:   attr |= FOREGROUND_GREEN; break;
            case C_YELLOW:  attr |= FOREGROUND_RED | FOREGROUND_GREEN; break;
            case C_BLUE:    attr |= FOREGROUND_BLUE; break;
            case C_MAGENTA: attr |= FOREGROUND_RED | FOREGROUND_BLUE; break;
            case C_CYAN:    attr |= FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            case C_WHITE:   attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            case C_RESET:
            default:
                SetConsoleTextAttribute(hConsole, defaultAttr);
                printf("%s", msg);
                return;
        }

        SetConsoleTextAttribute(hConsole, attr);
        printf("%s", msg);
        SetConsoleTextAttribute(hConsole, defaultAttr);
    }

#else   /* Linux / Unix */

    void print_color(const char *msg, int color)
    {
        const char *code;

        switch (color) {
            case C_RED:     code = "\033[1;31m"; break;
            case C_GREEN:   code = "\033[1;32m"; break;
            case C_YELLOW:  code = "\033[1;33m"; break;
            case C_BLUE:    code = "\033[1;34m"; break;
            case C_MAGENTA: code = "\033[1;35m"; break;
            case C_CYAN:    code = "\033[1;36m"; break;
            case C_WHITE:   code = "\033[1;37m"; break;
            case C_RESET:
            default:
                code = "\033[0m";
                break;
        }

        printf("\x1b[1m%s%s\033[0m", code, msg);
    }

#endif

typedef struct{ 
    double x;  /**< X-coordinate of the vector */
    double y;  /**< Y-coordinate of the vector */
} V2D;

typedef struct{
    double m;      /// Mass of the object
    V2D av;        /// Acceleration vector (a_x, a_y)
    V2D uv;        /// Velocity vector (u_x, u_y)
    V2D rv;        /// Position vector (x, y)
    V2D nextrv;    /// Next position vector (used for integration)
    int movable;   /// Flag indicating if object can move (1=movable, 0=fixed)
    char name[20]; /// Name identifier for the object
} TObj;

const double G = 1.0;
double dt, total_time;

/**
 * @brief Sum of two vectors.
 * @param V2D a - First vector
 * @param V2D b - Second vector
 * @return V2D a+b
 * @details Adds two vectors component-wise and returns their sum as a new vector.
 * @code
 * V2D result = vadd(vec1, vec2);
 * // result.x = vec1.x + vec2.x
 * // result.y = vec1.y + vec2.y
 * @endcode
 */
V2D vadd (V2D a, V2D b)
{
    V2D c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

/**
 * @brief Scalar multiplication of a vector.
 * @param double k - Scalar multiplier
 * @param V2D a - Vector to be multiplied
 * @return V2D k*a
 * @details Multiplies each component of vector a by scalar k. Returns a new vector scaled by factor k.
 * @code
 * V2D result = vkmult(2.5, vec);
 * // result.x = 2.5 * vec.x
 * // result.y = 2.5 * vec.y
 * @endcode
 */
V2D vkmult (double k , V2D a)
{
    V2D c;
    c.x = k * a.x;
    c.y = k * a.y;
    return c;
}

/**
 * @brief Magnitude (Euclidean norm) of a vector.
 * @param V2D a - Input vector
 * @return double |a|
 * @details Calculates the Euclidean length of vector a. Uses the formula: sqrt(x² + y²)
 * @code
 * double length = vmetro(vec);
 * // length = sqrt(vec.x*vec.x + vec.y*vec.y)
 * @endcode
 */
double vmetro(V2D a)
{
    double d;
    d = sqrt(a.x*a.x + a.y*a.y);
    return d;
}

/**
 * @brief Print vector components.
 * @param V2D a - Vector to be printed
 * @return void
 * @details Displays the vector components to standard output with 6 decimal precision. Format: "x.xxxxxx y.yyyyyy"
 * @code
 * printV2D(vec);
 * // Output: "1.234567 -2.345678"
 * @endcode
 */
void printV2D (V2D a)
{
    printf("%.6f %.6f\n", a.x, a.y);
}

/**
 * @brief Vector from point to point.
 * @param V2D r1 - Starting point (position vector of first object)
 * @param V2D r2 - Ending point (position vector of second object)
 * @return V2D r2 - r1
 * @details Calculates the displacement vector from point r1 to point r2. Used for determining relative positions between objects.
 * @code
 * V2D displacement = vpp(object1.rv, object2.rv);
 * // displacement points FROM object1 TO object2
 * @endcode
 */
V2D vpp(V2D r1, V2D r2)
{ 
    V2D c;
    c.x = r2.x - r1.x;
    c.y = r2.y - r1.y;
    return c;
}

/**
 * @brief Normalize a vector (convert to unit vector).
 * @param V2D a - Input vector
 * @return V2D â (unit vector in direction of a)
 * @details Returns a unit vector with the same direction as input vector a. If the magnitude is near zero (< 1e-12), returns zero vector. Uses the formula: a/|a|
 * @code
 * V2D unit_vec = vnorm(vec);
 * // |unit_vec| ˜ 1 (unless vec was zero)
 * @endcode
 */
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

/**
 * @brief Calculate gravitational force between two objects.
 * @param double m1 - Mass of first object
 * @param V2D r1v - Position vector of first object
 * @param double m2 - Mass of second object
 * @param V2D r2v - Position vector of second object
 * @return V2D Gravitational force vector on object 1 due to object 2
 * @details Calculates the gravitational force using Newton's law of universal gravitation:
 * F = G * m1 * m2 / r² * (r^)
 * where r^ is the unit vector from object 1 to object 2.
 * Avoids division by zero by setting minimum distance of 1e-6.
 * Force direction: from object 1 toward object 2 (attractive force).
 * @code
 * V2D force = gravforce(mass1, pos1, mass2, pos2);
 * // force on object 1 due to object 2
 * @endcode
 */
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

/**
 * @brief Create a vector with specified components.
 * @param double x - X-component
 * @param double y - Y-component
 * @return V2D Vector with components (x, y)
 * @details Convenience function to initialize vectors with given coordinates.
 * @code
 * V2D vec = vset(3.14, -2.71);
 * // vec.x = 3.14, vec.y = -2.71
 * @endcode
 */
V2D vset(double x, double y)
{
    V2D a;
    a.x = x;
    a.y = y;
    return a;
}

/**
 * @brief Calculate distance between two objects.
 * @param TObj obj1 - First object
 * @param TObj obj2 - Second object
 * @return double Euclidean distance between objects
 * @details Computes the straight-line distance between the positions
 * of two objects using their position vectors.
 * @code
 * double dist = dobjs(satellite, planet);
 * // dist = |satellite.rv - planet.rv|
 * @endcode
 */
double dobjs(TObj obj1, TObj obj2)
{
    return vmetro(vpp(obj1.rv, obj2.rv));
}

/**
 * @brief Move an object based on applied force.
 * @param TObj *pobj - Pointer to object to move (modified in place)
 * @param V2D fv - Total force vector acting on object
 * @param double dt - Time step for integration
 * @return void
 * @details Updates object's position and velocity using Newton's second law.
 * Algorithm (Euler integration):
 * 1. a = F/m (acceleration from force)
 * 2. ?u = a * dt (velocity change)
 * 3. u = u + ?u (update velocity)
 * 4. ?r = u * dt (position change)
 * 5. nextrv = r + ?r (store next position)
 * 
 * Only movable objects are updated (planets remain fixed).
 * @code
 * objmove(&satellite, totalForce, 0.001);
 * // Updates satellite's velocity and calculates next position
 * @endcode
 */
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

/**
 * @brief Initialize the three fixed planets.
 * @param TObj planets[] - Array to store planet objects
 * @return void
 * @details Sets up three planets with fixed positions and masses.
 * Planets are:
 * 1. Planet_A: Mass 5000 at (5, 5)
 * 2. Planet_B: Mass 5000 at (-5, 5)
 * 3. Planet_C: Mass 500 at (0, -5)
 * All planets have zero velocity and are immovable.
 * @code
 * initializePlanets(planet_array);
 * // Creates three fixed planets at specified positions
 * @endcode
 */
void initializePlanets(TObj planets[])
{
    planets[0].m = 5000;
    planets[0].rv = vset(5, 5);
    planets[0].uv = vset(0, 0);
    planets[0].movable = 0;
    strcpy(planets[0].name, "Planet_A");
    
    planets[1].m = 5000;
    planets[1].rv = vset(-5, 5);
    planets[1].uv = vset(0, 0);
    planets[1].movable = 0;
    strcpy(planets[1].name, "Planet_B");
    
    planets[2].m = 500;
    planets[2].rv = vset(0, -5);
    planets[2].uv = vset(0, 0);
    planets[2].movable = 0;
    strcpy(planets[2].name, "Planet_C");
}

/**
 * @brief Input satellite data from user.
 * @param TObj satellites[] - Array to store satellite objects
 * @param int *n - Pointer to variable storing number of satellites
 * @return void
 * @details Interactive function that prompts user for satellite parameters:
 * - Name (up to 19 characters)
 * - Mass (should be small compared to planets)
 * - Initial position (x, y)
 * - Initial velocity (vx, vy)
 * Also promts the user for the integration distance dt and total simualtion time
 * Validates input for number of satellites (1-10).
 * @code
 * int num_sats;
 * inputSatellites(satellite_array, &num_sats);
 * // Prompts user for satellite data
 * @endcode
 */
void inputSatellites(TObj satellites[], int *n)
{
	int i;
    printf("===============================================\n");
    printf("    PROGRAMMA PROSOMMOIWSHS DORUFORWN\n");
    printf("     Yparxoun 3 statheroi planhtes\n");
    printf("1. Planet_A (m=5000) sthn thesh (5, 5)\n");
    printf("2. Planet_B (m=5000) sthn thesh (-5, 5)\n");
    printf("3. Planet_C (m=500)  sthn thesh (0,-5)\n");
    printf("===============================================\n\n");
    
    do {
        printf("Dose arithmo doruforwn (1-10): ");
        scanf("%d", n);
        if (*n < 1 || *n > 10) 
        {
            print_color("Sfalma! Prepei 1-10 doruforoi.\n", C_RED);
        }
    } while (*n < 1 || *n > 10);
    
    printf("\nEisagwgh stoixeiwn doruforwn:\n");
    for (i = 0; i < *n; i++) 
    {
        printf("\n--- Doruforos %d ---\n", i+1);
        printf("Onoma (max 19 xaraktires): ");
        scanf(" %[^\n]", satellites[i].name);
        
        printf("Maza: ");
        scanf("%lf", &satellites[i].m);
        
        printf("Arxikh thesh (x y): ");
        scanf("%lf %lf", &satellites[i].rv.x, &satellites[i].rv.y);
        
        printf("Arxikh taxythta  (ux uy): ");
        scanf("%lf %lf", &satellites[i].uv.x, &satellites[i].uv.y);
        
        satellites[i].movable = 1;
        satellites[i].nextrv = satellites[i].rv;
        satellites[i].av = vset(0, 0);
    }

    print_color("WARNING ! O logos dt/time na einai isws h megalhteros tou 1000\n", C_YELLOW);

    do{
        printf("Doste xrono bhmatos dt: ");
        scanf("%lf", &dt);
    
        printf("Doste xrono prosomoiwshs: ");
        scanf("%lf", &total_time);
        if(total_time/dt < 1000.0)
        {
            print_color("Sfalma! O logos dt/time den einai megalitero tou 1000\n", C_RED);
        }
    } while (total_time/dt < 1000.0);

}

/**
 * @brief Create default internal satellites for testing.
 * @param TObj planets[] - Array to store satellite objects (appended after planets)
 * @return void
 * @details Creates three test satellites with predefined parameters:
 * 1. A: Mass 10 at (0.1, 0) with velocity (10, 10)
 * 2. B: Mass 10 at (-0.1, 0) with velocity (-10, 10)
 * 3. C: Mass 10 at (0, -5) with velocity (20, 5)
 * Used only for testing.
 * @code
 * internal_Satellites(objects);
 * // Adds three test satellites starting at index 3
 * @endcode
 */
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

/**
 * @brief Read satellite data and simulation parameters from file.
 * @param TObj satellites[] - Array to store satellite objects
 * @param int *n - Pointer to variable storing number of satellites
 * @param char **argv - Command line arguments (argv[1] should be filename)
 * @return int 0 on success, 1 on file error
 * @details Reads satellite data from specified file. File format:
 * - Each line: name mass x y vx vy
 * - Last line: dt = value time = value
 * Supports comments (lines starting with # or /) and empty lines.
 * Example file content:
 * @code
 * Satellite1 10.0 0.1 0.0 10.0 10.0
 * Satellite2 10.0 -0.1 0.0 -10.0 10.0
 * dt = 0.000001 time = 11.37
 * @endcode
 */
int readSatellitesFromFile(TObj satellites[], int *n, char **argv)
{
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) 
    {
        return 1;
    }
    
    printf("Reading satellites from %s ...\n", argv[1]);
    
    *n = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), fp) != NULL && *n < 10) 
    {
        // Skip empty lines and comments
        if (line[0] == '\n' || line[0] == '#' || line[0] == '/') 
        {
            continue;
        } 

        if (strstr(line, "dt =") != NULL || strstr(line, "time =") != NULL) 
        {
            // Try to extract dt and time values
            if (sscanf(line, "dt = %lf time = %lf", &dt, &total_time) == 2) 
            {
                //printf("  Found simulation parameters: dt = %g, total_time = %g\n", &dt, &total_time);
            } 
            else if (sscanf(line, "dt=%lf time=%lf", &dt, &total_time) == 2) 
            {
                //printf("  Found simulation parameters: dt = %g, total_time = %g\n", &dt, &total_time);
            }
            if(total_time/dt < 1000.0)
            {
                printf("\x1b[31m" "\x1b[1m" "Sfalma! O logos dt/time den einai megalitero tou 1000." "\x1b[0m" "\n");
                return 1;
            }
            else {continue;} // Skip this line as it's not satellite data
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
            
            //printf("  Loaded: %s (m=%.2f, pos=(%.1f,%.1f), vel=(%.2f,%.2f))\n", name, m, x, y, vx, vy);
            
            (*n)++;
        }
    }
    
    fclose(fp);
    
    if (*n == 0) 
    {
        printf("WARNING: No satellites found in file.\n");
        readSatellitesFromFile(satellites, n, argv); // Recursive fallback
    } 
    else 
    {
        printf("Successfully loaded %d satellites.\n", *n);
    }
    return 0;
}

/**
 * @brief Save current system state to file.
 * @param FILE *fp - File pointer for output (must be open for writing)
 * @param TObj objects[] - Array of all objects (planets + satellites)
 * @param int total_objects - Total number of objects in simulation
 * @param double t - Current simulation time
 * @return void
 * @details Writes a CSV-formatted line with current positions of all objects.
 * Format: time;obj1_x;obj1_y;obj2_x;obj2_y;...
 * Used for creating trajectory data for visualization.
 * @code
 * saveTrajectories(fp, objects, total_objects, 5.25);
 * // Saves positions at time 5.25
 * @endcode
 */
void saveTrajectories(FILE *fp, TObj objects[], int total_objects, double t)
{
	int i;
    fprintf(fp, "%.6f", t);
    for (i = 0; i < total_objects; i++) 
    {
        fprintf(fp, ";%.6f;%.6f", objects[i].rv.x, objects[i].rv.y);
    }
    fprintf(fp, "\n");
}

/**
 * @brief Print detailed information about the simulation system.
 * @param TObj objects[] - Array of all objects (planets + satellites)
 * @param int total_objects - Total number of objects in simulation
 * @param int num_satellites - Number of satellites (excluding planets)
 * @return void
 * @details Displays comprehensive information about all objects:
 * - Fixed planets (mass, position)
 * - Satellites (mass, position, velocity)
 * - Collision detection parameters
 * Useful for verification and debugging.
 * @code
 * printSystemInfo(objects, 6, 3);
 * // Prints info for 3 planets + 3 satellites
 * @endcode
 */
void printSystemInfo(TObj objects[], int total_objects, int num_satellites)
{
	int i;
    printf("\n===============================================\n");
    printf("SYSTHMA PROSOMMOIWSHS\n");
    printf("===============================================\n");
    printf("PLANHTES (STATHEROI):\n");

    for (i = 0; i < NUM_PLANETS; i++) 
    {
        printf("  %s: m=%.0f, thesh=(%.1f, %.1f)\n", objects[i].name, objects[i].m, objects[i].rv.x, objects[i].rv.y);
    }
    
    printf("\nDORUFOROI (KINOUMENOI):\n");
    for (i = NUM_PLANETS; i < total_objects; i++) 
    {
        printf("  %s: m=%.2f, thesh=(%.2f, %.2f), taxythta=(%.2f, %.2f)\n", objects[i].name, objects[i].m, objects[i].rv.x, objects[i].rv.y, objects[i].uv.x, objects[i].uv.y);
    }

    printf("\nXRONOS PROSOMIOSHS: %g \nBHMA dt: %g\n", total_time, dt);

    printf("===============================================\n\n");
}

/**
 * @brief Main simulation function.
 * @param int argc - Number of command line arguments
 * @param char *argv[] - Array of command line arguments
 * @return int 0 on successful execution, 1 on error
 * @details Main entry point for the gravitational simulation program.
 * Performs the following steps:
 * 1. Initialize system (planets and satellites)
 * 2. Set up simulation parameters (dt, total time)
 * 3. Open output file for trajectory data
 * 4. Run simulation loop
 * 5. Calculate gravitational forces and update positions
 * 6. Save results and close files
 * 
 * Command line usage: program_file.exe [input_file]
 * If no input file specified, promts the user to enter the parameters from the keyboard.
 * @see initializePlanets(), readSatellitesFromFile(), saveTrajectories(), gravforce(), objmove()
 */
int main(int argc, char **argv)
{
    FILE *fp;
    double t;
    int num_satellites, steps, i, j, k;
    int total_objects;
    TObj objects[MAX_OBJECTS];
    V2D totalForces[MAX_OBJECTS];
    
    // Arxikopoiisi twn 3 planhtwn
    initializePlanets(objects);
    
    // Anagnwsh doryforwn apo arxeio
    if(readSatellitesFromFile(&objects[NUM_PLANETS], &num_satellites, argv) == 1)
    {
        // Eidagwgh doruforwn apo xrhsth
        inputSatellites(&objects[NUM_PLANETS], &num_satellites);
    }

    // internal_Satellites(objects);
    // num_satellites = 5;
    // dt = 0.000001;
    // total_time = 11.37;
    
    total_objects = NUM_PLANETS + num_satellites;
    
    steps = total_time/dt;

    // Emfanish plhroforiwn systhmatos
    printSystemInfo(objects, total_objects, num_satellites);
    
    // Anoigma arxeiou gia apothikeysh
    fp = fopen("Kiriazis_orbit_data.csv", "w");
    if (fp == NULL) 
    {
        printf("Adynato to anoigma arxeiou!\n");
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
        if (i % (steps/SAMPLES) == 0) 
        {
            saveTrajectories(fp, objects, total_objects, t);
        }
        
        //Emfanish kathe 500 bhmata
        if (i % (steps/DISPLAY_RATE) == 0) {
            printf("t = %.2f: ", t);
            for (j = NUM_PLANETS; j < total_objects; j++) {
                double dist_to_A = dobjs(objects[j], objects[0]); // Apostash apo Planet_A
                printf("%s(dist=%.1f) ", objects[j].name, dist_to_A);
            }
            printf("\n");
        }
        
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