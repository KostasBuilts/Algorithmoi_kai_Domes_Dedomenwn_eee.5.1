#include "visualization.h"
#include <mgl2/mgl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // ADD THIS LINE for strcpy/sprintf
#include <math.h>

void plot_orbits(Simulation *sim) {
    printf("Creating orbit plot...\n");
    
    // Create MathGL graph
    mglGraph gr(0, 1200, 800);
    gr.SetQuality(3); // High quality
    
    // Prepare data arrays
    mglData x_data[MAX_OBJECTS];
    mglData y_data[MAX_OBJECTS];
    
    int max_points = sim->history_index;
    if (max_points < 10) {
        printf("Not enough history data to plot.\n");
        return;
    }
    
    // Extract trajectory data for each body
    for (int i = 0; i < sim->body_count; i++) {
        x_data[i].Create(max_points);
        y_data[i].Create(max_points);
        
        for (int j = 0; j < max_points; j++) {
            x_data[i].a[j] = sim->position_history[i][j].x;
            y_data[i].a[j] = sim->position_history[i][j].y;
        }
    }
    
    // Set up the plot
    gr.SetRanges(-60, 60, -60, 60);
    gr.Aspect(1, 1);
    gr.Title("Orbital Trajectories");
    gr.SetFontSize(2.5);
    
    // Draw coordinate grid
    gr.Grid("xy", "h:");
    gr.Axis("xy");
    
    // Draw planets as large points
    for (int i = 0; i < NUM_PLANETS; i++) {
        gr.Plot(x_data[i], y_data[i], "r*8");
        
        // Add planet labels
        char label[50];
        sprintf(label, "%s", sim->bodies[i].name);
        double x = sim->bodies[i].position.x;
        double y = sim->bodies[i].position.y;
        gr.Puts(mglPoint(x, y), label, ":r", 2.5);
    }
    
    // Draw satellite trajectories with different colors
    const char *colors = "bgcmy"; // Different colors for satellites
    int color_idx = 0;
    
    for (int i = NUM_PLANETS; i < sim->body_count; i++) {
        char style[20];
        sprintf(style, "%c-2", colors[color_idx % 5]);
        gr.Plot(x_data[i], y_data[i], style);
        color_idx++;
    }
    
    // Add legend
    gr.AddLegend("Planets (fixed)", "r*");
    gr.AddLegend("Satellites", "b-");
    gr.Legend();
    
    // Add time information
    char time_text[100];
    sprintf(time_text, "Time: %.1f, Steps: %d", sim->time, sim->step);
    gr.Puts(mglPoint(-55, 55), time_text, ":k", 2);
    
    // Save the plot
    gr.WritePNG("orbits_plot.png");
    printf("Orbit plot saved as 'orbits_plot.png'\n");
    
    // Create a 3D version if desired
    mglGraph gr3(0, 1200, 800);
    mglData x3d[MAX_OBJECTS], y3d[MAX_OBJECTS], z3d[MAX_OBJECTS];
    
    for (int i = 0; i < sim->body_count; i++) {
        x3d[i].Create(max_points);
        y3d[i].Create(max_points);
        z3d[i].Create(max_points);
        
        for (int j = 0; j < max_points; j++) {
            x3d[i].a[j] = sim->position_history[i][j].x;
            y3d[i].a[j] = sim->position_history[i][j].y;
            z3d[i].a[j] = j * 0.1; // Time as z-axis
        }
    }
    
    gr3.SetRanges(-60, 60, -60, 60, 0, max_points * 0.1);
    gr3.Rotate(50, 60);
    gr3.Box();
    gr3.Axis();
    gr3.Title("3D Orbital Trajectories (Z = Time)");
    
    for (int i = 0; i < NUM_PLANETS; i++) {
        gr3.Plot(x3d[i], y3d[i], z3d[i], "r*6");
    }
    
    color_idx = 0;
    for (int i = NUM_PLANETS; i < sim->body_count; i++) {
        char style[20];
        sprintf(style, "%c-", colors[color_idx % 5]);
        gr3.Plot(x3d[i], y3d[i], z3d[i], style);
        color_idx++;
    }
    
    gr3.WritePNG("orbits_3d.png");
    printf("3D orbit plot saved as 'orbits_3d.png'\n");
}

