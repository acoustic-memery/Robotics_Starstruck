/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
*/
void operatorControl() {
	int Y1 = 0;
	int A1 = 0;
	int deadZone = 40;
// deadzones for first controller, left stick

	while (1) {
		int leftStickVertical = joystickGetAnalog(1, 3); //joystick command for vertical movement
		int rightStickHorizontal = joystickGetAnalog(1, 1); //joystick command for turning left/right

		if (abs (leftStickVertical) > deadZone)
			Y1 = leftStickVertical;
			else
			Y1 = 0;

		if (abs (rightStickHorizontal) > deadZone)
			A1 = rightStickHorizontal;
			else
			A1 = 0;

			motorSet(10, (Y1 - A1)); //back right wheel
		motorSet(1, (-Y1 - A1)); //back left wheel
			motorSet(2, (Y1 + A1)); // front left wheel
			motorSet(3, (-Y1 + A1)); //front right wheel
			//Vertical Control



			if (joystickGetDigital(1, 5, JOY_DOWN)){ //for lift operation
			motorSet(5, -127); //bottom, left hand motor (front view)
			motorSet(7, 127); //top, left hand motor (front view)
			motorSet(4, -127); //bottom, right hand motor (front view)
			motorSet(6, 127); //top, right hand motor (front view)
		}
				else if(joystickGetDigital(1, 5, JOY_UP)){
					motorSet(5, 127); //bottom, left hand motor (front view)
					motorSet(7, -127); //top, left hand motor (front view)
		motorSet(4, 127); //bottom, right hand motor (front view)
					motorSet(6, -127); //top, right hand motor (front view)
				}
				else{
					motorSet(5, 0); //bottom, left hand motor (front view)
					motorSet(7, 0); //top, left hand motor (front view)
				motorSet(4, 0); //bottom, right hand motor (front view)
					motorSet(6, 0); //top, right hand motor (front view)
				}


//next section is for claw operation by the partner controller
if (joystickGetDigital(2, 6, JOY_UP)){
	motorSet(9, 127); //left claw motor (front view)
	motorSet(8, -127); // right claw motor (front view)
}
else if (joystickGetDigital(2, 6, JOY_DOWN)){
	motorSet(9, -127);
	motorSet(8, 127);
}
else {
	motorSet(9, 0);
	motorSet(8, 0);
}

		delay(20);
	}
}
