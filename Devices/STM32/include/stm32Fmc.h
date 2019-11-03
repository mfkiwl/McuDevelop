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

#ifndef _STM32FMC_H_
#define _STM32FMC_H_

typedef struct {
  __IO uint32_t         CR;
#define FMC_BCR_FMCEN_SHIFT     (31)
#define FMC_BCR_FMCEN_MASK      (1 << (FMC_BCR_FMCEN_SHIFT))
#define FMC_BCR_FMCEN_NO        (0 << (FMC_BCR_FMCEN_SHIFT))
#define FMC_BCR_FMCEN_YES       (1 << (FMC_BCR_FMCEN_SHIFT))
#define FMC_BCR_WFDIS_SHIFT     (21)
#define FMC_BCR_WFDIS_MASK      (1 << (FMC_BCR_WFDIS_SHIFT))
#define FMC_BCR_WFDIS_NO        (0 << (FMC_BCR_WFDIS_SHIFT))
#define FMC_BCR_WFDIS_YES       (1 << (FMC_BCR_WFDIS_SHIFT))
#define FMC_BCR_CCLKEN_SHIFT    (20)
#define FMC_BCR_CCLKEN_MASK     (1 << (FMC_BCR_CCLKEN_SHIFT))
#define FMC_BCR_CCLKEN_NO       (0 << (FMC_BCR_CCLKEN_SHIFT))
#define FMC_BCR_CCLKEN_YES      (1 << (FMC_BCR_CCLKEN_SHIFT))
#define FMC_BCR_CBURSTRW_SHIFT  (19)
#define FMC_BCR_CBURSTRW_MASK   (1 << (FMC_BCR_CBURSTRW_SHIFT))
#define FMC_BCR_CBURSTRW_NO     (0 << (FMC_BCR_CBURSTRW_SHIFT))
#define FMC_BCR_CBURSTRW_YES    (1 << (FMC_BCR_CBURSTRW_SHIFT))
#define FMC_BCR_CPSIZE_SHIFT    (16)
#define FMC_BCR_CPSIZE_MASK     (7 << (FMC_BCR_CPSIZE_SHIFT))
#define FMC_BCR_CPSIZE_NOBURST  (0 << (FMC_BCR_CPSIZE_SHIFT))
#define FMC_BCR_CPSIZE_128      (1 << (FMC_BCR_CPSIZE_SHIFT))
#define FMC_BCR_CPSIZE_256      (2 << (FMC_BCR_CPSIZE_SHIFT))
#define FMC_BCR_CPSIZE_1024     (4 << (FMC_BCR_CPSIZE_SHIFT))
#define FMC_BCR_ASYNCWAIT_SHIFT (15)
#define FMC_BCR_ASYNCWAIT_MASK  (1 << (FMC_BCR_ASYNCWAIT_SHIFT))
#define FMC_BCR_ASYNCWAIT_NO    (0 << (FMC_BCR_ASYNCWAIT_SHIFT))
#define FMC_BCR_ASYNCWAIT_YES   (1 << (FMC_BCR_ASYNCWAIT_SHIFT))
#define FMC_BCR_EXTMOD_SHIFT    (14)
#define FMC_BCR_EXTMOD_MASK     (1 << (FMC_BCR_EXTMOD_SHIFT))
#define FMC_BCR_EXTMOD_NO       (0 << (FMC_BCR_EXTMOD_SHIFT))
#define FMC_BCR_EXTMOD_YES      (1 << (FMC_BCR_EXTMOD_SHIFT))
#define FMC_BCR_WAITEN_SHIFT    (13)
#define FMC_BCR_WAITEN_MASK     (1 << (FMC_BCR_WAITEN_SHIFT))
#define FMC_BCR_WAITEN_NO       (0 << (FMC_BCR_WAITEN_SHIFT))
#define FMC_BCR_WAITEN_YES      (1 << (FMC_BCR_WAITEN_SHIFT))
#define FMC_BCR_WREN_SHIFT      (12)
#define FMC_BCR_WREN_MASK       (1 << (FMC_BCR_WREN_SHIFT))
#define FMC_BCR_WREN_NO         (0 << (FMC_BCR_WREN_SHIFT))
#define FMC_BCR_WREN_YES        (1 << (FMC_BCR_WREN_SHIFT))
#define FMC_BCR_MWID_SHIFT      (4)
#define FMC_BCR_MWID_MASK       (3 << (FMC_BCR_MWID_SHIFT))
#define FMC_BCR_MWID_8BIT       (0 << (FMC_BCR_MWID_SHIFT))
#define FMC_BCR_MWID_16BIT      (1 << (FMC_BCR_MWID_SHIFT))
#define FMC_BCR_MWID_32BIT      (2 << (FMC_BCR_MWID_SHIFT))
#define FMC_BCR_MWID_RESERVED   (3 << (FMC_BCR_MWID_SHIFT))
#define FMC_BCR_MTYP_SHIFT      (2)
#define FMC_BCR_MTYP_MASK       (3 << (FMC_BCR_MTYP_SHIFT))
#define FMC_BCR_MTYP_SRAM       (0 << (FMC_BCR_MTYP_SHIFT))
#define FMC_BCR_MTYP_PSRAM      (1 << (FMC_BCR_MTYP_SHIFT))
#define FMC_BCR_MTYP_NORFLASH   (2 << (FMC_BCR_MTYP_SHIFT))
#define FMC_BCR_MTYP_RESERVED   (3 << (FMC_BCR_MTYP_SHIFT))
#define FMC_BCR_MUX_SHIFT       (1)
#define FMC_BCR_MUXEN_MASK      (1 << (FMC_BCR_MUXEN_SHIFT))
#define FMC_BCR_MUXEN_NO        (0 << (FMC_BCR_MUXEN_SHIFT))
#define FMC_BCR_MUXEN_YES       (1 << (FMC_BCR_MUXEN_SHIFT))
#define FMC_BCR_MBKEN_SHIFT     (0)
#define FMC_BCR_MBKEN_MASK      (1 << (FMC_BCR_MBKEN_SHIFT))
#define FMC_BCR_MBKEN_NO        (0 << (FMC_BCR_MBKEN_SHIFT))
#define FMC_BCR_MBKEN_YES       (1 << (FMC_BCR_MBKEN_SHIFT))

  __IO uint32_t         TR;
#define FMC_BTR_ACCMOD_SHIFT    (28)
#define FMC_BTR_ACCMOD_MASK     (3    << (FMC_BTR_ACCMOD_SHIFT))
#define FMC_BTR_ACCMOD_A        (0    << (FMC_BTR_ACCMOD_SHIFT))
#define FMC_BTR_ACCMOD_B        (1    << (FMC_BTR_ACCMOD_SHIFT))
#define FMC_BTR_ACCMOD_C        (2    << (FMC_BTR_ACCMOD_SHIFT))
#define FMC_BTR_ACCMOD_D        (3    << (FMC_BTR_ACCMOD_SHIFT))
#define FMC_BTR_DATAST_SHIFT    (8)
#define FMC_BTR_DATAST_MASK     (0xff << (FMC_BTR_DATAST_SHIFT))
#define FMC_BTR_DATAST_MAX      (0xff << (FMC_BTR_DATAST_SHIFT))
#define FMC_BTR_DATAST(x)       ((x)  << (FMC_BTR_DATAST_SHIFT))
#define FMC_BTR_ADDHLD_SHIFT    (4)
#define FMC_BTR_ADDHLD_MASK     (0xf  << (FMC_BTR_ADDHLD_SHIFT))
#define FMC_BTR_ADDHLD_MAX      (0xf  << (FMC_BTR_ADDHLD_SHIFT))
#define FMC_BTR_ADDHLD(x)       ((x)  << (FMC_BTR_ADDHLD_SHIFT))
#define FMC_BTR_ADDSET_SHIFT    (0)
#define FMC_BTR_ADDSET_MASK     (0xf  << (FMC_BTR_ADDSET_SHIFT))
#define FMC_BTR_ADDSET_MAX      (0xf  << (FMC_BTR_ADDSET_SHIFT))
#define FMC_BTR_ADDSET(x)       ((x)  << (FMC_BTR_ADDSET_SHIFT))
} stm32Dev_FMC_bank;

typedef struct {
#define STM32FMC_NUMBEROF_BANK          4
  stm32Dev_FMC_bank     B[STM32FMC_NUMBEROF_BANK];           /*  0x00 --  0x1c */
  uint32_t              reserved20[0x18];
  __IO uint32_t         PCR;            /* 0x80 */
  __IO uint32_t         SR;             /* 0x84 */
  __IO uint32_t         PATT;           /* 0x88 */
  uint32_t              reserved90[2];
  __IO uint32_t         ECCR;           /* 0x94 */
  uint32_t              reserved9c[0x2a];
  stm32Dev_FMC_bank     BW[4];          /* 0x100 -- 0x11c */
  uint32_t              reserved120[8];
  __IO uint32_t         SDCR1;          /* 0x140 */
  __IO uint32_t         SDCR2;          /* 0x144 */
  __IO uint32_t         SDTR1;          /* 0x148 */
  __IO uint32_t         SDTR2;          /* 0x14c */
  __IO uint32_t         SDCMR;          /* 0x150 */
  __IO uint32_t         SDRTR;          /* 0x154 */
  __IO uint32_t         SDSR;           /* 0x148 */


  __IO uint32_t         BCR[4];           /* 0x00 */

} stm32Dev_FMC;

#endif
