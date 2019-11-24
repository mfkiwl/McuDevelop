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

#define _DEV_ADC_F730_C_

#include        <string.h>

#include        "system.h"
#include        "devCounter.h"
#include        "devAdcF730.h"

struct _stAdc         adc;

int
DevAdcInit(int unit, int ch, devAdcParam_t *param)
{
  int                   result = -1;
  uint32_t              cr1 = 0, cr2 = 0;
  uint32_t              sampleTime = 0;
  devAdcSc_t            *psc;
  stm32Dev_ADC          *p;
  uint32_t              val;

  if(unit == ADC_NUM_INIT) {
    memset(&adc, 0, sizeof(adc));
    adc.sc[ADC1_NUM].dev = ADC1_PTR;
#ifdef  ADC2_PTR
    adc.sc[ADC2_NUM].dev = ADC2_PTR;
#endif
#ifdef  ADC3_PTR
    adc.sc[ADC3_NUM].dev = ADC3_PTR;
#endif
    goto end;
  }

  psc = &adc.sc[unit];
  p = psc->dev;

  psc->cb = param->cb;

  /*** AD ENABLE */
  p->CR1 = 0;
  p->CR2 = ADC_CR2_ADON_YES;

  /* CCR */
  if(unit == ADC1_NUM) {
    ADC1_PTR->CCR = (ADC_CCR_TSVREFE_YES |
              ADC_CCR_ADCPRE_PCLK2_DIV8 |
              ADC_CCR_DELAY_VAL(2) |
              ADC_CCR_MULTI_INDEPENDENT
              );
  }

  /*** CR2 */
  if(param->resolution > 12) {
    cr1  |= ADC_CR1_RES_12BIT;
  } else if(param->resolution > 10) {
    cr1  |= ADC_CR1_RES_10BIT;
  } else if(param->resolution > 8) {
    cr1  |= ADC_CR1_RES_8BIT;
  } else {
    cr1  |= ADC_CR1_RES_6BIT;
  }
#ifdef  ADC_CR2_ALIGN_LEFT
  cr2 |= ADC_CR2_ALIGN_LEFT;
#endif


  uint32_t      chbit;
  int           cnt;
  psc->chbit = param->chbit;
  chbit = psc->chbit;

  p->SQR1 = 0;
  p->SQR2 = 0;
  p->SQR3 = 0;

#if 1
  cnt = 0;
  //chbit >>= 1;
  ch = 0;
  while(chbit) {
    if(chbit & 1) {
#if 0
      /* SQR */
      if(       cnt <= 6) {
        p->SQR3 |= ch << ((cnt   ) * ADC_SQR1_SQ2_SHIFT);
        printf("XXX1 ch%d %d %x\n", ch, cnt, ch << ((cnt   ) * ADC_SQR1_SQ2_SHIFT));
      } else if(cnt <= 12) {
        p->SQR2 |= ch << ((cnt- 7) * ADC_SQR1_SQ2_SHIFT);
        printf("XXX2 ch%d %d %x\n", ch, cnt, ch << ((cnt- 7) * ADC_SQR1_SQ2_SHIFT));
      } else if(cnt <= 18) {
        p->SQR1 |= ch << ((cnt-13) * ADC_SQR1_SQ2_SHIFT);
        printf("XXX3 ch%d %d %x %d\n", ch, cnt, ch << ((cnt-13) * ADC_SQR1_SQ2_SHIFT), ((ch-13) * ADC_SQR1_SQ2_SHIFT));
      }

      /*** SMPR */
      /* 480 ADC clk */
      sampleTime = ADC_SMPR_SMP_480CLKS;
      if(ch <= 9) {
        val  = p->SMPR1;
        val &= ~ADC_SMPR1_SMP_CH9_CH0_MASK(ch);
        val |=  ADC_SMPR1_SMP_CH9_CH0(ch, sampleTime);
        p->SMPR1 |= val;
      } else {
        val  = p->SMPR2;
        val &= ~ADC_SMPR2_SMP_CH18_CH10_MASK(ch);
        val |=  ADC_SMPR2_SMP_CH18_CH10(ch, sampleTime);
        p->SMPR2 |= val;
      }
#endif

      cnt++;
    }
    chbit >>= 1;
    ch++;
  }
  psc->cntCh = cnt;

#if 0
  if(cnt >= 1) {
    p->SQR1 |= ADC_SQR1_L_VAL(cnt-1);
  }
#endif
#endif

  //cr1 |= ADC_CR1_SCAN_YES | ADC_CR1_AWDEN_YES;
  //cr2 |= ADC_CR2_EOCS_EACHCONV;
  //cr1 |= ADC_CR1_DISCNUM_VAL(cnt-1) | ADC_CR1_DISCEN_YES;

  p->CR1 |= cr1;
  if(param->continuous) cr2 |= ADC_CR2_CONT_YES;
  p->CR2 |= cr2;


  // sampling interval
  p->SMPR1 = ADC_SMPR1_SMP_ALLSLOW;
  p->SMPR2 = ADC_SMPR2_SMP_ALLSLOW;

  // enable interrupt
  if(param->intr) {
    p->CR1 |= ADC_CR1_EOCIE_YES;
  }

#if 0
  printf("cr1 %x/%x, cr2 %x/%x, smpr1 %x, smpr2 %x\r\n",
         p->CR1, cr1, p->CR2, cr2, p->SMPR1, p->SMPR2);
  printf("sqr1 %x sqr2 %x sqr3 %x \r\n",
         p->SQR1, p->SQR2, p->SQR3);
  printf("ccr %x, cr %x\r\n", p->CCR, p->CR1);
#endif

  result = 0;

end:
  return result;
}


