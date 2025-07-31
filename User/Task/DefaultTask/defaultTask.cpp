//
// Created by L2248 on 2025/7/31.
//

#include "defaultTask.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "retarget.h"
#include "usart.h"
#include "Quadrotor/quadrotor.hpp"
#include "Driver/IMU/JY903s/JY903.hpp"

JY903 IMU_JY903(&huart4);

Quadrotor quadrotor(&IMU_JY903); // 这样写会有一个警告，使用下面的写法，延迟定义不会出现警告
// Quadrotor& quadrotor()   //quadrotor -> quadrotor() 函数返回一个 Quadrotor 的单例实例
// {
//   static Quadrotor instance(&IMU_JY903);
//   return instance;
// }


void StartDefaultTask(void *argument)
{

  printf("default task init OK\r\n");
  // printf("Task will be terminated...\r\n");

  // // 获取当前任务句柄并终止任务
  // osThreadId_t currentTaskHandle = osThreadGetId();
  // osThreadTerminate(currentTaskHandle);

  for (;;)
  {
    IMU::SensorDataFull sensorData = quadrotor.imu->getSensorData();
    printf("%.3f, %.3f, %.3f\r\n", sensorData.angle.x, sensorData.angle.y, sensorData.angle.z);
    osDelay(1);
  }
}
