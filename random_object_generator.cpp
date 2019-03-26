#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <random>
#include "classcpp.h"

// this file contains all class constructors that randomly assign values for simulation purposes.


CarInfo::CarInfo(){
	ID_track++;
	ID = ID_track;
	std::pair<numbers,numbers>position,velocity,acceleration;
	this-> time = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0));
	position.first = 
	position.second = 
	velocity.first = 
	velocity.second =
	acceleration.first =
	acceleration.second =

}

Car::Car(){
	ID_track++;
	ID = ID_track;
}