//
// Created by L2248 on 2025/7/31.
//

#ifndef IMU_HPP
#define IMU_HPP
#include <sys/_stdint.h>

class IMU
{
public:
  // 三维向量——可重用
  struct Vector3f
  {
    float x, y, z;
  };

  // 原始 9+3 数据的封装
  struct SensorDataFull
  {
    Vector3f acc;   // 加速度 (g)
    Vector3f gyro;  // 角速度 (°/s)
    Vector3f angle; // 姿态角 (°)
    int16_t mag[3]; // 磁场 (原始值)
  };

  IMU() = default;
  ~IMU() = default;

  SensorDataFull getSensorData() const;


protected:
  SensorDataFull sensorData{};
};

#endif // IMU_HPP
