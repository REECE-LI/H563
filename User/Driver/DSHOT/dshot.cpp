//
// Created by L2248 on 2025/8/1.
//

#include "dshot.hpp"
#include "User.hpp"


void Dshot::send(uint16_t _throttle, uint8_t _telemetry)
{
  uint16_t packet = buildPacket(_throttle, _telemetry);
  encodePacket(packet);
}

void Dshot::transferEnable(bool _enable)
{
  if (_enable)
  {
    startDmaTransfer();
  }
  else
    stopDmaTransfer();
}

void Dshot::unlockDhot()
{
  send(0, 0);
}

void Dshot::attachMotionPlanner(MotionPlanner *_motionPlanner)
{
  motionPlanner = _motionPlanner;
}

void Dshot::setTargetThrottle(uint16_t _throttle)
{
  targetThrottle = _throttle;
  motionPlanner->setTarget(targetThrottle);
}

void Dshot::motionPlannerUpdate()
{
  motionPlanner->update();
}

uint16_t Dshot::getMotionPlannerThrottle()
{
  return static_cast<uint16_t>(motionPlanner->getOutput());
}

uint16_t Dshot::getTargetThrottle()
{
  return static_cast<uint16_t>(targetThrottle);
}

void Dshot::onTransmissionComplete()
{
  // busy_ = false; // 传输完成，允许新的发送
}