int
DevAdcStartConvert(int unit)
{
  int                   result = -1;
  devAdcSc_t            *psc;
  stm32Dev_ADC          *p;
  int                   ch;
  uint32_t              bit;
  int                   num;

  psc = &adc.sc[unit];
  p = psc->dev;

  if(!psc->chbit) goto fail;

  /*** Search the first bit */
  bit = psc->chbit;
  num = 0;
  while(1) {
    if(bit & 1) break;
    bit >>= 1;
    num++;
  }

  psc->numCh = num;
  p->SQR3 = num;

  /*** CR ADSTART */
  p->CR2 |= ADC_CR2_SWSTART_YES;

  /*** Search next bit */
  psc->nextChBit = bit<<1;
  psc->nextCh = num+1;
  while(1) {
    if(psc->nextChBit) {
      if(psc->nextChBit & 1) {
        break;
      }
      psc->nextChBit >>= 1;
      psc->nextCh++;
    } else {
      psc->nextChBit = psc->chbit;
      psc->nextCh = 0;
    }
  }

fail:
  return result;
}


int
DevAdcGetData(int unit, int ch, uint16_t *ptr)
{
  int                   result = -1;
  devAdcSc_t            *psc;

  if(!ptr) goto fail;

  psc = &adc.sc[unit];
  *ptr = psc->val[ch];

  result = 0;
fail:
  return result;
}


int
DevAdcGetDataPointer(int unit, int ch, uint16_t **ptr)
{
}


void
DevAdcInterrupt1(void)
{
  DevAdcInterrupt(ADC1_NUM);
  return;
}

static void
DevAdcInterrupt(int unit)
{
  devAdcSc_t            *psc;
  stm32Dev_ADC          *p;
  uint32_t              intr;
  int                   ch;

  psc = &adc.sc[unit];
  p = psc->dev;

  intr = p->SR;
  p->SR = ~intr;

  if(intr & ADC_SR_EOC_MASK) {
    /*** get converted data */
    psc->val[psc->numCh] = p->DR;

    /*** CR ADSTART */
    psc->numCh = psc->nextCh;
    p->SQR3 = psc->nextCh;
    p->CR2 |= ADC_CR2_SWSTART_YES;

    /*** Search next bit */
    psc->nextChBit >>= 1;
    psc->nextCh++;
    while(1) {
      if(psc->nextChBit) {
        if(psc->nextChBit & 1) {
          break;
        }
        psc->nextChBit >>= 1;
        psc->nextCh++;
      } else {
        psc->nextChBit = psc->chbit;
        psc->nextCh = 0;
      }
    }
  }

#if 0
  if(intr & ADC_SR_STRT_MASK) {
  }

  if(intr & ADC_SR_AWD_MASK) {
  }
#endif

  return;
}
