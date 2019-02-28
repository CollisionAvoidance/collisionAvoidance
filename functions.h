/*
*	EC463 Senior Design Project
*	Group 6: Collision Avoidance
*	functions.h
*	Defines some functions for printing formats and logistics
*/

#define NUM_PAIRS 3 // 3 choose 2 (#cars choose 2)
#define CAR_NUM 3

typedef double numbers;

enum direction {
	straight,
	left,
	right
};

struct collision {
  numbers time;
  struct car * first;
  struct car * second;
};

struct car {
  char * id;
  numbers rx, ry, vx, vy;							    // Current position and velocity
  enum direction intent; 							    // Final intended direction
  numbers t_enter, t_exit;  						  // Time when the car enters and exits intersection
  numbers rx_enter, ry_enter, rx_exit, ry_exit;		// Coordinate points where intersection path starts and ends
  numbers vx_turn, vy_turn;							  // Velocities inside the intersection (assuming straight lines)
  struct decision * compromise;					  // Struct defining the decision taken ->
  													             // (how much to accelerate and for how long)
};

struct decision {
	char * id;
	numbers vx0, vy0, vx1, vy1; 				    // Velocities before and after accelerating
	numbers ax, ay;							        // Acceleration value. 0 initially
	numbers t_start, t_stop, new_t_exit;		// times when it starts and stops accelerating
	numbers rx0, ry0, rx1, ry1;					     // Points in the trajectory where they start and stop accelerating.
};

// Constructors
struct collision * new_collision (numbers t, struct car * one, struct car * two);
struct car * new_car(char * myid, numbers px, numbers py, numbers velx, numbers vely,
				numbers x_enter, numbers y_enter, numbers x_exit, numbers y_exit, enum direction turn);

// Printing
void print_array(numbers * times);
void print_cols(struct collision * cols);
void print_cars (struct car c);

// Comparing (for sorting)
int comp_numbers (const void * elem1, const void * elem2);
int comp_cols(const void *elem1, const void *elem2);
int comp_cars_t_entry(const void * elem1, const void * elem2);

// Rotations for finding new velocities
numbers rotation_matrix_x(numbers x, numbers y, enum direction intent);
numbers rotation_matrix_y(numbers x, numbers y, enum direction intent);

int arrives_before (struct car * one, struct car * two);

// Returns the number of collisions in an array (0, 1, 2+)
int num_collisions(struct collision * cols);

