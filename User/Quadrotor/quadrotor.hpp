//
// Created by L2248 on 2025/7/31.
//

#ifndef QUADROTOR_HPP
#define QUADROTOR_HPP

#include "Driver/IMU/IMU.hpp"

class Quadrotor
{
public:
  explicit Quadrotor(IMU *_imu) : imu(_imu) {}


  IMU *imu;
private:
};

Quadrotor& GetQuadrotor();

#endif // QUADROTOR_HPP
