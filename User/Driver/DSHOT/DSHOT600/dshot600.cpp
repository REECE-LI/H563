//
// Created by L2248 on 2025/8/1.
//

#include "dshot600.hpp"

#include <string.h>

void Dshot600::init()
{
  // HAL_TIM_PWM_Start_IT(htim_, channel_);
}

void Dshot600::encodePacket(uint16_t packet)
{
  for (int i = 0; i < DSHOT_BITS; ++i)
  {
    bool bit = packet & (1 << (15 - i)); // MSB first
    // bool bit = packet & (1 << i);   // LSB first
    ccr_buf_[i] = bit ? DSHOT_T1H : DSHOT_T0H;
  }
  ccr_buf_[16] = 0;
  ccr_buf_[17] = 0; // Reset pulse (2us)
}

uint8_t Dshot600::computeCRC(uint16_t valueWithTelemetry) const
{
  uint8_t csum = 0;
  // 每次只异或 v 的最低 4 位，然后右移 4 位
  for (int i = 0; i < 3; ++i)
  {
    csum ^= (valueWithTelemetry & 0xF);
    valueWithTelemetry >>= 4;
  }
  return csum & 0xF;
}

uint16_t Dshot600::buildPacket(uint16_t throttle, uint8_t telemetry) const
{
  if (throttle > 2047)
    throttle = 2047;
  uint16_t payload = (throttle << 1) | (telemetry ? 1 : 0); // 12-bit
  uint8_t crc = computeCRC(payload);
  return (payload << 4) | crc; // 16-bit packet
}

void Dshot600::startDmaTransfer()
{
  if (busy_)
    return;
  busy_ = true;

  HAL_TIM_PWM_Start_DMA(htim_, channel_, (uint32_t *)ccr_buf_, DSHOT_SEND_BITS * 2);
}

void Dshot600::stopDmaTransfer() { HAL_TIM_PWM_Stop_DMA(htim_, channel_); }