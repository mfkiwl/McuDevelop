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

#ifndef _STM32SDMMC_H_
#define _STM32SDMMC_H_

typedef struct {
  __IO uint32_t         POWER;          // 0x00
#define SDMMC_POWER_PWRCTRL_SHIFT       0
#define SDMMC_POWER_PWRCTRL_MASK        (3 << (SDMMC_POWER_PWRCTRL_SHIFT))
#define SDMMC_POWER_PWRCTRL_OFF         (0 << (SDMMC_POWER_PWRCTRL_SHIFT))
#define SDMMC_POWER_PWRCTRL_ON          (3 << (SDMMC_POWER_PWRCTRL_SHIFT))

  __IO uint32_t         CLKCR;          // 0x04
#define SDMMC_CLKCR_HWFC_EN_SHIFT       14
#define SDMMC_CLKCR_HWFC_EN_MASK        (1 << (SDMMC_CLKCR_HWFC_EN_SHIFT))
#define SDMMC_CLKCR_HWFC_EN_NO          (0 << (SDMMC_CLKCR_HWFC_EN_SHIFT))
#define SDMMC_CLKCR_HWFC_EN_YES         (1 << (SDMMC_CLKCR_HWFC_EN_SHIFT))
#define SDMMC_CLKCR_NEGEDGE_SHIFT       13
#define SDMMC_CLKCR_NEGEDGE_MASK        (1 << (SDMMC_CLKCR_NEGEDGE_SHIFT))
#define SDMMC_CLKCR_NEGEDGE_NO          (0 << (SDMMC_CLKCR_NEGEDGE_SHIFT))
#define SDMMC_CLKCR_NEGEDGE_YES         (1 << (SDMMC_CLKCR_NEGEDGE_SHIFT))
#define SDMMC_CLKCR_WIDBUS_SHIFT        11
#define SDMMC_CLKCR_WIDBUS_MASK         (3 << (SDMMC_CLKCR_WIDBUS_SHIFT))
#define SDMMC_CLKCR_WIDBUS_1BIT         (0 << (SDMMC_CLKCR_WIDBUS_SHIFT))
#define SDMMC_CLKCR_WIDBUS_4BIT         (1 << (SDMMC_CLKCR_WIDBUS_SHIFT))
#define SDMMC_CLKCR_WIDBUS_8BIT         (2 << (SDMMC_CLKCR_WIDBUS_SHIFT))
#define SDMMC_CLKCR_BYPASS_SHIFT        10
#define SDMMC_CLKCR_BYPASS_MASK         (1 << (SDMMC_CLKCR_BYPASS_SHIFT))
#define SDMMC_CLKCR_BYPASS_NO           (0 << (SDMMC_CLKCR_BYPASS_SHIFT))
#define SDMMC_CLKCR_BYPASS_YES          (1 << (SDMMC_CLKCR_BYPASS_SHIFT))
#define SDMMC_CLKCR_PWRSAV_SHIFT        9
#define SDMMC_CLKCR_PWRSAV_MASK         (1 << (SDMMC_CLKCR_PWRSAV_SHIFT))
#define SDMMC_CLKCR_PWRSAV_NO           (0 << (SDMMC_CLKCR_PWRSAV_SHIFT))
#define SDMMC_CLKCR_PWRSAV_YES          (1 << (SDMMC_CLKCR_PWRSAV_SHIFT))
#define SDMMC_CLKCR_CLKEN_SHIFT         8
#define SDMMC_CLKCR_CLKEN_MASK          (1 << (SDMMC_CLKCR_CLKEN_SHIFT))
#define SDMMC_CLKCR_CLKEN_NO            (0 << (SDMMC_CLKCR_CLKEN_SHIFT))
#define SDMMC_CLKCR_CLKEN_YES           (1 << (SDMMC_CLKCR_CLKEN_SHIFT))
  // freq = SDMMCCLK / (CLKDIV + 2)
#define SDMMC_CLKCR_CLKDIV_SHIFT        0
#define SDMMC_CLKCR_CLKDIV_MASK         (0xff << (SDMMC_CLKCR_CLKDIV_SHIFT))
#define SDMMC_CLKCR_CLKDIV_VAL(x)       (((x) << (SDMMC_CLKCR_CLKDIV_SHIFT)) & SDMMC_CLKCR_CLKDIV_MASK)

  __IO uint32_t         ARG;            // 0x08

  __IO uint32_t         CMD;            // 0x0c
#define SDMMC_CMD_SDIOSUSP_SHIFT        11
#define SDMMC_CMD_SDIOSUSP_MASK         (1 << (SDMMC_CMD_SDIOSUSP_SHIFT))
#define SDMMC_CMD_SDIOSUSP_NO           (0 << (SDMMC_CMD_SDIOSUSP_SHIFT))
#define SDMMC_CMD_SDIOSUSP_YES          (1 << (SDMMC_CMD_SDIOSUSP_SHIFT))
#define SDMMC_CMD_CPSMEN_SHIFT          10
#define SDMMC_CMD_CPSMEN_MASK           (1 << (SDMMC_CMD_CPSMEN_SHIFT))
#define SDMMC_CMD_CPSMEN_NO             (0 << (SDMMC_CMD_CPSMEN_SHIFT))
#define SDMMC_CMD_CPSMEN_YES            (1 << (SDMMC_CMD_CPSMEN_SHIFT))
#define SDMMC_CMD_WAITPEND_SHIFT        9
#define SDMMC_CMD_WAITPEND_MASK         (1 << (SDMMC_CMD_WAITPEND_SHIFT))
#define SDMMC_CMD_WAITPEND_NO           (0 << (SDMMC_CMD_WAITPEND_SHIFT))
#define SDMMC_CMD_WAITPEND_YES          (1 << (SDMMC_CMD_WAITPEND_SHIFT))
#define SDMMC_CMD_WAITINT_SHIFT         8
#define SDMMC_CMD_WAITINT_MASK          (1 << (SDMMC_CMD_WAITINT_SHIFT))
#define SDMMC_CMD_WAITINT_NO            (0 << (SDMMC_CMD_WAITINT_SHIFT))
#define SDMMC_CMD_WAITINT_YES           (1 << (SDMMC_CMD_WAITINT_SHIFT))
#define SDMMC_CMD_WAITRESP_SHIFT        6
#define SDMMC_CMD_WAITRESP_MASK         (3 << (SDMMC_CMD_WAITRESP_SHIFT))
#define SDMMC_CMD_WAITRESP_NO           (0 << (SDMMC_CMD_WAITRESP_SHIFT))
#define SDMMC_CMD_WAITRESP_SHORT        (1 << (SDMMC_CMD_WAITRESP_SHIFT))
#define SDMMC_CMD_WAITRESP_LONG         (3 << (SDMMC_CMD_WAITRESP_SHIFT))
#define SDMMC_CMD_CMDINDEX_SHIFT        0
#define SDMMC_CMD_CMDINDEX_MASK         (0x3f << (SDMMC_CMD_CMDINDEX_SHIFT))
#define SDMMC_CMD_CMDINDEX_VAL(x)       (((x) << (SDMMC_CMD_CMDINDEX_SHIFT)) & SDMMC_CMD_CMDINDEX_MASK)

#define SDMMC_CMD_TRANSFER              ((SDMMC_CMD_CPSMEN_YES) | (SDMMC_CMD_WAITRESP_SHORT))
#define SDMMC_CMD_READSINGLE            ((SDMMC_CMD_TRANSFER) | \
                                         (SDMMC_CMD_CMDINDEX_VAL(17)))
