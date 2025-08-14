//
// Created by L2248 on 2025/8/14.
//

#include "firstORG.hpp"


void FirstORG::update()
{
  // 计算目标值与当前值的差值
  float error = target - currentValue;

  // 计算本次更新的最大变化量
  float maxChange = rampRate * deltaTime;

  // 根据误差方向和最大变化量更新当前值
  if (error > maxChange) {
    // 目标值大于当前值，向上斜坡
    currentValue += maxChange;
  }
  else if (error < -maxChange) {
    // 目标值小于当前值，向下斜坡
    currentValue -= maxChange;
  }
  else {
    // 误差小于最大变化量，直接到达目标
    currentValue = target;
  }

  output = currentValue;
}