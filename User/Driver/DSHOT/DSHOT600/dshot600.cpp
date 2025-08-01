//
// Created by L2248 on 2025/8/1.
//

#include "dshot600.hpp"

void Dshot600::init()
{
  // HAL_TIM_PWM_Start_IT(htim_, channel_);
}

void Dshot600::encodePacket(uint16_t packet)
{
  for (int i = 0; i < DSHOT_BITS; ++i)
  {
    bool bit = packet & (1 << (15 - i)); // MSB first
    ccr_buf_[i] = bit ? DSHOT_T1H : DSHOT_T0H;
  }
}

uint8_t Dshot600::computeCRC(uint16_t valueWithTelemetry) const
{
  uint8_t csum = 0;
  uint16_t v = valueWithTelemetry;
  for (int i = 0; i < 3; ++i)
  {
    csum ^= (v & 0xF);
    v >>= 4;
  }
  return csum & 0xF;
}

uint16_t Dshot600::buildPacket(uint16_t throttle, uint8_t telemetry) const
{
  if (throttle > 2047)
    throttle = 2047;
  uint16_t payload = (throttle << 1) | (telemetry & 0x1); // 12-bit
  uint8_t crc = computeCRC(payload);
  return (payload << 4) | crc; // 16-bit packet
}
extern uint16_t dmaBuff[50];
void Dshot600::startDmaTransfer()
{
  // if (busy_)
  //   return;
  // busy_ = true;

  HAL_TIM_PWM_Start_DMA(htim_, channel_, (const uint32_t *)ccr_buf_, DSHOT_BITS*2+2);
}

void Dshot600::stopDmaTransfer()
{
  // HAL_TIM_PWM_Stop_IT(htim_, channel_);
  HAL_TIM_PWM_Stop_DMA(htim_, channel_);
}