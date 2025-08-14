//
// Created by L2248 on 2025/7/31.
//

#include "quadrotor.hpp"

IMU Quadrotor::getIMU() const { return *imu; }

void Quadrotor::motorUpdate(bool _useMotion)
{
  if (_useMotion)
  {
    firstMotor->motionPlannerUpdate();
    secondMotor->motionPlannerUpdate();
    thirdMotor->motionPlannerUpdate();
    forthMotor->motionPlannerUpdate();

    firstMotorThrottle = firstMotor->getMotionPlannerThrottle();
    secondMotorThrottle = secondMotor->getMotionPlannerThrottle();
    thirdMotorThrottle = thirdMotor->getMotionPlannerThrottle();
    forthMotorThrottle = forthMotor->getMotionPlannerThrottle();
  }
  else
  {
    firstMotorThrottle = firstMotor->getTargetThrottle();
    secondMotorThrottle = secondMotor->getTargetThrottle();
    thirdMotorThrottle = thirdMotor->getTargetThrottle();
    forthMotorThrottle = forthMotor->getTargetThrottle();
  }
}

uint16_t Quadrotor::getFirstMotorThrottle() { return firstMotorThrottle; }
uint16_t Quadrotor::getSecondMotorThrottle() { return secondMotorThrottle; }
uint16_t Quadrotor::getThirdMotorThrottle() { return thirdMotorThrottle; }
uint16_t Quadrotor::getForthMotorThrottle() { return forthMotorThrottle; }

void Quadrotor::setMotorTargetThrottle(uint16_t _throttle1, uint16_t _throttle2, uint16_t _throttle3,
                                       uint16_t _throttle4)
{
  firstMotor->setTargetThrottle(_throttle1);
  secondMotor->setTargetThrottle(_throttle2);
  thirdMotor->setTargetThrottle(_throttle3);
  forthMotor->setTargetThrottle(_throttle4);
}

void Quadrotor::setMotorThrottle(uint16_t _throttle1, uint16_t _throttle2, uint16_t _throttle3, uint16_t _throttle4)
{
  firstMotor->send(_throttle1, 0);
  secondMotor->send(_throttle2, 0);
  thirdMotor->send(_throttle3, 0);
  forthMotor->send(_throttle4, 0);
}

void Quadrotor::motorInit()
{
  setMotorThrottle(0, 0, 0, 0);
  firstMotor->transferEnable(true);
  secondMotor->transferEnable(true);
  thirdMotor->transferEnable(true);
  forthMotor->transferEnable(true);
}