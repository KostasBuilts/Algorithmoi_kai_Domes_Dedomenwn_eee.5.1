#ifndef SIMULATION_H
#define SIMULATION_H

#include "physics.h"

#define MAX_HISTORY 10000

typedef struct {
    CelestialBody bodies[MAX_OBJECTS];
    int body_count;
    double time;
    double dt;
    int step;
    
    // History for visualization
    Vector2D position_history[MAX_OBJECTS][MAX_HISTORY];
    int history_index;
    int max_history;
    
    // Statistics
    double total_energy;
    double total_angular_momentum;
    double energy_variation;
} Simulation;

void initialize_simulation(Simulation *sim);
void initialize_planets(CelestialBody planets[]);
void add_satellite(Simulation *sim, char *name, double mass, 
                   double x, double y, double vx, double vy);
void simulation_step(Simulation *sim);
void save_history(Simulation *sim);
void reset_simulation(Simulation *sim);

#endif