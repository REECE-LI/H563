//
// Created by L2248 on 2025/7/31.
//

#ifndef QUADROTOR_HPP
#define QUADROTOR_HPP

#include "Driver/DSHOT/dshot.hpp"
#include "Driver/IMU/IMU.hpp"

class Quadrotor
{
public:
  explicit Quadrotor(IMU *_imu, Dshot *_firstMotor, Dshot *_secondMotor, Dshot *_thirdMotor, Dshot *_forthMotor)
      : imu(_imu), firstMotor(_firstMotor), secondMotor(_secondMotor), thirdMotor(_thirdMotor), forthMotor(_forthMotor)
  {
  }



  void motorUpdate(bool _useMotion);

  uint16_t getFirstMotorThrottle();
  uint16_t getSecondMotorThrottle();
  uint16_t getThirdMotorThrottle();
  uint16_t getForthMotorThrottle();

  void setMotorTargetThrottle(uint16_t _throttle1, uint16_t _throttle2, uint16_t _throttle3, uint16_t _throttle4);

  void setMotorThrottle(uint16_t _throttle1, uint16_t _throttle2, uint16_t _throttle3, uint16_t _throttle4);

  void motorInit();

  [[nodiscard]] IMU getIMU() const;

  const IMU::SensorDataFull& getIMUData(bool xy_invert) noexcept;

  bool useMotion = false;
  bool xyInvert = true; // 是否需要 XY 轴反转
private:
  IMU *imu = nullptr;
  Dshot *firstMotor = nullptr;
  Dshot *secondMotor = nullptr;
  Dshot *thirdMotor = nullptr;
  Dshot *forthMotor = nullptr;

  uint16_t firstMotorThrottle = 0;
  uint16_t secondMotorThrottle = 0;
  uint16_t thirdMotorThrottle = 0;
  uint16_t forthMotorThrottle = 0;

  IMU::SensorDataFull sensorData{};
};

#endif // QUADROTOR_HPP
