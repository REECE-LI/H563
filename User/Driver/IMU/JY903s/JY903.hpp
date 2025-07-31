//
// Created by L2248 on 2025/7/30.
//

#ifndef JY903_HPP
#define JY903_HPP
#include "Driver/IMU/imu.hpp"
#include "REG.h" // Register address definitions
#include "usart.h"

#define JY903_BUF_LEN 11
#define ACC_UPDATE 0x01
#define GYRO_UPDATE 0x02
#define ANGLE_UPDATE 0x04
#define MAG_UPDATE 0x08
#define READ_UPDATE 0x80

class JY903 : public IMU
{
public:

  enum WIT_STATUS : int
  {
    WIT_HAL_OK = 0,       ///< No error
    WIT_HAL_BUSY = -1,    ///< Busy
    WIT_HAL_TIMEOUT = -2, ///< Timed out
    WIT_HAL_ERROR = -3,   ///< A generic error
    WIT_HAL_NOMEM = -4,   ///< No memory
    WIT_HAL_EMPTY = -5,   ///< Resource is empty
    WIT_HAL_INVAL = -6,   ///< Invalid argument
    PROTOCOL_NORMAL = 0   ///< Normal UART protocol
  };

  explicit JY903(UART_HandleTypeDef *_huart) : huart(_huart) {}

  WIT_STATUS witWriteReg(uint32_t _uiReg, uint16_t _usData);

  uint8_t *giveReceiveData();

  bool decodeData();

  void showData();

  volatile char s_cDataUpdate1 = 0, s_cDataUpdate2 = 0, s_cDataUpdate3 = 0, s_cDataUpdate4 = 0, s_cCmd = 0xff;

  bool isFirstDataProcess = true;

  bool checkData();

private:
  UART_HandleTypeDef *huart;

  int16_t sReg[REGSIZE]{};

  uint8_t ucRegIndex = 0;

  uint32_t uiRegDataLen = 0;

  uint16_t usRegDataBuff[4] = {0};

  uint8_t receiveData[JY903_BUF_LEN] = {};

  uint32_t s_uiWitDataCnt = 0, s_uiProtoclo = 0, s_uiReadRegIndex = 0;

  // 全精度缩放因子，编译期计算
  static constexpr float ACC_FACTOR   = 16.0f   / 32768.0f;   // ≈ 0.00048828125f
  static constexpr float GYRO_FACTOR  = 2000.0f / 32768.0f;   // ≈ 0.06103515625f
  static constexpr float ANGLE_FACTOR = 180.0f  / 32768.0f;   // ≈ 0.0054931640625f

  uint8_t calcSum(const uint8_t *data, uint32_t len);

  void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
};
#endif // JY903_HPP
