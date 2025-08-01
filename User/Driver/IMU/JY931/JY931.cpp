#include "JY931.hpp"

#include <cstdio>
#include <string.h> // for ::memcpy

JY931::WIT_STATUS JY931::witWriteReg(uint32_t _uiReg, uint16_t _usData)
{
  uint8_t ucBuff[8];
  if (_uiReg >= REGSIZE)
    return WIT_HAL_INVAL;
  ucBuff[0] = 0xFF;
  ucBuff[1] = 0xAA;
  ucBuff[2] = _uiReg & 0xFF;
  ucBuff[3] = _usData & 0xff;
  ucBuff[4] = _usData >> 8;

  HAL_UART_Transmit(huart, ucBuff, 5, HAL_MAX_DELAY);

  return WIT_HAL_OK;
}

// 给出接收数组的地址
uint8_t *JY931::giveReceiveData() { return receiveData; }

bool JY931::decodeData()
{
  uint32_t uiReg1 = 0, uiReg2 = 0, uiReg1Len = 0, uiReg2Len = 0;
  uint16_t *p_usReg1Val = usRegDataBuff;
  uint16_t *p_usReg2Val = usRegDataBuff + 3;
  uiReg1Len = 4;

  // 1）同步头必须是 0x55
  if (receiveData[0] != 0x55)
  {
    return false;
  }
  // 2）校验和校验：前 10 字节累加后必须与第 10 索引字节相等
  if (calcSum(receiveData, 10) != receiveData[10])
  {
    return false;
  }
  // 3）提取寄存器号和数据
  ucRegIndex = receiveData[1];
  // 每两字节一个 uint16，低字节在偶数位（2,4,6,8），高字节在奇数位（3,5,7,9）
  for (int i = 0; i < 4; i++)
  {
    usRegDataBuff[i] = (uint16_t)receiveData[2 * i + 2] | ((uint16_t)receiveData[2 * i + 3] << 8);
  }
  uiRegDataLen = 4;

  switch (ucRegIndex)
  {
  case WIT_ACC:
    uiReg1 = AX;
    uiReg1Len = 3;
    uiReg2 = TEMP;
    uiReg2Len = 1;
    break;
  case WIT_ANGLE:
    uiReg1 = Roll;
    uiReg1Len = 3;
    uiReg2 = VERSION;
    uiReg2Len = 1;
    break;
  case WIT_TIME:
    uiReg1 = YYMM;
    break;
  case WIT_GYRO:
    uiReg1 = GX;
    uiRegDataLen = 3;
    break;
  case WIT_MAGNETIC:
    uiReg1 = HX;
    uiRegDataLen = 3;
    break;
  case WIT_DPORT:
    uiReg1 = D0Status;
    break;
  case WIT_PRESS:
    uiReg1 = PressureL;
    break;
  case WIT_GPS:
    uiReg1 = LonL;
    break;
  case WIT_VELOCITY:
    uiReg1 = GPSHeight;
    break;
  case WIT_QUATER:
    uiReg1 = q0;
    break;
  case WIT_GSA:
    uiReg1 = SVNUM;
    break;
  case WIT_REGVALUE:
    uiReg1 = s_uiReadRegIndex;
    break;
  default:
    break;
  }
  if (uiRegDataLen == 3)
  {
    uiReg1Len = 3;
    uiReg2Len = 0;
  }
  if (uiReg1Len)
  {
    memcpy(&sReg[uiReg1], p_usReg1Val, uiReg1Len << 1);
    SensorDataUpdata(uiReg1, uiReg1Len);
  }
  if (uiReg2Len)
  {
    memcpy(&sReg[uiReg2], p_usReg2Val, uiReg2Len << 1);
    SensorDataUpdata(uiReg2, uiReg2Len);
  }

  return true;
}

