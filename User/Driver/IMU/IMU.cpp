//
// Created by L2248 on 2025/7/31.
//

#include "IMU.hpp"

#include "app_freertos.h"
#include "cmsis_os2.h"

IMU::SensorDataFull IMU::getSensorData() const
{
  if (osMutexAcquire(IMU_MutexHandle, osWaitForever) == osOK)
  {
    auto copy = sensorData;
    osMutexRelease(IMU_MutexHandle);          // 一定要先释放
    return copy;
  }
  // 如果获取锁失败，这里也要有返回值，视需求可以抛异常或返回默认构造
  return {};
}
