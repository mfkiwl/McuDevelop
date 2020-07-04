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

#define _SI5351_C_

#include        <string.h>
#include        <stdlib.h>
#include        <stdint.h>

#include        "config.h"
#include        "stm32f7.h"
#include        "system.h"
#include        "devI2c.h"

#include        "si5351.h"


#define Si5351ConvertABC2REG(p) \
  (p)->p1 = ((p)->a<<7) + ((p)->b<<7)/(p)->c - 512;  \
  (p)->p2 = ((p)->b<<7) - (p)->c * ((p)->b<<7)/(p)->c;  \
  (p)->p3 = (p)->c;




static const uint8_t     Si5351InitParam0[] = {
  /* output disable */
  SI5331_REG_OUTPUT_DIS, SI5331_OUTPUT_DIS_ALL,
  /* enter the power down mode */
  SI5331_REG_CLK0_CTRL, SI5331_CLK_PDN_YES,
  SI5331_REG_CLK1_CTRL, SI5331_CLK_PDN_YES,
  SI5331_REG_CLK2_CTRL, SI5331_CLK_PDN_YES,
  SI5331_REG_CLK3_CTRL, SI5331_CLK_PDN_YES,
  SI5331_REG_CLK4_CTRL, SI5331_CLK_PDN_YES,
  SI5331_REG_CLK5_CTRL, SI5331_CLK_PDN_YES,
  SI5331_REG_CLK6_CTRL, SI5331_CLK_PDN_YES,
  SI5331_REG_CLK7_CTRL, SI5331_CLK_PDN_YES,
  /* disable interrupt */
  SI5331_REG_INTRMASK, SI5331_INTRMASK_ALL
};
static const uint8_t     Si5351InitParam1[] = {
  /* PLLA reset */
  SI5331_REG_PLL_RST, SI5331_PLLA_RST_YES,

  /* enable output  ch0, ch2 */
  SI5331_REG_CLK0_CTRL, SI5331_CLK_PDN_NO | SI5331_CLK_SRC_MULTSYNTH_N,
  SI5331_REG_CLK2_CTRL, SI5331_CLK_PDN_NO | SI5331_CLK_SRC_MULTSYNTH_N,
  SI5331_REG_OUTPUT_DIS, SI5331_OUTPUT_EN_BIT(0) & SI5331_OUTPUT_EN_BIT(2),
};

int
Si5351Init(int unit,
           int      fInit,
           uint32_t in,
           uint32_t vco0,       /* PLLA */
           uint32_t vco1,       /* PLLB */
           uint32_t out0, uint32_t out1, uint32_t out2,
           uint32_t selVco
           )
{
  int           result = SI5351_ERRNO_UNKNOWN;
  devI2cPkt             pkt;
  const uint8_t         *p;
  uint8_t               reg, data;
  uint8_t               addr;

  struct _stSi5351MultiSynth val;
  uint8_t       buf[16];
  int           outdiv = 1;

  addr = CONFIG_I2C_ADDR_SI5351;

  /* initialize */
  if(fInit) {
    p = Si5351InitParam0;
    for(int i = 0; i < sizeof(Si5351InitParam0)/2; i++) {
      reg  = *p++;
      data = *p++;
      Si5351Send(0, reg, &data, 1);
    }
  }

  /* PLL settings */

  val.denomi  = in;
  if(in > 0 && vco0 > 0) {
    memset(buf, 0, SI5331_MSN_LEN);
    val.num = vco0;
    Si5351CalcVco(&val);
    Si5351ConvertABC2REG(&val);
    Si5351FillBufVco(buf, &val, 0);
    Si5351Send(0, SI5331_REG_MSNA_P3M, buf, SI5331_MSN_LEN);
  }

  if(in > 0 && vco1 > 0) {
    memset(buf, 0, SI5331_MSN_LEN);
    val.num = vco1;
    Si5351CalcVco(&val);
    Si5351ConvertABC2REG(&val);
    Si5351FillBufVco(buf, &val, outdiv);
    Si5351Send(0, SI5331_REG_MSNB_P3M, buf, SI5331_MSN_LEN);
  }

  /* MultiSynth settings */
  if(out0 > 0) {
    memset(buf, 0, SI5331_MSN_LEN);
    val.num  = (selVco&1)? vco1: vco0;
    val.denomi = out0 << outdiv;
    Si5351CalcMultiSynth(&val);
    Si5351ConvertABC2REG(&val);
    Si5351FillBufDiv(buf, &val, outdiv);
    Si5351Send(0, SI5331_REG_MS0_P3M, buf, SI5331_MSN_LEN);
  }

  if(out1 > 0) {
    memset(buf, 0, SI5331_MSN_LEN);
    val.num  = (selVco&2)? vco1: vco0;
    val.denomi = out1 << outdiv;
    Si5351CalcMultiSynth(&val);
    Si5351ConvertABC2REG(&val);
    Si5351FillBufDiv(buf, &val, outdiv);
    Si5351Send(0, SI5331_REG_MS1_P3M, buf, SI5331_MSN_LEN);
  }

  if(out2 > 0) {
    memset(buf, 0, SI5331_MSN_LEN);
    val.num  = (selVco&4)? vco1: vco0;
    val.denomi = out2 << outdiv;
    Si5351CalcMultiSynth(&val);
    Si5351ConvertABC2REG(&val);
    Si5351FillBufDiv(buf, &val, outdiv);
    Si5351Send(0, SI5331_REG_MS2_P3M, buf, SI5331_MSN_LEN);
  }

  if(fInit) {
    /* enable output */
    p = Si5351InitParam1;
    for(int i = 0; i < sizeof(Si5351InitParam1)/2; i++) {
      reg  = *p++;
      data = *p++;
      Si5351Send(0, reg, &data, 1);
    }
  }

  result = SI5351_SUCCESS;

fail:
  return result;
}


