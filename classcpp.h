#include <string>
/* variables defined but didn't used

*/
#define NUM_PAIRS 3 // 3 choose 2 (#cars choose 2)
#define CAR_NUM 3

typedef double numbers; // change to either double or float depending on the situation

enum Direction{straight,left,right}; 

// t_stop : the time when you reach execution velocity from initial velocity

class CarInfo{
public: 
	static int ID_track;
	int ID;
	std::pair<numbers,numbers>position,velocity,acceleration;
	numbers time; // the momoent the car enters the corresponding state
	CarInfo();
	CarInfo(int id);

};

class Decision {
public:
	static double ID_track;
	double ID;
	numbers vx0, vy0, vx1, vy1; 				    // Velocities before and after accelerating
	numbers ax, ay;							        // Acceleration value. 0 initially
	numbers t_start, t_stop, new_t_exit;		// times when it starts and stops accelerating
	numbers rx0, ry0, rx1, ry1;				     // Points in the trajectory where they start and stop accelerating.
	Decision();
};

class Car{
public: 
	static int ID_track;
	int ID;
	CarInfo IN_info; 
	CarInfo DM_info; 
	CarInfo XE_info;
	CarInfo FI_info;
	numbers toa; // time of arrival
	Decision decision;
	//Decision compromise[];
	Direction dir;
	Direction intent;
	Car(CarInfo a);
	void algorithm(Car self, Car a, Car b);
};


class Collision{
public:
   static int ID_track;
   int ID;
   numbers time;
   Car * first;
   Car * second;
   Collision(Car *first,Car *second,numbers time);
};
/*
class Case{
public:
	//static int Case_count;
	std::string ID;
	Direction inte_a; // intention of car a.
	Direction inte_b;
	Direction inte_c;
	Case(std::string ID);
};

class Intersection{
public:
	std::string ID;
	Intersection(std::string id);
	//int CaseID;
	//int width; // the width of road in units of centimeters.
};*/
// the class intersection can be different depending on the algorithm as an algorithm based on machine learning can have intesections stored in it as a training set.