#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// -----------------------------------------
// Original struct from your code
// -----------------------------------------
typedef struct {
    double x, y;
    bool is_covered;
    bool has_school;
} village;

// -----------------------------------------
double get_Distance(village a, village b) {
    return sqrt((a.x - b.x) * (a.x - b.x) +  (a.y - b.y) * (a.y - b.y));
}

// -----------------------------------------
// GA parameters
// -----------------------------------------
#define POP_SIZE 120
#define GENERATIONS 1500
#define MUTATION_RATE 0.01
#define ELITE_COUNT 3
#define MAX_VIL 1000

// Each chromosome is an array of bits (0= no school, 1 = school)
int population[POP_SIZE][MAX_VIL];
int new_population[POP_SIZE][MAX_VIL];
double fitness_values[POP_SIZE];

village villages[MAX_VIL];
int V;
double max_distance;

// ----------------------------------------------------------
// Check if village v is covered by ANY school in chromosome
// ----------------------------------------------------------
bool is_covered_by_chromosome(int chrom[], int v) {
    for (int i = 0; i < V; i++) {
        if (chrom[i] == 1 && get_Distance(villages[i], villages[v]) <= max_distance)
            return true;
    }
    return false;
}

// ----------------------------------------------------------
// Fitness: fewer schools is better, but uncovered villages
// get a huge penalty to force valid solutions.
// ----------------------------------------------------------
double compute_fitness(int chrom[]) {
    int school_count = 0;
    int uncovered = 0;

    for (int i = 0; i < V; i++) {
        if (chrom[i] == 1) school_count++;
        if (!is_covered_by_chromosome(chrom, i)) uncovered++;
    }

    double penalty = uncovered * 1000;  // huge penalty
    return (double)(V - school_count) - penalty;
}

// ----------------------------------------------------------
void random_chromosome(int chrom[]) {
    for (int i = 0; i < V; i++)
        chrom[i] = rand() % 2;
}

// ----------------------------------------------------------
void copy_chrom(int dest[], int src[]) {
    for (int i = 0; i < V; i++)
        dest[i] = src[i];
}

// ----------------------------------------------------------
void mutate(int chrom[]) {
    for (int i = 0; i < V; i++) {
        if (((double)rand() / RAND_MAX) < MUTATION_RATE)
            chrom[i] = 1 - chrom[i];
    }
}

// ----------------------------------------------------------
void crossover(int p1[], int p2[], int child[]) {
    int point = rand() % V;
    for (int i = 0; i < point; i++) child[i] = p1[i];
    for (int i = point; i < V; i++) child[i] = p2[i];
}

// ----------------------------------------------------------
int tournament_select() {
    int a = rand() % POP_SIZE;
    int b = rand() % POP_SIZE;
    return (fitness_values[a] > fitness_values[b]) ? a : b;
}

// ----------------------------------------------------------
int main() {
    srand(time(NULL));

    printf("Dose arithmo xorion? ");
    scanf("%d", &V);
    printf("Dose tin megisti apostash d: ");
    scanf("%lf", &max_distance);
    while (getchar() != '\n');

    for (int i = 0; i < V; i++) {
        villages[i].is_covered = false;
        villages[i].has_school = false;
    }

    for (int i = 0; i < V; i++) {
        printf("Xorio No%d dose syntetagmenes me morfh x,y : ", i + 1);
        scanf("%lf,%lf", &villages[i].x, &villages[i].y);
        while (getchar() != '\n');
    }

    // -------------------------
    // Initialize population
    // -------------------------
    for (int i = 0; i < POP_SIZE; i++)
        random_chromosome(population[i]);

    // -------------------------
    // GA loop
    // -------------------------
    for (int gen = 0; gen < GENERATIONS; gen++) {

        // Compute fitness
        for (int i = 0; i < POP_SIZE; i++)
            fitness_values[i] = compute_fitness(population[i]);

        // Find best chromosome
        int best_index = 0;
        for (int i = 1; i < POP_SIZE; i++)
            if (fitness_values[i] > fitness_values[best_index])
                best_index = i;

        // Elitism: copy the best few
        for (int e = 0; e < ELITE_COUNT; e++)
            copy_chrom(new_population[e], population[best_index]);

        // Generate rest
        for (int i = ELITE_COUNT; i < POP_SIZE; i++) {
            int p1 = tournament_select();
            int p2 = tournament_select();
            crossover(population[p1], population[p2], new_population[i]);
            mutate(new_population[i]);
        }

        // Replace population
        for (int i = 0; i < POP_SIZE; i++)
            copy_chrom(population[i], new_population[i]);

        if (gen % 200 == 0)
            printf("Generation %d | Best fitness: %.2f\n", gen, fitness_values[best_index]);
    }

    // Final fitness evaluation
    for (int i = 0; i < POP_SIZE; i++)
        fitness_values[i] = compute_fitness(population[i]);

    int best_index = 0;
    for (int i = 1; i < POP_SIZE; i++)
        if (fitness_values[i] > fitness_values[best_index])
            best_index = i;

    // -------------------------------
    // Output final school placements
    // -------------------------------
    printf("\n-------------------------------------------------\n");
    printf("Teliki lysi apo to Genetic Algorithm:\n");

    int school_count = 0;
    for (int i = 0; i < V; i++) {
        if (population[best_index][i] == 1) {
            school_count++;
            printf("  Sxoleio sto xorio %d (%.2f, %.2f)\n",
                   i + 1, villages[i].x, villages[i].y);
        }
    }

    printf("\nSynolika sxoleia: %d\n", school_count);

    return 0;
}