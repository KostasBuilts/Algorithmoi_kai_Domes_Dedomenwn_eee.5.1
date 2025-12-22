#include "physics.h"
#include <math.h>
#include <stdio.h>

Vector2D vector_add(Vector2D a, Vector2D b) {
    Vector2D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Vector2D vector_subtract(Vector2D a, Vector2D b) {
    Vector2D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

Vector2D vector_multiply(double scalar, Vector2D v) {
    Vector2D result;
    result.x = scalar * v.x;
    result.y = scalar * v.y;
    return result;
}

double vector_magnitude(Vector2D v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

Vector2D vector_normalize(Vector2D v) {
    double mag = vector_magnitude(v);
    Vector2D result;
    if (mag > 1e-12) {
        result.x = v.x / mag;
        result.y = v.y / mag;
    } else {
        result.x = 0;
        result.y = 0;
    }
    return result;
}

double vector_distance(Vector2D a, Vector2D b) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

double vector_dot(Vector2D a, Vector2D b) {
    return a.x * b.x + a.y * b.y;
}

Vector2D calculate_gravitational_force(double m1, Vector2D pos1, 
                                       double m2, Vector2D pos2) {
    Vector2D force;
    Vector2D r = vector_subtract(pos2, pos1);
    double distance = vector_magnitude(r);
    
    // Softening parameter to avoid singularity
    double epsilon = 1e-3;
    double distance_soft = sqrt(distance * distance + epsilon * epsilon);
    
    if (distance_soft > 1e-12) {
        double force_magnitude = (G * m1 * m2) / (distance_soft * distance_soft);
        Vector2D direction = vector_normalize(r);
        force = vector_multiply(force_magnitude, direction);
    } else {
        force.x = 0;
        force.y = 0;
    }
    
    return force;
}

void calculate_forces(CelestialBody bodies[], int count) {
    // Reset accelerations
    for (int i = 0; i < count; i++) {
        bodies[i].acceleration.x = 0;
        bodies[i].acceleration.y = 0;
    }
    
    // Calculate gravitational forces between all pairs
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            Vector2D force = calculate_gravitational_force(
                bodies[i].mass, bodies[i].position,
                bodies[j].mass, bodies[j].position);
            
            // Apply force to body i (F = ma => a = F/m)
            if (bodies[i].is_movable) {
                bodies[i].acceleration = vector_add(
                    bodies[i].acceleration,
                    vector_multiply(1.0 / bodies[i].mass, force));
            }
            
            // Apply opposite force to body j
            if (bodies[j].is_movable) {
                Vector2D opposite_force = vector_multiply(-1.0, force);
                bodies[j].acceleration = vector_add(
                    bodies[j].acceleration,
                    vector_multiply(1.0 / bodies[j].mass, opposite_force));
            }
        }
    }
}

void update_positions(CelestialBody bodies[], int count, double dt) {
    // Verlet integration for better accuracy
    for (int i = 0; i < count; i++) {
        if (bodies[i].is_movable) {
            // Update velocity: v = v + a * dt
            bodies[i].velocity = vector_add(
                bodies[i].velocity,
                vector_multiply(dt, bodies[i].acceleration));
            
            // Update position: x = x + v * dt
            bodies[i].next_position = vector_add(
                bodies[i].position,
                vector_multiply(dt, bodies[i].velocity));
        }
    }
    
    // Commit new positions
    for (int i = 0; i < count; i++) {
        if (bodies[i].is_movable) {
            bodies[i].position = bodies[i].next_position;
        }
    }
}

void calculate_energy(CelestialBody bodies[], int count) {
    for (int i = 0; i < count; i++) {
        // Kinetic energy: K = 1/2 * m * v^2
        double v_squared = bodies[i].velocity.x * bodies[i].velocity.x +
                          bodies[i].velocity.y * bodies[i].velocity.y;
        double kinetic = 0.5 * bodies[i].mass * v_squared;
        
        // Potential energy: U = -sum(G * m_i * m_j / r_ij)
        double potential = 0;
        for (int j = 0; j < count; j++) {
            if (i != j) {
                double distance = vector_distance(
                    bodies[i].position, bodies[j].position);
                if (distance > 1e-12) {
                    potential -= G * bodies[i].mass * bodies[j].mass / distance;
                }
            }
        }
        
        bodies[i].energy = kinetic + 0.5 * potential; // Factor 0.5 to avoid double counting
        
        // Angular momentum: L = m * (r × v)
        bodies[i].angular_momentum = bodies[i].mass * 
            (bodies[i].position.x * bodies[i].velocity.y - 
             bodies[i].position.y * bodies[i].velocity.x);
    }
}