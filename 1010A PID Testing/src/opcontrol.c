#include "main.h"
#define LEFT_SIDE_OUT = 2;
#define LEFT_SIDE_IN = 1;
#define RIGHT_SIDE_OUT = 3;
#define RIGHT_SIDE_IN = 10;

void armLeftSide(int power) {
	motorSet(9, power);
	motorSet(6, -power);
}

void armRightSide(int power) {
	motorSet(2, -power);
	motorSet(5, -power);
}

void armPid(void * perameter) {
	encoderReset(armEncoder);
	int error;
	int setPoint = 100;
	float gain = 0.5;
	while(1) {
		error = setPoint - abs(encoderGet(armEncoder));
		error = error * gain;
		if (joystickGetDigital(1, 6, JOY_UP)) {
			armLeftSide(error);
			armRightSide(error);
		}
		else if (joystickGetDigital(1, 5, JOY_UP)) {
			armLeftSide(127);
			armRightSide(127);
		}
		else if (joystickGetDigital(1, 5, JOY_DOWN)) {
			armLeftSide(-127);
			armRightSide(-127);
		}
		else {
			armLeftSide(0);
			armRightSide(0);
		}


 	/// reset lift encoder

if (joystickGetDigital(1, 8, JOY_UP)){
	encoderReset(armEncoder);
}

}}


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
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {
	while (1) {
		int Y1 = 0;
		int X1 = 0;

		int deadZone = 20;
	// deadzones for first controller, left stick

	while (1) {



		TaskHandle armTaskHandle = taskCreate(armPid, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

		int leftStickVertical = joystickGetAnalog(1, 3); //joystick command for vertical movement
		int leftStickHorizontal = joystickGetAnalog(1, 1); //joystick command for horizontal movement


		if (abs (leftStickVertical) > deadZone) {
			Y1 = leftStickVertical; }
			else {
			Y1 = 0;
}
		if (abs (leftStickHorizontal) > deadZone) {
			X1 = leftStickHorizontal; }
			else {
			X1 = 0;
}

		 motorSet(1, Y1 + X1);
		motorSet(4, -Y1 - X1);

		motorSet(7, Y1 - X1);
		motorSet(10, -Y1 + X1);


//next section is for claw operation by the partner controller
if (joystickGetDigital(2, 6, JOY_UP)){
	motorSet(3, -127); //left claw motor (front view)
	motorSet(8, -127); // right claw motor (front view)
}
else if (joystickGetDigital(2, 6, JOY_DOWN)){
	motorSet(3, 127);
	motorSet(8, 127);
}
else if (joystickGetDigital(2, 5, JOY_UP)){
	motorSet(3, -40); //left claw motor (front view)
	motorSet(8, -40); // right claw motor (front view)
}
else if (joystickGetDigital(2, 5, JOY_DOWN)){
	motorSet(3, 40);
	motorSet(8, 40);
}

else {
	motorSet(3, 0);
	motorSet(8, 0);
}





delay(20);
}}}
