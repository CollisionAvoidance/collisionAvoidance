/*
* 	EC463 Senior Design Project
* 	Group 6: Collision Avoidance
* 	collision_detection.h
*/

#include <stdbool.h>

numbers CoM_turn(struct car one, struct car two);

void advance_pos(struct car * c, numbers dt);

int v2v_collision(struct car * first, struct car * second, struct collision * cols, int count);

void update_t_enter (struct car ** all_cars);

void update_vel_turn (struct car ** all_cars);

void update_t_exit (struct car ** all_cars);

void get_all_collisions_turn(struct car ** cars, int car_num, struct collision * cols, bool initial);