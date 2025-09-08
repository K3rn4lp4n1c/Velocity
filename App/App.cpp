#include <iostream>
#include "vehicles/IVehicle.hpp"
#include "main/Vehicle.hpp"

int main() {
    std::cout << "VehicleSim running.\n";
    Vehicle car;
    car.accelerate();
    return 0;
}
