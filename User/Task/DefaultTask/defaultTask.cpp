//
// Created by L2248 on 2025/7/31.
//

#include "defaultTask.h"
#include "DSHOT600/dshot600.hpp"
#include "Driver/IMU/JY931/JY931.hpp"
#include "First-OrderRampGenerator/firstORG.hpp"
#include "FreeRTOS.h"
#include "Quadrotor/quadrotor.hpp"
#include "cmsis_os2.h"
#include "parameterStruct.hpp"
#include "retarget.h"
#include "usart.h"

JY931 IMU_JY931(&huart4);

FirstORG motionPlannerDshot600_First(1200.0f, 0.0001f);
FirstORG motionPlannerDshot600_Second(1200.0f, 0.0001f);
FirstORG motionPlannerDshot600_Third(1200.0f, 0.0001f);
FirstORG motionPlannerDshot600_Forth(1200.0f, 0.0001f);

// 电机顺序需要具体确认
Dshot600 dshot600_First(&htim3, TIM_CHANNEL_4);
Dshot600 dshot600_Second(&htim3, TIM_CHANNEL_1);
Dshot600 dshot600_Third(&htim3, TIM_CHANNEL_3);
Dshot600 dshot600_Forth(&htim3, TIM_CHANNEL_2);

Quadrotor quadrotor(&IMU_JY931, &dshot600_First, &dshot600_Second, &dshot600_Third, &dshot600_Forth);

void StartDefaultTask(void *argument)
{
  // osDelay(1000);

#if DSHOT_TEST
  dshot600_First.send(0, 0);
  dshot600_Second.send(0, 0);
  dshot600_Third.send(0, 0);
  dshot600_Forth.send(0, 0);

  dshot600_First.transferEnable(true);
  dshot600_Second.transferEnable(true);
  dshot600_Third.transferEnable(true);
  dshot600_Forth.transferEnable(true);
#endif
  dshot600_First.attachMotionPlanner(&motionPlannerDshot600_First);
  dshot600_Second.attachMotionPlanner(&motionPlannerDshot600_Second);
  dshot600_Third.attachMotionPlanner(&motionPlannerDshot600_Third);
  dshot600_Forth.attachMotionPlanner(&motionPlannerDshot600_Forth);
  quadrotor.motorInit();
#if 0
  // 获取当前任务句柄并终止任务
  osThreadId_t currentTaskHandle = osThreadGetId();
  osThreadTerminate(currentTaskHandle);
#endif
  HAL_TIM_Base_Start_IT(&htim4);
  for (;;)
  {
#if IMU_TEST
    IMU::SensorDataFull sensorData = quadrotor.imu->getSensorData();
    printf("%.3f, %.3f, %.3f\r\n", sensorData.angle.x, sensorData.angle.y, sensorData.angle.z);
#endif

#if DSHOT_TEST
    // 什么都不做，DMA循环发送
#endif

    osDelay(2);
  }
}
