//
// Created by L2248 on 2025/8/14.
//

#ifndef FIRSTORG_HPP
#define FIRSTORG_HPP

#include "motion_planner.hpp"

class FirstORG : public MotionPlanner
{
public:
  // 构造函数
  explicit FirstORG(float _rampRate = 1.0f, float _deltaTime = 0.0001f)
      : rampRate(_rampRate), currentValue(0.0f), deltaTime(_deltaTime) {};


  void update() override;

private:
  float rampRate;     // 斜坡速率 (单位/秒)
  float currentValue; // 当前值
  float deltaTime;    // 时间步长
};

#endif // FIRSTORG_HPP
