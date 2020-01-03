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

#define _RTOS_C_

#include        "rtos.h"



/*********************************************************
 *  Kernel
 */
/**
  * @brief  start the FreeRTOS kenrel
  * @details start kernel
  * @param  none
  * @retval status [ok]
  */
rtosStatus_t
RtosKernelStart(void)
{
  vTaskStartScheduler();

  return RTOS_SUCCESS;
}


/*********************************************************
 *  Task
 */
/**
  * @brief  create task
  * @details create task and register task parameters
  * @param  pInfo task information structure
  * @param  arg user argument
  * @retval task id
  */
rtosStatus_t
RtosTaskCreate(const rtosTaskInfo_t *pInfo, void *arg, rtosTaskId *pId)
{
  rtosStatus_t          result = RTOS_SUCCESS;
  TaskHandle_t  handle;
  portBASE_TYPE re;

  if(pId == RTOS_NULL) pId = &handle;
  re = xTaskCreate(pInfo->pFunc,
                   pInfo->pName,
                   pInfo->szStack,
                   arg,
                   pInfo->priority,
                   pId
                   );

  if(re == portSTACK_GROWTH) {
    result = RTOS_LACKOFSTACK;
  }

  return result;
}
/**
  * @brief  sleep this task
  * @details sleep a while in msec. wait minimum tick if the  millisec is set 0
  * @param  millisec wait time in msec
  * @retval status [ok]
  */
rtosStatus_t
RtosTaskSleep(uint32_t millisec)
{
  TickType_t    tick = millisec / portTICK_PERIOD_MS;
  if(!tick) tick = 1;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    vTaskDelay(tick);
  }

  return RTOS_SUCCESS;
}


/*********************************************************
 *  Memory control
 */
void *
RtosMalloc(int size)
{
  void  *p = NULL;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    p = pvPortMalloc(size);
  }

  return p;
}
void
RtosFree(void *ptr)
{
  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    vPortFree(ptr);
  }
  return;
}


/*********************************************************
 *  Semaphore
 */
/**
  * @brief  create the mutex semaphore
  * @details create the mutex semaphore
  * @param  none
  * @retval sem id
  */
rtosSemId
RtosSemCreateMutex(void)
{
  SemaphoreHandle_t     re = NULL;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    re = xSemaphoreCreateMutex();
  }

  return (rtosSemId)re;
}
/**
  * @brief  take the semaphore
  * @details take the semaphore
  * @param  id sem id
  * @param  ms timeout in millisec
  * @retval status
  */
rtosStatus_t
RtosSemTake(rtosSemId id, uint32_t ms)
{
  rtosStatus_t    result = RTOS_NOTSTARTEDYET;
  BaseType_t    re;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    re = xSemaphoreTake(id, ms);
    if(re == pdTRUE) {
      result = RTOS_SUCCESS;
    } else {
      result = RTOS_TIMEOUT;
      goto fail;
    }
  }

fail:
  return result;
}
/**
  * @brief  release the semaphore
  * @details release the semaphore
  * @param  id sem id
  * @retval status
  */
rtosStatus_t
RtosSemRelease(rtosSemId id)
{
  rtosStatus_t    result = RTOS_NOTSTARTEDYET;
  BaseType_t    re;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    re = xSemaphoreGive(id);
    if(re == pdTRUE) {
      result = RTOS_SUCCESS;
    } else {
      result = RTOS_TIMEOUT;
      goto fail;
    }
  }

fail:
  return result;
}


/*********************************************************
 *  Queue
 */
/**
  * @brief  create the queue
  * @details create the queue
  * @param  none
  * @retval id queue id
  */
rtosQueueId
RtosQueueCreate(uint32_t lenQueue, uint32_t szItem)
{
  QueueHandle_t         re = NULL;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    re = (rtosQueueId)xQueueCreate(lenQueue, szItem);
  }

  return (rtosQueueId)re;
}
/**
  * @brief  send message to the queue
  * @details send message to the queue
  * @param  id rtosQueueId (return value of RtosQueueCreate())
  * @param  ptr the pointer of message
  * @param  timeout timeout
  * @retval id queue id
  */
rtosStatus_t
RtosQueueSend(rtosQueueId id, void *ptr, uint32_t timeout)
{
  rtosStatus_t          result = RTOS_SUCCESS;
  BaseType_t            re = pdFAIL;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    re = xQueueSend(id, ptr, timeout);
    if(re == pdFAIL) result = RTOS_TIMEOUT;
  } else {
    result = RTOS_NOTSTARTEDYET;
  }

  return result;
}
/**
  * @brief  send message to the queue in interrupt
  * @details send message to the queue
  * @param  id rtosQueueId (return value of RtosQueueCreate())
  * @param  ptr the pointer of message
  * @param  pri the receive task is set higher priority after put the queue
  * @retval id queue id
  */
rtosStatus_t
RtosQueueSendIsr(rtosQueueId id, void *ptr, int pri)
{
  rtosStatus_t          result = RTOS_SUCCESS;
  BaseType_t            re = pdFAIL;
  BaseType_t            xHigherPriorityTaskWoken = pdFALSE;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    if(pri) xHigherPriorityTaskWoken = pdTRUE;
    //xHigherPriorityTaskWoken = pdFALSE;
    re = xQueueSendFromISR(id, ptr, &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken) {
      portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    if(re == pdFAIL) result = RTOS_TIMEOUT;
  } else {
    result = RTOS_NOTSTARTEDYET;
  }

  return result;
}
/**
  * @brief  recv message to the queue
  * @details recv message to the queue
  * @param  id rtosQueueId (return value of RtosQueueCreate())
  * @param  ptr the pointer of message
  * @param  timeout timeout
  * @retval id queue id
  */
rtosStatus_t
RtosQueueRecv(rtosQueueId id, void *ptr, uint32_t timeout)
{
  rtosStatus_t          result = RTOS_SUCCESS;
  BaseType_t            re = pdFAIL;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    re = xQueueReceive(id, ptr, timeout);
    if(re == pdFAIL) result = RTOS_TIMEOUT;
  } else {
    result = RTOS_NOTSTARTEDYET;
  }

  return result;
}


/*********************************************************
 *  Event flag
 */
/**
  * @brief  create the event group flag
  * @details create the event group flag
  * @param  none
  * @retval id event id
  */
rtosEventId
RtosEventCreate(void)
{
  EventGroupHandle_t    re = NULL;

  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    re = xEventGroupCreate();
  }

  return (rtosEventId)re;
}


/*********************************************************
 *  misc
 */
#if 0
/**
  * @brief  svc
  * @details svc call
  * @param  none
  * @retval none
  */
void
RtosSvcEntry(void)
{
  vPortSVCHandler();
  return;
}
/**
  * @brief  pend SV
  * @details call pend SV
  * @param  none
  * @retval none
  */
void
RtosPendSvEntry(void)
{
  xPortPendSVHandler();
  return;
}
/**
  * @brief  tick entry
  * @details call this every ticks
  * @param  none
  * @retval none
  */
void
RtosSystickEntry(void)
{
  xPortSysTickHandler();
  return;
}
#endif
