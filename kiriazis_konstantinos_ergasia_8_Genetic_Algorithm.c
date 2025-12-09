#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_CITIES 100
#define POP_SIZE 200
#define GENERATIONS 2000
#define MUT_RATE 0.05

typedef struct {
    double x;
    double y;
} City;

City cities[MAX_CITIES];
int numCities;

double distanceMatrix[MAX_CITIES][MAX_CITIES];

int population[POP_SIZE][MAX_CITIES];
int newPopulation[POP_SIZE][MAX_CITIES];
double fitness[POP_SIZE];

double dist(int a, int b) {
    return distanceMatrix[a][b];
}

double tourLength(int *tour) {
    double sum = 0;
    int i;
    for (i = 0; i < numCities - 1; i++) {
        sum += dist(tour[i], tour[i+1]);
    }
    sum += dist(tour[numCities - 1], tour[0]);
    return sum;
}

void computeFitness() {
    int i;
    for (i = 0; i < POP_SIZE; i++) {
        fitness[i] = 1.0 / tourLength(population[i]);
    }
}

int selectParent() {
    double total = 0;
    int i;
    for (i = 0; i < POP_SIZE; i++) total += fitness[i];
    double r = ((double) rand() / RAND_MAX) * total;

    double csum = 0;
    for (i = 0; i < POP_SIZE; i++) {
        csum += fitness[i];
        if (csum >= r) return i;
    }
    return POP_SIZE - 1;
}

void crossover(int *p1, int *p2, int *child) {
    int start, end, i, j, k;

    start = rand() % numCities;
    end = rand() % numCities;
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }

    for (i = 0; i < numCities; i++) child[i] = -1;

    for (i = start; i <= end; i++) {
        child[i] = p1[i];
    }

    k = 0;
    for (i = 0; i < numCities; i++) {
        int city = p2[i];
        int found = 0;
        for (j = start; j <= end; j++) {
            if (child[j] == city) {
                found = 1;
                break;
            }
        }
        if (!found) {
            while (child[k] != -1) k++;
            child[k] = city;
        }
    }
}

void mutate(int *tour) {
    double r = (double) rand() / RAND_MAX;
    if (r < MUT_RATE) {
        int a = rand() % numCities;
        int b = rand() % numCities;
        int tmp = tour[a];
        tour[a] = tour[b];
        tour[b] = tmp;
    }
}

void initPopulation() {
    int i, j;
    for (i = 0; i < POP_SIZE; i++) {
        for (j = 0; j < numCities; j++) {
            population[i][j] = j;
        }
        for (j = 0; j < numCities; j++) {
            int a = rand() % numCities;
            int b = rand() % numCities;
            int temp = population[i][a];
            population[i][a] = population[i][b];
            population[i][b] = temp;
        }
    }
}

void computeDistanceMatrix() {
    int i, j;
    for (i = 0; i < numCities; i++) {
        for (j = 0; j < numCities; j++) {
            double dx = cities[i].x - cities[j].x;
            double dy = cities[i].y - cities[j].y;
            distanceMatrix[i][j] = sqrt(dx*dx + dy*dy);
        }
    }
}

int main() {
    srand(time(NULL));

    int choice;

    printf("Choose input method:\n");
    printf("1 = Manual input\n");
    printf("2 = Load TSPLIB-style stress cluster\n");

    printf("Enter choice: "); /* prompt before scanf */
    scanf("%d", &choice);     /* user chooses input mode */

    if (choice == 1) {
        printf("Enter number of villages: "); /* prompt */
        scanf("%d", &numCities);              /* read N */

        if (numCities < 3 || numCities > MAX_CITIES) {
            printf("Invalid number.\n");
            return 0;
        }

        int i;
        for (i = 0; i < numCities; i++) {
            printf("Enter x coordinate for village %d: ", i); /* prompt */
            scanf("%lf", &cities[i].x);                      /* read x */

            printf("Enter y coordinate for village %d: ", i); /* prompt */
            scanf("%lf", &cities[i].y);                      /* read y */
        }
    }
    else {
        /* TSPLIB-style cluster: dense + irregular geometry */
        numCities = 20;

        double fixed[20][2] = {
            {10,10},{12,11},{11,14},{13,9},{9,12},
            {40,40},{41,43},{39,42},{42,39},{38,41},
            {70,10},{73,11},{75,9},{72,13},{71,8},
            {20,70},{22,73},{18,69},{21,72},{19,68}
        };

        int i;
        for (i = 0; i < numCities; i++) {
            cities[i].x = fixed[i][0];
            cities[i].y = fixed[i][1];
        }

        printf("Loaded TSPLIB-style 20-city stress cluster.\n");
    }

    computeDistanceMatrix();
    initPopulation();
    computeFitness();

    int gen, pi, idx, k;
    for (gen = 0; gen < GENERATIONS; gen++) {
        for (pi = 0; pi < POP_SIZE; pi++) {
            int p1 = selectParent();
            int p2 = selectParent();

            crossover(population[p1], population[p2], newPopulation[pi]);
            mutate(newPopulation[pi]);
        }

        for (idx = 0; idx < POP_SIZE; idx++) {
            for (k = 0; k < numCities; k++) {
                population[idx][k] = newPopulation[idx][k];
            }
        }

        computeFitness();
    }

    double bestFit = -1;
    int bestIndex = 0;

    int i;
    for (i = 0; i < POP_SIZE; i++) {
        if (fitness[i] > bestFit) {
            bestFit = fitness[i];
            bestIndex = i;
        }
    }

    printf("\nBest route found:\n");
    for (i = 0; i < numCities; i++) {
        printf("%d ", population[bestIndex][i]);
    }

    printf("\nTotal distance: %.3f\n", tourLength(population[bestIndex]));

    return 0;
}
