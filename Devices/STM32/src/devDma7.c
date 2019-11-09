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

#define _DEV_DMA7_C_

#include        <stdio.h>
#include        <string.h>

#include        "system.h"
#include        "devDma7.h"

struct _stDma         dma;

/*
 * unit: 1=DMA1, 2=DMA2, 3=BDMA
 * ch:   0--15
 *
 */
int
DevDmaInit(int unit, int ch, devDmaParam_t *param)
{
  int                   result = -1;
  uint32_t              cr = 0;

  __IO uint32_t         *fcr;

  devDmaSc_t            *psc;
  stm32Dev_DMA          *p;
  int                   aSize;

  psc = &dma.sc[unit];

  if(unit == -1) {
    memset(&dma, 0, sizeof(dma));
#ifdef DMA1_PTR
    dma.sc[DMA1_NUM].dev = DMA1_PTR;
#endif
#ifdef DMA2_PTR
    dma.sc[DMA2_NUM].dev = DMA2_PTR;
#endif
    result = 0;
    goto end;
  }

  if(unit == 3) {
    printf("#DevDmaInit() BDMA is not supported yet\r\n");
    goto end;
  }

  p  = dma.sc[unit].dev;

  /* disable dma */
  p->S[ch].CR   = 0;

  if(ch&4) {
    fcr = (uint32_t *)&p->HIFCR;
  } else {
    fcr = (uint32_t *)&p->LIFCR;
  }

  aSize = param->aSize;
  if(param->aSize == DEVDMA_SIZE_32BITS) aSize = sizeof(uint32_t);

  /* channel */
  cr |= DMA_CR_CHSEL_VAL(param->req);
  /* direction */
  if(param->mem) {
    cr |= DMA_CR_DIR_MTOM;
  } else {
    if(param->dirBA) cr |= DMA_CR_DIR_PTOM;
    else             cr |= DMA_CR_DIR_MTOP;
  }
  /* increment */
  if(param->aInc) {
    cr |= DMA_CR_MINC_YES;
    switch(param->aInc) {
    case  1: cr |= DMA_CR_MBURST_SINGLE; break;
    case  4: cr |= DMA_CR_MBURST_INCR4;  break;
    case  8: cr |= DMA_CR_MBURST_INCR8;  break;
    case 16: cr |= DMA_CR_MBURST_INCR16; break;
    }
  }
  if(param->bInc) {
    cr |= DMA_CR_PINC_YES;
    switch(param->bInc) {
    case  1: cr |= DMA_CR_PBURST_SINGLE; break;
    case  4: cr |= DMA_CR_PBURST_INCR4;  break;
    case  8: cr |= DMA_CR_PBURST_INCR8;  break;
    case 16: cr |= DMA_CR_PBURST_INCR16; break;
    }
  }
  /* size */
  switch(param->aSize) {
  case DEVDMA_SIZE_8BITS:  cr |= DMA_CR_MSIZE_8BITS;  break;
  case DEVDMA_SIZE_16BITS: cr |= DMA_CR_MSIZE_16BITS; break;
  case DEVDMA_SIZE_32BITS: cr |= DMA_CR_MSIZE_32BITS; break;
  }
  switch(param->bSize) {
  case DEVDMA_SIZE_8BITS:  cr |= DMA_CR_PSIZE_8BITS;  break;
  case DEVDMA_SIZE_16BITS: cr |= DMA_CR_PSIZE_16BITS; break;
  case DEVDMA_SIZE_32BITS: cr |= DMA_CR_PSIZE_32BITS; break;
  }
  /* circular mode */
  if(param->circ) cr |= DMA_CR_CIRC_YES;
  /* flow control mode */
  if(param->flow) cr |= DMA_CR_PFCTRL_YES;
  /* interrupts */
  if(param->intrTC) cr |= DMA_CR_TCIE_YES;
  if(param->intrHC) cr |= DMA_CR_HTIE_YES;
  if(param->intrER) cr |= DMA_CR_TEIE_YES;
  psc->cr = cr;

  /* set to registers */
  p->S[ch].PAR  = (uint32_t)param->b;
  p->S[ch].M0AR = (uint32_t)param->a;
  p->S[ch].M1AR = (uint32_t)param->a + param->cnt*aSize/2;
  p->S[ch].NDTR = param->cnt;
  p->S[ch].FCR  = DMA_FCR_FTH_FULL;
  p->S[ch].CR   = psc->cr;

  /* clear interrupt status */
  *fcr = 0x3f << ( (ch << 3) - ((ch&1)<<1) );

end:
  result = 0;

  return result;
}


