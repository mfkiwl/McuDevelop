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

#ifndef _STM32DMA7_H_
#define _STM32DMA7_H_

typedef struct {
  __IO uint32_t CR;
#define DMA_CR_CHSEL_SHIFT         25
#define DMA_CR_CHSEL_MASK          (15 << (DMA_CR_CHSEL_SHIFT))
#define DMA_CR_CHSEL_VAL(x)        (((x) << (DMA_CR_CHSEL_SHIFT)) & DMA_CR_CHSEL_MASK)
#define DMA_CR_MBURST_SHIFT        23
#define DMA_CR_MBURST_MASK         (3 << (DMA_CR_MBURST_SHIFT))
#define DMA_CR_MBURST_SINGLE       (0 << (DMA_CR_MBURST_SHIFT))
#define DMA_CR_MBURST_INCR4        (1 << (DMA_CR_MBURST_SHIFT))
#define DMA_CR_MBURST_INCR8        (2 << (DMA_CR_MBURST_SHIFT))
#define DMA_CR_MBURST_INCR16       (3 << (DMA_CR_MBURST_SHIFT))
#define DMA_CR_PBURST_SHIFT        21
#define DMA_CR_PBURST_MASK         (3 << (DMA_CR_PBURST_SHIFT))
#define DMA_CR_PBURST_SINGLE       (0 << (DMA_CR_PBURST_SHIFT))
#define DMA_CR_PBURST_INCR4        (1 << (DMA_CR_PBURST_SHIFT))
#define DMA_CR_PBURST_INCR8        (2 << (DMA_CR_PBURST_SHIFT))
#define DMA_CR_PBURST_INCR16       (3 << (DMA_CR_PBURST_SHIFT))
#define DMA_CR_CT_SHIFT            19
#define DMA_CR_CT_MASK             (1 << (DMA_CR_CT_SHIFT))
#define DMA_CR_CT_NO               (0 << (DMA_CR_CT_SHIFT))
#define DMA_CR_CT_YES              (1 << (DMA_CR_CT_SHIFT))
#define DMA_CR_DBM_SHIFT           18
#define DMA_CR_DBM_MASK            (1 << (DMA_CR_DBM_SHIFT))
#define DMA_CR_DBM_NO              (0 << (DMA_CR_DBM_SHIFT))
#define DMA_CR_DBM_YES             (1 << (DMA_CR_DBM_SHIFT))
#define DMA_CR_PL_SHIFT            16
#define DMA_CR_PL_MASK             (3 << (DMA_CR_PL_SHIFT))
#define DMA_CR_PL_LOW              (0 << (DMA_CR_PL_SHIFT))
#define DMA_CR_PL_MID              (1 << (DMA_CR_PL_SHIFT))
#define DMA_CR_PL_HID              (2 << (DMA_CR_PL_SHIFT))
#define DMA_CR_PL_VERYHIGH         (3 << (DMA_CR_PL_SHIFT))
#define DMA_CR_PINCOS_SHIFT        15
#define DMA_CR_PINCOS_MASK         (1 << (DMA_CR_PINCOS_SHIFT))
#define DMA_CR_PINCOS_NO           (0 << (DMA_CR_PINCOS_SHIFT))
#define DMA_CR_PINCOS_YES          (1 << (DMA_CR_PINCOS_SHIFT))
#define DMA_CR_MSIZE_SHIFT         13
#define DMA_CR_MSIZE_MASK          (3 << (DMA_CR_MSIZE_SHIFT))
#define DMA_CR_MSIZE_8BITS         (0 << (DMA_CR_MSIZE_SHIFT))
#define DMA_CR_MSIZE_16BITS        (1 << (DMA_CR_MSIZE_SHIFT))
#define DMA_CR_MSIZE_32BITS        (2 << (DMA_CR_MSIZE_SHIFT))
#define DMA_CR_MSIZE_RESERVED      (3 << (DMA_CR_MSIZE_SHIFT))
#define DMA_CR_PSIZE_SHIFT         11
#define DMA_CR_PSIZE_MASK          (3 << (DMA_CR_PSIZE_SHIFT))
#define DMA_CR_PSIZE_8BITS         (0 << (DMA_CR_PSIZE_SHIFT))
#define DMA_CR_PSIZE_16BITS        (1 << (DMA_CR_PSIZE_SHIFT))
#define DMA_CR_PSIZE_32BITS        (2 << (DMA_CR_PSIZE_SHIFT))
#define DMA_CR_PSIZE_RESERVED      (3 << (DMA_CR_PL_SHIFT))
#define DMA_CR_MINC_SHIFT          10
#define DMA_CR_MINC_MASK           (1 << (DMA_CR_MINC_SHIFT))
#define DMA_CR_MINC_NO             (0 << (DMA_CR_MINC_SHIFT))
#define DMA_CR_MINC_YES            (1 << (DMA_CR_MINC_SHIFT))
#define DMA_CR_PINC_SHIFT          9
#define DMA_CR_PINC_MASK           (1 << (DMA_CR_PINC_SHIFT))
#define DMA_CR_PINC_NO             (0 << (DMA_CR_PINC_SHIFT))
#define DMA_CR_PINC_YES            (1 << (DMA_CR_PINC_SHIFT))
#define DMA_CR_CIRC_SHIFT          8
#define DMA_CR_CIRC_MASK           (1 << (DMA_CR_CIRC_SHIFT))
#define DMA_CR_CIRC_NO             (0 << (DMA_CR_CIRC_SHIFT))
#define DMA_CR_CIRC_YES            (1 << (DMA_CR_CIRC_SHIFT))
#define DMA_CR_DIR_SHIFT           6
#define DMA_CR_DIR_MASK            (3 << (DMA_CR_DIR_SHIFT))
#define DMA_CR_DIR_PTOM            (0 << (DMA_CR_DIR_SHIFT))
#define DMA_CR_DIR_MTOP            (1 << (DMA_CR_DIR_SHIFT))
#define DMA_CR_DIR_MTOM            (2 << (DMA_CR_DIR_SHIFT))
#define DMA_CR_PFCTRL_SHIFT        5
#define DMA_CR_PFCTRL_MASK         (1 << (DMA_CR_PFCTRL_SHIFT))
#define DMA_CR_PFCTRL_NO           (0 << (DMA_CR_PFCTRL_SHIFT))
#define DMA_CR_PFCTRL_YES          (1 << (DMA_CR_PFCTRL_SHIFT))
#define DMA_CR_TCIE_SHIFT          4
#define DMA_CR_TCIE_MASK           (1 << (DMA_CR_TCIE_SHIFT))
#define DMA_CR_TCIE_NO             (0 << (DMA_CR_TCIE_SHIFT))
#define DMA_CR_TCIE_YES            (1 << (DMA_CR_TCIE_SHIFT))
#define DMA_CR_HTIE_SHIFT          3
#define DMA_CR_HTIE_MASK           (1 << (DMA_CR_HTIE_SHIFT))
#define DMA_CR_HTIE_NO             (0 << (DMA_CR_HTIE_SHIFT))
#define DMA_CR_HTIE_YES            (1 << (DMA_CR_HTIE_SHIFT))
#define DMA_CR_TEIE_SHIFT          2
#define DMA_CR_TEIE_MASK           (1 << (DMA_CR_TEIE_SHIFT))
#define DMA_CR_TEIE_NO             (0 << (DMA_CR_TEIE_SHIFT))
#define DMA_CR_TEIE_YES            (1 << (DMA_CR_TEIE_SHIFT))
#define DMA_CR_DMEIE_SHIFT         1
#define DMA_CR_DMEIE_MASK          (1 << (DMA_CR_DMEIE_SHIFT))
#define DMA_CR_DMEIE_NO            (0 << (DMA_CR_DMEIE_SHIFT))
#define DMA_CR_DMEIE_YES           (1 << (DMA_CR_DMEIE_SHIFT))
#define DMA_CR_EN_SHIFT            0
#define DMA_CR_EN_MASK             (1 << (DMA_CR_EN_SHIFT))
#define DMA_CR_EN_NO               (0 << (DMA_CR_EN_SHIFT))
#define DMA_CR_EN_YES              (1 << (DMA_CR_EN_SHIFT))

  __IO uint32_t NDTR;
#define DMA_CNDTR_MASK              (0xffff)
  __IO uint32_t PAR;
  __IO uint32_t M0AR;
  __IO uint32_t M1AR;
  __IO uint32_t FCR;
#define DMA_FCR_DMDIS_SHIFT     2
#define DMA_FCR_DMDIS_MASK      (1 << (DMA_FCR_DMDIS_SHIFT))
#define DMA_FCR_DMDIS_NO        (0 << (DMA_FCR_DMDIS_SHIFT))
#define DMA_FCR_DMDIS_YES       (1 << (DMA_FCR_DMDIS_SHIFT))
#define DMA_FCR_FTH_SHIFT       0
#define DMA_FCR_FTH_MASK        (1 << (DMA_FCR_FTH_SHIFT))
#define DMA_FCR_FTH_QUATER      (0 << (DMA_FCR_FTH_SHIFT))
#define DMA_FCR_FTH_HALF        (1 << (DMA_FCR_FTH_SHIFT))
#define DMA_FCR_FTH_3QUATER     (2 << (DMA_FCR_FTH_SHIFT))
#define DMA_FCR_FTH_FULL        (3 << (DMA_FCR_FTH_SHIFT))

} stm32Dev_DMACH;

typedef struct {
  __IO uint32_t LISR;
  __IO uint32_t HISR;
#define DMA_ISR_TEIF_MASK(d)        (1 << ((((d)-1)<<2) + 3))
#define DMA_ISR_HTIF_MASK(d)        (1 << ((((d)-1)<<2) + 2))
#define DMA_ISR_TCIF_MASK(d)        (1 << ((((d)-1)<<2) + 1))
#define DMA_ISR_GIF_MASK(d)         (1 << ((((d)-1)<<2) + 0))

  __IO uint32_t LIFCR;
  __IO uint32_t HIFCR;
#define DMA_ISR_CTEIF_CLEAR(d)      (1 << ((((d)-1)<<2) + 3))
#define DMA_ISR_CHTIF_CLEAR(d)      (1 << ((((d)-1)<<2) + 2))
#define DMA_ISR_CTCIF_CLEAR(d)      (1 << ((((d)-1)<<2) + 1))
#define DMA_ISR_CGIF_CLEAR(d)       (1 << ((((d)-1)<<2) + 0))
#define DMA_ISR_ALL_CLEAR(d)        (15 << ((((d)-1)<<2) + 0))

  stm32Dev_DMACH        S[8];

} stm32Dev_DMA;

#endif