void JY931::showData()
{
#if 0
  char dataStr[JY931_BUF_LEN * 3 + 3]; // 每个字节需要3个字符(两位十六进制+空格)，加上换行和结束符
  int pos = 0;

  for (uint8_t i = 0; i < JY931_BUF_LEN; i++)
  {
    pos += sprintf(dataStr + pos, "%02X ", receiveData[i]);
  }
  sprintf(dataStr + pos, "\r\n");

  // 通过HAL库发送字符串
  HAL_UART_Transmit(&huart1, (uint8_t *)dataStr, strlen(dataStr), HAL_MAX_DELAY);
#else
  for (uint8_t i = 0; i < JY931_BUF_LEN; i++)
  {
    printf("%02X ", receiveData[i]);
  }
  printf("\r\n");
#endif
}

bool JY931::checkData()
{
  if (receiveData[0] != 0x55) // 检查帧头
  {
    isFirstDataProcess = true;
    return false;
  }

  return true;
}

uint8_t JY931::calcSum(const uint8_t *data, uint32_t len)
{
  uint8_t sum = 0;
  while (len--)
    sum += *data++;
  return sum;
}

void JY931::SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
  int i;
  for (i = 0; i < uiRegNum; i++)
  {
    switch (uiReg)
    {
      //            case AX:
      //            case AY:
    case AZ:
      s_cDataUpdate1 |= ACC_UPDATE;
      break;
      //            case GX:
      //            case GY:
    case GZ:
      s_cDataUpdate2 |= GYRO_UPDATE;
      break;
      //            case HX:
      //            case HY:
    case HZ:
      s_cDataUpdate4 |= MAG_UPDATE;
      break;
      //            case Roll:
      //            case Pitch:
    case Yaw:
      s_cDataUpdate3 |= ANGLE_UPDATE;
      break;
      //            default:
      //				s_cDataUpdate1 |= READ_UPDATE;
      //			break;
    }
    uiReg++;
  }

  if (s_cDataUpdate1 && s_cDataUpdate2 && s_cDataUpdate3 && s_cDataUpdate4)
  {
    for (i = 0; i < 3; i++)
    {
      // (&sensorData.acc.x)[i] = sReg[AX + i] / 32768.0f * 16.0f;
      (&sensorData.acc.x)[i] = sReg[AX + i] * ACC_FACTOR;

      // (&sensorData.gyro.x)[i] = sReg[GX + i] / 32768.0f * 2000.0f;
      (&sensorData.gyro.x)[i] = sReg[GX + i] * GYRO_FACTOR;

      // (&sensorData.angle.x)[i] = sReg[Roll + i] / 32768.0f * 180.0f;
      (&sensorData.angle.x)[i] = sReg[Roll + i] * ANGLE_FACTOR;
    }
    if (s_cDataUpdate1 | ACC_UPDATE)
    {
      // printf("acc:%.3f %.3f %.3f\r\n", (&sensorData.acc.x)[0], (&sensorData.acc.x)[1], (&sensorData.acc.x)[2]);
      s_cDataUpdate1 &= ~ACC_UPDATE;
    }
    if (s_cDataUpdate2 | GYRO_UPDATE)
    {
      // printf("gyro:%.3f %.3f %.3f\r\n", (&sensorData.gyro.x)[0], (&sensorData.gyro.x)[1], (&sensorData.gyro.x)[2]);
      s_cDataUpdate2 &= ~GYRO_UPDATE;
    }
    if (s_cDataUpdate3 | ANGLE_UPDATE)
    {
      // printf("%.3f, %.3f, %.3f\r\n", (&sensorData.angle.x)[0], (&sensorData.angle.x)[1], (&sensorData.angle.x)[2]);
      s_cDataUpdate3 &= ~ANGLE_UPDATE;
    }
    if (s_cDataUpdate4 | MAG_UPDATE)
    {
      // printf("mag:%d %d %d\r\n", sReg[HX], sReg[HY], sReg[HZ]);
      // sReg
      sensorData.mag[0] = sReg[HX];
      sensorData.mag[1] = sReg[HY];
      sensorData.mag[2] = sReg[HZ];
      s_cDataUpdate4 &= ~MAG_UPDATE;
    }
    // 测量时间的
    // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
  }
}