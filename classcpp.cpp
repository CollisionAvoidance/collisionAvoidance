#include <iostream>
#include "classcpp.h"
int CarInfo::id_track = 0;
CarInfo::CarInfo(){
	id_track++;
	ID = id_track;
}
CarInfo::CarInfo(int id){
	ID = id;
}

/*
Car::Car(std::string a)
{
		this->In_info.ID = a;
}

Car::Car(CarInfo a)
{ 
		this->IN_info.ID = a.ID;
		this->IN_info.position = a.position;
		this->IN_info.velocity = a.velocity;
		this->IN_info.acceleration = a.acceleration;
	    this->IN_info.direction = a.direction;
	    this->IN_info.intention = a.intention;
}

//void Car::algorithm(CarInfo car_a,CarInfo car_b, CarInfo car_c){
	// from another file. this is template placeholder
//};

Case::Case(){
		this->inte_a = rand()%3;
		this->inte_b = rand()%3;
		this->inte_c = rand()%3;
		Car a("1"); // should be stored on heap, change later
		Car b("2");
		Car c("3");
		// a.algorithm(a.info, b.info, c.info); // placeholder
		// b.algorithm(b.info, a.info, b.info); // placeholder
		// c.algorithm(c.info, b.info, a.info); // placeholder
}

Case::Case(std::string id){
		this->ID = id;
		this->inte_a = rand()%3;
		this->inte_b = rand()%3;
		this->inte_c = rand()%3;
		Car a("1"); // should be stored on heap, change later
		Car b("2");
		Car c("3");
		// a.algorithm(a.info, b.info, c.info); // placeholder
		// b.algorithm(b.info, a.info, b.info); // placeholder
		// c.algorithm(c.info, b.info, a.info); // placeholder
}


Decision::Decision(std::string id){
	this->ID = id;
}

Intersection::Intersection(std::string id){
	this->ID = id;
}
void Output_generate(float global_time);
		for (double i = 0; i <= global_time; i += 0.5){
			//this-> algorithm(); // algorithm supposedly updated velocity
			this-> info.position.first += this->info.velocity.first * 0.5; // time_step = 0.5
			this-> info.position.second += this->info.velocity.second * 0.5;
			ofstream myfile;
			myfile.open("simulation_result.txt");
			myfile << "car " << this->info.ID << "position x" << this->info.position.first << "position y" << this->info.position.second << "velocity x" << this->info.velocity.first << "velocity y" << this->info.velocity.second << "time " << i;
			myfile.close();	*/	
int main(){
	CarInfo a;
	std::cout << a.ID; // << endl;
	return 0;
}	
