//
// Created by L2248 on 2025/8/14.
//

#include "motion_planner.hpp"


void MotionPlanner::setTarget(float _target)
{
  target = _target;
}

float MotionPlanner::getOutput() const
{
  return output;
}