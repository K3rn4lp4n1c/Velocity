#pragma once
#include <string>
#include <chrono>

namespace veh {
    struct VehicleState {
        double speed_mps{ 0.0 };
        double energy_pct{ 100.0 };
    };
    class IVehicle {
    public:
        virtual ~IVehicle() = default;
        virtual void accelerate(double delta_mps) = 0;
        virtual void brake(double delta_mps) = 0;
        virtual void update(std::chrono::duration<double> dt) = 0;
        virtual void fillEnergy(double amount) = 0;
        virtual const VehicleState& state() const noexcept = 0;
        virtual std::string name() const = 0;
    };
} // namespace veh
