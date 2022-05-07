//Upload from file directory with normal terminal using 'prosv5 mu --slot n', the slots are not working for me yet
//Look into this https://github.com/purduesigbots/JINX for realtime data vis. This could help with tuning PIDs a lot.
//TODO: Wireless uploads, general sensor testing, experiment with multitasking, setting up vision, monitor batteries and currents
#include <string>
#include "main.h"
#include "okapi/api.hpp"
using namespace okapi;
void opcontrol() {
	auto chassis =
		ChassisControllerBuilder()
			.withMotors(18, -20, -19, 17) // topleft motor is 18, top right motor is 20 (reversed), bottom right is 19, bottom left is 17
			.withGains(
				{0.002, 0.00001, 0.00001}, // distance controller gains
				{0.002, 0.00001, 0.00001}, // turn controller gains
				{0.002, 0.00001, 0.00001}  // angle controller gains (helps drive straight)
			)
			.withSensors(
				ADIEncoder{'C', 'D'}, // left encoder in ADI ports A & B
				ADIEncoder{'A', 'B', true},  // right encoder in ADI ports C & D (reversed)
				ADIEncoder{'E', 'F'}  // middle encoder in ADI ports E & F
			)
			// green gearset, tracking wheel diameter (2.75 in), center to center distance between wheels (track) (7 in), and TPR (360)
			// 1 inch middle encoder distance from center of rotation, and 2.75 inch middle wheel diameter
			.withDimensions(AbstractMotor::gearset::green, {{2.75_in, 17.25_in, 8.25_in, 2.75_in}, quadEncoderTPR})
			.withOdometry() // use the same scales as the chassis (above)
			.buildOdometry(); // build an odometry chassis

	Controller masterController;

	auto xModel = std::dynamic_pointer_cast<XDriveModel>(chassis->getModel());

	while (true) {
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 0, ("X: " + std::to_string(chassis->getOdometry()->getState().x.getValue())).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 1, ("Y: " + std::to_string(chassis->getOdometry()->getState().y.getValue())).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 2, ("Theta: " + std::to_string(chassis->getOdometry()->getState().theta.getValue())).c_str());
		xModel->xArcade(
			masterController.getAnalog(ControllerAnalog::leftX),
			masterController.getAnalog(ControllerAnalog::leftY),
			masterController.getAnalog(ControllerAnalog::rightX)
		);

		if (masterController.getDigital(ControllerDigital::A))
		{
			chassis->setState(OdomState{0_m, 0_m, 0_deg});
		}
		
		pros::delay(20);
	}
	//drive->moveDistance(12_in);
	//drive->turnAngle(90_deg);
}
