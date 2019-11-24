/*
 * Copyright (c) 2018, 2019 zhtlab
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

#ifndef _STM32ADC_F730_
#define _STM32ADC_F730_

typedef struct {
  __IO uint32_t         SR;            /* 0x00 */
#define ADC_SR_OVR_SHIFT        (5)
#define ADC_SR_OVR_MASK         (1 << (ADC_SR_OVR_SHIFT))
#define ADC_SR_STRT_SHIFT       (4)
#define ADC_SR_STRT_MASK        (1 << (ADC_SR_STRT_SHIFT))
#define ADC_SR_JSTRT_SHIFT      (3)
#define ADC_SR_JSTRT_MASK       (1 << (ADC_SR_JSTRT_SHIFT))
#define ADC_SR_JEOC_SHIFT       (2)
#define ADC_SR_JEOC_MASK        (1 << (ADC_SR_JEOC_SHIFT))
#define ADC_SR_EOC_SHIFT        (1)
#define ADC_SR_EOC_MASK         (1 << (ADC_SR_EOC_SHIFT))
#define ADC_SR_AWD_SHIFT        (0)
#define ADC_SR_AWD_MASK         (1 << (ADC_SR_AWD_SHIFT))

  __IO uint32_t         CR1;            /* 0x04 */
#define ADC_CR1_RES_SHIFT       (24)
#define ADC_CR1_RES_MASK        (1 << (ADC_CR1_RES_SHIFT))
#define ADC_CR1_RES_12BIT       (0 << (ADC_CR1_RES_SHIFT))
#define ADC_CR1_RES_10BIT       (1 << (ADC_CR1_RES_SHIFT))
#define ADC_CR1_RES_8BIT        (2 << (ADC_CR1_RES_SHIFT))
#define ADC_CR1_RES_6BIT        (3 << (ADC_CR1_RES_SHIFT))
#define ADC_CR1_AWDEN_SHIFT     (23)
#define ADC_CR1_AWDEN_MASK      (1 << (ADC_CR1_AWDEN_SHIFT))
#define ADC_CR1_AWDEN_NO        (0 << (ADC_CR1_AWDEN_SHIFT))
#define ADC_CR1_AWDEN_YES       (1 << (ADC_CR1_AWDEN_SHIFT))
#define ADC_CR1_DISCNUM_SHIFT   (13)
#define ADC_CR1_DISCNUM_MASK    (7 << (ADC_CR1_DISCNUM_SHIFT))
#define ADC_CR1_DISCNUM_VAL(x)  (((x) << (ADC_CR1_DISCNUM_SHIFT)) & ADC_CR1_DISCNUM_MASK)
#define ADC_CR1_DISCEN_SHIFT    (11)
#define ADC_CR1_DISCEN_MASK     (1 << (ADC_CR1_DISCEN_SHIFT))
#define ADC_CR1_DISCEN_NO       (0 << (ADC_CR1_DISCEN_SHIFT))
#define ADC_CR1_DISCEN_YES      (1 << (ADC_CR1_DISCEN_SHIFT))
#define ADC_CR1_SCAN_SHIFT      (8)
#define ADC_CR1_SCAN_MASK       (1 << (ADC_CR1_SCAN_SHIFT))
#define ADC_CR1_SCAN_NO         (0 << (ADC_CR1_SCAN_SHIFT))
#define ADC_CR1_SCAN_YES        (1 << (ADC_CR1_SCAN_SHIFT))
#define ADC_CR1_JEOCIE_SHIFT    (7)
#define ADC_CR1_JEOCIE_MASK     (1 << (ADC_CR1_JEOCIE_SHIFT))
#define ADC_CR1_JEOCIE_NO       (0 << (ADC_CR1_JEOCIE_SHIFT))
#define ADC_CR1_JEOCIE_YES      (1 << (ADC_CR1_JEOCIE_SHIFT))
#define ADC_CR1_AWDIE_SHIFT     (6)
#define ADC_CR1_AWDIE_MASK      (1 << (ADC_CR1_AWDIE_SHIFT))
#define ADC_CR1_AWDIE_NO        (0 << (ADC_CR1_AWDIE_SHIFT))
#define ADC_CR1_AWDIE_YES       (1 << (ADC_CR1_AWDIE_SHIFT))
#define ADC_CR1_EOCIE_SHIFT     (5)
#define ADC_CR1_EOCIE_MASK      (1 << (ADC_CR1_EOCIE_SHIFT))
#define ADC_CR1_EOCIE_NO        (0 << (ADC_CR1_EOCIE_SHIFT))
#define ADC_CR1_EOCIE_YES       (1 << (ADC_CR1_EOCIE_SHIFT))
#define ADC_CR1_AWDCH_SHIFT     (0)
#define ADC_CR1_AWDCH_MASK      (0x1f << (ADC_CR1_AWDCH_SHIFT))
#define ADC_CR1_AWDCH_VAL(ch)   ((ch) << (ADC_CR1_AWDCH_SHIFT))

  __IO uint32_t         CR2;            /* 0x08 */