static int
Si5351CalcVco(struct _stSi5351MultiSynth *p)
{
  int           result = SI5351_ERRNO_UNKNOWN;
  int           re;

  re = Si5351GetMultiSynthValue(p);
  if(re < 0) goto fail;

  if(p->a < 15 || p->a > 90) goto fail;

  result = SI5351_SUCCESS;

fail:
  return result;
}


static int
Si5351CalcMultiSynth(struct _stSi5351MultiSynth *p)
{
  int           result = SI5351_ERRNO_UNKNOWN;
  int           re;

  re = Si5351GetMultiSynthValue(p);
  if(re < 0) goto fail;

  //if(p->a < 8 || p->a > 900) goto fail;
  if(p->a < 6 || p->a > 1800 || (p->a == 1800 & p->b > 0)) goto fail;

  result = SI5351_SUCCESS;

fail:
  return result;
}



static int
Si5351GetMultiSynthValue(struct _stSi5351MultiSynth *p)
{
  int           result = SI5351_ERRNO_UNKNOWN;
  uint32_t      a, b, c;
  uint32_t      val;
  uint32_t      gcd;
  float         f;

  a = p->num / p->denomi;
  c = p->denomi;
  b = p->num - a * c;

#if 0
  gcd = Si5351Gcd(b, c);
  b /= gcd;
  c /= gcd;
#endif

  f = (float)b * 1000000.0;
  f = f / (float) c;
  b = (int)f;
  c = 1000000;

  printf("xx abc %d %d %d\n", a, b, c);

  if(b > 1048575) goto fail;
  if(c > 1048575) goto fail;

  p->a = a;
  p->b = b;
  p->c = c;

  result = SI5351_SUCCESS;

fail:
  return result;
}


static uint32_t
Si5351Gcd(uint32_t a, uint32_t b)
{
  uint32_t      t;

  t = a % b;
  if (t == 0) {
    return b;
  } else {
    return Si5351Gcd(b, t);
  }
}


/*
 * vco: 0=VCO0, 1=VCO1
 */
static void
Si5351FillBufVco(uint8_t *buf, struct _stSi5351MultiSynth *p, int vco)
{
  buf[SI5331_MSN_P3M]    = ( (p)->p3 >>  8) & 0xff;
  buf[SI5331_MSN_P3L]    = ( (p)->p3      ) & 0xff;
  if(vco == 0) {
    buf[SI5331_MSN_P1H]  = ( (p)->p1 >> 16) & 0x03;
  } else {
    buf[SI5331_MSN_P1H]  = ( (p)->p1 >> 10) & 0x03;
  }
  buf[SI5331_MSN_P1M]    = ( (p)->p1 >>  8) & 0xff;
  buf[SI5331_MSN_P1L]    = ( (p)->p1      ) & 0xff;
  buf[SI5331_MSN_P3HP2H] = (((p)->p3 >> 12) & 0xf0) | (((p)->p2 >>  16) & 0x0f);
  buf[SI5331_MSN_P2M]    = ( (p)->p2 >>  8) & 0xff;
  buf[SI5331_MSN_P2L]    = ( (p)->p2      ) & 0xff;

  return;
}


static void
Si5351FillBufDiv(uint8_t *buf, struct _stSi5351MultiSynth *p, uint32_t outdiv)
{
  buf[SI5331_MS_P3M]     = ( (p)->p3 >>  8) & 0xff;
  buf[SI5331_MS_P3L]     = ( (p)->p3      ) & 0xff;
  buf[SI5331_MS_P1H_DIV] = (((p)->p1 >> 16) & 0x03) | ((outdiv << 4) & 0x70);
  buf[SI5331_MS_P1M]     = ( (p)->p1 >>  8) & 0xff;
  buf[SI5331_MS_P1L]     = ( (p)->p1      ) & 0xff;
  buf[SI5331_MS_P3HP2H]  = (((p)->p3 >> 12) & 0xf0) | (((p)->p2 >>  16) & 0x0f);
  buf[SI5331_MS_P2M]     = ( (p)->p2 >>  8) & 0xff;
  buf[SI5331_MS_P2L]     = ( (p)->p2      ) & 0xff;

  return;
}


static int
Si5351Send(int unit, int reg, uint8_t *ptr, int size)
{
  int           result = SI5351_ERRNO_UNKNOWN;
  int           re;
  uint8_t       c;

  devI2cPkt     pkt;

  c = reg;

  pkt.addr = CONFIG_I2C_ADDR_SI5351;
  pkt.ptrCmd = &c;
  pkt.lenCmd = 1;
  pkt.ptrSend = ptr;
  pkt.lenSend = size;
  pkt.ptrRecv = NULL;
  pkt.lenRecv = 0;

  re = DevI2cTransmit(I2C2_NUM, &pkt);
  if(re < 0) goto fail;

  result = SI5351_SUCCESS;

fail:
  return result;

}
