// New collision avoidance (compatible with simulation)
// FUNCTIONAL
// COMMENTED

/*
* EC463 Senior Design Project
* Group 6: Collision Avoidance
*/

/* Basic predictive unit - No stopping human DU 
* 	In case of collision, sort cars in increasing order by time of entry in the intersection
*   First car does not compromise
* 	Second slows down to enter when First exits
* 	Third slows down to enter when Second exits
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"
#include "collision_detection.h"
#include "collision_avoidance.h"

#define MAX_NAME_SIZE 20
#define ACCELERATION_DISTANCE 8 // in cm. It depends on how big we want our course to be 


#define V_MAX 30   			// in cm/s. Speed limit in intersection scenario
#define A_MAX 15			// in cm/s^2. Acceleration limit
#define ITER_NUM 10			// Number of iterations in the optimization process
#define MINUS_FACTOR 0.7	// For modifying the optimal velocity in the optimization iterations
#define PLUS_FACTOR 1.1


// Assumes a sorted array
struct collision * get_first_collision(struct collision * cols) {
	int i;
	for (i=0; i<CAR_NUM; i++) {		// Search for the earliest non-zero collision
		if (cols[i].time > 0) {
			return &cols[i];
		}
	}
	return &cols[0];					// In case there are no collisions, just return the first one (0)
}

struct car * get_third_car (struct collision * col, struct car ** all_cars) {
	int i;
	for (i = 0; i<CAR_NUM; i++) {
		if ((all_cars[i]->id != col->first->id) && (all_cars[i]->id != col->second->id)) {
			return all_cars[i];
		}
	}
	return all_cars[i];
}

struct car * get_mycar (struct car ** all_cars, char * mycar) {
	int i;
	for (i=0; i<CAR_NUM; i++) {
		if (all_cars[i]->id == mycar) {
			return all_cars[i];
		}
	}
	return all_cars[i];
}

/* Return the x and y coordinates, and the timestamp where the car starts accelerating/decelerating */

numbers get_start_x(struct car c) {
	if (c.vx == 0) { 		// approaching intersection form north or south
		return c.rx;
	}
	else if (c.vx > 0) {	// approaching intersection from west
		return c.rx_enter - ACCELERATION_DISTANCE;
	}
	return c.rx_enter + ACCELERATION_DISTANCE;		// apr. intersec. form east
}

numbers get_start_y(struct car c) {
	if (c.vy == 0) {		// approaching intersection form east or west
		return c.ry;
	}
	else if (c.vy > 0) {	// approaching from south
		return c.ry_enter - ACCELERATION_DISTANCE;
	}
	return c.ry_enter + ACCELERATION_DISTANCE;		// aproaching from north
}

numbers get_start_t (struct car c) {
	if (c.vy != 0) {		// North or South
		return (c.compromise->ry0 - c.ry)/c.vy;
	}
	else if (c.vx != 0) {	// East or West
		return (c.compromise->rx0 - c.rx)/c.vx;
	}
	return 0;

}

/*
numbers get_acceleration (struct car * c, numbers t) {
	// eq:  r = r0 + v0*t + (1/2)*a*(t^2) ----> a = 2(x-x0-v0*t)/(t^2)

	numbers r, r0, v0;

	if(c->vy != 0) {		// North or South
		r = c->ry_enter;
		r0 = get_start_y(*c);
		v0 = c->vy;
	}
	if (c->vx != 0) {	// East or West
		r = c->rx_enter;
		r0 = get_start_x(*c);
		v0 = c->vx;
	}
	//printf("get_acceleration returns %.2f for car %s\n", (2*(r - r0-v0*t))/(t*t), c->id);
	return (2*(r - r0-v0*t))/(t*t);
}
*/
numbers get_ax (struct car * c, numbers t) {
	// eq:  r = r0 + v0*t + (1/2)*a*(t^2) ----> a = 2(x-x0-v0*t)/(t^2)

	numbers r, r0, v0;

	if (c->vx != 0) {
		r = c->rx_enter;
		r0 = get_start_x(*c);
		v0 = c->vx;
		return (2*(r - r0-v0*t))/(t*t);
	}
	return 0;
}
numbers get_ay (struct car * c, numbers t) {
	// eq:  r = r0 + v0*t + (1/2)*a*(t^2) ----> a = 2(x-x0-v0*t)/(t^2)

	numbers r, r0, v0;

	if (c->vy != 0) {
		r = c->ry_enter;
		r0 = get_start_y(*c);
		v0 = c->vy;
		return (2*(r - r0-v0*t))/(t*t);
	}
	return 0;
}




