/*
*   EC463 Senior Design Project
*   Group 6: Collision Avoidance
*   collision_detection.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"
#include "collision_detection.h"


#define RADIUS 1  // sqrt(LENGTH^2 * WIDTH^2)

// Width and length values taken from center to edge of car, so half the actual length of car

/*
numbers CoM(struct car one, struct car two) {

  numbers distance = RADIUS;

  numbers a = one.rx - two.rx;
  numbers b = one.ry - two.ry;
  numbers c = one.vx - two.vx;
  numbers d = one.vy - two.vy;

  numbers A = c * c + d * d;
  numbers B = 2 * (a * c + b * d);
  numbers C = (a * a + b * b - distance * distance);

  numbers rootarg = B * B - 4 * A * C;

  if (rootarg < 0) return 0;

  numbers root = sqrt(rootarg);

  numbers t1 = (-B - root) / (2 * A);
  numbers t2 = (-B + root) / (2 * A);

  if (t1 > 0)
    return t1;
  if (t2 > 0)
    return t2;

  return 0;
}
*/

numbers CoM_turn(struct car one, struct car two) {

  numbers distance = RADIUS;

  numbers a = one.rx_enter - two.rx_enter;
  numbers b = one.ry_enter - two.ry_enter;
  numbers c = one.vx_turn - two.vx_turn;
  numbers d = one.vy_turn - two.vy_turn;

  numbers A = c * c + d * d;
  numbers B = 2 * (a * c + b * d);
  numbers C = (a * a + b * b - distance * distance);

  numbers rootarg = B * B - 4 * A * C;

  if (rootarg < 0) return -1;

  numbers root = sqrt(rootarg);

  numbers t1 = (-B - root) / (2 * A);
  numbers t2 = (-B + root) / (2 * A);

  if (t1 > 0)
    return t1;
  if (t2 > 0)
    return t2;

  return -1;
}

void advance_pos(struct car * c, numbers dt) {
  
  // x = x0 + v * dt
  c->rx_enter = c->rx_enter + (c->vx_turn * dt);
  c->ry_enter = c->ry_enter + (c->vy_turn * dt);
}

int v2v_collision(struct car * first, struct car * second, struct collision * cols, int count) {
  
  numbers dt;
  numbers rx_enter_old, ry_enter_old;

  dt = (second->t_enter) - (first->t_enter);
  rx_enter_old = first->rx_enter;
  ry_enter_old = first->ry_enter;
  advance_pos(first, dt);

  numbers t = CoM_turn(*first, *second);

  // Restore values that were previously saved
  first->rx_enter = rx_enter_old;
  first->ry_enter = ry_enter_old;

  if (t > 0 ){
    t += second->t_enter;

    if ((t < first->t_exit) && (t < second->t_exit)) {  // Valid collision inside the intersection

      cols[count] = *(new_collision(t, first, second));
      return 1;
    }
  }
  
  cols[count] = *(new_collision(0, first, second));

  return 0;
}



// Calculate time of entrance in the intersection for every car
void update_t_enter (struct car ** all_cars) {
  int i;
  for (i = 0; i < CAR_NUM; i++) {

    if (all_cars[i]->vx != 0) {
      all_cars[i]->t_enter = ((all_cars[i]->rx_enter - all_cars[i]->rx)/all_cars[i]->vx);
    }
    else if (all_cars[i]->vy != 0) {
      all_cars[i]->t_enter = ((all_cars[i]->ry_enter - all_cars[i]->ry)/all_cars[i]->vy);
      // t enter = 20 - 1.5 / -2.74
    }
    else
      all_cars[i]->t_enter = -1;
  }
}

// Calculate new velocities for every car (velocities during the turn)
void update_vel_turn (struct car ** all_cars) {
  int i;
  for (i = 0; i < CAR_NUM; i++) {
    all_cars[i]->vx_turn = rotation_matrix_x(all_cars[i]->vx, all_cars[i]->vy, all_cars[i]->intent);
    all_cars[i]->vy_turn = rotation_matrix_y(all_cars[i]->vx, all_cars[i]->vy, all_cars[i]->intent);
  }
}



// Calculate time of exit of the intersection for every car
void update_t_exit (struct car ** all_cars) {
  int i;
  for (i = 0; i < CAR_NUM; i++) {
    if (all_cars[i]->vx_turn != 0) {
      all_cars[i]->t_exit = ((all_cars[i]->rx_exit - all_cars[i]->rx_enter)/all_cars[i]->vx_turn) + all_cars[i]->t_enter;
    }
    else if (all_cars[i]->vy_turn != 0) {
      all_cars[i]->t_exit = ((all_cars[i]->ry_exit - all_cars[i]->ry_enter)/all_cars[i]->vy_turn) + all_cars[i]->t_enter;
    }
    else
      all_cars[i]->t_exit = -1;
  }
}

/*

(Note: There may be ways to make it less messy)
IDEA:
Cars enter intersection at different times and CoM algo assumes both 
starting positions are taken at time 0, so times have to be adjusted

      For each pair of cars:
      Find the car that first enters the intersection
      Check that a collision is possible
      If it is
      ∆t = tmax - tmin
      save pos_enter and exit of first car
      advance pos_turn for first car
      run CoM
      add t_enter
      restore pos_enter and exit
*/

void get_all_collisions_turn(struct car ** cars, int car_num, struct collision * cols, bool initial) {

  int count = 0;
  int i;
  numbers t;

  if (initial) {
    update_t_enter(cars);
    update_vel_turn(cars);
    update_t_exit(cars);
  }


  /*
  // Display cars (testing purposes)
  for (i = 0; i < 3; i++)
    print_cars(*(cars[i]));
  printf("\n");
  */
  
  // All combinations of 2 cars (should get 3choose2 = 3 iterations)
  for(i = 0; i < car_num; i++){
    for (int j = i+1; j < car_num; j++) {

      if (arrives_before(cars[i], cars[j]))   // i arrives before j
      {   
        v2v_collision(cars[i], cars[j], cols, count);

      }
      else        // j arrives before i, or they arrive at the same time
      {             
        v2v_collision(cars[j], cars[i], cols, count);
      }
      count++;
    }
  } 

  // Sort all collsions from earliest to latest
  qsort(cols, CAR_NUM, sizeof(struct collision), comp_cols);  

}


