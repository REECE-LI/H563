//
// Created by L2248 on 2025/8/1.
//

#ifndef DSHOT600_HPP
#define DSHOT600_HPP
#include "dshot.hpp"
#include "tim.h"

class Dshot600 : public Dshot
{
public:
  explicit Dshot600(TIM_HandleTypeDef *_htim, uint32_t _channel) : htim_(_htim), channel_(_channel) {}

  ~Dshot600() override = default;

  void init() override;

protected:
  [[nodiscard]] uint8_t computeCRC(uint16_t valueWithTelemetry) const override;
  [[nodiscard]] uint16_t buildPacket(uint16_t throttle, uint8_t telemetry) const override;

  void encodePacket(uint16_t packet) override;
  void startDmaTransfer() override;
  void stopDmaTransfer() override;

private:
  static constexpr int DSHOT_BITS = 16;
  static constexpr uint16_t TIMER_PERIOD = 415; // ARR = 415 for ~1.6667µs bit @250MHz
  static constexpr uint16_t DSHOT_T1H = 277;    // “1” high length
  static constexpr uint16_t DSHOT_T0H = 138;    // “0” high length

  TIM_HandleTypeDef *htim_;
  uint32_t channel_; // e.g., TIM_CHANNEL_1

  uint16_t ccr_buf_[DSHOT_BITS]{0};
};

#endif // DSHOT600_HPP
