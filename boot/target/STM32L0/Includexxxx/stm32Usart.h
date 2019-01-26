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

#ifndef _STM32USART_H_
#define _STM32USART_H_

typedef struct {
  __IO uint32_t         CR1;
#define USART_CR1_RXFFIE_SHIFT		31
#define USART_CR1_RXFFIE_MASK		(1 << (USART_CR1_RXFFIE_SHIFT))
#define USART_CR1_RXFFIE_NO		(0 << (USART_CR1_RXFFIE_SHIFT))
#define USART_CR1_RXFFIE_YES		(1 << (USART_CR1_RXFFIE_SHIFT))
#define USART_CR1_TXFEIE_SHIFT		30
#define USART_CR1_TXFEIE_MASK		(1 << (USART_CR1_TXFEIE_SHIFT))
#define USART_CR1_TXFEIE_NO		(0 << (USART_CR1_TXFEIE_SHIFT))
#define USART_CR1_TXFEIE_YES		(1 << (USART_CR1_TXFEIE_SHIFT))
#define USART_CR1_FIFOEN_SHIFT		29
#define USART_CR1_FIFOEN_MASK		(1 << (USART_CR1_FIFOEN_SHIFT))
#define USART_CR1_FIFOEN_NO		(0 << (USART_CR1_FIFOEN_SHIFT))
#define USART_CR1_FIFOEN_YES		(1 << (USART_CR1_FIFOEN_SHIFT))
#define USART_CR1_M1_SHIFT		28
#define USART_CR1_M1_MASK		(1 << (USART_CR1_M1_SHIFT))
#define USART_CR1_M1_NO		(0 << (USART_CR1_M1_SHIFT))
#define USART_CR1_M1_YES		(1 << (USART_CR1_M1_SHIFT))
#define USART_CR1_EOBIE_SHIFT		27
#define USART_CR1_EOBIE_MASK		(1 << (USART_CR1_EOBIE_SHIFT))
#define USART_CR1_EOBIE_NO		(0 << (USART_CR1_EOBIE_SHIFT))
#define USART_CR1_EOBIE_YES		(1 << (USART_CR1_EOBIE_SHIFT))
#define USART_CR1_RTOIE_SHIFT		26
#define USART_CR1_RTOIE_MASK		(1 << (USART_CR1_RTOIE_SHIFT))
#define USART_CR1_RTOIE_NO		(0 << (USART_CR1_RTOIE_SHIFT))
#define USART_CR1_RTOIE_YES		(1 << (USART_CR1_RTOIE_SHIFT))
#define USART_CR1_DEAT_SHIFT		21
#define USART_CR1_DEAT_MASK		(0x1f << (USART_CR1_DEDT_SHIFT))
#define USART_CR1_DEDT_SHIFT		16
#define USART_CR1_DEDT_MASK		(0x1f << (USART_CR1_DEDT_SHIFT))
#define USART_CR1_OVER8_SHIFT		15
#define USART_CR1_OVER8_MASK		(1 << (USART_CR1_OVER8_SHIFT))
#define USART_CR1_OVER8_NO		(0 << (USART_CR1_OVER8_SHIFT))
#define USART_CR1_OVER8_YES		(1 << (USART_CR1_OVER8_SHIFT))
#define USART_CR1_CMIE_SHIFT		14
#define USART_CR1_CMIE_MASK		(1 << (USART_CR1_CMIE_SHIFT))
#define USART_CR1_CMIE_NO		(0 << (USART_CR1_CMIE_SHIFT))
#define USART_CR1_CMIE_YES		(1 << (USART_CR1_CMIE_SHIFT))
#define USART_CR1_MMIE_SHIFT		13
#define USART_CR1_MMIE_MASK		(1 << (USART_CR1_MMIE_SHIFT))
#define USART_CR1_MMIE_NO		(0 << (USART_CR1_MMIE_SHIFT))
#define USART_CR1_MMIE_YES		(1 << (USART_CR1_MMIE_SHIFT))
#define USART_CR1_M0_SHIFT		12
#define USART_CR1_M0_MASK		(1 << (USART_CR1_M0_SHIFT))
#define USART_CR1_M0_NO		(0 << (USART_CR1_M0_SHIFT))
#define USART_CR1_M0_YES		(1 << (USART_CR1_M0_SHIFT))
#define USART_CR1_M_MASK		((USART_CR1_M1_MASK) | (USART_CR1_M0_MASK))
#define USART_CR1_M_7BIT		((USART_CR1_M1_MASK)               )
#define USART_CR1_M_8BIT		(0                                 )
#define USART_CR1_M_9BIT		(               (USART_CR1_M0_MASK))
#define USART_CR1_WAKE_SHIFT		11
#define USART_CR1_WAKE_MASK		(1 << (USART_CR1_WAKE_SHIFT))
#define USART_CR1_WAKE_NO		(0 << (USART_CR1_WAKE_SHIFT))
#define USART_CR1_WAKE_YES		(1 << (USART_CR1_WAKE_SHIFT))
#define USART_CR1_PCE_SHIFT		10
#define USART_CR1_PCE_MASK		(1 << (USART_CR1_PCE_SHIFT))
#define USART_CR1_PCE_NO		(0 << (USART_CR1_PCE_SHIFT))
#define USART_CR1_PCE_YES		(1 << (USART_CR1_PCE_SHIFT))
#define USART_CR1_PS_SHIFT		9
#define USART_CR1_PS_MASK		(1 << (USART_CR1_PS_SHIFT))
#define USART_CR1_PS_EVEN		(0 << (USART_CR1_PS_SHIFT))
#define USART_CR1_PS_ODD		(1 << (USART_CR1_PS_SHIFT))
#define USART_CR1_PEIE_SHIFT		8
#define USART_CR1_PEIE_MASK		(1 << (USART_CR1_PEIE_SHIFT))
#define USART_CR1_PEIE_NO		(0 << (USART_CR1_PEIE_SHIFT))
#define USART_CR1_PEIE_YES		(1 << (USART_CR1_PEIE_SHIFT))
#define USART_CR1_TXEIE_SHIFT		7
#define USART_CR1_TXEIE_MASK		(1 << (USART_CR1_TXEIE_SHIFT))
#define USART_CR1_TXEIE_NO		(0 << (USART_CR1_TXEIE_SHIFT))
#define USART_CR1_TXEIE_YES		(1 << (USART_CR1_TXEIE_SHIFT))
#define USART_CR1_TCIE_SHIFT		6
#define USART_CR1_TCIE_MASK		(1 << (USART_CR1_TCIE_SHIFT))
#define USART_CR1_TCIE_NO		(0 << (USART_CR1_TCIE_SHIFT))
#define USART_CR1_TCIE_YES		(1 << (USART_CR1_TCIE_SHIFT))
#define USART_CR1_RXNEIE_SHIFT	5
#define USART_CR1_RXNEIE_MASK		(1 << (USART_CR1_RXNEIE_SHIFT))
#define USART_CR1_RXNEIE_NO		(0 << (USART_CR1_RXNEIE_SHIFT))
#define USART_CR1_RXNEIE_YES		(1 << (USART_CR1_RXNEIE_SHIFT))
#define USART_CR1_IDLEIE_SHIFT	4
#define USART_CR1_IDLEIE_MASK		(1 << (USART_CR1_IDLEIE_SHIFT))
#define USART_CR1_IDLEIE_NO		(0 << (USART_CR1_IDLEIE_SHIFT))
#define USART_CR1_IDLEIE_YES		(1 << (USART_CR1_IDLEIE_SHIFT))
#define USART_CR1_TE_SHIFT		3
#define USART_CR1_TE_MASK		(1 << (USART_CR1_TE_SHIFT))
#define USART_CR1_TE_NO		(0 << (USART_CR1_TE_SHIFT))
#define USART_CR1_TE_YES		(1 << (USART_CR1_TE_SHIFT))
#define USART_CR1_RE_SHIFT		2
#define USART_CR1_RE_MASK		(1 << (USART_CR1_RE_SHIFT))
#define USART_CR1_RE_NO		(0 << (USART_CR1_RE_SHIFT))
#define USART_CR1_RE_YES		(1 << (USART_CR1_RE_SHIFT))
#define USART_CR1_UESM_SHIFT		1
#define USART_CR1_UESM_MASK		(1 << (USART_CR1_UESM_SHIFT))
#define USART_CR1_UESM_NO		(0 << (USART_CR1_UESM_SHIFT))
#define USART_CR1_UESM_YES		(1 << (USART_CR1_UESM_SHIFT))
#define USART_CR1_UE_SHIFT		0
#define USART_CR1_UE_MASK		(1 << (USART_CR1_UE_SHIFT))
#define USART_CR1_UE_NO		(0 << (USART_CR1_UE_SHIFT))
#define USART_CR1_UE_YES		(1 << (USART_CR1_UE_SHIFT))

  __IO uint32_t         CR2;
#define USART_CR2_STOP_SHIFT          (12)
#define USART_CR2_STOP_MASK           (3 << (USART_CR2_STOP_SHIFT))
#define USART_CR2_STOP_1BIT           (0 << (USART_CR2_STOP_SHIFT))
#define USART_CR2_STOP_0_5BIT         (1 << (USART_CR2_STOP_SHIFT))
#define USART_CR2_STOP_2BIT           (2 << (USART_CR2_STOP_SHIFT))
#define USART_CR2_STOP_1_5BIT         (3 << (USART_CR2_STOP_SHIFT))
#define USART_CR2_ABRMOD_SHIFT        21
#define USART_CR2_ABRMOD_MASK         (3 << (USART_CR2_ABRMOD_SHIFT))
#define USART_CR2_ABRMOD_MODE0_START  (0 << (USART_CR2_ABRMOD_SHIFT))
#define USART_CR2_ABRMOD_MODE1_FALL   (1 << (USART_CR2_ABRMOD_SHIFT))
#define USART_CR2_ABRMOD_MODE2_7F     (2 << (USART_CR2_ABRMOD_SHIFT))
#define USART_CR2_ABRMOD_MODE3_55     (3 << (USART_CR2_ABRMOD_SHIFT))
#define USART_CR2_ABREN_SHIFT         20
#define USART_CR2_ABREN_MASK          (1 << (USART_CR2_ABREN_SHIFT))
#define USART_CR2_ABREN_NO            (0 << (USART_CR2_ABREN_SHIFT))
#define USART_CR2_ABREN_YES           (1 << (USART_CR2_ABREN_SHIFT))

  __IO uint32_t         CR3;
#define USART_CR3_RXFTIE_SHIFT          28
#define USART_CR3_RXFTIE_MASK           (1 << (USART_CR3_RXFTIE_SHIFT))
#define USART_CR3_RXFTIE_NO             (0 << (USART_CR3_RXFTIE_SHIFT))
#define USART_CR3_RXFTIE_YES            (1 << (USART_CR3_RXFTIE_SHIFT))
#define USART_CR3_RXFTCFG_SHIFT         (25)
#define USART_CR3_RXFTCFG_MASK          (7 << (USART_CR3_RXFTCFG_SHIFT))
#define USART_CR3_RXFTCFG_1_8           (0 << (USART_CR3_RXFTCFG_SHIFT))
#define USART_CR3_RXFTCFG_1_4           (1 << (USART_CR3_RXFTCFG_SHIFT))
#define USART_CR3_RXFTCFG_1_2           (2 << (USART_CR3_RXFTCFG_SHIFT))
#define USART_CR3_RXFTCFG_3_4           (3 << (USART_CR3_RXFTCFG_SHIFT))
#define USART_CR3_RXFTCFG_7_8           (4 << (USART_CR3_RXFTCFG_SHIFT))
#define USART_CR3_RXFTCFG_FULL          (5 << (USART_CR3_RXFTCFG_SHIFT))
#define USART_CR3_DMAT_SHIFT            7
#define USART_CR3_DMAT_MASK             (1 << (USART_CR3_DMAT_SHIFT))
#define USART_CR3_DMAT_NO               (0 << (USART_CR3_DMAT_SHIFT))
#define USART_CR3_DMAT_YES              (1 << (USART_CR3_DMAT_SHIFT))
#define USART_CR3_DMAR_SHIFT            6
#define USART_CR3_DMAR_MASK             (1 << (USART_CR3_DMAR_SHIFT))
#define USART_CR3_DMAR_NO               (0 << (USART_CR3_DMAR_SHIFT))
#define USART_CR3_DMAR_YES              (1 << (USART_CR3_DMAR_SHIFT))
#define USART_CR3_EIE_SHIFT             0
#define USART_CR3_EIE_MASK              (1 << (USART_CR3_EIE_SHIFT))
#define USART_CR3_EIE_NO                (0 << (USART_CR3_EIE_SHIFT))
#define USART_CR3_EIE_YES               (1 << (USART_CR3_EIE_SHIFT))

  __IO uint32_t         BRR;
#define USART_BRR_LSB_SHIFT             0
#define USART_BRR_LSB_MASK              (0xf << (USART_BRR_LSB_SHIFT))
  __IO uint32_t         GTPR;
  __IO uint32_t         RTOR;
  __IO uint32_t         RQR;
  __IO uint32_t         ISR;
#define USART_ISR_TXFT_SHIFT		27
#define USART_ISR_TXFT_MASK		(1 << (USART_ISR_TXFT_SHIFT))
#define USART_ISR_RXFT_SHIFT		26
#define USART_ISR_RXFT_MASK		(1 << (USART_ISR_RXFT_SHIFT))
#define USART_ISR_TCBGT_SHIFT		25
#define USART_ISR_TCBGT_MASK		(1 << (USART_ISR_TCBGT_SHIFT))
#define USART_ISR_RXFF_SHIFT		24
#define USART_ISR_RXFF_MASK		(1 << (USART_ISR_RXFF_SHIFT))
#define USART_ISR_TXFE_SHIFT		23
#define USART_ISR_TXFE_MASK		(1 << (USART_ISR_TXFE_SHIFT))
#define USART_ISR_REACK_SHIFT		22
#define USART_ISR_REACK_MASK		(1 << (USART_ISR_REACK_SHIFT))
#define USART_ISR_TEACK_SHIFT		21
#define USART_ISR_TEACK_MASK		(1 << (USART_ISR_TEACK_SHIFT))
#define USART_ISR_WUF_SHIFT		20
#define USART_ISR_WUF_MASK		(1 << (USART_ISR_WUF_SHIFT))
#define USART_ISR_RWU_SHIFT		19
#define USART_ISR_RWU_MASK		(1 << (USART_ISR_RWU_SHIFT))
#define USART_ISR_SBKF_SHIFT		18
#define USART_ISR_SBKF_MASK		(1 << (USART_ISR_SBKF_SHIFT))
#define USART_ISR_CMF_SHIFT		17
#define USART_ISR_CMF_MASK		(1 << (USART_ISR_CMF_SHIFT))
#define USART_ISR_BUSY_SHIFT		16
#define USART_ISR_BUSY_MASK		(1 << (USART_ISR_BUSY_SHIFT))
#define USART_ISR_ABRF_SHIFT		15
#define USART_ISR_ABRF_MASK		(1 << (USART_ISR_ABRF_SHIFT))
#define USART_ISR_ABRE_SHIFT		14
#define USART_ISR_ABRE_MASK		(1 << (USART_ISR_ABRE_SHIFT))
#define USART_ISR_EOBF_SHIFT		12
#define USART_ISR_EOBF_MASK		(1 << (USART_ISR_EOBF_SHIFT))
#define USART_ISR_RTOF_SHIFT		11
#define USART_ISR_RTOF_MASK		(1 << (USART_ISR_RTOF_SHIFT))
#define USART_ISR_CTS_SHIFT		10
#define USART_ISR_CTS_MASK		(1 << (USART_ISR_CTS_SHIFT))
#define USART_ISR_CTSIF_SHIFT		9
#define USART_ISR_CTSIF_MASK		(1 << (USART_ISR_CTSIF_SHIFT))
#define USART_ISR_LBDF_SHIFT		8
#define USART_ISR_LBDF_MASK		(1 << (USART_ISR_LBDF_SHIFT))
#define USART_ISR_TXE_SHIFT		7
#define USART_ISR_TXE_MASK		(1 << (USART_ISR_TXE_SHIFT))
#define USART_ISR_TC_SHIFT		6
#define USART_ISR_TC_MASK		(1 << (USART_ISR_TC_SHIFT))
#define USART_ISR_RXNE_SHIFT		5
#define USART_ISR_RXNE_MASK		(1 << (USART_ISR_RXNE_SHIFT))
#define USART_ISR_IDLE_SHIFT		4
#define USART_ISR_IDLE_MASK		(1 << (USART_ISR_IDLE_SHIFT))
#define USART_ISR_ORE_SHIFT		3
#define USART_ISR_ORE_MASK		(1 << (USART_ISR_ORE_SHIFT))
#define USART_ISR_NF_SHIFT		2
#define USART_ISR_NF_MASK		(1 << (USART_ISR_NF_SHIFT))
#define USART_ISR_FE_SHIFT		1
#define USART_ISR_FE_MASK		(1 << (USART_ISR_FE_SHIFT))
#define USART_ISR_PE_SHIFT		0
#define USART_ISR_PE_MASK		(1 << (USART_ISR_PE_SHIFT))
  __IO uint32_t         ICR;
  __IO uint32_t         RDR;
  __IO uint32_t         TDR;
  __IO uint32_t         PRESC;
  __IO uint32_t         reserved[244];
} stm32Dev_USART;

#if defined(STM32H743xx)
#define USART_RX_FIFO_SIZE         (16)
#define USART_TX_FIFO_SIZE         (16)
#else
#define USART_RX_FIFO_SIZE         (0)
#define USART_TX_FIFO_SIZE         (0)
#endif

#endif
