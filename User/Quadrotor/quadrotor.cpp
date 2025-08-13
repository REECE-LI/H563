//
// Created by L2248 on 2025/7/31.
//

#include "quadrotor.hpp"

IMU Quadrotor::getIMU() const { return *imu; }

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