void print_decision(struct car c){
	printf("\n");
	printf("Car %s decision\n", c.id);
	printf(" ax, ay = %f, %f\n", c.compromise->ax, c.compromise->ay);
	printf(" (rx0, ry0) = (%.2f,%.2f)\n", c.compromise->rx0, c.compromise->ry0);
	printf(" (rx1, ry1) = (%.2f,%.2f)\n", c.compromise->rx1, c.compromise->ry1);
	printf(" t_start = %.2f, t_stop = %.2f\n", c.compromise->t_start, c.compromise->t_stop);
	printf(" vx0, vy0 = %.2f, %.2f\n", c.compromise->vx0, c.compromise->vy0);
	printf(" vx1, vy1 = %.2f, %.2f\n", c.compromise->vx1, c.compromise->vy1);
}



/* "Constructor" for decision struct */

void new_decision (struct car * car) {

	car->compromise = (struct decision *)malloc(sizeof(struct decision));
	car->compromise->id = car->id;	

	car->compromise->rx0 = get_start_x(*car);
	//printf("rx0 = %.2f\n", car->compromise->rx0);
	car->compromise->ry0 = get_start_y(*car);
	//printf("ry0 = %.2f\n", car->compromise->ry0);
	car->compromise->vx0 = car->vx;
	car->compromise->vy0 = car->vy;	

	car->compromise->t_start = get_start_t(*car);
	printf("t_start = %.2f\n", car->compromise->t_start);


	car->compromise->t_stop = car->t_enter;			// Does not really matter for now because it 
													// will change when the acceleration is computed
	car->compromise->rx1 = car->rx_enter;		// For now, assume they stop accelerating when they enter the intersection
	car->compromise->ry1 = car->ry_enter;

	car->compromise->ax = 0;
	car->compromise->ay = 0;
	car->compromise->vx1 = 0;
	car->compromise->vy1 = 0;

}



/* Returns the final velocities after acceleration time */
// Assumes t_start and x, y start are already set in the decision struct

/*
numbers get_vx1(struct car c) {
	// v = v0 + a*t
	if (c.vx != 0) {
		return c.compromise->vx0 + (c.compromise->ax)*(c.compromise->t_stop - c.compromise->t_start);
	}
	return 0;
}
numbers get_vy1(struct car c) {
	// v = v0 + a*t
	if (c.vy != 0) {
		return c.compromise->vy0 + (c.compromise->ay)*(c.compromise->t_stop - c.compromise->t_start);
	}
	return 0;
}
*/


numbers get_vx1(struct car c) {		// I REALLY HOPE THIS IS RIGHT
	
	// v = sqrt(v0^2 + 2ad)
	numbers unit_x = c.vx / sqrt(c.vx*c.vx + c.vy*c.vy);
	numbers vx1 = sqrt(c.compromise->vx0 * c.compromise->vx0 + 2 * c.compromise->ax * ACCELERATION_DISTANCE * unit_x);
	return vx1;

}

numbers get_vy1(struct car c) {		// I REALLY HOPE THIS IS RIGHT
	
	// v = sqrt(v0^2 + 2ad)
	numbers unit_y = c.vy / sqrt(c.vx*c.vx + c.vy*c.vy);
	numbers vy1 = sqrt(c.compromise->vy0 * c.compromise->vy0 + 2 * c.compromise->ay * ACCELERATION_DISTANCE * unit_y);
	

	//printf("New vy1 for car %s = %.2f\n", c.id, vy1);

	return vy1;
}


