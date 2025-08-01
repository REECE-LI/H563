//
// Created by L2248 on 2025/7/31.
//

#ifndef IMU_TASK_H
#define IMU_TASK_H
#include <sys/_stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif


extern uint8_t jy931Data[11];



  void Start_IMU_Task(void *argument);
#ifdef __cplusplus
}
#endif

#endif // IMU_TASK_H
