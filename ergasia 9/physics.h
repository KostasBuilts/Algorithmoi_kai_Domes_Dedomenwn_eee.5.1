#ifndef PHYSICS_H
#define PHYSICS_H

#define MAX_OBJECTS 15      // 3 planets + 12 satellites
#define NUM_PLANETS 3       // Fixed planets
#define G 1.0              // Gravitational constant

typedef struct {
    double x;
    double y;
} Vector2D;

typedef struct {
    double mass;
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    Vector2D next_position;
    int is_movable;
    int is_planet;
    char name[20];
    double energy;
    double angular_momentum;
} CelestialBody;

// Vector operations
Vector2D vector_add(Vector2D a, Vector2D b);
Vector2D vector_subtract(Vector2D a, Vector2D b);
Vector2D vector_multiply(double scalar, Vector2D v);
double vector_magnitude(Vector2D v);
Vector2D vector_normalize(Vector2D v);
double vector_distance(Vector2D a, Vector2D b);
double vector_dot(Vector2D a, Vector2D b);

// Physics calculations
Vector2D calculate_gravitational_force(double m1, Vector2D pos1, 
                                       double m2, Vector2D pos2);
void calculate_forces(CelestialBody bodies[], int count);
void update_positions(CelestialBody bodies[], int count, double dt);
void calculate_energy(CelestialBody bodies[], int count);

#endif