void set_decision (struct car * car, numbers a_x, numbers a_y, numbers vx1, numbers vy1) {
	car->compromise->ax = a_x;
	car->compromise->ay = a_y;
	car->compromise->vx1 = vx1;
	car->compromise->vy1 = vy1;
	
	// update t_stop as well based on the new final velocity

	if (car->vy != 0) {		// North or South
		car->compromise->t_stop = (car->compromise->vy1 - car->compromise->vy0)/car->compromise->ay + car->compromise->t_start;
	}
	else if (car->vx != 0) {	// East or West
		car->compromise->t_stop = (car->compromise->vx1 - car->compromise->vx0)/car->compromise->ax + car->compromise->t_start;
	}
	car->t_enter = car->compromise->t_stop;


	printf("New t_stop for car %s = %.2f\n", car->id, car->compromise->t_stop);

}
/*
// Version where I use get_vx1(*car) as a separate function
void set_decision (struct car * car, numbers a_x, numbers a_y) {
	car->compromise->ax = a_x;
	car->compromise->ay = a_y;
	car->compromise->vx1 = get_vx1(*car);
	printf("New vx1 for car %s = %.2f\n", car->id, car->compromise->vx1);

	car->compromise->vy1 = get_vy1(*car);
	
	// update t_stop as well based on the new final velocity

	if (car->vy != 0) {		// North or South
		car->compromise->t_stop = (car->compromise->vy1 - car->compromise->vy0)/car->compromise->ay;
	}
	else if (car->vx != 0) {	// East or West
		car->compromise->t_stop = (car->compromise->vx1 - car->compromise->vx0)/car->compromise->ax;
	}
	car->t_enter = car->compromise->t_stop;


	printf("New t_stop for car %s = %.2f\n", car->id, car->compromise->t_stop);

}
*/

/*
// DONT NEED THIS BECAUSE WE'RE DOING IT IN set_decision
// Calculate time of entrance in the intersection for every car
void new_t_enter (struct car * c) {

      c->t_enter = c->compromise->t_stop;
      printf("new t_enter for %s is %.2f\n", c->id, c->t_enter);

}
*/

// Calculate new velocities for every car (velocities during the turn)
void new_vel_turn (struct car * c) {

    c->vx_turn = rotation_matrix_x(c->compromise->vx1, c->compromise->vy1, c->intent);
    c->vy_turn = rotation_matrix_y(c->compromise->vx1, c->compromise->vy1, c->intent);
    printf("Car %s new vel turn is: %.2f,%.2f\n", c->id, c->vx_turn, c->vy_turn);

}

// Calculate time of exit of the intersection for every car
void new_t_exit (struct car *c) {

    if (c->vx_turn != 0) {
      c->t_exit = ((c->rx_exit - c->rx_enter)/c->vx_turn) + c->t_enter;
    }
    else if (c->vy_turn != 0) {
      c->t_exit = ((c->ry_exit - c->ry_enter)/c->vy_turn) + c->t_enter;
    }
    else
      c->t_exit = -1;

}



bool is_collision (struct car ** cars) {

	// update cars with optimal velocities
	// check for collisions with new velocities
// void get_all_collisions_turn(struct car ** cars, int car_num, struct collision * cols, bool initial) {

	struct collision c [NUM_PAIRS]; 
	get_all_collisions_turn(cars, CAR_NUM, c, 0);
	int num_c = num_collisions(c);

	if (num_c) {
		return true;
	}
	return false;
}



