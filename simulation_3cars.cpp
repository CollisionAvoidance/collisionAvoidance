#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <stdlib.h>  

//#include "algorithm.h" // importing algorithm as a header file.
// algorithm.h should only contain the definition of car algorithm.
using namespace std;

// below are declarations of global variables for analysis.
double global_time = 0; // initializing total traffic time for simulation
float standard_width = 10;
double car_radius = 10; // standard
// below are classes for testing cases.
int Case_count = 0 ;
int Car_total = 0;

class CarInfo{
public: 
	//static int Car_total;
	int ID;
	pair<double,double> position,velocity,acceleration;
	double radius = car_radius;
	double toa; // time of arrival
	int direction;
	int intention;
	CarInfo(){
		Car_total += 1;
		ID = Car_total;	
	}
	CarInfo(int id){
		Car_total += 1;
		ID = Car_total;
	}
};

class Car{
public: 
	CarInfo info; // CarInfo stored in a case.
	Car(){

	}
	Car(CarInfo a){
		Car_total++; 
		info.ID = Car_total;
		info.position = a.position;
		info.velocity = a.velocity;
		info.acceleration = a.acceleration;
		info.radius = a.radius;
		info.toa = a.toa; // time of arrival
	    info.direction = a.direction;
	    info.intention = a.intention;
	}

	void algorithm(CarInfo car_a,CarInfo car_b, CarInfo car_c){ // needs information from other cars too.
		cout << "tehehhehehehhehe i am mad" << endl; 
	}
	void Output_generate(){
		for (double i = 0; i <= global_time; i += 0.5){
			//this-> algorithm(); // algorithm supposedly updated velocity
			this-> info.position.first += this->info.velocity.first * 0.5; // time_step = 0.5
			this-> info.position.second += this->info.velocity.second * 0.5;
			ofstream myfile;
			myfile.open("simulation_result.txt");
			myfile << "car " << this->info.ID << "position x" << this->info.position.first << "position y" << this->info.position.second << "velocity x" << this->info.velocity.first << "velocity y" << this->info.velocity.second << "time " << i;
			myfile.close();
		}
	}
};

class Case{
public:
	//static int Case_count;
	int ID;
	int inte_a; // intention of car a.
	int inte_b;
	int inte_c;
	Case(){
		Case_count++;
		ID = Case_count;
		inte_a = rand()%3;
		inte_b = rand()%3;
		inte_c = rand()%3;
		Car a; // should be stored on heap, change later
		Car b;
		Car c;
		a.algorithm(a.info, b.info, c.info);
		b.algorithm(b.info, a.info, b.info);
		c.algorithm(c.info, b.info, a.info);
		a.Output_generate();
		b.Output_generate();
		c.Output_generate();
	}
};
class Intersection{
public:
	//int ID;
	//int CaseID;
	//int width; // the width of road in units of centimeters.
	Intersection(){
		// ID = id;
		// width = standard_width;
		// CaseID = Caseid;
		Case();
	}
};

Case case_generator(int factor1, int factor2, int factor3){
	int width = standard_width * factor3;
	Intersection();
}

/*
Case case_generator(vector<double> randomness_factor){

}*/

int main(){
	cout<< "Please input the randomness factor for the case generator." << '\n';
	double factor1,factor2,factor3; // made up for later edits
	cin >> factor1 >> factor2 >> factor3;
	vector<double> randomness_factor;
	randomness_factor.push_back(factor1);
	randomness_factor.push_back(factor2);
	randomness_factor.push_back(factor3);
	//case_generator(randomness_factor);
	//case_output();

}