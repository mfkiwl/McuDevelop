/*
 * Copyright (c) 2019 zhtlab
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

#define _PERIUART_C_

#include        <stdint.h>
#include        <string.h>

#include        "config.h"
#include        "system.h"
#include        "gpio.h"

#include        "peri.h"
#include        "periUart.h"

extern struct _stPeri   peri;

static stm32Dev_USART *const pu[PERIUART_UNIT_MAX] = {
#ifdef USART1_PTR
  USART1_PTR,
#endif
#ifdef USART2_PTR
  USART2_PTR,
#endif
#ifdef USART3_PTR
  USART3_PTR,
#endif
};
const static uint16_t   gpioInitRxd[] = {
  PORT_CTRL(GPIO_USART1_RXD, GPIO_MODE_FUNC,  GPIO_PULL_UP,   GPIO_FUNC(7)),
  PORT_CTRL(GPIO_USART2_RXD, GPIO_MODE_FUNC,  GPIO_PULL_UP,   GPIO_FUNC(7)),
};
const static uint16_t   gpioInitTxd[] = {
  PORT_CTRL(GPIO_USART1_TXD, GPIO_MODE_FUNC,  GPIO_PULL_NONE, GPIO_FUNC(7)),
  PORT_CTRL(GPIO_USART2_TXD, GPIO_MODE_FUNC,  GPIO_PULL_NONE, GPIO_FUNC(7)),
};
const static uint16_t   gpioInitTxdDefault[] = {
  PORT_CTRL(GPIO_USART1_TXD, GPIO_MODE_INPUT, GPIO_PULL_UP, 0),
  PORT_CTRL(GPIO_USART2_TXD, GPIO_MODE_INPUT, GPIO_PULL_UP, 0),
};
const static uint16_t   gpioUninitRxd[] = {
  PORT_CTRL(GPIO_USART1_RXD, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_USART2_RXD, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
};
const static uint16_t   gpioUninitTxd[] = {
  PORT_CTRL(GPIO_USART1_TXD, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_USART2_TXD, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
};


int
PeriUartInit(int unit)
{
  int                   result = PERI_ERROR_UNKNOWN;
  stm32Dev_USART        *u;

  if(unit >= PERIUART_UNIT_MAX) goto fail;

  switch(unit) {
  case  0:
    //RCC_PTR->CCIPR &= ~RCC_CCIPR_USART1SEL_MASK; /* so PCLK */
    RCC_PTR->APB2ENR |= RCC_APB2ENR_USART1EN_YES;
    break;
  case  1:
    //RCC_PTR->CCIPR &= ~RCC_CCIPR_USART2SEL_MASK; /* so PCLK */
    RCC_PTR->APB1ENR |= RCC_APB1ENR_USART2EN_YES;
    break;
#if 0
  case  2:
    //RCC_PTR->CCIPR &= ~RCC_CCIPR_USART3SEL_MASK;
    //RCC_PTR->CCIPR |=  RCC_CCIPR_USART3SEL_PCLK;
    RCC_PTR->APB1ENR |= RCC_AP12ENR_USART3EN_YES;
    break;
#endif
  }

  u = pu[unit];

  /* gpio function setting */
  SystemGpioSet(gpioInitRxd[unit]);
  SystemGpioSet(gpioInitTxdDefault[unit]);              /* input, pu */

  /* usart setting */
  u->CR1 = 0;
  u->CR1 = USART_CR1_M0_YES | USART_CR1_PCE_YES | USART_CR1_PS_EVEN;
  u->CR2 = USART_CR2_ABRMOD_MODE2_7F | USART_CR2_ABREN_YES;
  u->CR1 |=  USART_CR1_TE_YES | USART_CR1_RE_YES | USART_CR1_UE_YES;      /* enable usart */

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriUartUninit(int unit)
{
  int           result = PERI_ERROR_UNKNOWN;

  stm32Dev_USART        *u;

  if(unit >= PERIUART_UNIT_MAX) goto fail;

  u = pu[unit];
  u->CR1 = 0;                   /* disable USART */

  SystemGpioSet(gpioUninitRxd[unit]);
  SystemGpioSet(gpioUninitTxd[unit]);

  switch(unit) {
  case  0: RCC_PTR->APB2ENR &= ~RCC_APB2ENR_USART1EN_MASK; break;
  case  1: RCC_PTR->APB1ENR &= ~RCC_APB1ENR_USART2EN_MASK; break;
    /*case  2: RCC_PTR->APB1ENR &= ~RCC_APB1ENR_USART3EN_MASK; break;*/
  }

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriUartProbe(int unit)
{
  int           result = PERI_ERROR_UNKNOWN;
  uint8_t       c;
  int           len;

  if(unit >= PERIUART_UNIT_MAX) goto fail;

  len = sizeof(c);
  PeriUartRecv(unit, &c, &len);
  if(len >= 1) {
    SystemGpioSet(gpioInitTxd[unit]);
    result = PERI_SUCCESS;
  }

fail:
  return result;
}


int
PeriUartSend(int unit, const uint8_t *ptr, int len)
{
  int           result = PERI_ERROR_UNKNOWN;
  int           i;
  stm32Dev_USART        *u;

  if(unit >= PERIUART_UNIT_MAX) goto fail;

  u = pu[unit];

  for(i = 0; i < len; i++) {
    while(!(u->ISR & USART_ISR_TXE_MASK));
    u->TDR = ptr[i];
  }

fail:
  result = PERI_SUCCESS;

  return result;
}


int
PeriUartRecv(int unit, uint8_t *ptr, int *len)
{
  int           result = PERI_ERROR_UNKNOWN;

  int           sz;

  if(unit >= PERIUART_UNIT_MAX) goto fail;
  if(!ptr || !len || *len <= 0) goto fail;

  PeriUartRxd(unit);

  sz = peri.lenRx;
  if(sz > *len) sz = *len;
  if(sz > sizeof(peri.bufRx)) sz = sizeof(peri.bufRx);
  memcpy(ptr, peri.bufRx, sz);
  *len = sz;
  peri.lenRx = 0;

  result = PERI_SUCCESS;
fail:

  return result;
}


int
PeriUartIoctl(int unit, int req, void *ptr, int len)
{
  int           result = PERI_ERROR_UNKNOWN;

  if(unit >= PERIUART_UNIT_MAX) goto fail;
  if(!ptr || len <= 0) goto fail;

  switch(req) {
  case  PERI_GET_RECV_SIZE:
  PeriUartRxd(unit);
    *(uint32_t *) ptr = peri.lenRx;
    break;

  default:
    goto fail;
  }

  result = PERI_SUCCESS;
fail:
  return result;
}


static void
PeriUartRxd(int unit)
{
  stm32Dev_USART        *u;

  u = pu[unit];
  if(u->ISR & USART_ISR_RXNE_MASK) {
    peri.bufRx[peri.lenRx] = u->RDR;
    peri.lenRx++;
  }

  return;
}