void optimizedFunction (struct car ** all_cars) {
/* Sets the decisions for the three cars in the array all_cars based on optimal final velocity */
	
	numbers * v_amax_x = malloc(CAR_NUM * sizeof(numbers));	// Maximum x velocity the cars can go without exceding the acceleration limit
	numbers * v_amax_y = malloc(CAR_NUM * sizeof(numbers));	// Maximum y velocity the cars can go without exceding the acceleration limit
	numbers * v_opt_x = malloc(CAR_NUM * sizeof(numbers)); // optimal velocity with which the cars enter the intersection
	numbers * v_opt_y = malloc(CAR_NUM * sizeof(numbers)); // optimal velocity with which the cars enter the intersection

	numbers v_magnitude;
	numbers unit_x, unit_y;
	numbers a_max_x, a_max_y;


	// Compute the max acceleration velocities for all cars using v^2 = v0^2 + 2ad
	int i;
	for (i=0; i<CAR_NUM; i++) {	

		// compute magnitude of velocity vector
		v_magnitude = sqrt((all_cars[i]->vx)*(all_cars[i]->vx) + (all_cars[i]->vy)*(all_cars[i]->vy));
		
		// divide each component by the magnitude to fing unit vectors
		unit_x = (all_cars[i]->vx) / v_magnitude;
		unit_y = (all_cars[i]->vy) / v_magnitude;

		// multiply x and y components of unit vector by the magnitude to find max a components
		a_max_x = A_MAX * unit_x;
		a_max_y = A_MAX * unit_y;

		// Compute max acceleration velocities
		v_amax_x[i] = sqrt((all_cars[i]->vx)*(all_cars[i]->vx) + 2 * a_max_x * fabs(all_cars[i]->compromise->rx1 - all_cars[i]->compromise->rx0));
		v_amax_y[i] = sqrt((all_cars[i]->vy)*(all_cars[i]->vy) + 2 * a_max_y * fabs(all_cars[i]->compromise->ry1 - all_cars[i]->compromise->ry0));
		

		// If v<0 and a<0 and d<0 (Car moving in negative direction)
		if ((all_cars[i]->vx < 0) &&  (a_max_x < 0)) {
			v_amax_x[i] = sqrt((-all_cars[i]->vx)*(-all_cars[i]->vx) + 2 * (-a_max_x) * (- all_cars[i]->compromise->rx1 + all_cars[i]->compromise->rx0));
			v_amax_x[i] = - v_amax_x[i];
		}
		if ((all_cars[i]->vy < 0) &&  (a_max_y < 0)) {
			v_amax_y[i] = sqrt((-all_cars[i]->vy)*(-all_cars[i]->vy) + 2 * (-a_max_y) * (- all_cars[i]->compromise->ry1 + all_cars[i]->compromise->ry0));
			v_amax_y[i] = - v_amax_y[i];
		}		


		//printf("d = %.2f, ax = %.2f, velocity x at maximum acceleration =  %.2f\n", fabs(all_cars[i]->compromise->rx1 - all_cars[i]->compromise->rx0), a_max_x, v_amax_x[i]);
		//printf("ay = %.2f, velocity y at maximum acceleration =  %.2f\n", a_max_y, v_amax_y[i]);
		//printf("magnitude of above = %.2f\n", sqrt(v_amax_x[i] * v_amax_x[i] + v_amax_y[i] * v_amax_y[i]));

		
		// If the velocity that yields the maximum acceleration is smaller than the speed limit, set it as the threshold
		if (sqrt(v_amax_x[i] * v_amax_x[i] + v_amax_y[i] * v_amax_y[i]) <= V_MAX) {		// if (v_amax <= V_MAX) 
			v_opt_x[i] = v_amax_x[i];
			v_opt_y[i] = v_amax_y[i];

			set_decision(all_cars[i], a_max_x, a_max_y, v_opt_x[i], v_opt_y[i]);	// Temporary to test collision rate
			new_vel_turn(all_cars[i]);
			new_t_exit(all_cars[i]);

			printf("ax = %.2f, ay = %.2f\n", a_max_x, a_max_y);


		}
		else {		// opt = vel_VMAX;
			
			// multiply x and y components of unit vector by the magnitude to find actual v components
			v_opt_x[i] = V_MAX * unit_x;
			v_opt_y[i] = V_MAX * unit_y;

			numbers a_vmax_x = 0.5 * (v_opt_x[i]*v_opt_x[i] - (all_cars[i]->vx)*(all_cars[i]->vx)) / (ACCELERATION_DISTANCE * unit_x);
			numbers a_vmax_y = 0.5 * (v_opt_y[i]*v_opt_y[i] - (all_cars[i]->vy)*(all_cars[i]->vy)) / (ACCELERATION_DISTANCE * unit_y);

			set_decision(all_cars[i], a_max_x, a_max_y, v_opt_x[i], v_opt_y[i]);
			new_vel_turn(all_cars[i]);
			new_t_exit(all_cars[i]);
			printf("ax = %.2f, ay = %.2f\n", a_vmax_x, a_vmax_y);

		}


	}
	
	int car_decrease = 0;

		int it = 0;
		while (it < ITER_NUM) {  // Is this checked sequentially or in parallel? 
			

			if (is_collision(all_cars)) {		// Reduce one of the car's velocities
				
				v_opt_x[car_decrease] *= MINUS_FACTOR;
				v_opt_y[car_decrease] *= MINUS_FACTOR;

				int signx;
				if (all_cars[car_decrease]->vx < 0) signx = -1;
				else signx = 1;

				int signy;
				if (all_cars[car_decrease]->vy < 0) signy = -1;
				else signy = 1;

				a_max_x = 0.5 * (v_opt_x[car_decrease]*v_opt_x[car_decrease] - (all_cars[car_decrease]->vx)*(all_cars[car_decrease]->vx)) / (ACCELERATION_DISTANCE*signx);
				a_max_y = 0.5 * (v_opt_y[car_decrease]*v_opt_y[car_decrease] - (all_cars[car_decrease]->vy)*(all_cars[car_decrease]->vy)) / (ACCELERATION_DISTANCE*signy);


				printf("Modified ax = %.2f, ay = %.2f\n", a_max_x, a_max_y);

				printf("Modifying decision of car %d\n", car_decrease);
				set_decision(all_cars[car_decrease], a_max_x, a_max_y, v_opt_x[car_decrease], v_opt_y[car_decrease]);
				new_vel_turn(all_cars[car_decrease]);
				new_t_exit(all_cars[car_decrease]);
				
				car_decrease = (car_decrease+1) % CAR_NUM;

			}
			
			else {
				printf("NO COLLISIONS ANYMORE..............\n");
				break;
			}

			car_decrease = (car_decrease+1) % CAR_NUM;
			it++;	
		}
}
/*

	int car_decrease = 0;

	//if (!collison(all_cars, v_opt_x, v_opt_y)) {		// Try optimal case: v is maximized and no collisions
	//	return;
	//}

	//else {		// maximum velocities DO cause collision

		int it = 0;
		while (it < ITER_NUM) {  // Is this checked sequentially or in parallel? 
			
			// compute magnitude of velocity vector
			v_magnitude = sqrt((all_cars[car_decrease]->vx)*(all_cars[car_decrease]->vx) + (all_cars[car_decrease]->vy)*(all_cars[car_decrease]->vy));
			
			// divide each component by the magnitude to fing unit vectors
			unit_x = (all_cars[car_decrease]->vx) / v_magnitude;
			unit_y = (all_cars[car_decrease]->vy) / v_magnitude;

			a_max_x = 0.5 * (v_opt_x[car_decrease]*v_opt_x[car_decrease] - (all_cars[car_decrease]->vx)*(all_cars[car_decrease]->vx)) / (ACCELERATION_DISTANCE * unit_x);
			a_max_y = 0.5 * (v_opt_y[car_decrease]*v_opt_y[car_decrease] - (all_cars[car_decrease]->vy)*(all_cars[car_decrease]->vy)) / (ACCELERATION_DISTANCE * unit_y);

			printf("Modifying decision of car %d\n", car_decrease);
			set_decision(all_cars[car_decrease], a_max_x, a_max_y, v_opt_x[car_decrease], v_opt_y[car_decrease]);
			new_vel_turn(all_cars[car_decrease]);
			new_t_exit(all_cars[car_decrease]);
			

			printf("Here...............................\n");


			if (is_collision(all_cars)) {		// Reduce one of them - separate function to select which one
				printf("COLLISION DETECTED...............................\n");
				v_opt_x[car_decrease] *= MINUS_FACTOR;
				v_opt_x[car_decrease] *= MINUS_FACTOR;

				all_cars[car_decrease]->compromise->vx1 *= MINUS_FACTOR;
				all_cars[car_decrease]->compromise->vy1 *= MINUS_FACTOR;

			}
			else {
				break;
			}	
			car_decrease = (car_decrease+1) % CAR_NUM;

			it++;
		

		}
*/



