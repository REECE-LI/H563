//
// Created by L2248 on 2025/8/14.
//

#ifndef MOTION_PLANNER_HPP
#define MOTION_PLANNER_HPP

class MotionPlanner
{
public:

  virtual ~MotionPlanner() = default;

  void setTarget(float _target);

  [[nodiscard]] float getOutput() const;

  virtual void update() = 0;

protected:
  float target = 0.0f; // 目标值，默认为0
  float output = 0.0f; // 输出值，默认为0
};

#endif // MOTION_PLANNER_HPP
