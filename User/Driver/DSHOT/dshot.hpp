//
// Created by L2248 on 2025/8/1.
//

#ifndef DSHOT_HPP
#define DSHOT_HPP
#include <cstdint>

// 当前是单缓冲，可以拓展至双缓冲
class Dshot
{
public:
  virtual ~Dshot() = default;

  virtual void init() = 0;

  // throttle: 0~2047, telemetry: 0 or 1
  void send(uint16_t throttle, uint8_t telemetry);

  void transferEnable(bool _enable);

  void onTransmissionComplete();

protected:
  [[nodiscard]] virtual uint8_t computeCRC(uint16_t valueWithTelemetry) const = 0;
  [[nodiscard]] virtual uint16_t buildPacket(uint16_t throttle, uint8_t telemetry) const= 0;

  // 16-bit packet -> CCR buffer
  virtual void encodePacket(uint16_t packet) = 0;
  virtual void startDmaTransfer() = 0;
  virtual void stopDmaTransfer() = 0;

  // 状态防抖：正在发送中则拒绝新发
  bool busy_ = false;
};

#endif // DSHOT_HPP
