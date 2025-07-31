//
// Created by L2248 on 2025/7/30.
//

#include "User.hpp"

#include "IMU_Task.h"
#include "JY903.hpp"
#include "app_freertos.h"
#include "retarget.h"
#include "usart.h"

#include <string.h>

void User(void)
{
  RetargetInit(&huart1);
  // for (;;)
  // {
  //   break;
  // }
}


extern JY903 IMU_JY903;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART4)
  {
    static uint8_t lastJY903Data[JY903_BUF_LEN];
    // static bool isFirst = true;
    static uint8_t headIndex = 0;
    uint8_t IMU_Data[JY903_BUF_LEN];

    // 第一次寻找帧头
    if (IMU_JY903.isFirstDataProcess)
    {
      for (int i = 0; i < JY903_BUF_LEN; i++)
      {
        if (jy903Data[i] == 0x55)
        {
          headIndex = i;
          IMU_JY903.isFirstDataProcess = false;
          // 在找到帧头0x55后，将数据拷贝到lastJY903Data
          memcpy(lastJY903Data, jy903Data, JY903_BUF_LEN);
          break;
        }
      }
    }
    else
    {
      memcpy(IMU_Data, lastJY903Data + headIndex, JY903_BUF_LEN - headIndex);
      memcpy(IMU_Data + JY903_BUF_LEN - headIndex, jy903Data, headIndex);

      osMessageQueuePut(IMU_QueueHandle, IMU_Data, 0, 0);
      memcpy(lastJY903Data, jy903Data, JY903_BUF_LEN);
    }

  }
}