#define SDMMC_CMD_READMULTI             ((SDMMC_CMD_TRANSFER) | \
                                         (SDMMC_CMD_CMDINDEX_VAL(18)))
#define SDMMC_CMD_WRITESINGLE           ((SDMMC_CMD_TRANSFER) | \
                                         (SDMMC_CMD_CMDINDEX_VAL(24)))
#define SDMMC_CMD_WRITEMULTI            ((SDMMC_CMD_TRANSFER) | \
                                         (SDMMC_CMD_CMDINDEX_VAL(25)))
#define SDMMC_CMD_STOPTRANFERING        ((SDMMC_CMD_TRANSFER) | \
                                         (SDMMC_CMD_CMDINDEX_VAL(12)))

  __IO uint32_t         RESPCMD;        // 0x10
#define SDMMC_RESPCMD_SHIFT             0
#define SDMMC_RESPCMD_MASK              (0x3f << (SDMMC_RESPCMD_SHIFT))
#define SDMMC_RESPCMD_INVALID           (0x3f << (SDMMC_RESPCMD_SHIFT))
#define SDMMC_RESPCMD_VAL(x)            (((x) << (SDMMC_RESPCMD_SHIFT)) & SDMMC_RESPCMD_MASK)

  __IO uint32_t         RESP1;          // 0x14
  __IO uint32_t         RESP2;          // 0x18
  __IO uint32_t         RESP3;          // 0x1c
  __IO uint32_t         RESP4;          // 0x20
  __IO uint32_t         DTIMER;         // 0x24
  __IO uint32_t         DLEN;           // 0x28