#define ADC_CR2_SWSTART_SHIFT   (30)
#define ADC_CR2_SWSTART_MASK    (1 << (ADC_CR2_SWSTART_SHIFT))
#define ADC_CR2_SWSTART_NO      (0 << (ADC_CR2_SWSTART_SHIFT))
#define ADC_CR2_SWSTART_YES     (1 << (ADC_CR2_SWSTART_SHIFT))
#define ADC_CR2_EXTEN_SHIFT     (28)
#define ADC_CR2_EXTEN_MASK      (3 << (ADC_CR2_EXTEN_SHIFT))
#define ADC_CR2_EXTEN_DIS       (0 << (ADC_CR2_EXTEN_SHIFT))
#define ADC_CR2_EXTEN_RISING    (1 << (ADC_CR2_EXTEN_SHIFT))
#define ADC_CR2_EXTEN_FALLING   (2 << (ADC_CR2_EXTEN_SHIFT))
#define ADC_CR2_EXTEN_BOTH      (3 << (ADC_CR2_EXTEN_SHIFT))
#define ADC_CR2_ALIGN_SHIFT     (11)
#define ADC_CR2_ALIGN_MASK      (1 << (ADC_CR2_ALIGN_SHIFT))
#define ADC_CR2_ALIGN_RIGHT     (0 << (ADC_CR2_ALIGN_SHIFT))
#define ADC_CR2_ALIGN_LEFT      (1 << (ADC_CR2_ALIGN_SHIFT))
#define ADC_CR2_EOCS_SHIFT      (10)
#define ADC_CR2_EOCS_MASK       (1 << (ADC_CR2_EOCS_SHIFT))
#define ADC_CR2_EOCS_EACHSEQ    (0 << (ADC_CR2_EOCS_SHIFT))
#define ADC_CR2_EOCS_EACHCONV   (1 << (ADC_CR2_EOCS_SHIFT))
#define ADC_CR2_DDS_SHIFT       (9)
#define ADC_CR2_DDS_MASK        (1 << (ADC_CR2_DDS_SHIFT))
#define ADC_CR2_DDS_NO          (0 << (ADC_CR2_DDS_SHIFT))
#define ADC_CR2_DDS_YES         (1 << (ADC_CR2_DDS_SHIFT))
#define ADC_CR2_DMA_SHIFT       (8)
#define ADC_CR2_DMA_MASK        (1 << (ADC_CR2_DMA_SHIFT))
#define ADC_CR2_DMA_DIS         (0 << (ADC_CR2_DMA_SHIFT))
#define ADC_CR2_DMA_EN          (1 << (ADC_CR2_DMA_SHIFT))
#define ADC_CR2_CONT_SHIFT      (1)
#define ADC_CR2_CONT_MASK       (1 << (ADC_CR2_CONT_SHIFT))
#define ADC_CR2_CONT_NO         (0 << (ADC_CR2_CONT_SHIFT))
#define ADC_CR2_CONT_YES        (1 << (ADC_CR2_CONT_SHIFT))
#define ADC_CR2_ADON_SHIFT      (0)
#define ADC_CR2_ADON_MASK       (1 << (ADC_CR2_ADON_SHIFT))
#define ADC_CR2_ADON_NO         (0 << (ADC_CR2_ADON_SHIFT))
#define ADC_CR2_ADON_YES        (1 << (ADC_CR2_ADON_SHIFT))

  __IO uint32_t         SMPR1;          /* 0x0c */
  __IO uint32_t         SMPR2;          /* 0x10 */