void plot_energy_evolution(Simulation *sim) {
    printf("Creating energy evolution plot...\n");
    
    // This would track energy over time
    // For now, we'll create a simple plot of current energy distribution
    
    mglGraph gr(0, 800, 600);
    
    // Prepare energy data
    mglData energy_data(sim->body_count);
    mglData labels(sim->body_count);
    
    for (int i = 0; i < sim->body_count; i++) {
        energy_data.a[i] = fabs(sim->bodies[i].energy);
        labels.a[i] = i;
    }
    
    gr.SetRanges(0, sim->body_count-1, 0, energy_data.Maximal());
    gr.Box();
    gr.Axis();
    gr.SetFontSize(3);
    gr.Title("Energy Distribution");
    gr.Bars(energy_data, "r");
    
    // Add body names
    for (int i = 0; i < sim->body_count; i++) {
        gr.Puts(mglPoint(i, energy_data.a[i] * 1.1), 
                sim->bodies[i].name, ":b", 2);
    }
    
    gr.WritePNG("energy_distribution.png");
    printf("Energy plot saved as 'energy_distribution.png'\n");
}

void plot_phase_space(Simulation *sim, int body_index) {
    if (body_index < 0 || body_index >= sim->body_count) {
        printf("Invalid body index for phase space plot.\n");
        return;
    }
    
    printf("Creating phase space plot for %s...\n", sim->bodies[body_index].name);
    
    mglGraph gr(0, 800, 600);
    
    // Prepare phase space data (position vs velocity)
    int max_points = sim->history_index;
    if (max_points < 10) return;
    
    mglData x_data(max_points);
    mglData vx_data(max_points);
    
    for (int i = 0; i < max_points; i++) {
        x_data.a[i] = sim->position_history[body_index][i].x;
        // Need velocity history - for simplicity, use approximate derivative
        if (i > 0 && i < max_points - 1) {
            vx_data.a[i] = (sim->position_history[body_index][i+1].x - 
                           sim->position_history[body_index][i-1].x) / (2 * sim->dt * 10);
        } else {
            vx_data.a[i] = 0;
        }
    }
    
    gr.SetRanges(x_data.Minimal(), x_data.Maximal(), 
                vx_data.Minimal(), vx_data.Maximal());
    gr.Box();
    gr.Axis();
    gr.Title("Phase Space (x vs v_x)");
    gr.Label('x', "Position x", 0);
    gr.Label('y', "Velocity v_x", 0);
    
    // Color points by time
    mglData t_data(max_points);
    for (int i = 0; i < max_points; i++) {
        t_data.a[i] = i;
    }
    
    gr.Plot(x_data, vx_data, t_data, "o2");
    gr.Colorbar(">");
    
    char filename[100];
    sprintf(filename, "phase_space_%s.png", sim->bodies[body_index].name);
    gr.WritePNG(filename);
    printf("Phase space plot saved as '%s'\n", filename);
}