#define SDMMC_DLEN_SHIFT                0
#define SDMMC_DLEN_MASK                 (0x1ffffff << (SDMMC_DLEN_SHIFT))
#define SDMMC_DLEN_VAL(x)               (((x) << (SDMMC_DLEN_SHIFT)) & SDMMC_DLEN_MASK)

  __IO uint32_t         DCTRL;          // 0x2c
#define SDMMC_DCTRL_SDIOEN_SHIFT        11
#define SDMMC_DCTRL_SDIOEN_MASK         (1 << (SDMMC_DCTRL_SDIOEN_SHIFT))
#define SDMMC_DCTRL_SDIOEN_NO           (0 << (SDMMC_DCTRL_SDIOEN_SHIFT))
#define SDMMC_DCTRL_SDIOEN_YES          (1 << (SDMMC_DCTRL_SDIOEN_SHIFT))
#define SDMMC_DCTRL_RWMOD_SHIFT         10
#define SDMMC_DCTRL_RWMOD_MASK          (1 << (SDMMC_DCTRL_RWMOD_SHIFT))
#define SDMMC_DCTRL_RWMOD_D2            (0 << (SDMMC_DCTRL_RWMOD_SHIFT))
#define SDMMC_DCTRL_RWMOD_CK            (1 << (SDMMC_DCTRL_RWMOD_SHIFT))
#define SDMMC_DCTRL_RWSTOP_SHIFT        9
#define SDMMC_DCTRL_RWSTOP_MASK         (1 << (SDMMC_DCTRL_RWSTOP_SHIFT))
#define SDMMC_DCTRL_RWSTOP_NO           (0 << (SDMMC_DCTRL_RWSTOP_SHIFT))
#define SDMMC_DCTRL_RWSTOP_YES          (1 << (SDMMC_DCTRL_RWSTOP_SHIFT))
#define SDMMC_DCTRL_RWSTART_SHIFT       8
#define SDMMC_DCTRL_RWSTART_MASK        (1 << (SDMMC_DCTRL_RWSTART_SHIFT))
#define SDMMC_DCTRL_RWSTART_NO          (0 << (SDMMC_DCTRL_RWSTART_SHIFT))
#define SDMMC_DCTRL_RWSTART_YES         (1 << (SDMMC_DCTRL_RWSTART_SHIFT))
#define SDMMC_DCTRL_DBLOCKSIZE_SHIFT    4
#define SDMMC_DCTRL_DBLOCKSIZE_MASK     (0xf  << (SDMMC_DCTRL_DBLOCKSIZE_SHIFT))
#define SDMMC_DCTRL_DBLOCKSIZE_VAL(x)   (((x) << (SDMMC_DCTRL_DBLOCKSIZE_SHIFT)) & SDMMC_DCTRL_DBLOCKSIZE_MASK)
#define SDMMC_DCTRL_DBLOCKSIZE_8B       (3    << (SDMMC_DCTRL_DBLOCKSIZE_SHIFT))
#define SDMMC_DCTRL_DBLOCKSIZE_512B     (9    << (SDMMC_DCTRL_DBLOCKSIZE_SHIFT))
#define SDMMC_DCTRL_DBLOCKSIZE_1024B    (10   << (SDMMC_DCTRL_DBLOCKSIZE_SHIFT))
#define SDMMC_DCTRL_DMAEN_SHIFT         3
#define SDMMC_DCTRL_DMAEN_MASK          (1 << (SDMMC_DCTRL_DMAEN_SHIFT))
#define SDMMC_DCTRL_DMAEN_NO            (0 << (SDMMC_DCTRL_DMAEN_SHIFT))
#define SDMMC_DCTRL_DMAEN_YES           (1 << (SDMMC_DCTRL_DMAEN_SHIFT))
#define SDMMC_DCTRL_DTMODE_SHIFT        2
#define SDMMC_DCTRL_DTMODE_MASK         (1 << (SDMMC_DCTRL_DTMODE_SHIFT))
#define SDMMC_DCTRL_DTMODE_BLOCK        (0 << (SDMMC_DCTRL_DTMODE_SHIFT))
#define SDMMC_DCTRL_DTMODE_STREAM       (1 << (SDMMC_DCTRL_DTMODE_SHIFT))
#define SDMMC_DCTRL_DTDIR_SHIFT         1
#define SDMMC_DCTRL_DTDIR_MASK          (1 << (SDMMC_DCTRL_DTDIR_SHIFT))
#define SDMMC_DCTRL_DTDIR_CTRL2CARD     (0 << (SDMMC_DCTRL_DTDIR_SHIFT))
#define SDMMC_DCTRL_DTDIR_CARD2CTRL     (1 << (SDMMC_DCTRL_DTDIR_SHIFT))
#define SDMMC_DCTRL_DTEN_SHIFT          0
#define SDMMC_DCTRL_DTEN_MASK           (1 << (SDMMC_DCTRL_DTEN_SHIFT))
#define SDMMC_DCTRL_DTEN_NO             (0 << (SDMMC_DCTRL_DTEN_SHIFT))
#define SDMMC_DCTRL_DTEN_YES            (1 << (SDMMC_DCTRL_DTEN_SHIFT))

  __IO uint32_t         DCOUNT;         // 0x30
