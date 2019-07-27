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

#ifndef _SI5351_H_
#define _SI5351_H_

#define SI5351_SUCCESS          (0)
#define SI5351_ERRNO_UNKNOWN    (-1)

/* I2C address */
#define SI5351_I2C_ADDR0        (0x60)
#define SI5351_I2C_ADDR1        (0x61)

#define SI5331_REG_DEVSTATUS    0x00
#define SI5331_REG_INTRSTICKY   0x01
#define SI5331_REG_INTRMASK     0x02
#define         SI5331_INTRMASK_ALL            0xff
#define SI5331_REG_OUTPUT_DIS   0x03
#define         SI5331_OUTPUT_DIS_BIT(x)       ((1) << (x))
#define         SI5331_OUTPUT_EN_BIT(x)        (~((1) << (x)))
#define         SI5331_OUTPUT_DIS_ALL          (0xff)
#define SI5331_REG_OEB_MASK     0x09
#define SI5331_REG_PLL_SOURCE   0x0f
#define SI5331_REG_CLK0_CTRL    0x10
#define SI5331_REG_CLK1_CTRL    0x11
#define SI5331_REG_CLK2_CTRL    0x12
#define SI5331_REG_CLK3_CTRL    0x13
#define SI5331_REG_CLK4_CTRL    0x14
#define SI5331_REG_CLK5_CTRL    0x15
#define SI5331_REG_CLK6_CTRL    0x16
#define SI5331_REG_CLK7_CTRL    0x17
#define         SI5331_CLK_PDN_SHIFT    7
#define         SI5331_CLK_PDN_MASK     (1<<(SI5331_CLK_PDN_SHIFT))
#define         SI5331_CLK_PDN_NO       (0<<(SI5331_CLK_PDN_SHIFT))
#define         SI5331_CLK_PDN_YES      (1<<(SI5331_CLK_PDN_SHIFT))
#define         SI5331_CLK_SRC_SHIFT    2
#define         SI5331_CLK_SRC_MASK     (3<<(SI5331_CLK_SRC_SHIFT))
#define         SI5331_CLK_SRC_XTAL     (0<<(SI5331_CLK_SRC_SHIFT))
#define         SI5331_CLK_SRC_CLKIN    (1<<(SI5331_CLK_SRC_SHIFT))
#define         SI5331_CLK_SRC_MULTSYNTH_0 (2<<(SI5331_CLK_SRC_SHIFT))
#define         SI5331_CLK_SRC_MULTSYNTH_N (3<<(SI5331_CLK_SRC_SHIFT))
#define SI5331_REG_CLK30DIS     0x18
#define SI5331_REG_CLK74DIS     0x19

#define SI5331_REG_MSNA_P3M     0x1a
#define SI5331_REG_MSNA_P3L     0x1b
#define SI5331_REG_MSNA_P1H     0x1c
#define SI5331_REG_MSNA_P1M     0x1d
#define SI5331_REG_MSNA_P1L     0x1e
#define SI5331_REG_MSNA_P3HP2L  0x1f
#define SI5331_REG_MSNA_P2M     0x20
#define SI5331_REG_MSNA_P2L     0x21

#define SI5331_REG_MSNB_P3M     0x22
#define SI5331_REG_MSNB_P3L     0x23
#define SI5331_REG_MSNB_P1H     0x24
#define SI5331_REG_MSNB_P1M     0x25
#define SI5331_REG_MSNB_P1L     0x26
#define SI5331_REG_MSNB_P3HP2H  0x27
#define SI5331_REG_MSNB_P2M     0x28
#define SI5331_REG_MSNB_P2L     0x29

#define SI5331_MSN_P3M     0
#define SI5331_MSN_P3L     1
#define SI5331_MSN_P1H     2
#define SI5331_MSN_P1M     3
#define SI5331_MSN_P1L     4
#define SI5331_MSN_P3HP2H  5
#define SI5331_MSN_P2M     6
#define SI5331_MSN_P2L     7
#define SI5331_MSN_LEN     8

#define SI5331_REG_MS0_P3M      0x2a
#define SI5331_REG_MS0_P3L      0x2b
#define SI5331_REG_MS0_P1H_DIV  0x2c
#define SI5331_REG_MS0_P1M      0x2d
#define SI5331_REG_MS0_P1L      0x2e
#define SI5331_REG_MS0_P3HP2H   0x2f
#define SI5331_REG_MS0_P2M      0x30
#define SI5331_REG_MS0_P2L      0x31

