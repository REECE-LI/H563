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
Dshot600 dshot600_First(&htim3, TIM_CHANNEL_1);  // Dshot600 对象，使用 TIM3 的通道 1
Dshot600 dshot600_Second(&htim3, TIM_CHANNEL_2); // Dshot600 对象，使用 TIM3 的通道 2
Dshot600 dshot600_Third(&htim3, TIM_CHANNEL_3);  // Dshot600 对象，使用 TIM3 的通道 3
Dshot600 dshot600_Forth(&htim3, TIM_CHANNEL_4);  // Dshot600 对象，使用 TIM3 的通道 4

Quadrotor quadrotor(&IMU_JY931); // 这样写会有一个警告，使用下面的写法，延迟定义不会出现警告

void StartDefaultTask(void *argument)
{
  osDelay(1000);

#if 0
  // 获取当前任务句柄并终止任务
  osThreadId_t currentTaskHandle = osThreadGetId();
  osThreadTerminate(currentTaskHandle);
#endif
#if DSHOT_TEST
  // __HAL_TIM_DISABLE_DMA(&htim3, TIM_DMA_UPDATE);   // 关 UDE
  dshot600_First.send(0, 0);
  dshot600_Second.send(0, 0);
  dshot600_Third.send(0, 0);
  dshot600_Forth.send(0, 0);

  dshot600_First.transferEnable(true);
  // dshot600_Second.transferEnable(true);
  // dshot600_Third.transferEnable(true);
  // dshot600_Forth.transferEnable(true);
#endif

  for (;;)
  {
#if IMU_TEST
    IMU::SensorDataFull sensorData = quadrotor.imu->getSensorData();
    printf("%.3f, %.3f, %.3f\r\n", sensorData.angle.x, sensorData.angle.y, sensorData.angle.z);
#endif

#if DSHOT_TEST
    // __HAL_TIM_ENABLE_DMA(&htim3, TIM_DMA_CC1);
    dshot600_First.transferEnable(true);
    dshot600_Second.transferEnable(true);
    dshot600_Third.transferEnable(true);
    dshot600_Forth.transferEnable(true);

#endif

    osDelay(1);
  }
}