#define SDMMC_DATACOUNT_MASK            (0x1ffffff << (SDMMC_DATACOUNT_SHIFT))
#define SDMMC_DATACOUNT_VAL(x)          (((x) << (SDMMC_DATACOUNT_SHIFT)) & SDMMC_DATACOUNT_MASK)

  __IO uint32_t         STA;            // 0x34
#define SDMMC_STA_SDIOIT_SHIFT          22
#define SDMMC_STA_SDIOIT_MASK           (1 << (SDMMC_STA_SDIOIT_SHIFT))
#define SDMMC_STA_RXDAVL_SHIFT          21
#define SDMMC_STA_RXDAVL_MASK           (1 << (SDMMC_STA_RXDAVL_SHIFT))
#define SDMMC_STA_TXDAVL_SHIFT          20
#define SDMMC_STA_TXDAVL_MASK           (1 << (SDMMC_STA_TXDAVL_SHIFT))
#define SDMMC_STA_RXFIFOE_SHIFT         19
#define SDMMC_STA_RXFIFOE_MASK          (1 << (SDMMC_STA_RXFIFOE_SHIFT))
#define SDMMC_STA_TXFIFOE_SHIFT         18
#define SDMMC_STA_TXFIFOE_MASK          (1 << (SDMMC_STA_TXFIFOE_SHIFT))
#define SDMMC_STA_RXFIFOF_SHIFT         17
#define SDMMC_STA_RXFIFOF_MASK          (1 << (SDMMC_STA_RXFIFOF_SHIFT))
#define SDMMC_STA_TXFIFOF_SHIFT         16
#define SDMMC_STA_TXFIFOF_MASK          (1 << (SDMMC_STA_TXFIFOF_SHIFT))
#define SDMMC_STA_RXFIFOHF_SHIFT        15
#define SDMMC_STA_RXFIFOHF_MASK         (1 << (SDMMC_STA_RXFIFOHF_SHIFT))
#define SDMMC_STA_TXFIFOHE_SHIFT        14
#define SDMMC_STA_TXFIFOHE_MASK         (1 << (SDMMC_STA_TXFIFOHE_SHIFT))
#define SDMMC_STA_RXACT_SHIFT           13
#define SDMMC_STA_RXACT_MASK            (1 << (SDMMC_STA_RXACT_SHIFT))
#define SDMMC_STA_TXACT_SHIFT           12
#define SDMMC_STA_TXACT_MASK            (1 << (SDMMC_STA_TXACT_SHIFT))
#define SDMMC_STA_CMDACT_SHIFT          11
#define SDMMC_STA_CMDACT_MASK           (1 << (SDMMC_STA_CMDACT_SHIFT))
#define SDMMC_STA_DBCKEND_SHIFT         10
#define SDMMC_STA_DBCKEND_MASK          (1 << (SDMMC_STA_DBCKEND_SHIFT))
#define SDMMC_STA_DATAEND_SHIFT         8
#define SDMMC_STA_DATAEND_MASK          (1 << (SDMMC_STA_DATAEND_SHIFT))
#define SDMMC_STA_CMDSENT_SHIFT         7
#define SDMMC_STA_CMDSENT_MASK          (1 << (SDMMC_STA_CMDSENT_SHIFT))
#define SDMMC_STA_CMDREND_SHIFT         6
#define SDMMC_STA_CMDREND_MASK          (1 << (SDMMC_STA_CMDREND_SHIFT))
#define SDMMC_STA_RXOVERR_SHIFT         5
#define SDMMC_STA_RXOVERR_MASK          (1 << (SDMMC_STA_RXOVERR_SHIFT))
#define SDMMC_STA_TXUNDERR_SHIFT        4
#define SDMMC_STA_TXUNDERR_MASK         (1 << (SDMMC_STA_TXUNDERR_SHIFT))
#define SDMMC_STA_DTIMEOUT_SHIFT        3
#define SDMMC_STA_DTIMEOUT_MASK         (1 << (SDMMC_STA_DTIMEOUT_SHIFT))
#define SDMMC_STA_CTIMEOUT_SHIFT        2
#define SDMMC_STA_CTIMEOUT_MASK         (1 << (SDMMC_STA_CTIMEOUT_SHIFT))
#define SDMMC_STA_DCRCFAIL_SHIFT        1
#define SDMMC_STA_DCRCFAIL_MASK         (1 << (SDMMC_STA_DCRCFAIL_SHIFT))
#define SDMMC_STA_CCRCFAIL_SHIFT        0
#define SDMMC_STA_CCRCFAIL_MASK         (1 << (SDMMC_STA_CCRCFAIL_SHIFT))
#define SDMMC_STA_FAILS_MASK            ((SDMMC_STA_RXOVERR_MASK)  | \
                                         (SDMMC_STA_TXUNDERR_MASK) | \
                                         (SDMMC_STA_DTIMEOUT_MASK) | \
                                         (SDMMC_STA_CTIMEOUT_MASK) | \
                                         (SDMMC_STA_DCRCFAIL_MASK) \
                                         )
  //  (SDMMC_STA_CCRCFAIL_MASK)

  __IO uint32_t         ICR;            // 0x38
