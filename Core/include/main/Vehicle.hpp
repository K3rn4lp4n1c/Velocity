#pragma once

namespace veh {
	class Vehicle {
		double speed, steering, direction, mass, wheels, energy;
	public:
		Vehicle();
		Vehicle(double, double);
		void accelerate();
	private:
		struct limits
		{
			double energy[2]{0, 100};
			double direction[2]{0, 360};
			double speed[2]{ 0, 299999999 };
		};
		void resistance();
		void unit_conversion();
	};
}