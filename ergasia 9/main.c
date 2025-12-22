#include "simulation.h"
#include "visualization.h"
#include <stdio.h>    // Should be here
#include <stdlib.h>
#include <string.h>
#include <math.h>

void print_menu() {
    printf("\n===============================================\n");
    printf("GRAVITATIONAL ORBIT SIMULATION WITH MATHGL\n");
    printf("===============================================\n");
    printf("System has 3 fixed massive planets:\n");
    printf("  1. Planet_A: mass=10000 at (0,0)\n");
    printf("  2. Planet_B: mass=5000 at (40,40)\n");
    printf("  3. Planet_C: mass=8000 at (-30,30)\n");
    printf("\nAvailable commands:\n");
    printf("  add    - Add a new satellite\n");
    printf("  run    - Run simulation for N steps\n");
    printf("  plot   - Create various plots\n");
    printf("  info   - Show current system status\n");
    printf("  reset  - Reset simulation (keep planets)\n");
    printf("  save   - Save data to files\n");
    printf("  help   - Show this menu\n");
    printf("  quit   - Exit program\n");
    printf("===============================================\n\n");
}

void add_satellite_interactive(Simulation *sim) {
    char name[20];
    double mass, x, y, vx, vy;
    
    printf("Enter satellite name: ");
    scanf("%s", name);
    
    printf("Enter mass (recommended 1-50): ");
    scanf("%lf", &mass);
    
    printf("Enter initial position (x y): ");
    scanf("%lf %lf", &x, &y);
    
    printf("Enter initial velocity (vx vy): ");
    scanf("%lf %lf", &vx, &vy);
    
    // Calculate circular orbit velocity for reference
    double r = sqrt(x*x + y*y); // Distance from Planet_A
    double v_circular = sqrt(G * 10000 / r);
    printf("Circular orbit velocity around Planet_A: %.2f\n", v_circular);
    
    add_satellite(sim, name, mass, x, y, vx, vy);
}

void run_simulation(Simulation *sim) {
    int steps;
    printf("Enter number of steps to simulate: ");
    scanf("%d", &steps);
    
    printf("Simulation running...\n");
    printf("Time step: %.4f\n", sim->dt);
    
    for (int i = 0; i < steps; i++) {
        simulation_step(sim);
        
        // Print progress every 1000 steps
        if (i % 1000 == 0 && i > 0) {
            printf("Step %d/%d, Time: %.2f, Bodies: %d\n", 
                   i, steps, sim->time, sim->body_count);
        }
    }
    
    printf("Simulation completed.\n");
    printf("Final time: %.2f\n", sim->time);
    printf("Energy conservation: %.6f%%\n", (1 - sim->energy_variation) * 100);
}

void plot_menu(Simulation *sim) {
    int choice;
    
    do {
        printf("\n--- PLOT MENU ---\n");
        printf("1. Save trajectory data to files\n");
        printf("2. System snapshot (ASCII display)\n");
        printf("3. Create GNUplot script\n");
        printf("4. Return to main menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                save_plot_data(sim);
                break;
            case 2:
                plot_system_snapshot(sim);
                break;
            case 3:
                {
                    // Create GNUplot script
                    FILE *fp = fopen("plot_orbits.gnuplot", "w");
                    if (fp) {
                        fprintf(fp, "set terminal png\n");
                        fprintf(fp, "set output 'orbits.png'\n");
                        fprintf(fp, "set title 'Orbital Trajectories'\n");
                        fprintf(fp, "plot 'trajectories.csv' using 1:2 with lines\n");
                        fclose(fp);
                        printf("GNUplot script created. Run: gnuplot plot_orbits.gnuplot\n");
                    }
                }
                break;
            case 4:
                return;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);
}

void show_system_info(Simulation *sim) {
    printf("\n=== SYSTEM INFORMATION ===\n");
    printf("Simulation time: %.2f\n", sim->time);
    printf("Time step: %.4f\n", sim->dt);
    printf("Total steps: %d\n", sim->step);
    printf("Total bodies: %d\n", sim->body_count);
    printf("History points: %d\n", sim->history_index);
    printf("Energy variation: %.2e\n", sim->energy_variation);
    printf("Total angular momentum: %.2f\n", sim->total_angular_momentum);
    
    printf("\n=== BODY LIST ===\n");
    for (int i = 0; i < sim->body_count; i++) {
        printf("%s: ", sim->bodies[i].name);
        if (sim->bodies[i].is_planet) {
            printf("PLANET ");
        } else {
            printf("SATELLITE ");
        }
        printf("m=%.1f ", sim->bodies[i].mass);
        printf("pos=(%.2f,%.2f) ", 
               sim->bodies[i].position.x, sim->bodies[i].position.y);
        printf("vel=(%.2f,%.2f) ", 
               sim->bodies[i].velocity.x, sim->bodies[i].velocity.y);
        printf("E=%.2f\n", sim->bodies[i].energy);
    }
}

int main() {
    Simulation sim;
    char command[20];
    
    // Initialize simulation
    initialize_simulation(&sim);
    
    printf("\nGRAVITATIONAL ORBIT SIMULATION\n");
    printf("===============================\n");
    printf("Using MathGL for scientific visualization\n");
    printf("Three massive planets are fixed in space.\n");
    printf("Add satellites and watch their orbits!\n\n");
    
    // Add some default satellites for demonstration
    printf("Adding demonstration satellites...\n");
    add_satellite(&sim, "Sat1", 10.0, 20.0, 0.0, 0.0, 22.36);  // Circular
    add_satellite(&sim, "Sat2", 15.0, 0.0, 25.0, -20.0, 0.0);  // Circular
    add_satellite(&sim, "Sat3", 5.0, -20.0, 0.0, 0.0, -18.0);  // Elliptical
    
    print_menu();
    
    // Main command loop
    while (1) {
        printf("\nCommand> ");
        scanf("%s", command);
        
        if (strcmp(command, "add") == 0) {
            add_satellite_interactive(&sim);
        }
        else if (strcmp(command, "run") == 0) {
            run_simulation(&sim);
        }
        else if (strcmp(command, "plot") == 0) {
            plot_menu(&sim);
        }
        else if (strcmp(command, "info") == 0) {
            show_system_info(&sim);
        }
        else if (strcmp(command, "reset") == 0) {
            reset_simulation(&sim);
        }
        else if (strcmp(command, "save") == 0) {
            save_plot_data(&sim);
        }
        else if (strcmp(command, "help") == 0) {
            print_menu();
        }
        else if (strcmp(command, "quit") == 0) {
            printf("Exiting program.\n");
            break;
        }
        else {
            printf("Unknown command. Type 'help' for available commands.\n");
        }
    }
    
    return 0;
}