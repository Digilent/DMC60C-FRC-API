/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>
#include <digilent/dmc60/WPI_DMC60C.h>
#include <frc/Joystick.h>

//Create two DMC60C Objects with Device number 2 and 5.
//Since this is open loop, we don't need wheel and gearbox measurements.
DMC60::WPI_DMC60C * _dmc = new DMC60::WPI_DMC60C(2);
DMC60::WPI_DMC60C * _dmc2 = new DMC60::WPI_DMC60C(5);

frc::Joystick * _joy = new frc::Joystick(0);

void Robot::RobotInit() {
  //Invert second motor
  _dmc2->SetInverted(true);
  
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
  double Ltrigger = -100*_joy->GetRawAxis(2);//0 to 100
  double Rtrigger = 100*_joy->GetRawAxis(3);//0 to -100
  double stickL = _joy->GetRawAxis(0); //Left stick -1 left to 1 right

  double throttle = Ltrigger+Rtrigger;

  if (stickL>0){//Right turn (left motor on, right motor less on)
    _dmc->driveVoltage(throttle);

    //Right motor is scaled based on how far the left stick is turned right.
    //If the left stick is full right, stickL=1, so the motor will be
    //throttle-throttle=0.
    _dmc2->driveVoltage(throttle - throttle * stickL);
  }
  else{//Left or straight (right motor on, left motor less on)

    //Left motor is scaled based on how far the left stick is turned left.
    //If it is all the way left, stickL=-1, so the motor will be
    //throttle + (throttle * -1) = 0.
    _dmc->driveVoltage(throttle + throttle * stickL);
    _dmc2->driveVoltage(throttle);
  }

}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