// Decrese options

// 1. Negaively bias 1 car
//	Keep decreasing its velocity until there are no collisions or until you reach the minimum. 
// 	In that case, try doing the same with another car.

// 2. Decrease multiple cars
// Decrease the first one. if collision, decrease second one. If collsion, decrease third one. 
// Continue decreasing without resetting velocities

// 3. Same as 2 but resetting velocities to maximum if you have decreased the three of them 
// and there are still colliding.

/*

void CA_make_decisions (struct collision * cols, struct car ** all_cars) {

	int num_col = num_collisions(cols);
	printf("Number of collisions is %d\n", num_collisions(cols));

	numbers dt, a_x, a_y;

	// NO collisions -> No compromises
	if (num_col == 0) {			
		int i;
		for (i = 0; i < CAR_NUM; i++) {
			set_decision(all_cars[i], 0, 0);
		}
	}
	
	// 1 or more collisions -> 2 cars compromise

	else {		
	
		//Two cars modify velocities:
		//first one stays the same, second one and third one slow down in order of arrival
		

	  	qsort(all_cars, CAR_NUM, sizeof(struct car *), comp_cars_t_entry);    // Sorts all_cars by increasing entry time

	  	set_decision(all_cars[0], 0, 0);			// a's decision stays the same

  		all_cars[1]->compromise->t_stop = all_cars[0]->t_exit;		// wait to enter until previous car has exited
  		dt = all_cars[1]->compromise->t_stop - all_cars[1]->compromise->t_start;
  		a_x = get_ax(all_cars[1], dt);
  		a_y = get_ay(all_cars[1], dt);
  		set_decision(all_cars[1], a_x, a_y);			// SLOW DOWN

  		//new_t_enter(all_cars[1]);
  		new_vel_turn(all_cars[1]);
  		new_t_exit(all_cars[1]);

  		set_decision(all_cars[2], 0, 0);


	}

}
*/
void CA_file_output(struct car * mycar, FILE *fp) {
	
	// timestamps:
	// t_start, t_stop, t_exit

	fprintf(fp,"%.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
		mycar->compromise->t_start,
		mycar->compromise->rx0,
		mycar->compromise->ry0,
		mycar->compromise->vx0,
		mycar->compromise->vy0,
		mycar->compromise->ax,
		mycar->compromise->ay);


	fprintf(fp,"%.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
		mycar->compromise->t_stop,
		mycar->compromise->rx1,
		mycar->compromise->ry1,
		mycar->vx_turn,
		mycar->vy_turn,
		(numbers)0,
		(numbers)0);	

	fprintf(fp,"%.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
		mycar->t_exit,
		mycar->rx_exit,
		mycar->ry_exit,
		rotation_matrix_x(mycar->vx_turn, mycar->vy_turn, mycar->intent),
		rotation_matrix_y(mycar->vx_turn, mycar->vy_turn, mycar->intent),
		(numbers)0,
		(numbers)0);	

}


