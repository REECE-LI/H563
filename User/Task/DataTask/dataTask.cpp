//
// Created by xiaojun on 8/14/25.
//

#include "dataTask.hpp"
#include "freertos_os2.h"
#include "quadrotor.hpp"
#include "task.h"
#include "usart.h"

#include <cstdio>

extern Quadrotor quadrotor;
// 通过UART 7 向外部发送飞控信息
void StartDataTask(void *argument)
{
  // 任务初始化
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(2); // 10ms周期，100Hz频率

  // 数据缓冲区
  uint8_t txBuffer[256];

  while (1)
  {
    // 绝对定时器 - 等待下一个周期
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    auto quadrotorIMUData = quadrotor.getIMUData(quadrotor.xyInvert);

    uint16_t len = snprintf((char*)txBuffer, sizeof(txBuffer),
                       "%.3f,%.3f,%.3f,"   // acc
                       "%.3f,%.3f,%.3f,"   // gyro
                       "%.2f,%.2f,%.2f,"   // angle
                       "%d,%d,%d\r\n",     // mag
                       quadrotorIMUData.acc.x, quadrotorIMUData.acc.y, quadrotorIMUData.acc.z,
                       quadrotorIMUData.gyro.x, quadrotorIMUData.gyro.y, quadrotorIMUData.gyro.z,
                       quadrotorIMUData.angle.x, quadrotorIMUData.angle.y, quadrotorIMUData.angle.z,
                       quadrotorIMUData.mag[0], quadrotorIMUData.mag[1], quadrotorIMUData.mag[2]);

    // 通过UART 7发送数据
    HAL_UART_Transmit_DMA(&huart7, txBuffer, len);
  }
}