/*
 * Copyright (c) 2018 zhtlab
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _RTOS_H_
#define _RTOS_H_

#include        <FreeRTOS.h>
#include        <task.h>
#include        <semphr.h>
#include        <queue.h>
#include        <event_groups.h>
#include        <queue.h>

#define RtosSvcEntry            SVC_Handler
#define RtosPendSvEntry         xPortPendSVHandler
#define RtosSysTickEntry        SysTick_Handler


#ifdef  _RTOS_C_
void    vPortSVCHandler(void);
void    xPortPendSVHandler(void);
void    xPortSysTickHandler(void);
#endif


#define RTOS_NULL       ((void *) 0)

typedef enum {
  RTOS_PRI_IDLE           = 0,
  RTOS_PRI_LOWER,
  RTOS_PRI_LOW,
  RTOS_PRI_NORMAL,
  RTOS_PRI_HIGH,
  RTOS_PRI_HIGHER,
  RTOS_PRI_HIGHEST,
  RTOS_PRI_END,
} rtosTaskPriority;

/* definitions */
typedef TaskHandle_t            rtosTaskId;
typedef TaskFunction_t          rtosTaskFunc;
typedef SemaphoreHandle_t       rtosSemId;
typedef QueueHandle_t           rtosQueueId;
typedef EventGroupHandle_t      rtosEventId;


typedef struct {
  rtosTaskFunc          pFunc;
  uint8_t               *pName;
  rtosTaskPriority      priority;
  uint32_t              szStack;
  rtosTaskId            id;
} rtosTaskInfo_t;

typedef enum {
  RTOS_SUCCESS = 0,
  RTOS_UNKNOWN          = -1,
  RTOS_NOTSTARTEDYET    = -10,
  RTOS_INVALIDARG       = -11,
  RTOS_TIMEOUT          = -12,
  RTOS_LACKOFSTACK      = -13,
} rtosStatus_t;


/* kernel */
rtosStatus_t            RtosKernelStart(void);

/* task */
rtosStatus_t            RtosTaskCreate(const rtosTaskInfo_t *pInfo, void *arg, rtosTaskId *pId);
rtosStatus_t            RtosTaskSleep(uint32_t millisec);

/* mutex */
rtosSemId               RtosSemCreateMutex(void);
rtosStatus_t            RtosSemTake(rtosSemId id, uint32_t ms);
rtosStatus_t            RtosSemRelease(rtosSemId id);

void *                  RtosMalloc(int size);
void                    RtosFree(void *ptr);

rtosQueueId             RtosQueueCreate(uint32_t lenQueue, uint32_t szItem);
rtosStatus_t            RtosQueueSend(rtosQueueId id, void *ptr, uint32_t timeout);
rtosStatus_t            RtosQueueSendIsr(rtosQueueId id, void *ptr, int pri);
rtosStatus_t            RtosQueueRecv(rtosQueueId id, void *ptr, uint32_t timeout);

void                    RtosSvcEntry(void);
void                    RtosPendSvEntry(void);
void                    RtosSystickEntry(void);

#endif
