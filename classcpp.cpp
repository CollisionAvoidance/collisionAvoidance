#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <random>
#include "classcpp.h"
int CarInfo::ID_track = 0;
double Decision::ID_track = 0;
int Car::ID_track = 0;
int Collision::ID_track = 0;
int Intersection::ID_track = 0;

CarInfo::CarInfo(){
	ID_track++;
	ID = ID_track;
}
CarInfo::CarInfo(int id){
	ID = id;
}

Car::Car(CarInfo a)
{ 
		ID_track++;
		ID = ID_track;
		IN_info.position = a.position;
		IN_info.velocity = a.velocity;
		IN_info.acceleration = a.acceleration;
		IN_info.time = a.time;
}

//void Car::algorithm(CarInfo car_a,CarInfo car_b, CarInfo car_c){
	// from another file. this is template placeholder
//};

//Case::Case(){
		// placeholder for now
		//inte_a = rand()%3;
		//this->inte_b = rand()%3;
		//this->inte_c = rand()%3;
		//Car a("1"); // should be stored on heap, change later
		//Car b("2");
		//Car c("3");
//}

Collision::Collision(Car *first,Car *second,numbers time){
	ID_track++;
	ID = ID_track;
	this->first = first;
	this->second = second;
	this->time = time;
}



Decision::Decision(){
	ID_track++;
	ID = ID_track;
}

Intersection::Intersection(){
	ID_track++;
	ID = ID_track;
	this->width = ROAD_WIDTH; // road width should at least have 2 lanes

}

void Intersection::Generate_Cars(int count){

	std::default_random_engine generator;
    std::normal_distribution<double> velocity_distribution(SPEED_LIMIT,VELOCITY_DEVIATION);
    std::normal_distribution<double> position_distribution(this->width/4*3,this->width/4*3*POSITION_DEVIATION);
	for (int i= 0; i < count; i++){
		numbers x_velocity = velocity_distribution(generator);
		numbers x_position = position_distribution(generator);
		// y_position is marked by toa. no need to put distribution.
		CarInfo a;
		a.velocity.first =  x_velocity; // x is normal
		double rand_a = rand()%101*0.01*1;
		int rand_b = rand()%2;
		if (rand_b == 0) rand_b = -1;
		else rand_b = 1;
		a.velocity.second = SPEED_LIMIT + (5)*rand_a*rand_b;// y is uniform
		a.position.first = x_position;
		a.time = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0));
		std::cout << "a.time is "<< a.time << std::endl; 
		// a.toa put another distribution
		Car b(a);
		this->Car_queue.push_back(b);
	}

}
void Intersection::Congestion_Analysis(){
	numbers ini_time = 100;
	numbers end_time = 0;
	for (int i = 0; i< Car_queue.size(); i++){
		if(this->Car_queue[i].IN_info.time < ini_time){
			ini_time = this->Car_queue[i].IN_info.time;
		}
		// else : pass
		/*if (this->Car_queue[i].FI_info.time > end_time){
			end_time = this->Car_queue[i].FI_info.time;
		}*/
		// above: placeholder
	}
	std::cout << "first car entered at: " << ini_time << " second." << std::endl;
	std::cout << "last car exited at: " << end_time << " second." << std::endl;
	std::cout << "total time used: " << end_time - ini_time << " second." << std::endl;c
	
}
int main(){
	std::cout << "hello"<< '\n';
	Intersection a;
	a.Generate_Cars(50);
	for (int i=0; i<50; i++){
		std::cout << "car "<< a.Car_queue[i].ID << " x position is " << a.Car_queue[i].IN_info.position.first << std::endl;
	}
	a.Congestion_Analysis();
	// need random generator to work properly.
	return 0;
}

