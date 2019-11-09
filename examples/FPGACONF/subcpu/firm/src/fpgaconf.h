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

#ifndef _FPGACONF_H_
#define _FPGACONF_H_

#ifdef  _FPGACONF_C_

typedef struct {
  int           bus;
#define FPGACONF_BUS_SPI        0
#define FPGACONF_BUS_8BIT       1
  uint8_t       file[64];
  uint32_t      id;
  uint32_t      vendor;
} fpgaconfUnit_t;

typedef struct {
  int           fChkDmaFinish: 1;
  int           wrhigh;         // term of wr high (clocks)
  int           wrlow;          // term of wr high (clocks)
  int           spiclk;         // spi clock (in MHz)
  int           tProgx;         // low time of progx (in ms)
  int           tWaitSend;      // wait time before send (in ms)

  fpgaconfUnit_t        sc[1];
  uint32_t      access;
#define FPGACONF_ACCESS_SDMMC_SHIFT     0
#define FPGACONF_ACCESS_SDMMC_MASK      (1 << FPGACONF_ACCESS_SDMMC_SHIFT)
#define FPGACONF_ACCESS_SDMMC_OFF       (0 << FPGACONF_ACCESS_SDMMC_SHIFT)
#define FPGACONF_ACCESS_SDMMC_ON        (1 << FPGACONF_ACCESS_SDMMC_SHIFT)
#define FPGACONF_ACCESS_FPGA_SHIFT      1
#define FPGACONF_ACCESS_FPGA_MASK       (1 << FPGACONF_ACCESS_FPGA_SHIFT)
#define FPGACONF_ACCESS_FPGA_OFF        (0 << FPGACONF_ACCESS_FPGA_SHIFT)
#define FPGACONF_ACCESS_FPGA_ON         (1 << FPGACONF_ACCESS_FPGA_SHIFT)


} fpgaconf_t;

#endif


void            FpgaconfMain(void);
int             FpgaconfProgram(void);
int             FpgaconfSpiSend(int unit, uint8_t *ptr, int len);
int             FpgaconfSpiSendCmd(int unit, uint8_t *ptr, int len);
int             FpgaconfSpiRecv(int unit, uint8_t *ptr, int len);
int             FpgaconfCsControl(int num, int active);

#ifdef  _FPGACONF_C_
static int      FpgaconfInitSdmmc(void);

static int      FpgaconfSendSpi(int unit, uint8_t *ptr, int size);
static int      FpgaconfSend8bitBus(int unit, uint8_t *ptr, int size);
static int      FpgaconfExecFpga(int argc, uint8_t *argv[]);
static int      FpgaconfExecSendBitstream(int argc, uint8_t *argv[]);

static int      FpgaconfChomp(uint8_t *str, int size);
static int      FpgaconfDelimit(uint8_t *av[], int ac, char *p, int len);
static int      FpgaconfParse(int argc, uint8_t *argv[]);     // 0: success, 1: end, -1: fail

static void     FpgaconfAccessLed(int bit, int on);
#endif


#endif
