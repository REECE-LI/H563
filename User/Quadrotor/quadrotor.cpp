//
// Created by L2248 on 2025/7/31.
//

#include "quadrotor.hpp"

#include <bits/move.h>

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

IMU Quadrotor::getIMU() const { return *imu; }

const IMU::SensorDataFull& Quadrotor::getIMUData(bool xy_invert) noexcept {
  // 拿到“快照”（你的 IMU::getSensorData() 当前是按值返回，线程安全且简单）
  IMU::SensorDataFull raw = imu->getSensorData();

  // 基础复制
  sensorData = raw;

  // 可选 XY 互换（加速度/陀螺/磁力计）
  if (xy_invert) {
    std::swap(sensorData.acc.x,  sensorData.acc.y);
    std::swap(sensorData.gyro.x, sensorData.gyro.y);
    std::swap(sensorData.angle.x, sensorData.angle.y);
    std::swap(sensorData.mag[0], sensorData.mag[1]); // 若无mag就去掉这一行
  }

  return sensorData; // 返回只读引用，无拷贝、无需判空
}