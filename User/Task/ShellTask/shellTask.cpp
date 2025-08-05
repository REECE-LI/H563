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
extern Dshot600 dshot600_One;
static void shellDshotCommand(int argc, char *argv[])
{
  if (argc < 2)
  {
    shellDisplay(&shell, "Usage: dshot <throttle> [telemetry]\r\n");
    return;
  }

  uint16_t throttle = atoi(argv[1]);
  uint8_t telemetry = (argc > 2) ? atoi(argv[2]) : 0;
  dshot600_One.send(throttle, telemetry);
}

// 在 shellDshotCommand 定义之后，且不在任何函数里：
SHELL_EXPORT_CMD_EX(dshot, shellDshotCommand,
                    dshot <throttle> <telemetry>,
                    "Send Dshot command with throttle and telemetry");

#endif