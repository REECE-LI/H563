//
// Created by L2248 on 2025/7/31.
//

#include "IMU_Task.h"
#include "FreeRTOS.h"
#include "app_freertos.h"
#include "cmsis_os2.h"
#include "jy903.hpp"
#include "queue.h"
#include "usart.h"
#include <cstdio>
#include <string.h>

extern JY903 IMU_JY903;


uint8_t jy903Data[JY903_BUF_LEN] = {0}; // JY903 数据缓冲区
void Start_IMU_Task(void *argument)
{
  /* USER CODE BEGIN IMU_Task */

  printf("IMU_Task init OK\r\n");

  // 可能需要校准IMU，但是当前程序上电自动校准

  HAL_UART_Receive_DMA(&huart4, jy903Data, JY903_BUF_LEN);
  /* Infinite loop */
  for (;;)
  {
    // 等待 IMU 数据接收完成
    if (osOK == osMessageQueueGet(IMU_QueueHandle, IMU_JY903.giveReceiveData(), nullptr, HAL_MAX_DELAY))
    {
      // IMU_JY903.showData();
      if (!IMU_JY903.isFirstDataProcess && IMU_JY903.checkData())
      {
        // decode data
        if (osMutexAcquire(IMU_MutexHandle, osWaitForever) == osOK)
        {
          IMU_JY903.decodeData();
          osMutexRelease(IMU_MutexHandle);
        }
      }
    }
  }
  /* USER CODE END IMU_Task */
}
