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

#ifndef _SDMMC_DEF_H_
#define _SDMMC_DEF_H_

enum {
  SDMMC_CMD0_GO_IDLE_STATE              = 0,
  SDMMC_CMD1_SEND_OP_CND                = 1,
  SDMMC_CMD2_ALL_SEND_CID               = 2,
  SDMMC_CMD3_SET_RELATIVE_ADDR          = 3,
  SDMMC_CMD4_SET_DSR                    = 4,
  SDMMC_CMD6_SWTICH                     = 6,
  SDMMC_CMD7_SEL_DESEL_CARD             = 7,
  SDMMC_CMD8_SEND_IF_COND               = 8,
  SDMMC_CMD9_SEND_CSD                   = 9,
  SDMMC_CMD10_SEND_CID                  = 10,
  SDMMC_CMD11_READ_DAT_UNTIL_STOP       = 11,
  SDMMC_CMD12_STOP_TRANSMISSION         = 12,
  SDMMC_CMD13_SEND_STATUS               = 13,
  SDMMC_CMD15_GO_INACTIVE_STATE         = 15,      // only MMC
  SDMMC_CMD16_SET_BLOCKLEN              = 16,
  SDMMC_CMD17_READ_SINGLE_BLOCK         = 17,
  SDMMC_CMD18_READ_MULTIPLE_BLOCK       = 18,
  SDMMC_CMD20_WRITE_DAT_UNTIL_STOP      = 20,
  SDMMC_CMD23_SET_BLOCK_COUNT           = 23,
  SDMMC_CMD24_WRITE_SINGLE_BLOCK        = 24,
  SDMMC_CMD25_WRITE_MULTIPLE_BLOCK      = 25,
  SDMMC_CMD26_PROGRAM_CID               = 26,
  SDMMC_CMD27_PROGRAM_CSD               = 27,
  SDMMC_CMD28_SET_WRITE_PROT            = 28,
  SDMMC_CMD29_CLR_WRITE_PROT            = 29,
  SDMMC_CMD32_TAG_SECTOR_START          = 32,
  SDMMC_CMD33_TAG_SECTOR_END            = 33,
  SDMMC_CMD34_UNTAG_SECTOR              = 34,
  SDMMC_CMD35_TAG_ERASE_GROUP_START     = 35,
  SDMMC_CMD36_TAG_ERASE_GROUP_END       = 36,
  SDMMC_CMD37_UNTAG_ERASE_GROUP         = 37,
  SDMMC_CMD38_ERASE                     = 38,
  SDMMC_CMD41_STATUS_REGISTER           = 41,
  SDMMC_CMD42_LOCK_UNLOCK               = 42,
  SDMMC_CMD55_APP_CMD                   = 55,
  SDMMC_CMD58_READ_OCR                  = 58,
  SDMMC_CMD59_CRC_ON_OFF                = 59,
  SDMMC_ACMD51_SEND_SCR                 = 51,
  SDMMC_ACMD41_APP_SEND_OP_COND         = 41,
};


// ACMD41
#define     SDMMC_ACMD41_BUSY_SHIFT      (31)
#define     SDMMC_ACMD41_BUSY_MASK       (1<<(SDMMC_ACMD41_BUSY_SHIFT))
#define     SDMMC_ACMD41_READY_MASK      (1<<(SDMMC_ACMD41_BUSY_SHIFT))
#define     SDMMC_ACMD41_CCS_SHIFT       (30)
#define     SDMMC_ACMD41_CCS_MASK        (1<<(SDMMC_ACMD41_CCS_SHIFT))
#define     SDMMC_ACMD41_CCS_SDSC        (0<<(SDMMC_ACMD41_CCS_SHIFT))
#define     SDMMC_ACMD41_CCS_SDXC        (1<<(SDMMC_ACMD41_CCS_SHIFT))
#define     SDMMC_ACMD41_S18_SHIFT       (24)
#define     SDMMC_ACMD41_S18_MASK        (1<<(SDMMC_ACMD41_S18_SHIFT))
#define     SDMMC_ACMD41_S18R_3300MV     (0<<(SDMMC_ACMD41_S18_SHIFT))
#define     SDMMC_ACMD41_S18R_1800MV     (1<<(SDMMC_ACMD41_S18_SHIFT))
#define     SDMMC_ACMD41_S18A_3300MV     (0<<(SDMMC_ACMD41_S18_SHIFT))
#define     SDMMC_ACMD41_S18A_1800MV     (1<<(SDMMC_ACMD41_S18_SHIFT))
#define     SDMMC_ACMD41_VDD_SHIFT       (15)
#define     SDMMC_ACMD41_VDD_MASK        (0x1ff<<(SDMMC_ACMD41_VDD_SHIFT))
#define     SDMMC_ACMD41_LOWVOLT_SHIFT   (7)
#define     SDMMC_ACMD41_LOWVOLT_MASK    (0x1ff<<(SDMMC_ACMD41_LOWVOLT_SHIFT))

#if 0
#define     SDMMC_ACMD41_VOLTAGE_WINDOW_SHIFT  (0)
#define     SDMMC_ACMD41_VOLTAGE_WINDOW     (0<<(SDMMC_ACMD41_VOLTAGE_WINDOW_SHIFT))
#endif
#define     SDMMC_ACMD41_VOLTAGE_WINDOW_SD  ((SDMMC_ACMD41_READY_MASK) | \
                                             (0x100000))



#endif