#define SDMMC_STA_SDIOITC_SHIFT         22
#define SDMMC_STA_SDIOITC_MASK          (1 << (SDMMC_STA_SDIOITC_SHIFT))
#define SDMMC_STA_DBCKENDC_SHIFT        10
#define SDMMC_STA_DBCKENDC_MASK         (1 << (SDMMC_STA_DBCKENDC_SHIFT))
#define SDMMC_ICR_DATAENDC_SHIFT        8
#define SDMMC_ICR_DATAENDC_MASK         (1 << (SDMMC_ICR_DATAENDC_SHIFT))
#define SDMMC_ICR_CMDSENTC_SHIFT        7
#define SDMMC_ICR_CMDSENTC_MASK         (1 << (SDMMC_ICR_CMDSENTC_SHIFT))
#define SDMMC_ICR_CMDRENDC_SHIFT        6
#define SDMMC_ICR_CMDRENDC_MASK         (1 << (SDMMC_ICR_CMDRENDC_SHIFT))
#define SDMMC_ICR_RXOVERRC_SHIFT        5
#define SDMMC_ICR_RXOVERRC_MASK         (1 << (SDMMC_ICR_RXOVERRC_SHIFT))
#define SDMMC_ICR_TXUNDERRC_SHIFT       4
#define SDMMC_ICR_TXUNDERRC_MASK        (1 << (SDMMC_ICR_TXUNDERRC_SHIFT))
#define SDMMC_ICR_DTIMEOUTC_SHIFT       3
#define SDMMC_ICR_DTIMEOUTC_MASK        (1 << (SDMMC_ICR_DTIMEOUTC_SHIFT))
#define SDMMC_ICR_CTIMEOUTC_SHIFT       2
#define SDMMC_ICR_CTIMEOUTC_MASK        (1 << (SDMMC_ICR_CTIMEOUTC_SHIFT))
#define SDMMC_ICR_DCRCFAILC_SHIFT       1
#define SDMMC_ICR_DCRCFAILC_MASK        (1 << (SDMMC_ICR_DCRCFAILC_SHIFT))
#define SDMMC_ICR_CCRCFAILC_SHIFT       0
#define SDMMC_ICR_CCRCFAILC_MASK        (1 << (SDMMC_ICR_CCRCFAILC_SHIFT))
#define SDMMC_ICR_FAILS_MASK            ((SDMMC_STA_DBCKENDC_MASK)  | \
                                         (SDMMC_ICR_CMDSENTC_MASK)  | \
                                         (SDMMC_ICR_CMDRENDC_MASK)  | \
                                         (SDMMC_ICR_DATAENDC_MASK)  | \
                                         (SDMMC_ICR_RXOVERRC_MASK)  | \
                                         (SDMMC_ICR_TXUNDERRC_MASK) | \
                                         (SDMMC_ICR_DTIMEOUTC_MASK) | \
                                         (SDMMC_ICR_CTIMEOUTC_MASK) | \
                                         (SDMMC_ICR_DCRCFAILC_MASK) | \
                                         (SDMMC_ICR_CCRCFAILC_MASK))

  __IO uint32_t         MASK;           // 0x3c