#define ADC_SMPR_SMP_SHIFT      (0)
#define ADC_SMPR_SMP_MASK       (7 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR_SMP_3CLKS      (0 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR_SMP_15CLKS     (1 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR_SMP_28CLKS     (2 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR_SMP_56CLKS     (3 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR_SMP_84CLKS     (4 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR_SMP_112CLKS    (5 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR_SMP_144CLKS    (6 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR_SMP_480CLKS    (7 << (ADC_SMPR_SMP_SHIFT))
#define ADC_SMPR1_SMP_ALLSLOW    (0x07ffffff)
#define ADC_SMPR2_SMP_ALLSLOW    (0x3fffffff)
#define ADC_SMPR1_SMP_CH9_CH0(ch, val)    ((val) << (3* (ch)    ))
#define ADC_SMPR1_SMP_CH9_CH0_MASK(ch)    (7     << (3* (ch)    ))
#define ADC_SMPR2_SMP_CH18_CH10(ch, val)  ((val) << (3*((ch)-10)))
#define ADC_SMPR2_SMP_CH18_CH10_MASK(ch)  (7     << (3*((ch)-10)))

  __IO uint32_t         JORF[4];        /* 0x14 -- 0x20 */
  __IO uint32_t         HTR1;           /* 0x24 */
  __IO uint32_t         LTR1;           /* 0x28 */
  __IO uint32_t         SQR1;           /* 0x2c */
  __IO uint32_t         SQR2;           /* 0x30 */
  __IO uint32_t         SQR3;           /* 0x34 */
#define ADC_SQR1_L_SHIFT         (20)
#define ADC_SQR1_L_MASK          (0x1f << (ADC_SQR1_L_SHIFT))
#define ADC_SQR1_L_VAL(x)        ((x) << (ADC_SQR1_L_SHIFT) & ADC_SQR1_L_MASK)
#define ADC_SQR1_SQ1_SHIFT       (0)
#define ADC_SQR1_SQ1_MASK        (0x1f << (ADC_SQR1_SQ1_SHIFT))
#define ADC_SQR1_SQ1_VAL(x)      (((x) << (ADC_SQR1_SQ1_SHIFT)) & ADC_SQR1_SQ1_MASK)
#define ADC_SQR1_SQ2_SHIFT       (5)
#define ADC_SQR1_SQ2_MASK        (0xf << (ADC_SQR1_SQ2_SHIFT))
#define ADC_SQR1_SQ2_VAL(x)      (((x) << (ADC_SQR1_SQ2_SHIFT)) & ADC_SQR1_SQ2_MASK)

#define ADC_SQR_TEMPETURE       16
#define ADC_SQR_VREFINT         17
#define ADC_SQR_VBATT           18

  __IO uint32_t         JSQR;           /* 0x38 */
  __IO uint32_t         JDR[4];         /* 0x3c -- 0x48 */
  __IO uint32_t         DR;             /* 0x4c */
  uint32_t              reserved28[0xac];

  /* common settings */
  __IO uint8_t          CSR[4];         /* 0x300 */
#define ADC_CSR_OVR_SHIFT       (5)
#define ADC_CSR_OVR_MASK        (1 << (ADC_CSR_OVR_SHIFT))
#define ADC_CSR_OVR_NO          (0 << (ADC_CSR_OVR_SHIFT))
#define ADC_CSR_OVR_YES         (1 << (ADC_CSR_OVR_SHIFT))
#define ADC_CSR_STRT_SHIFT      (4)
#define ADC_CSR_STRT_MASK       (1 << (ADC_CSR_STRT_SHIFT))
#define ADC_CSR_STRT_NO         (0 << (ADC_CSR_STRT_SHIFT))
#define ADC_CSR_STRT_YES        (1 << (ADC_CSR_STRT_SHIFT))
#define ADC_CSR_JSTRT_SHIFT     (3)
#define ADC_CSR_JSTRT_MASK      (1 << (ADC_CSR_JSTRT_SHIFT))
#define ADC_CSR_JSTRT_NO        (0 << (ADC_CSR_JSTRT_SHIFT))
#define ADC_CSR_JSTRT_YES       (1 << (ADC_CSR_JSTRT_SHIFT))
#define ADC_CSR_JEOC_SHIFT      (2)
#define ADC_CSR_JEOC_MASK       (1 << (ADC_CSR_JEOC_SHIFT))
#define ADC_CSR_JEOC_NO         (0 << (ADC_CSR_JEOC_SHIFT))
#define ADC_CSR_JEOC_YES        (1 << (ADC_CSR_JEOC_SHIFT))
#define ADC_CSR_EOC_SHIFT       (1)
#define ADC_CSR_EOC_MASK        (1 << (ADC_CSR_EOC_SHIFT))
#define ADC_CSR_EOC_NO          (0 << (ADC_CSR_EOC_SHIFT))
#define ADC_CSR_EOC_YES         (1 << (ADC_CSR_EOC_SHIFT))
#define ADC_CSR_AWD_SHIFT       (0)
#define ADC_CSR_AWD_MASK        (1 << (ADC_CSR_AWD_SHIFT))
#define ADC_CSR_AWD_NO          (0 << (ADC_CSR_AWD_SHIFT))
#define ADC_CSR_AWD_YES         (1 << (ADC_CSR_AWD_SHIFT))

  __IO uint32_t         CCR;            /* 0x304 */