void plot_system_snapshot(Simulation *sim) {
    printf("Creating system snapshot...\n");
    
    mglGraph gr(0, 1000, 800);
    
    // Current positions
    mglData x_pos(sim->body_count);
    mglData y_pos(sim->body_count);
    mglData sizes(sim->body_count);
    mglData colors(sim->body_count);
    
    for (int i = 0; i < sim->body_count; i++) {
        x_pos.a[i] = sim->bodies[i].position.x;
        y_pos.a[i] = sim->bodies[i].position.y;
        
        // Size proportional to log(mass)
        sizes.a[i] = 5 * log10(1 + sim->bodies[i].mass / 100);
        
        // Color: red for planets, blue for satellites
        colors.a[i] = sim->bodies[i].is_planet ? 0 : 1;
    }
    
    gr.SetRanges(-70, 70, -70, 70);
    gr.Box();
    gr.Axis();
    gr.Title("Current System State");
    
    // Plot planets
    for (int i = 0; i < NUM_PLANETS; i++) {
        gr.Puts(mglPoint(x_pos.a[i], y_pos.a[i]), 
                sim->bodies[i].name, ":r", 3);
    }
    
    // Plot all bodies with size according to mass
    gr.Plot(x_pos, y_pos, sizes, "o");
    
    // Add velocity vectors
    double scale = 0.5; // Scaling factor for velocity vectors
    for (int i = 0; i < sim->body_count; i++) {
        if (sim->bodies[i].is_movable) {
            double x_end = x_pos.a[i] + scale * sim->bodies[i].velocity.x;
            double y_end = y_pos.a[i] + scale * sim->bodies[i].velocity.y;
            
            gr.Arrow(mglPoint(x_pos.a[i], y_pos.a[i]),
                    mglPoint(x_end, y_end), "Ar2");
        }
    }
    
    // Add info box
    char info[200];
    sprintf(info, "Time: %.1f\nBodies: %d\nEnergy variation: %.2e",
            sim->time, sim->body_count, sim->energy_variation);
    gr.Puts(mglPoint(-65, 65), info, ":k", 2.5);
    
    gr.WritePNG("system_snapshot.png");
    printf("System snapshot saved as 'system_snapshot.png'\n");
}

void save_plot_data(Simulation *sim) {
    printf("Saving plot data to files...\n");
    
    // Save trajectories to CSV for external plotting
    FILE *fp = fopen("trajectories.csv", "w");
    if (fp) {
        fprintf(fp, "Time");
        for (int i = 0; i < sim->body_count; i++) {
            fprintf(fp, ",%s_x,%s_y", sim->bodies[i].name, sim->bodies[i].name);
        }
        fprintf(fp, "\n");
        
        for (int t = 0; t < sim->history_index; t++) {
            fprintf(fp, "%.4f", t * sim->dt * 10); // Time
            for (int i = 0; i < sim->body_count; i++) {
                fprintf(fp, ",%.6f,%.6f", 
                        sim->position_history[i][t].x,
                        sim->position_history[i][t].y);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
        printf("Trajectory data saved to 'trajectories.csv'\n");
    }
    
    // Save body information
    fp = fopen("bodies_info.txt", "w");
    if (fp) {
        fprintf(fp, "CELESTIAL BODIES INFORMATION\n");
        fprintf(fp, "============================\n\n");
        fprintf(fp, "Simulation time: %.2f\n", sim->time);
        fprintf(fp, "Total steps: %d\n", sim->step);
        fprintf(fp, "Energy conservation: %.6f%%\n", 
                (1 - sim->energy_variation) * 100);
        fprintf(fp, "\n");
        
        fprintf(fp, "FIXED PLANETS:\n");
        fprintf(fp, "--------------\n");
        for (int i = 0; i < NUM_PLANETS; i++) {
            fprintf(fp, "%s: mass=%.1f, position=(%.2f, %.2f)\n",
                    sim->bodies[i].name, sim->bodies[i].mass,
                    sim->bodies[i].position.x, sim->bodies[i].position.y);
        }
        
        fprintf(fp, "\nSATELLITES:\n");
        fprintf(fp, "-----------\n");
        for (int i = NUM_PLANETS; i < sim->body_count; i++) {
            fprintf(fp, "%s: mass=%.2f, position=(%.4f, %.4f), "
                    "velocity=(%.4f, %.4f), energy=%.4f\n",
                    sim->bodies[i].name, sim->bodies[i].mass,
                    sim->bodies[i].position.x, sim->bodies[i].position.y,
                    sim->bodies[i].velocity.x, sim->bodies[i].velocity.y,
                    sim->bodies[i].energy);
        }
        fclose(fp);
        printf("Body information saved to 'bodies_info.txt'\n");
    }
}