#define SDMMC_MASK_SDIOITE_SHIFT        22
#define SDMMC_MASK_SDIOITE_MASK         (1 << (SDMMC_MASK_SDIOITE_SHIFT))
#define SDMMC_MASK_RXDAVLE_SHIFT        21
#define SDMMC_MASK_RXDAVLE_MASK         (1 << (SDMMC_MASK_RXDAVLE_SHIFT))
#define SDMMC_MASK_TXDAVLE_SHIFT        20
#define SDMMC_MASK_TXDAVLE_MASK         (1 << (SDMMC_MASK_TXDAVLE_SHIFT))
#define SDMMC_MASK_RXFIFOEE_SHIFT       19
#define SDMMC_MASK_RXFIFOEE_MASK        (1 << (SDMMC_MASK_RXFIFOEE_SHIFT))
#define SDMMC_MASK_TXFIFOEE_SHIFT       18
#define SDMMC_MASK_TXFIFOEE_MASK        (1 << (SDMMC_MASK_TXFIFOEE_SHIFT))
#define SDMMC_MASK_RXFIFOFE_SHIFT       17
#define SDMMC_MASK_RXFIFOFE_MASK        (1 << (SDMMC_MASK_RXFIFOFE_SHIFT))
#define SDMMC_MASK_TXFIFOFE_SHIFT       16
#define SDMMC_MASK_TXFIFOFE_MASK        (1 << (SDMMC_MASK_TXFIFOFE_SHIFT))
#define SDMMC_MASK_RXFIFOHFE_SHIFT      15
#define SDMMC_MASK_RXFIFOHFE_MASK       (1 << (SDMMC_MASK_RXFIFOHFE_SHIFT))
#define SDMMC_MASK_TXFIFOHEE_SHIFT      14
#define SDMMC_MASK_TXFIFOHEE_MASK       (1 << (SDMMC_MASK_TXFIFOHEE_SHIFT))
#define SDMMC_MASK_RXACTE_SHIFT         13
#define SDMMC_MASK_RXACTE_MASK          (1 << (SDMMC_MASK_RXACTE_SHIFT))
#define SDMMC_MASK_TXACTE_SHIFT         12
#define SDMMC_MASK_TXACTE_MASK          (1 << (SDMMC_MASK_TXACTE_SHIFT))
#define SDMMC_MASK_CMDACTE_SHIFT        11
#define SDMMC_MASK_CMDACTE_MASK         (1 << (SDMMC_MASK_CMDACTE_SHIFT))
#define SDMMC_MASK_DBCKENDE_SHIFT       10
#define SDMMC_MASK_DBCKENDE_MASK        (1 << (SDMMC_MASK_DBCKENDE_SHIFT))
#define SDMMC_MASK_DATAENDE_SHIFT       8
#define SDMMC_MASK_DATAENDE_MASK        (1 << (SDMMC_MASK_DATAENDE_SHIFT))
#define SDMMC_MASK_CMDSENTE_SHIFT       7
#define SDMMC_MASK_CMDSENTE_MASK        (1 << (SDMMC_MASK_CMDSENTE_SHIFT))
#define SDMMC_MASK_CMDRENDE_SHIFT       6
#define SDMMC_MASK_CMDRENDE_MASK        (1 << (SDMMC_MASK_CMDRENDE_SHIFT))
#define SDMMC_MASK_RXOVERRE_SHIFT       5
#define SDMMC_MASK_RXOVERRE_MASK        (1 << (SDMMC_MASK_RXOVERRE_SHIFT))
#define SDMMC_MASK_TXUNDERRE_SHIFT      4
#define SDMMC_MASK_TXUNDERRE_MASK       (1 << (SDMMC_MASK_TXUNDERRE_SHIFT))
#define SDMMC_MASK_DTIMEOUTE_SHIFT      3
#define SDMMC_MASK_DTIMEOUTE_MASK       (1 << (SDMMC_MASK_DTIMEOUTE_SHIFT))
#define SDMMC_MASK_CTIMEOUTE_SHIFT      2
#define SDMMC_MASK_CTIMEOUTE_MASK       (1 << (SDMMC_MASK_CTIMEOUTE_SHIFT))
#define SDMMC_MASK_DCRCFAILE_SHIFT      1
#define SDMMC_MASK_DCRCFAILE_MASK       (1 << (SDMMC_MASK_DCRCFAILE_SHIFT))
#define SDMMC_MASK_CCRCFAILE_SHIFT      0
#define SDMMC_MASK_CCRCFAILE_MASK       (1 << (SDMMC_MASK_CCRCFAILE_SHIFT))

  uint32_t              reserved40;     // 0x40
  uint32_t              reserved44;     // 0x44
  __IO uint32_t         FIFOCNT;        // 0x48
#define SDMMC_FIFOCOUNT_MASK            (0xffffff << (SDMMC_FIFOCOUNT_SHIFT))
#define SDMMC_FIFOCOUNT_VAL(x)          (((x) << (SDMMC_FIFOCOUNT_SHIFT)) & SDMMC_FIFOCOUNT_MASK)
  uint32_t              reserved4c[13]; // 0x4c
  __IO uint32_t         FIFO;           // 0x80

} stm32Dev_SDMMC;

#endif
