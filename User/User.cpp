//
// Created by L2248 on 2025/7/30.
//

#include "User.hpp"

#include "DSHOT600/dshot600.hpp"
#include "Driver/IMU/JY931/JY931.hpp"
#include "IMU_Task.h"
#include "app_freertos.h"
#include "retarget.h"
#include "tim.h"
#include "usart.h"

#include <string.h>

void User(void)
{
  RetargetInit(&huart1);
  HAL_Delay(1);
  // for (;;)
  // {
  //   break;
  // }
}

extern JY931 IMU_JY931;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART4)
  {
    static uint8_t lastjy931Data[JY931_BUF_LEN];
    // static bool isFirst = true;
    static uint8_t headIndex = 0;
    uint8_t IMU_Data[JY931_BUF_LEN];

    // 第一次寻找帧头
    if (IMU_JY931.isFirstDataProcess)
    {
      for (int i = 0; i < JY931_BUF_LEN; i++)
      {
        if (jy931Data[i] == 0x55)
        {
          headIndex = i;
          IMU_JY931.isFirstDataProcess = false;
          // 在找到帧头0x55后，将数据拷贝到lastjy931Data
          memcpy(lastjy931Data, jy931Data, JY931_BUF_LEN);
          break;
        }
      }
    }
    else
    {
      memcpy(IMU_Data, lastjy931Data + headIndex, JY931_BUF_LEN - headIndex);
      memcpy(IMU_Data + JY931_BUF_LEN - headIndex, jy931Data, headIndex);

      osMessageQueuePut(IMU_QueueHandle, IMU_Data, 0, 0);
      memcpy(lastjy931Data, jy931Data, JY931_BUF_LEN);
    }
  }
}



extern Dshot600 dshot600_One;
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim3) {
    // dshot600_One.onTransmissionComplete();
    dshot600_One.transferEnable(false);
  }

}