#define ADC_CCR_TSVREFE_SHIFT   (23)
#define ADC_CCR_TSVREFE_MASK    (1 << (ADC_CCR_TSVREFE_SHIFT))
#define ADC_CCR_TSVREFE_NO      (0 << (ADC_CCR_TSVREFE_SHIFT))
#define ADC_CCR_TSVREFE_YES     (1 << (ADC_CCR_TSVREFE_SHIFT))
#define ADC_CCR_VBATE_SHIFT     (22)
#define ADC_CCR_VBATE_MASK      (1 << (ADC_CCR_VBATE_SHIFT))
#define ADC_CCR_VBATE_NO        (0 << (ADC_CCR_VBATE_SHIFT))
#define ADC_CCR_VBATE_YES       (1 << (ADC_CCR_VBATE_SHIFT))
#define ADC_CCR_ADCPRE_SHIFT    (16)
#define ADC_CCR_ADCPRE_MASK     (3 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_ADCPRE_PCLK2_DIV2 (0 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_ADCPRE_PCLK2_DIV4 (1 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_ADCPRE_PCLK2_DIV6 (2 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_ADCPRE_PCLK2_DIV8 (3 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_DAM_SHIFT       (14)
#define ADC_CCR_DAM_MASK        (3 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DAM_DUALDAC     (0 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DAM_RESERVED    (1 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DAM_10BIT       (2 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DAM_8BIT        (3 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DELAY_SHIFT     (8)
#define ADC_CCR_DELAY_MASK      (0xf << (ADC_CCR_DELAY_SHIFT))
#define ADC_CCR_DELAY_VAL(x)    ((x) << (ADC_CCR_DELAY_SHIFT))
#define ADC_CCR_MULTI_SHIFT      (0)
#define ADC_CCR_MULTI_MASK       (0x1f << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_INDEPENDENT (0 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_COMBINE_INJECTED  (1 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_COMBINE_ALTERNATE (2 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_COMBINE_INTER_ALT (3 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_RESERVED4         (4 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_INJECTED          (5 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_REGULAR           (6 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_INTEREAVED        (7 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_RESERVED8         (8 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_ALTERNATE_TRIGGER (9 << (ADC_CCR_MULTI_SHIFT))

  __IO uint32_t         CDR;            /* 0x308 */

} stm32Dev_ADC;


