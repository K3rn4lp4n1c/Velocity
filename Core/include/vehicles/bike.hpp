#pragma once
#include <string>
#include <chrono>
#include "../main/Vehicle.hpp"

class Bike {
private:
    std::string company;
    std::string model;
    int year;
    double mileage;   // km per liter
    double fuelTank;  // total fuel capacity (liters)
    double fuel;      // current fuel (liters)
    int gears;        // number of gears
    double speed;     // current speed (km/h)

public:
    // Constructor
    Bike(const std::string& comp, const std::string& mdl, int yr, double mil, double tank, int g)
        : company(comp), model(mdl), year(yr), mileage(mil),
        fuelTank(tank), fuel(tank), gears(g), speed(0) {
    }

    // Getters
    std::string getCompany() { return company; }
    std::string getModel() { return model; }
    int getYear() { return year; }
    double getMileage() { return mileage; }
    double getFuel() { return fuel; }
    int getGears() { return gears; }
    double getSpeed() { return speed; }

    // Refuel
    void refuel(double amount) {
        fuel += amount;
        if (fuel > fuelTank) fuel = fuelTank;
    }

    // Fuel needed for a distance
    double fuelNeeded(double distance) {
        return distance / mileage;
    }

    // Ride the bike
    bool ride(double distance, double avgSpeed) {
        double needed = fuelNeeded(distance);
        if (fuel >= needed) {
            fuel -= needed;
            speed = avgSpeed;
            return true; // Trip successful
        }
        else {
            return false; // Not enough fuel
        }
    }

    // Fun feature: try popping a wheelie
    bool popWheelie() {
        if (speed >= 20) {  // needs at least 20 km/h
            return true;
        }
        return false;
    }
};

