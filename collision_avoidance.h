/*
*   EC463 Senior Design Project
*   Group 6: Collision Avoidance
*   collision_avoidance.c
*   Basic predictive unit - No stopping human DU
*/


struct collision * get_first_collision(struct collision * cols);

numbers get_acceleration(struct car * c, numbers t);

void new_decision (struct car * car);

void set_decision(struct car * car, numbers ax, numbers ay, numbers vx, numbers vy);

void print_decision(struct car c);

void CA_make_decisions (struct collision * cols, struct car ** all_cars);

void DU (struct car ** all_cars, char * mycar);


/*
Input: 3 cars as car structs
Output: 3 decision structs, one for each car

Get collisions

1. If there are no collisions (NO cars involved in collision):
	No car modifies velocity

2. if there is only one collision (2 cars involved in collision):
	
	One car modifies velocity:

	Select first collision (Let's say it's cars A & B)

	If C exits the intersection before the collision happens: 	// car C->t_exit < t_collision_AB
		C_decision stays the same
		If A enters the intersection before B:
			Slow down B until B_enter = A_exit
		Else, B enters before A or they enter at the same time:
			Slow down A until A_enter = B_exit

	else if C enters after the collision happen:
		C_decision stays the same
		If A enters the intersection before B:
			Speed up A
		Else, B enters before A or they enter at the same time:
			Speed up B
		

3. Else (3 cars involved in collision):
	Two cars modify velocities:
		first one stays the same, second one and third one slow down in order of arrival
*/