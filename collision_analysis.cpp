#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "classcpp.h"

#define COS45 0.707107
#define SIN45 0.707107
#define RADIUS 2 

using namespace std;

void print_collisions(vector<Collision> list){
	ofstream result;
	result.open("collision_result.txt");
	for(int i = 0; i<list.size();i++){
		result << "ID: " << list[i].ID << endl;
		result << "Car a: " << list[i].first->ID << endl;
		result << "Car b: " << list[i].second->ID << endl;
		result << "Collided at time: " << list[i].time << endl;
	}
}

numbers CoM_turn( Car one, Car two) {

  numbers distance = RADIUS;

  numbers a = one.XE_info.position.first - two.XE_info.position.first;
  numbers b = one.XE_info.position.second - two.XE_info.position.second;
  numbers c = one.XE_info.velocity.first - two.XE_info.velocity.first;
  numbers d = one.XE_info.velocity.second - two.XE_info.velocity.second;

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

void advance_pos(Car * c, numbers dt) {
  // x = x0 + v * dt
  c->XE_info.position.first = c->XE_info.position.first + (c->XE_info.velocity.first * dt);
  c->XE_info.position.second = c->XE_info.position.second + (c->XE_info.velocity.second * dt);
}

int v2v_collision(Car * first, Car * second, vector<Collision> col){
  numbers dt;
  numbers rx_enter_old, ry_enter_old;

  dt = (second->XE_info.time) - (first->XE_info.time);
  rx_enter_old = first-> XE_info.position.first;
  ry_enter_old = first-> XE_info.position.second;
  advance_pos(first, dt);
  numbers t = CoM_turn(*first, *second);

  // Restore values that were previously saved
  first->XE_info.position.first = rx_enter_old;
  first->XE_info.position.second = ry_enter_old;

  if (t > 0 ){
    t += second->XE_info.time;

  /*  if ((t < first->FI_info.time) && (t < second->FI_info.time)) {  // Valid collision inside the intersection
      Collision a(first, second, t);
      col.push_back(a);
      return 1;
    }
  }
  Collision b(first, second, 0);
  col.push_back(b);
*/
    // problem above: where is FI_info calculated? it can be uninitialized and cause seg fault
  return 0;
}

int main(){

  return 0;
}
