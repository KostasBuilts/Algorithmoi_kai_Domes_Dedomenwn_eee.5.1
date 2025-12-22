#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "simulation.h"  // ADD THIS LINE to get Simulation definition

// MathGL plotting functions
void plot_orbits(Simulation *sim);
void plot_energy_evolution(Simulation *sim);
void plot_phase_space(Simulation *sim, int body_index);
void plot_system_snapshot(Simulation *sim);
void create_animation(Simulation *sim, int frames);
void save_plot_data(Simulation *sim);

#endif