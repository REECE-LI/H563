//
// Created by L2248 on 2025/7/30.
//

#include "User.hpp"

#include "DSHOT600/dshot600.hpp"
#include "Driver/IMU/JY931/JY931.hpp"
#include "IMU_Task.h"
#include "LetterShell/shell.h"
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

extern Dshot600 dshot600_First;
extern Dshot600 dshot600_Second;
extern Dshot600 dshot600_Third;
extern Dshot600 dshot600_Forth;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  ShellMsgPtr_t msg{};
  if (huart == &huart1)
  {
    memcpy(msg.data, shellRxBuffer, Size);
    msg.len = Size;
    if (osMessageQueuePut(shellQueueHandle, &msg, 0, 0) != osOK)
    {
      // 投递失败，要 free(buf)
    }
    memset(shellRxBuffer, 0, Size);
  }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
      // 这里处理 CC1 完成
      dshot600_First.transferEnable(false);
    }

    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
      // 这里处理 CC2 完成
      dshot600_Second.transferEnable(false);
    }
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
      // 这里处理 CC3 完成
      dshot600_Third.transferEnable(false);
    }
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
      // 这里处理 CC4 完成
      dshot600_Forth.transferEnable(false);
    }
  }
}

void TIM_DMADelayPulseCplt(DMA_HandleTypeDef *hdma) {}