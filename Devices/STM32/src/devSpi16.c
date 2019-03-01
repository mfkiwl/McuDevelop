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

#define _DEV_SPI16_C_

#include        <stdio.h>
#include        <string.h>

#include        "system.h"
#include        "devErrno.h"
#include        "devDma4.h"

#include        "devSpi16.h"

struct _stSpi         spi;
static devDmaParam_t       paramDmaRx;
const static uint8_t    devSpiRecvDmaReqTbl[]    = DMA_REQ_SPIRX_TBL;

/**
  * @brief  initilize the devcie
  * @param  unit   unit number
  * @param  param  the pointer of initialize parameters
  * @retval int  0: success
  */
int
DevSpiInit(int unit, devSpiParam_t *param)
{
  int                   result = -1;
  uint16_t              cr1 = 0, cr2 = 0, br;

  stm32Dev_SPI        *p;
  devSpiSc_t          *psc;

  if(unit == -1) {
    memset(&spi, 0, sizeof(spi));
#ifdef  SPI1_PTR
    spi.sc[1].dev = SPI1_PTR;
#endif
#ifdef  SPI2_PTR
    spi.sc[2].dev = SPI2_PTR;
#endif
#ifdef  SPI3_PTR
    spi.sc[3].dev = SPI3_PTR;
#endif
#ifdef  SPI4_PTR
    spi.sc[4].dev = SPI4_PTR;
#endif
#ifdef  SPI5_PTR
    spi.sc[5].dev = SPI5_PTR;
#endif
#ifdef  SPI6_PTR
    spi.sc[6].dev = SPI6_PTR;
#endif

    goto end;
  }

  if(unit > SPI_MODULE_COUNT) goto fail;
  psc = &spi.sc[unit];
  p = psc->dev;

  /* copy the parameters */
  psc->param = *param;

  /* clear first */
  p->CR1  = 0;
  p->CR2  = 0;
  p->SR;

  psc->unit = unit;

  cr1 |= (SPI_CR1_BIDIMODE_YES | SPI_CR1_BIDIOE_YES |
          SPI_CR1_LSBFRST_NO | SPI_CR1_MSTR_YES);
  br = psc->param.prescaler << SPI_CR1_BR_SHIFT;
  if(br > SPI_CR1_BR_MAXVALUE) br = SPI_CR1_BR_MAXVALUE;
  cr1 |= br;
  cr1 |= ((psc->param.clkmode << SPI_CR1_CPHA_SHIFT) &
          (SPI_CR1_CPOL_MASK | SPI_CR1_CPHA_MASK));
  cr2 |= (SPI_CR2_DS_VAL(psc->param.bit) | SPI_CR2_FRF_MOTOROLA |
          SPI_CR2_SSOE_YES | SPI_CR2_FRXTH_8BIT);

  p->CR2 = cr2;
  p->CR1 = cr1;

  if(param->dmaTx || param->dmaRx)  {
    int                   chDma;

    memset(&paramDmaRx, 0, sizeof(paramDmaRx));
    paramDmaRx.req = devSpiRecvDmaReqTbl[psc->unit];
    //paramDmaRx.a = (void *)  0;
    //paramDmaRx.b = (void *) &p->DR;
    //paramDmaRx.cnt = 0;
    paramDmaRx.dirBA = 1;
    paramDmaRx.aInc = 1;
    paramDmaRx.aSize = DEVDMA_SIZE_8BITS;
    paramDmaRx.bSize = DEVDMA_SIZE_8BITS;
    paramDmaRx.intrTC = psc->param.dmaIntr;

    chDma = (devSpiRecvDmaReqTbl[unit] >> 4) & 0xf;

    DevDmaInit(1, chDma, &paramDmaRx);

  }

  psc->up = 1;

end:
  result = 0;

fail:
  return result;
}


/**
  * @brief  interrupt process routine
  * @param  unit   unit number
  * @retval none
  */
static void
DevSpiInterrupt(int unit)
{
  stm32Dev_SPI        *p;
  devSpiSc_t          *psc;

  if(unit > SPI_MODULE_COUNT) goto fail;
  psc = &spi.sc[unit];

  p = psc->dev;
fail:

  return;
}
/**
  * @brief  interrupt entry point for unit 4
  * @param  noen
  * @retval none
  */
void
DevSpi4Interrupt(void)
{
  DevSpiInterrupt(4);
}
/**
  * @brief  interrupt entry point for unit 5
  * @param  noen
  * @retval none
  */
void
DevSpi5Interrupt(void)
{
  DevSpiInterrupt(5);
}