/* common settings */
typedef struct {
  __IO uint8_t          CSR[4];         /* 0x300 */
#define ADC_CSR_OVR_SHIFT       (5)
#define ADC_CSR_OVR_MASK        (1 << (ADC_CSR_OVR_SHIFT))
#define ADC_CSR_OVR_NO          (0 << (ADC_CSR_OVR_SHIFT))
#define ADC_CSR_OVR_YES         (1 << (ADC_CSR_OVR_SHIFT))
#define ADC_CSR_STRT_SHIFT      (4)
#define ADC_CSR_STRT_MASK       (1 << (ADC_CSR_STRT_SHIFT))
#define ADC_CSR_STRT_NO         (0 << (ADC_CSR_STRT_SHIFT))
#define ADC_CSR_STRT_YES        (1 << (ADC_CSR_STRT_SHIFT))
#define ADC_CSR_JSTRT_SHIFT     (3)
#define ADC_CSR_JSTRT_MASK      (1 << (ADC_CSR_JSTRT_SHIFT))
#define ADC_CSR_JSTRT_NO        (0 << (ADC_CSR_JSTRT_SHIFT))
#define ADC_CSR_JSTRT_YES       (1 << (ADC_CSR_JSTRT_SHIFT))
#define ADC_CSR_JEOC_SHIFT      (2)
#define ADC_CSR_JEOC_MASK       (1 << (ADC_CSR_JEOC_SHIFT))
#define ADC_CSR_JEOC_NO         (0 << (ADC_CSR_JEOC_SHIFT))
#define ADC_CSR_JEOC_YES        (1 << (ADC_CSR_JEOC_SHIFT))
#define ADC_CSR_EOC_SHIFT       (1)
#define ADC_CSR_EOC_MASK        (1 << (ADC_CSR_EOC_SHIFT))
#define ADC_CSR_EOC_NO          (0 << (ADC_CSR_EOC_SHIFT))
#define ADC_CSR_EOC_YES         (1 << (ADC_CSR_EOC_SHIFT))
#define ADC_CSR_AWD_SHIFT       (0)
#define ADC_CSR_AWD_MASK        (1 << (ADC_CSR_AWD_SHIFT))
#define ADC_CSR_AWD_NO          (0 << (ADC_CSR_AWD_SHIFT))
#define ADC_CSR_AWD_YES         (1 << (ADC_CSR_AWD_SHIFT))

  __IO uint32_t         CCR;            /* 0x304 */
#define ADC_CCR_TSVREFE_SHIFT   (23)
#define ADC_CCR_TSVREFE_MASK    (1 << (ADC_CCR_TSVREFE_SHIFT))
#define ADC_CCR_TSVREFE_NO      (0 << (ADC_CCR_TSVREFE_SHIFT))
#define ADC_CCR_TSVREFE_YES     (1 << (ADC_CCR_TSVREFE_SHIFT))
#define ADC_CCR_VBATE_SHIFT     (22)
#define ADC_CCR_VBATE_MASK      (1 << (ADC_CCR_VBATE_SHIFT))
#define ADC_CCR_VBATE_NO        (0 << (ADC_CCR_VBATE_SHIFT))
#define ADC_CCR_VBATE_YES       (1 << (ADC_CCR_VBATE_SHIFT))
#define ADC_CCR_ADCPRE_SHIFT    (16)
#define ADC_CCR_ADCPRE_MASK     (3 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_ADCPRE_PCLK2_DIV2 (0 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_ADCPRE_PCLK2_DIV4 (1 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_ADCPRE_PCLK2_DIV6 (2 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_ADCPRE_PCLK2_DIV8 (3 << (ADC_CCR_ADCPRE_SHIFT))
#define ADC_CCR_DAM_SHIFT       (14)
#define ADC_CCR_DAM_MASK        (3 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DAM_DUALDAC     (0 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DAM_RESERVED    (1 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DAM_10BIT       (2 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DAM_8BIT        (3 << (ADC_CCR_DAM_SHIFT))
#define ADC_CCR_DELAY_SHIFT     (8)
#define ADC_CCR_DELAY_MASK      (0xf << (ADC_CCR_DELAY_SHIFT))
#define ADC_CCR_DELAY_VAL(x)    ((x) << (ADC_CCR_DELAY_SHIFT))
#define ADC_CCR_MULTI_SHIFT      (0)
#define ADC_CCR_MULTI_MASK       (0x1f << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_INDEPENDENT (0 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_COMBINE_INJECTED  (1 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_COMBINE_ALTERNATE (2 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_COMBINE_INTER_ALT (3 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_RESERVED4         (4 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_INJECTED          (5 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_REGULAR           (6 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_INTEREAVED        (7 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_RESERVED8         (8 << (ADC_CCR_MULTI_SHIFT))
#define ADC_CCR_MULTI_ALTERNATE_TRIGGER (9 << (ADC_CCR_MULTI_SHIFT))

  __IO uint32_t         CDR;            /* 0x308 */

} stm32Dev_ADC_COM;


#endif