/* Decision Unit: Gets called once for each car */

void DU (struct car ** all_cars, char * mycar) {

	// Create a collisions array
	struct collision cols [NUM_PAIRS]; 

	get_all_collisions_turn(all_cars, CAR_NUM, cols, 1);   // Collisions detected only inside the intersection
	print_cols(cols);									// Print collisions to terminal

	// Open file and write first event only for my car
	FILE * fptr;
	char filename [MAX_NAME_SIZE] = "inputs";		// Construct personalized filename
	strcat(filename, mycar);						// Concatenate car ID
	strcat(filename, ".txt");

	fptr = fopen(filename,"w");						// Open/create output file
	printf("Opening file: %s\n", filename);


	struct car * my_car = get_mycar(all_cars, mycar);	// Get a pointer to my car
		

	// Write initial conditions at t=0
	fprintf(fptr, "0 %.2f %.2f %.2f %.2f 0 0\n", my_car->rx, my_car->ry, my_car->vx, my_car->vy);

	// Costructor for decision structs
	new_decision(all_cars[0]);						// ** If I do this in all cars wont it be overwritten 3 times?
	new_decision(all_cars[1]);
	new_decision(all_cars[2]);

	//CA_make_decisions(cols, all_cars);	// Decision Unit
	optimizedFunction(all_cars);

/*
	print_decision(*all_cars[0]);
	print_decision(*all_cars[1]);
	print_decision(*all_cars[2]);
*/
	print_decision(*my_car);
	CA_file_output(my_car, fptr);	// Write output in the file

	fclose(fptr);		// Close output file
	

}