#define SI5331_REG_MS1_P3M      0x32
#define SI5331_REG_MS1_P3L      0x33
#define SI5331_REG_MS1_P1H_DIV  0x34
#define SI5331_REG_MS1_P1M      0x35
#define SI5331_REG_MS1_P1L      0x36
#define SI5331_REG_MS1_P3HP2H   0x37
#define SI5331_REG_MS1_P2M      0x38
#define SI5331_REG_MS1_P2L      0x39

#define SI5331_REG_MS2_P3M      0x3a
#define SI5331_REG_MS2_P3L      0x3b
#define SI5331_REG_MS2_P1H_DIV  0x3c
#define SI5331_REG_MS2_P1M      0x3d
#define SI5331_REG_MS2_P1L      0x3e
#define SI5331_REG_MS2_P3HP2H   0x3f
#define SI5331_REG_MS2_P2M      0x40
#define SI5331_REG_MS2_P2L      0x41

#define SI5331_MS_P3M     0
#define SI5331_MS_P3L     1
#define SI5331_MS_P1H_DIV 2
#define SI5331_MS_P1M     3
#define SI5331_MS_P1L     4
#define SI5331_MS_P3HP2H  5
#define SI5331_MS_P2M     6
#define SI5331_MS_P2L     7
#define SI5331_MS_LEN     8

#define SI5331_REG_SSDN_P2H_EN    149
#define SI5331_REG_SSDN_P2L       150
#define SI5331_REG_SSDN_P3H_MODE  141
#define SI5331_REG_SSDN_P3H       152
#define SI5331_REG_SSDN_P3L       153
#define SI5331_REG_SSDN_UDPH_P1H  154
#define SI5331_REG_SSDN_UDPL      155
#define SI5331_REG_SSUP_P2H     156
#define SI5331_REG_SSUP_P2L     157
#define SI5331_REG_SSUP_P3H     158
#define SI5331_REG_SSUP_P3L     159
#define SI5331_REG_SSUP_P1L     160
#define SI5331_REG_SSUP_P1H     161
#define SI5331_REG_VCXO_L       162
#define SI5331_REG_VCXO_M       163
#define SI5331_REG_VCXO_H       164
#define SI5331_REG_CLK0_PF      165
#define SI5331_REG_CLK1_PF      166
#define SI5331_REG_CLK2_PF      167
#define SI5331_REG_CLK3_PF      168
#define SI5331_REG_CLK4_PF      169
#define SI5331_REG_CLK5_PF      170
#define SI5331_REG_PLL_RST      177
#define         SI5331_PLLB_RST_SHIFT   7
#define         SI5331_PLLB_RST_MASK    (1<<(SI5331_PLLB_RST_SHIFT))
#define         SI5331_PLLB_RST_NO      (0<<(SI5331_PLLB_RST_SHIFT))
#define         SI5331_PLLB_RST_YES     (1<<(SI5331_PLLB_RST_SHIFT))
#define         SI5331_PLLA_RST_SHIFT   5
#define         SI5331_PLLA_RST_MASK    (1<<(SI5331_PLLA_RST_SHIFT))
#define         SI5331_PLLA_RST_NO      (0<<(SI5331_PLLA_RST_SHIFT))
#define         SI5331_PLLA_RST_YES     (1<<(SI5331_PLLA_RST_SHIFT))

#define SI5331_REG_XTAL_CL      183
#define SI5331_REG_CLK_CTRL     187


struct _stSi5351MultiSynth {
  uint32_t      num;            // num > denomi
  uint32_t      denomi;

  uint32_t      a;
  uint32_t      b;
  uint32_t      c;

  uint32_t      p1;
  uint32_t      p2;
  uint32_t      p3;
};


int             Si5351Init(int unit,
                           uint32_t in,
                           uint32_t vco0,       /* PLLA */
                           uint32_t vco1,       /* PLLB */
                           uint32_t out0, uint32_t out1, uint32_t out2,
                           uint32_t selVco
                           );

#ifdef  _SI5351_C_
static int      Si5351CalcVco(struct _stSi5351MultiSynth *p);
static int      Si5351CalcMultiSynth(struct _stSi5351MultiSynth *p);
static int      Si5351GetMultiSynthValue(struct _stSi5351MultiSynth *p);
static uint32_t Si5351Gcd(uint32_t a, uint32_t b);
static void     Si5351FillBufVco(uint8_t *buf, struct _stSi5351MultiSynth *p, int vco);
static void     Si5351FillBufDiv(uint8_t *buf, struct _stSi5351MultiSynth *p, uint32_t outdiv);

static int      Si5351Send(int unit, int reg, uint8_t *ptr, int size);

#endif


#endif
