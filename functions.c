/*
*   EC463 Senior Design Project
*   Group 6: Collision Avoidance
*   functions.h
*   Defines some functions for printing formats and logistics
*/


#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

#define COS45 0.707107
#define SIN45 0.707107

struct collision * new_collision (numbers t, struct car * one, struct car * two) {
  struct collision * newCol = malloc(sizeof(struct collision));
  newCol->time = t;
  newCol->first = one;
  newCol->second = two;
  return newCol;
}

struct car * new_car(char * myid, numbers px, numbers py, numbers velx, numbers vely,
  numbers x_enter, numbers y_enter, numbers x_exit, numbers y_exit, enum direction turn) {

  struct car * newCar = malloc(sizeof(struct car));
  newCar->id = myid;
  newCar->rx = px;
  newCar->ry = py;
  newCar->vx = velx;
  newCar->vy = vely;
  newCar->intent = turn;
  newCar->t_enter = 0;
  newCar->t_exit = 0;            // Time when the car enters and exits intersection. Should be computed later
  newCar->rx_enter = x_enter;
  newCar->ry_enter = y_enter;
  newCar->rx_exit = x_exit;
  newCar->ry_exit = y_exit;
  newCar->vx_turn = velx;       // Velocities inside the intersection (assuming straight lines)
  newCar->vy_turn = vely;       // As an initial value, they are the same as the initial velocities        

  return newCar;
}


void print_array(numbers * times) {
  printf("Times are:\n");
  int i;
  for (i = 0; i < NUM_PAIRS; i++) {
    printf("%d = %f\n", i, times[i]);
  }
}

void print_cols(struct collision * cols) {
  printf("Collisions:\n");
  int i;
  for (i = 0; i < NUM_PAIRS; i++) {
    printf("(%s, %s) = %.2f\n", ((cols[i]).first)->id, ((cols[i]).second)->id, (cols[i]).time);
  }
  printf("\n");
}

void print_cars (struct car c) {
  printf("\n");
  printf("Car %s\n", c.id);
  printf(" (x, y) = (%.2f,%.2f)\n", c.rx, c.ry);
  printf(" (vx, vy) = (%.2f,%.2f)\n", c.vx, c.vy);
  printf(" (x_enter, y_enter) = (%.2f,%.2f)\n", c.rx_enter, c.ry_enter);
  printf(" (x_exit, y_exit) = (%.2f,%.2f)\n", c.rx_exit, c.ry_exit);
  printf(" (vx_turn, vy_turn) = (%.2f,%.2f)\n", c.vx_turn, c.vy_turn);
  printf(" (t_enter, t_exit) = (%.2f, %.2f)\n", c.t_enter, c.t_exit);

}

int comp_numbers (const void * elem1, const void * elem2) 
{
    numbers f = *((numbers*)elem1);
    numbers s = *((numbers*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int comp_cols(const void * elem1, const void * elem2)
{
  struct collision e1 = *((struct collision *)elem1);
  struct collision e2 = *((struct collision *)elem2);
  if ((e1.time) > (e2.time)) return 1;
  if ((e1.time) < (e2.time)) return -1;
  return 0;
}

int comp_cars_t_entry(const void * elem1, const void * elem2)
{
  struct car e1 = **((struct car **)elem1);
  struct car e2 = **((struct car **)elem2);
  if ((e1.t_enter) > (e2.t_enter)) return 1;
  if ((e1.t_enter) < (e2.t_enter)) return -1;
  return 0;
}


numbers rotation_matrix_x(numbers x, numbers y, enum direction intent) {

  if (intent == straight) {
    return x;
  }
  if (intent == left) {
    return (COS45*x - SIN45*y);
  }
  if (intent == right) {
    return (COS45*x + SIN45*y);
  }
  return 0;
}

numbers rotation_matrix_y(numbers x, numbers y, enum direction intent) {

  if (intent == straight) {
    return y;
  }
  if (intent == left) {
    return (SIN45*x + COS45*y);
  }
  if (intent == right) {
    return (-SIN45*x + COS45*y);
  }
  return 0;
}


// returns a pseudo-boolean: 
int arrives_before (struct car * one, struct car * two) {
  if (one->t_enter < two->t_enter) {
    return 1;
  }
  return 0;
}


// Make it more readable (maybe modifying the qsort comparision function)

int num_collisions(struct collision * cols) { // cols is an array (size NUM_PAIRS) of collision structs
  // Possible outputs: 0, 1, 2+

  if (cols[NUM_PAIRS-1].time == 0) {          // 0 COLLISIONS
    return 0;
  }
  else if (cols[NUM_PAIRS-1].time > 0) {
    if (cols[NUM_PAIRS-2].time == 0) {        // 1 COLLISIONS
      return 1;
    }
  }
  return 2;           // 2+ COLLISIONS
}


