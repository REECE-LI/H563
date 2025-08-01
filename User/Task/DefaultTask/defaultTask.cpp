//
// Created by L2248 on 2025/7/31.
//

#include "defaultTask.h"
#include "DSHOT600/dshot600.hpp"
#include "Driver/IMU/JY931/JY931.hpp"
#include "FreeRTOS.h"
#include "Quadrotor/quadrotor.hpp"
#include "cmsis_os2.h"
#include "parameterStruct.hpp"
#include "retarget.h"
#include "usart.h"

JY931 IMU_JY931(&huart4);
Dshot600 dshot600_One(&htim3, TIM_CHANNEL_1); // Dshot600 对象，使用 TIM3 的通道 1

Quadrotor quadrotor(&IMU_JY931); // 这样写会有一个警告，使用下面的写法，延迟定义不会出现警告

uint16_t dmaBuff[50] = {100, 200, 300, 100, 200, 300, 100, 200, 300, 100,
                        200, 300, 100, 200, 300, 100, 200, 300, 100, 200}; // CCR 缓冲区

void StartDefaultTask(void *argument)
{
  // HAL_TIM_PWM_Init(&htim3);
  // __HAL_LINKDMA(&htim3, hdma[TIM_DMA_ID_CC1], hdma_tim3_ch1);
  // __HAL_TIM_ENABLE_OCxPRELOAD(&htim3, TIM_CHANNEL_1);

  // dshot600_One.init();
  osDelay(100);
  printf("default task init OK\r\n");
  // printf("Task will be terminated...\r\n");

  // // 获取当前任务句柄并终止任务
  // osThreadId_t currentTaskHandle = osThreadGetId();
  // osThreadTerminate(currentTaskHandle);
#if DSHOT_TEST
  dshot600_One.send(1000, 0); // 发送 Dshot 数据包，1000 是油门值，0 是无遥测数据
  dshot600_One.transferEnable(true);
#endif


  for (;;)
  {
#if IMU_TEST
    IMU::SensorDataFull sensorData = quadrotor.imu->getSensorData();
    printf("%.3f, %.3f, %.3f\r\n", sensorData.angle.x, sensorData.angle.y, sensorData.angle.z);
#endif

#if DSHOT_TEST
    dshot600_One.transferEnable(true);
#endif

    // HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (const uint32_t *)dmaBuff, 50);
    osDelay(2);
  }
}
