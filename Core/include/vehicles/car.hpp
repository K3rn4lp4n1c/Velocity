#pragma once
#include <string>
#include <chrono>

class Car {
private:
    std::string company;
    std::string model;
    int year;
    double mileage;   // km per liter
    double fuelTank;  // total fuel capacity (liters)
    double fuel;      // current fuel (liters)

public:
    // Constructor
    Car(const std::string& comp, const std::string& mdl, int yr, double mil, double tank)
        : company(comp), model(mdl), year(yr), mileage(mil), fuelTank(tank), fuel(tank) {
    }

    // Getters
    std::string getCompany() { return company; }
    std::string getModel() { return model; }
    int getYear() { return year; }
    double getMileage() { return mileage; }
    double getFuel() { return fuel; }

    // Refuel
    void refuel(double amount) {
        fuel += amount;
        if (fuel > fuelTank) fuel = fuelTank;
    }

    // Calculate fuel needed for a distanc
    double fuelNeeded(double distance) {
        return distance / mileage; // distance divided by mileage
    }

    // Try to drive a distance
    bool drive(double distance) {
        double needed = fuelNeeded(distance);
        if (fuel >= needed) {
            fuel -= needed;
            return true; // Trip successful
        }
        else {
            return false; // Not enough fuel
        }
    }
};