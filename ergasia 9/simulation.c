#include "simulation.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>  // ADD THIS LINE

void initialize_simulation(Simulation *sim) {
    sim->body_count = 0;
    sim->time = 0.0;
    sim->dt = 0.001;
    sim->step = 0;
    sim->history_index = 0;
    sim->max_history = MAX_HISTORY;
    sim->total_energy = 0.0;
    sim->total_angular_momentum = 0.0;
    sim->energy_variation = 0.0;
    
    // Initialize planets
    initialize_planets(sim->bodies);
    sim->body_count = NUM_PLANETS;
    
    // Clear history
    for (int i = 0; i < MAX_OBJECTS; i++) {
        for (int j = 0; j < MAX_HISTORY; j++) {
            sim->position_history[i][j].x = 0;
            sim->position_history[i][j].y = 0;
        }
    }
}

void initialize_planets(CelestialBody planets[]) {
    // Planet A - Central massive planet
    planets[0].mass = 10000.0;
    planets[0].position.x = 0.0;
    planets[0].position.y = 0.0;
    planets[0].velocity.x = 0.0;
    planets[0].velocity.y = 0.0;
    planets[0].is_movable = 0;
    planets[0].is_planet = 1;
    strcpy(planets[0].name, "Planet_A");
    planets[0].energy = 0.0;
    planets[0].angular_momentum = 0.0;
    
    // Planet B
    planets[1].mass = 5000.0;
    planets[1].position.x = 40.0;
    planets[1].position.y = 40.0;
    planets[1].velocity.x = 0.0;
    planets[1].velocity.y = 0.0;
    planets[1].is_movable = 0;
    planets[1].is_planet = 1;
    strcpy(planets[1].name, "Planet_B");
    planets[1].energy = 0.0;
    planets[1].angular_momentum = 0.0;
    
    // Planet C
    planets[2].mass = 8000.0;
    planets[2].position.x = -30.0;
    planets[2].position.y = 30.0;
    planets[2].velocity.x = 0.0;
    planets[2].velocity.y = 0.0;
    planets[2].is_movable = 0;
    planets[2].is_planet = 1;
    strcpy(planets[2].name, "Planet_C");
    planets[2].energy = 0.0;
    planets[2].angular_momentum = 0.0;
}

void add_satellite(Simulation *sim, char *name, double mass, 
                   double x, double y, double vx, double vy) {
    if (sim->body_count < MAX_OBJECTS) {
        CelestialBody *sat = &sim->bodies[sim->body_count];
        
        sat->mass = mass;
        sat->position.x = x;
        sat->position.y = y;
        sat->velocity.x = vx;
        sat->velocity.y = vy;
        sat->acceleration.x = 0.0;
        sat->acceleration.y = 0.0;
        sat->is_movable = 1;
        sat->is_planet = 0;
        strncpy(sat->name, name, 19);
        sat->name[19] = '\0';
        sat->energy = 0.0;
        sat->angular_momentum = 0.0;
        
        sim->body_count++;
        
        printf("Added satellite: %s (m=%.1f) at (%.1f, %.1f) with v=(%.1f, %.1f)\n",
               name, mass, x, y, vx, vy);
    } else {
        printf("Cannot add more satellites. Maximum limit reached.\n");
    }
}

void simulation_step(Simulation *sim) {
    // Calculate forces
    calculate_forces(sim->bodies, sim->body_count);
    
    // Update positions
    update_positions(sim->bodies, sim->body_count, sim->dt);
    
    // Calculate energies
    calculate_energy(sim->bodies, sim->body_count);
    
    // Update statistics
    double current_energy = 0.0;
    double current_angular_momentum = 0.0;
    for (int i = 0; i < sim->body_count; i++) {
        current_energy += sim->bodies[i].energy;
        current_angular_momentum += sim->bodies[i].angular_momentum;
    }
    
    if (sim->step == 0) {
        sim->total_energy = current_energy;
    }
    sim->energy_variation = fabs((current_energy - sim->total_energy) / sim->total_energy);
    sim->total_angular_momentum = current_angular_momentum;
    
    // Save history every 10 steps
    if (sim->step % 10 == 0) {
        save_history(sim);
    }
    
    sim->time += sim->dt;
    sim->step++;
}

void save_history(Simulation *sim) {
    if (sim->history_index < sim->max_history) {
        for (int i = 0; i < sim->body_count; i++) {
            sim->position_history[i][sim->history_index] = sim->bodies[i].position;
        }
        sim->history_index++;
    } else {
        // Shift history to make room (FIFO)
        for (int i = 0; i < sim->body_count; i++) {
            for (int j = 0; j < sim->max_history - 1; j++) {
                sim->position_history[i][j] = sim->position_history[i][j + 1];
            }
            sim->position_history[i][sim->max_history - 1] = sim->bodies[i].position;
        }
    }
}

void reset_simulation(Simulation *sim) {
    // Keep planets, reset satellites
    sim->body_count = NUM_PLANETS;
    sim->time = 0.0;
    sim->step = 0;
    sim->history_index = 0;
    
    // Reinitialize planets to original positions
    initialize_planets(sim->bodies);
    
    printf("Simulation reset. Planets remain, satellites removed.\n");
}