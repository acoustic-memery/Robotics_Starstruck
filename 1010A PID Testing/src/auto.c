/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#define FOREWARD 1
#define BACKWARDS 2
#define RIGHT 1
#define LEFT 2
#define UP 1
#define DOWN 2
#define OPEN 1
#define CLOSED 2


void motorsForeward (float output) {
  motorSet(1, output);
  motorSet(4, -output);
  motorSet(7, output);
  motorSet(10, -output);
}

void motorsBackward (float output) {
  motorSet(1, -output);
  motorSet(4, output);
  motorSet(7, -output);
  motorSet(10, output);
}

void motorsRight () {
  motorSet(1, -127);
  motorSet(4, 127);
  motorSet(7, 127);
  motorSet(10, -127);
}

void motorsLeft () {
  motorSet(1, 127);
  motorSet(4, -127);
  motorSet(7, -127);
  motorSet(10, 127);
}

void motorsStop () {
  motorSet(1, 0);
  motorSet(4, 0);
  motorSet(7, 0);
  motorSet(10, 0);
}

void motorsArmUp () {
  motorSet(9, 127);
  motorSet(6, -127);
  motorSet(2, -127);
  motorSet(5, -127);
}

void motorsArmDown () {
  motorSet(9, -127);
  motorSet(6, 127);
  motorSet(2, 127);
  motorSet(5, 127);
}

void motorsArmStop () {
  motorSet(9, 0);
  motorSet(6, 0);
  motorSet(2, 0);
  motorSet(5, 0);
}

void motorsClawOpen () {
  motorSet(3, 127);
  motorSet(8, 127);
}

void motorsClawClosed () {
  motorSet(3, -127);
  motorSet(8, -127);
}

void motorsClawStop () {
  motorSet(3, 0);
  motorSet(8, 0);
}

void stopEverything() {
   motorsStop();
   motorsArmStop();
   motorsClawStop();
 }

 #define INTEGRAL 1
 #define DERIVATIVE 0.0


 void go(int way, int distance) {
   encoderReset(driveEncLeft);
   encoderReset(driveEncRight);

   float encTickLastLeft;
   float encTickLastRight; //tick last left and right


   float outputLeft;
   float outputRight; //power to be outputed left and right

   float errLeft;
   float errRight; //target distance - actual distance left and right

   float errTotalRight;
   float errTotalLeft; //total accumulated error left and right

   int errAccuLeft = 50;
   int errAccuRight = 50; //total accumulated error left and right if less than integral cap

   float gain = 0.25; //motor gain for proportional term

   float integCap = 50/INTEGRAL; //integral cap

   float propLeft;
   float propRight; //calculated proportional term left and right
   float integLeft;
   float integRight; //calculated integral term left and right
   float derivLeft;
   float derivRight; //calculated derivative term left and right

   int motorAdjust = 5; //motor adjustment amount



   while(((abs(encoderGet(driveEncLeft)) + (abs(encoderGet(driveEncRight)))) / 2) < distance) { //while less than target distance



      errLeft = distance - (abs(encoderGet(driveEncLeft)));
      errRight = distance - (abs(encoderGet(driveEncRight))); //calculate error

      if(errRight < errAccuRight) {errTotalRight += errRight;}
      else {errTotalRight = 0;}

      if(errLeft < errAccuLeft) {
        errTotalLeft += errLeft;}
      else {errTotalLeft = 0;} //makes integral term 0 if error is less than 50 (this has not been properly tuned)

      if(errTotalLeft > integCap) {
        errTotalLeft = 50/INTEGRAL;
      }

      if(errTotalRight > integCap) {
        errTotalRight = 50/INTEGRAL; //if integral term is greater than 50 (after previous calculation) integral term is incorperated
      }

//the above part of the function is only for managing when the integral is enganged

      propLeft = errLeft * gain;
      propRight = errRight * gain; //calculate proprtional term

      integLeft = errTotalLeft * INTEGRAL;
      integRight = errTotalRight * INTEGRAL; //calculate integral term

      derivLeft = (errLeft - encTickLastLeft) * DERIVATIVE;
      derivRight = (errRight - encTickLastRight) * DERIVATIVE; //calculate derivative term

encTickLastRight = errRight;
encTickLastLeft = errLeft; //calculate enctick last

outputLeft = propLeft + integLeft + derivLeft;
outputRight = propRight + integRight + derivRight; //output P + I + D for both sides

if (outputLeft > 127) {
  outputLeft = 127;
}

if (outputRight > 127) {
  outputRight = 127;
}

//make output 127 if greater than 127

if(way == 1) {
//if forewards

//adjusts motors to make both sides turn at approx the same speed

  if (outputLeft < (outputRight +- 20)) {
    motorSet(1, (outputLeft + motorAdjust));
    motorSet(4, -(outputLeft + motorAdjust)); // left side

    motorSet(7, outputRight);
    motorSet(10, -(outputRight)); //right side
  }
  if (outputRight < (outputLeft +- 20)) {
    motorSet(1, (outputLeft));
    motorSet(4, -(outputLeft)); // left side

    motorSet(7, (outputRight + motorAdjust));
    motorSet(10, -(outputRight + motorAdjust));

  }
  else {
    motorSet(1, outputLeft);
    motorSet(4, -(outputLeft));
    motorSet(7, (outputRight));
    motorSet(10, -(outputRight));
  }

}

if(way == 2) {
//backwards

//adjusts motors to keep both sides the same speed
  if (outputLeft < (outputRight +- 20)) {
    motorSet(1, -(outputLeft + motorAdjust));
    motorSet(4, (outputLeft + motorAdjust)); // left side

    motorSet(7, -(outputRight));
    motorSet(10, (outputRight)); //right side
  }
  if (outputRight < (outputLeft +- 20)) {
    motorSet(1, -(outputLeft));
    motorSet(4, (outputLeft)); // left side

    motorSet(7, -(outputRight + motorAdjust));
    motorSet(10, (outputRight + motorAdjust));

  }
  else {
    motorSet(1, -(outputLeft));
    motorSet(4, (outputLeft));
    motorSet(7, -(outputRight));
    motorSet(10, (outputRight));
  }


}


delay(20);

   }



 }



 void turnRight(int degree){
encoderReset(driveEncLeft);
encoderGet(driveEncLeft);
   while(abs(encoderGet(driveEncLeft)) < degree){
     encoderGet(driveEncLeft);
     motorsRight();
     delay(20);
   }
   encoderGet(driveEncLeft);
  if(abs(encoderGet(driveEncLeft)) >= degree) {
    motorsStop();
  }
 }

 void turnLeft(int degree2){
  encoderReset(driveEncLeft);
  encoderGet(driveEncLeft);
   while(abs(encoderGet(driveEncLeft)) < degree2){
     encoderGet(driveEncLeft);
     motorsLeft();
     delay(20);
   }
   encoderGet(driveEncLeft);
   if(abs(encoderGet(driveEncLeft)) >= degree2) {
     motorsStop();
   }
 }

