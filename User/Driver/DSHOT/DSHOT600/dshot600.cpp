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
  // 创建临时缓冲区
  uint16_t temp_buf[DSHOT_SEND_BITS] = {0}; // 32 bits for 16 bits of DSHOT data

  // 在临时缓冲区中编码数据
  for (int i = 0; i < DSHOT_BITS; ++i)
  {
    bool bit = packet & (1 << (15 - i)); // MSB first
    temp_buf[i] = bit ? DSHOT_T1H : DSHOT_T0H;
  }
  // temp_buf[16] = 0;
  // temp_buf[17] = 0; // Reset pulse (2us)

  // 一次性复制到主缓冲区
  memcpy(ccr_buf_, temp_buf, sizeof(temp_buf));
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
  // 清理输入，避开命令区
  if (throttle > 2047) throttle = 2047;
  if (throttle > 0 && throttle < 48) throttle = 48;

  uint16_t payload = (throttle << 1) | (telemetry ? 1 : 0); // 12-bit
  uint8_t crc = computeCRC(payload);
  return (payload << 4) | crc; // 16-bit packet
}

HAL_StatusTypeDef status;
void Dshot600::startDmaTransfer()
{
  // if (busy_)
  //   return;
  // busy_ = true;

  HAL_TIM_PWM_Start_DMA(htim_, channel_, (uint32_t *)ccr_buf_, DSHOT_SEND_BITS*2);
}

void Dshot600::stopDmaTransfer() { HAL_TIM_PWM_Stop_DMA(htim_, channel_); }