/*
 * param->a
 * param->b
 * param->cnt
 */
int
DevDmaInitAddrSize(int unit, int ch, devDmaParam_t *param)
{
  int                   result = -1;
  devDmaSc_t            *psc;
  stm32Dev_DMA          *p;
  __IO uint32_t         *fcr;

  psc = &dma.sc[unit];
  p  = dma.sc[unit].dev;

  p->S[ch].CR   = 0;

  /* set to registers */
  p->S[ch].PAR  = (uint32_t)param->b;
  p->S[ch].M0AR = (uint32_t)param->a;
  p->S[ch].M1AR = (uint32_t)(param->a + param->cnt/2);
  p->S[ch].NDTR = param->cnt;
  //p->S[ch].FCR  = DMA_FCR_FTH_FULL;
  p->S[ch].CR   = psc->cr;

  if(ch&4) {
    fcr = (uint32_t *)&p->HIFCR;
  } else {
    fcr = (uint32_t *)&p->LIFCR;
  }
  *fcr = 0x3f << ( (ch << 3) - ((ch&1)<<1) );

  //printf("xx dma size %x %x %x %x %x\n", p->S[ch].NDTR, p->S[ch].CR, p->S[ch].FCR, p->S[ch].PAR, p->S[ch].M0AR);

end:
  result = 0;

  return result;
}


int
DevDmaStart(int unit, int ch)
{
  int                   result = -1;
  stm32Dev_DMA          *p;
  __IO uint32_t         *fcr;
  uint32_t              val;

  if(unit == 3) {
    printf("#DevDmaStart() BDMA is not supported yet\r\n");
    goto end;
  }

  p  = dma.sc[unit].dev;

  if(ch&4) {
    fcr = &p->HIFCR;
  } else {
    fcr = &p->LIFCR;
  }

  /* clear interrupt status */
  *fcr = 0x3f << ( (ch << 3) - ((ch&1)<<1) );

  /* enable */
  p->S[ch].CR |= DMA_CR_EN_YES;

end:
  result = 0;

  return result;
}


int
DevDmaStop(int unit, int ch)
{
  int                   result = -1;
  stm32Dev_DMA          *p;

  if(unit == 3) {
    printf("#DevDmaStop() BDMA is not supported yet\r\n");
    goto end;
  }

  p  = dma.sc[unit].dev;

  /* disable */
  p->S[ch&7].CR &= ~DMA_CR_EN_MASK;

end:
  result = 0;

  return result;
}


int
DevDmaIsFinished(int unit, int ch)
{
  int           result = 0;
  stm32Dev_DMA  *p;
  __IO uint32_t *isr;

  if(unit == 3) {
    printf("#DevDmaIsFinished() BDMA is not supported yet\r\n");
    goto end;
  }

  p  = dma.sc[unit].dev;

  if(ch&4) {
    isr = &p->HISR;
  } else {
    isr = &p->LISR;
  }

  if(*isr & (0x20<<( (ch << 3) - ((ch&1)<<1) )) ) {
    result = 1;       /* finished, if NDT is 0 */
  }

end:
  return result;
}


int
DevDmaGetTransferedCount(int unit, int ch)
{
  int           result = 0;
  stm32Dev_DMA  *p;

  if(unit == 3) {
    printf("#DevDmaIsFinished() BDMA is not supported yet\r\n");
    goto end;
  }

  p  = dma.sc[unit].dev;
  result = 0xffff - p->S[ch].NDTR;

end:
  return result;
}


void
DevDmaDebugShowRegs(int unit, int ch)
{
  stm32Dev_DMA          *p;

  //if(unit == 2) ch |= (1<<3);

  p  = dma.sc[unit].dev;
  /*p += ch & 7;*/


  printf("cr: %x,  fcr %x, mux.ccr: %x\r\n", p->S[ch&7].CR, p->S[ch&7].FCR, 0);
  printf("par: %x, m0ar: %x, m1ar: %x, cnt %x\r\n", p->S[ch&7].PAR, p->S[ch&7].M0AR, p->S[ch&7].M1AR, p->S[ch&7].NDTR);

  return;
}