void arm(int direction, int amount) {
  encoderReset(armEncoder);
  encoderGet(armEncoder);

  while(abs(encoderGet(armEncoder)) < amount) {
    encoderGet(armEncoder);
    if (direction == 1) {
      motorsArmUp();
    }
    else if (direction == 2) {
      motorsArmDown();
    }
    delay(20);
  }
  if (abs(encoderGet(armEncoder)) >= amount) {
    motorsArmStop();
    encoderReset(armEncoder);
  }

}

void claw(int direction, int amount) {
  encoderReset(clawEncoder);
  encoderGet(clawEncoder);

  while(abs(encoderGet(clawEncoder)) < amount) {
    encoderGet(clawEncoder);
    if (direction == 1){
      motorsClawOpen();
    }
    else  if (direction == 2){
      motorsClawClosed();
    }
    delay(20);
  }
  if (abs(encoderGet(clawEncoder)) >= amount) {
    encoderReset(clawEncoder);
    motorsClawStop();
  }
}

void attack(int drive, int arm, int claw){
  encoderReset(driveEncLeft);
  while(abs(encoderGet(driveEncLeft)) < drive){
    encoderGet(driveEncLeft);
    motorsBackward(127);
    if (abs(encoderGet(driveEncLeft)) < arm){
      motorsArmUp();
  }
    else if(abs(encoderGet(driveEncLeft)) < claw){
      motorsClawOpen();
    }
    delay(20);
  }
  if (abs(encoderGet(driveEncLeft)) >= drive) {
    encoderReset(driveEncLeft);
    stopEverything();

  }
}

void grab(int direction, int amount, int clawDelay) {
  encoderReset(driveEncLeft);
  while(abs(encoderGet(driveEncLeft)) < amount){
    go(direction, amount);
    if (abs(encoderGet(driveEncLeft)) >= clawDelay) {
      motorsClawClosed();
    }
    delay(20);
  }
  if(abs(encoderGet(driveEncLeft)) >= amount){
    encoderReset(driveEncLeft);
    motorsStop();
  }
}



void autonomous() {


}
