//
// Created by L2248 on 2025/7/31.
//

#include "IMU_Task.h"
#include "./LetterShell/shell.h"
#include "Driver/IMU/JY931/JY931.hpp"
#include "FreeRTOS.h"
#include "app_freertos.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "usart.h"

#include <cstdio>
#include <string.h>

extern JY931 IMU_JY931;

uint8_t jy931Data[JY931_BUF_LEN] = {0}; // JY903 数据缓冲区
void Start_IMU_Task(void *argument)
{
  /* USER CODE BEGIN IMU_Task */

  // 可能需要校准IMU，但是当前程序上电自动校准

  osDelay(500);
#if 1
  shellDisplay(&shell, "\r\nIMU Task is not running, please check the configuration.\r\n");
  // 获取当前任务句柄并终止任务
  osThreadId_t currentTaskHandle = osThreadGetId();
  osThreadTerminate(currentTaskHandle);

#endif

  HAL_UART_Receive_DMA(&huart4, jy931Data, JY931_BUF_LEN);
  /* Infinite loop */
  for (;;)
  {
    // 等待 IMU 数据接收完成
    if (osOK == osMessageQueueGet(IMU_QueueHandle, IMU_JY931.giveReceiveData(), nullptr, HAL_MAX_DELAY))
    {
      // IMU_JY931.showData();
      if (!IMU_JY931.isFirstDataProcess && IMU_JY931.checkData())
      {
        // decode data
        if (osMutexAcquire(IMU_MutexHandle, osWaitForever) == osOK)
        {
          IMU_JY931.decodeData();
          osMutexRelease(IMU_MutexHandle);
        }
      }
    }
  }
  /* USER CODE END IMU_Task */
}
