#include "main/Vehicle.hpp"
#include <iostream>

using namespace veh;

Vehicle::Vehicle() {
	speed = steering = direction = 0;
	mass = 1910; wheels = 4; energy = 100;
}
Vehicle::Vehicle(double m, double w) {
	speed = steering = direction = 0;
	mass = m; wheels = w; energy = 100;
}
void Vehicle::accelerate() {
	speed++;
	std::cout << "My speed is now " << speed << std::endl;
}