/**
  * @brief  send routine
  * @param  unit  unit number
  * @param  ptr   the pointer of send data
  * @param  size  send data size
  * @retval result  success/fail
  */
int
DevSpiSend(int unit, uint8_t *ptr, int size)
{
  int                   result = -1;
  devSpiSc_t            *psc;

  if(unit > SPI_MODULE_COUNT) goto fail;
  psc = &spi.sc[unit];
  if(!psc->up) goto fail;

  if(psc->param.dmaTx) {
    result = DevSpiSendDma(psc, ptr, size);
  } else {
    result = DevSpiSendPio(psc, ptr, size);
  }

fail:
  return result;
}
/**
  * @brief  recv routine
  * @param  unit  unit number
  * @param  ptr   the pointer of send data
  * @param  size  recv buffer size
  * @retval size  size of recv
  */
int
DevSpiRecv(int unit, uint8_t *ptr, int size)
{
  int                   result = -1;
  devSpiSc_t            *psc;

  if(unit > SPI_MODULE_COUNT) goto fail;
  psc = &spi.sc[unit];
  if(!psc->up) goto fail;

  if(psc->param.dmaRx) {
    result = DevSpiRecvDma(psc, ptr, size);
  } else {
    result = DevSpiRecvPio(psc, ptr, size);
  }

fail:
  return result;
}

/*********************************
 * pio
 */
/**
  * @brief  send routine PIO
  * @param  psc   pointer of internal descriptor
  * @param  ptr   the pointer of send data
  * @param  size  send data size
  * @retval result  sent size
  */
static int
DevSpiSendPio(devSpiSc_t *psc, uint8_t *ptr, int size)
{
  stm32Dev_SPI          *p;

  int                   sz = 0;

  p = psc->dev;

  p->SR;
  p->CR1 |= SPI_CR1_MSTR_YES;
  p->CR1 |= SPI_CR1_SPE_YES;

  sz = size;
  while(size > 0) {
#ifdef SPI_MODULE_FIFO_NO
    while(!(p->SR & SPI_SR_TXE_MASK));
#else
#ifdef SPI_MODULE_FIFO_YES
    while((p->SR & SPI_SR_FTLVL_MASK) == SPI_SR_FTLVL_FULL);
#else
#error no fifo definition
#endif
#endif
    *(uint8_t *)&p->DR = *ptr++;
    size--;
  }

  while(p->SR & SPI_SR_BSY_MASK);
  p->CR1 &= ~SPI_CR1_SPE_MASK;

fail:
  return sz;
}
/**
  * @brief  recv routine PIO
  * @param  psc   pointer of internal descriptor
  * @param  ptr   the pointer of send data
  * @param  size  recv buffer size
  * @retval size  size of recv
  */
static int
DevSpiRecvPio(devSpiSc_t *psc, uint8_t *ptr, int size)
{
  stm32Dev_SPI          *p;

  int                   sz = 0;

  p = psc->dev;
  /* flush rx buffer */
  while(p->SR & SPI_SR_RXNE_MASK) {
    *(__IO uint8_t *)&p->DR;
  }
  p->SR;

  p->CR1 |= SPI_CR1_RXONLY_YES;
  p->CR1 |= SPI_CR1_MSTR_YES;
  p->CR1 |= SPI_CR1_SPE_YES;

  sz = size;
  for(int i = 0; i < size; i++) {
#ifdef SPI_MODULE_FIFO_NO
    while(!(p->SR & SPI_SR_RXNE_MASK));
#else
#ifdef SPI_MODULE_FIFO_YES
    while(!(p->SR & SPI_SR_FRLVL_MASK));
#else
#error no fifo definition
#endif
#endif
    *ptr++ = *(__IO uint8_t *)&p->DR;
  }
  p->CR1 &= ~SPI_CR1_SPE_MASK;
  while(p->SR & SPI_SR_BSY_MASK);

  if(p->SR & SPI_SR_OVR_MASK) {
    sz = -2;            /* overflow */
    //printf("# DevSpiRecvPio() overflow\r\n");
    //goto fail;
  }
  p->CR1 &= ~SPI_CR1_RXONLY_MASK;

fail:
  return sz;
}


/*********************************
 * dma
 */
const static uint8_t    devSpiSendDmaReqTbl[]    = DMA_REQ_SPITX_TBL;
/**
  * @brief  send routine DMA
  * @param  psc   pointer of internal descriptor
  * @param  ptr   the pointer of send data
  * @param  size  send data size
  * @retval result  sent size
  */
