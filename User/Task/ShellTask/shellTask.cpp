//
// Created by L2248 on 2025/8/5.
//

#include "shellTask.hpp"
#include "./LetterShell/shell.h"
#include "DSHOT600/dshot600.hpp"
#include "FreeRTOS.h"
#include "app_freertos.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "usart.h"

#include <cstdio>

void StartShellTask(void *argument)
{
  osDelay(1);
  shell.write = ShellWrite;
  shellInit(&shell);

  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, shellRxBuffer, SHELL_COMMAND_MAX_LENGTH);
  ShellMsgPtr_t msg{};
  for (;;)
  {
    if (osOK == osMessageQueueGet(shellQueueHandle, &msg, nullptr, HAL_MAX_DELAY))
    {
      for (uint16_t i = 0; i < msg.len; i++)
      {
        shellInput(&shell, msg.data[i]);
      }
    }
  }
}

#if USE_SHELL
char buffer[100];
extern Dshot600 dshot600_First;
extern Dshot600 dshot600_Second;
extern Dshot600 dshot600_Third;
extern Dshot600 dshot600_Forth;
static void shellDshotCommand(int argc, char *argv[])
{
  if (argc != 4)
  {
    shellDisplay(&shell, "Usage: dshot <id> <throttle> [telemetry]\r\n");
    return;
  }
  uint8_t dshotId = atoi(argv[1]);
  uint16_t throttle = atoi(argv[2]);
  uint8_t telemetry = (argc > 2) ? atoi(argv[3]) : 0;

  switch (dshotId)
  {
  case 0:
    // dshot600_First.transferEnable(false);
    // dshot600_Second.transferEnable(false);
    // dshot600_Third.transferEnable(false);
    // dshot600_Forth.transferEnable(false);
    //
    // dshot600_First.setTargetThrottle(throttle);
    // dshot600_Second.setTargetThrottle(throttle);
    // dshot600_Third.setTargetThrottle(throttle);
    // dshot600_Forth.setTargetThrottle(throttle);
    //
    // dshot600_First.transferEnable(true);
    // dshot600_Second.transferEnable(true);
    // dshot600_Third.transferEnable(true);
    // dshot600_Forth.transferEnable(true);

    dshot600_First.send(throttle, 0);
    dshot600_Second.send(throttle, 0);
    dshot600_Third.send(throttle, 0);
    dshot600_Forth.send(throttle, 0);

    // sprintf(buffer, "Dshot sent: %d with telemetry %d\r\n", throttle, telemetry);
    // shellDisplay(&shell, buffer);
    break;
  case 1:
    dshot600_First.transferEnable(false);
    dshot600_First.send(throttle, telemetry);
    dshot600_First.transferEnable(true);

    break;
  case 2:
    dshot600_Second.transferEnable(false);
    dshot600_Second.send(throttle, telemetry);
    dshot600_Second.transferEnable(true);

    break;
  case 3:
    dshot600_Third.transferEnable(false);
    dshot600_Third.send(throttle, telemetry);
    dshot600_Third.transferEnable(true);

    break;
  case 4:
    dshot600_Forth.transferEnable(false);
    dshot600_Forth.send(throttle, telemetry);
    dshot600_Forth.transferEnable(true);

    break;
  default:

    // sprintf(buffer, "Invalid Dshot ID: %d\r\n", dshotId);
    // shellDisplay(&shell, buffer);
    break;
  }
}

// 在 shellDshotCommand 定义之后，且不在任何函数里：
SHELL_EXPORT_CMD_EX(dshot, shellDshotCommand, dshot<throttle><telemetry>,
                    "Send Dshot command with throttle and telemetry");

#endif