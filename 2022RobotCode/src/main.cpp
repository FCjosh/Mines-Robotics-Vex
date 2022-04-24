#include "main.h"
#include "okapi/api.hpp"
using namespace okapi;
void opcontrol() {
	std::shared_ptr<ChassisController> drive =
		ChassisControllerBuilder()
			.withMotors(1, -2, 3, 4) // topleft motor is 1, top right motor is 2 (reversed), bottom right is 3, bottom left is 4
			.withGains(
				{0.001, 0, 0.0001}, // distance controller gains
				{0.001, 0, 0.0001}, // turn controller gains
				{0.001, 0, 0.0001}  // angle controller gains (helps drive straight)
			)
			.withSensors(
				ADIEncoder{'A', 'B'}, // left encoder in ADI ports A & B
				ADIEncoder{'C', 'D', true},  // right encoder in ADI ports C & D (reversed)
				ADIEncoder{'E', 'F'}  // middle encoder in ADI ports E & F
			)
			// green gearset, tracking wheel diameter (2.75 in), center to center distance between wheels (track) (7 in), and TPR (360)
			// 1 inch middle encoder distance from center of rotation, and 2.75 inch middle wheel diameter
			.withDimensions(AbstractMotor::gearset::green, {{2.75_in, 7_in, 1_in, 2.75_in}, quadEncoderTPR})
			.withOdometry() // use the same scales as the chassis (above)
			.buildOdometry(); // build an odometry chassis

	while (true) {
		drive->moveDistance(12_in);
		drive->turnAngle(90_deg);
	}
}