static int
DevSpiSendDma(devSpiSc_t *psc, uint8_t *ptr, int size)
{
  stm32Dev_SPI          *p;
  uint32_t              val;
  int                   chDma;

  p = psc->dev;

  p->SR;
  p->CR1 |= SPI_CR1_MSTR_YES;
  p->CR2 |= SPI_CR2_TXDMAEN_YES;
  p->CR1 |= SPI_CR1_SPE_YES;

  /* start dma */
  {
    devDmaParam_t       param;

    memset(&param, 0, sizeof(param));
    param.req = devSpiSendDmaReqTbl[psc->unit];
    param.a = (void *)  ptr;
    param.b = (void *) &p->DR;
    param.cnt = size;
    param.dirBA = 0;
    param.aInc = 1;
    param.aSize = DEVDMA_SIZE_8BITS;
    param.bSize = DEVDMA_SIZE_8BITS;
    param.intrTC = psc->param.dmaIntr;

    chDma = (devSpiSendDmaReqTbl[psc->unit] >> 4) & 0xf;
    DevDmaInit(1, chDma, &param);
  }

  while(DevDmaIsFinished(1, chDma) != DEV_ERRNO_SUCCESS);
  DevDmaStop(1, chDma);

  while(p->SR & SPI_SR_BSY_MASK);
  p->CR1 &= ~SPI_CR1_SPE_MASK;


fail:
  return size;
}
/**
  * @brief  recv routine DMA
  * @param  psc   pointer of internal descriptor
  * @param  ptr   the pointer of send data
  * @param  size  recv buffer size
  * @retval size  size of recv
  */
static int
DevSpiRecvDma(devSpiSc_t *psc, uint8_t *ptr, int size)
{
  stm32Dev_SPI          *p;
  int                   sz = 0;
  uint32_t              val;

  uint32_t              cfg1 = 0, cfg2 = 0;
  int                   chDma;

  p = psc->dev;

  /* flush rx buffer */
  while(p->SR & SPI_SR_RXNE_MASK) {
    *(__IO uint8_t *)&p->DR;
  }
  p->SR;

  p->CR1 |= SPI_CR1_RXONLY_YES | SPI_CR1_MSTR_YES;
  p->CR2 |= SPI_CR2_RXDMAEN_YES;

  /* start dma */
  paramDmaRx.a = (void *)  ptr;
  paramDmaRx.b = (void *) &p->DR;
  paramDmaRx.cnt = size;
  paramDmaRx.intrTC = psc->param.dmaIntr;

  chDma = (devSpiRecvDmaReqTbl[psc->unit] >> 4) & 0xf;
  DevDmaInitAddrSize(1, chDma, &paramDmaRx);
  DevDmaStart(1, chDma);

  /* start receiving */
  p->CR1 |= SPI_CR1_SPE_YES;

  if(psc->param.dmaNonblock) {
    size = DEV_ERRNO_NONBLOCK;
    goto end;
  }

  while(DevDmaIsFinished(1, chDma) != DEV_ERRNO_SUCCESS);
  DevDmaStop(1, chDma);

  p->CR1 &= ~SPI_CR1_SPE_MASK;
  //while(p->SR & SPI_SR_BSY_MASK);

fail:
  p->CR1 &= ~SPI_CR1_RXONLY_MASK;
end:
  return size;
}


int
DevSpiRecvIsFinishDma(int unit)
{
  int           result = DEV_ERRNO_UNKNOWN;

  devSpiSc_t            *psc;
  stm32Dev_SPI          *p;

  int                   chDma;

  if(unit > SPI_MODULE_COUNT) goto fail;
  psc = &spi.sc[unit];
  if(!psc->up) goto fail;

  p = psc->dev;

  chDma = (devSpiRecvDmaReqTbl[psc->unit] >> 4) & 0xf;

  result = DevDmaIsFinished(1, chDma);

fail:
  return result;
}

int
DevSpiRecvStopDma(int unit)
{
  int                   result = -1;

  devSpiSc_t            *psc;
  stm32Dev_SPI          *p;

  int                   chDma;

  if(unit > SPI_MODULE_COUNT) goto fail;
  psc = &spi.sc[unit];
  if(!psc->up) goto fail;

  p = psc->dev;

  chDma = (devSpiRecvDmaReqTbl[psc->unit] >> 4) & 0xf;
  //while(DevDmaIsFinished(1, chDma) != DEV_ERRNO_SUCCESS);
  DevDmaStop(1, chDma);

  p->CR1 &= ~SPI_CR1_SPE_MASK;
  //while(p->SR & SPI_SR_BSY_MASK);

  result = 0;

fail:
  p->CR1 &= ~SPI_CR1_RXONLY_MASK;

  return result;
}
