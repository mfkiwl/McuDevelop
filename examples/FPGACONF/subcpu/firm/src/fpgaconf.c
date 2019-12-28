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

#define _FPGACONF_C_


/*
 * init.txt
 *
 *     progx en/dis:    progx assert enable / disable
 *     delay [val]:     wait [val] ms,  val is decimal
 *
 *     spiclk x:        clock speed x kHz
 *
 *     wrhigh x:        number of clock
 *     wrlow  x:        number of clock
 *
 *     fpga unit vendor [name]:  vendor name
 *     fpga unit file [name]:  fpga bitstream file name
 *     fpga unit id [val]:     fpga id (in hex)
 *     fpga unit check:        check the fpga id
 *
 * #fpga[0123].bit: send to SPI i/f  (0->CS0X, 1->CS1X, ...)
 * #fpga[4567].bit: send to 8bit bus i/f  (4->NE1, 51->NE2, ...)
 *
 *
 */

#include        <string.h>
#include        <stdio.h>
#include        <stdlib.h>

#include        "config.h"
#include        "system.h"

#include        "rtos.h"

#include        "main.h"
#include        "sdmmc.h"
#include        "fatfsdio.h"

#include        "ff.h"
#include        "fpgaconf.h"
#include        "fpga_lattice.h"


#define ChAscToLong(a,b,c,d)    ( \
  (((a) & 0xff) << 24) |          \
  (((b) & 0xff) << 16) |          \
  (((c) & 0xff) <<  8) |          \
  (((d) & 0xff) <<  0) )


static fpgaconf_t       fpgaconf;
static const fatfsdioFunc_t func = {
  .init  = SdmmcInit,
  .read  = SdmmcReadBlock,
  .write = SdmmcWriteBlock,
  .ioctl = SdmmcIoctl,
};


void
FpgaconfMain(void)
{
  int           result = -1;

  memset(&fpgaconf, 0, sizeof(fpgaconf));
  for(int i = 0; i < 8; i++) {
    fpgaconf.sc[i].unit = i;
  }

  while(1) {
    // program, if the card is inserted
    if(GpioIsSdmmc1CardInserted()) {
      puts("start\n");
      result = FpgaconfProgram();
      if(result == 0) {
        puts("done\n");
        GpioSetPowerLedOn();
      }
    } else {
      puts("no_card\n");
    }

    // wait, if the power sw is pushed
    while(GpioIsPowerSwPushed()) {
      RtosTaskSleep(10);
    }
    RtosTaskSleep(1000);

    // re-program, if the power sw is pushed
    while(1) {
      RtosTaskSleep(1);
      if(GpioIsPowerSwPushed()) {
        GpioSetPowerLedOff();
        break;
      }
      if(result < 0) {
        FpgaconfAccessLed(FPGACONF_ACCESS_FPGA_SHIFT, 1);
        RtosTaskSleep(100);
        FpgaconfAccessLed(FPGACONF_ACCESS_FPGA_SHIFT, 0);
        RtosTaskSleep(100);
      }
    }
  }
}


int
FpgaconfProgram(void)
{
  int           result = -1;
  FATFS         fatfs;
  FIL           fp;
  FRESULT       re;
  uint8_t       *recv;
  uint8_t       str[512];
  UINT          size;
#define FPGACONF_DELIMIT_ENT    20
  uint8_t       *argv[FPGACONF_DELIMIT_ENT];
  int           argc;

  FpgaconfInitSdmmc();

  RtosTaskSleep(10);

  // the sdmmc entry is registered to fatfs diskio
  FatfsdioRegsiterFunc(0, (fatfsdioFunc_t *)&func);

  // read init.txt file
  re = f_mount(&fatfs, "", 0);               // 0: delay mount, 1: mount now
  puts("# XXX FpgaconfMain(): call disk_initialize() twice for done\n");
  re = f_open(&fp, "0:init.txt", FA_READ);

  if(re != FR_OK) {
    result = -1;
    goto fail;
  }

  while(1) {
    FpgaconfAccessLed(FPGACONF_ACCESS_SDMMC_SHIFT, 1);
    recv = f_gets(str, sizeof(str), &fp);
    FpgaconfAccessLed(FPGACONF_ACCESS_SDMMC_SHIFT, 0);
    if(recv == NULL || f_error(&fp)) goto fail;

    size = strlen(str);
    size = FpgaconfChomp(str, size);

    if(size > 0 && str[0] != '#') {
      argc = FpgaconfDelimit(argv, FPGACONF_DELIMIT_ENT, str, size);
      re = FpgaconfParse(argc, argv);
      if(re > 0) break;
    }

    if(f_eof(&fp)) break;
  }

  result = 0;

fail:
  f_close(&fp);

  return result;
}


static int
FpgaconfInitSdmmc(void)
{
  devSdmmcParam_t       param;

  DevSdmmcInit(SDMMC_NUM_INIT, NULL);

  memset(&param, 0, sizeof(param));

  param.hwFlow = 1;
  param.clkPolNeg = 0;
  param.clkPowerEn = 1;
  param.busWidth = 1;
  param.dma = 0;
  param.clk = 400000;
  param.cb = SdmmcCallback;

#if 1
  param.intr = 1;
  param.nonblock = 0;
  NVIC_SetPriority(SDMMC1_IRQn, 0);
  NVIC_EnableIRQ(SDMMC1_IRQn);
#endif

  DevSdmmcInit(SDMMC1_NUM, &param);

  return 0;
}


static int
FpgaconfSend8bitBus(int unit, uint8_t *ptr, int size)
{
#if 0
  for(int i = 0; i < size; i++) {
    *(uint8_t *)FMC_BANK1SRAM0_PTR = *ptr++;
  }

#else
  devDmaParam_t     param;

  fpgaconfUnit_t        *psc;

  psc = &fpgaconf.sc[unit];

  // wait to finish sending
  if(fpgaconf.fChkDmaFinish) {
    while(!DevDmaIsFinished(CONFIG_FPGA_8BIT_DMA_MODULE, CONFIG_FPGA_8BIT_DMA_CH));
  }
  fpgaconf.fChkDmaFinish = 1;

  /* start dma */
  memset(&param, 0, sizeof(param));
  param.a = (void *) FMC_BANK1SRAM0_PTR;
  param.b = (void *) ptr;
  param.cnt = size>>2;
  param.dirBA = 1;
  param.mem = 1;
  param.bInc = 1;
  param.flow = 1;
  param.aSize = DEVDMA_SIZE_8BITS;
  param.bSize = DEVDMA_SIZE_32BITS;
  //param.intrTC = psc->param.intrDma? 1: 0;

  DevDmaInit(CONFIG_FPGA_8BIT_DMA_MODULE, CONFIG_FPGA_8BIT_DMA_CH, &param);
  DevDmaStart(CONFIG_FPGA_8BIT_DMA_MODULE, CONFIG_FPGA_8BIT_DMA_CH);
  //while(!DevDmaIsFinished(DMA2_NUM, CONFIG_FPGA_8BIT_DMA_CH));

#endif

  return 0;
}


static int
FpgaconfChomp(uint8_t *str, int size)
{
  int           i;

  /* delete '\r' and '\n' */
  for(int i = size-1; i >= 0; i--) {
    if(str[i] == '\r' || str[i] == '\n') {
      str[i] = '\0';
      size--;
      continue;
    } else {
      break;
    }
  }

  return size;
}


static int
FpgaconfDelimit(uint8_t *av[], int ac, char *p, int len)
{
  char          *tp;
  int           cnt = 0;

  tp = strtok(p, " \t");
  while(tp != NULL) {
    av[cnt] = tp;
    tp = strtok(NULL, " \t");
    cnt++;
    if(cnt >= ac) break;
  }

  return cnt;
}
static int
FpgaconfParse(int argc, uint8_t *argv[])
{
  int           result = 0;
  int           i;

  if(argv[0][0] == '#') goto end;

  // remove word which has '#' in top letter
  for(i = 0; i < argc; i++) {
    if(argv[i][0] == '#') {
      argc = i;
      break;
    }
  }

#if 1
  printf("### parse(%d)", argc);
  for(int i = 0; i < argc; i++) {
    if(argv[i][0] == '#') break;
    printf(" %s", argv[i]);
  }
  puts("\n");
#endif


  if(0) {

  } else if(!strcmp(argv[0], "fpga")) {
    FpgaconfExecFpga(argc, argv);

  } else if(!strcmp(argv[0], "spiclk")) {
    fpgaconf.spiclk = strtoul(argv[1], NULL, 10);

    // i/f settings
    DevSpiSetSpeed(CONFIG_FPGA_SPI_DEVNUM, fpgaconf.spiclk);

  } else if(!strcmp(argv[0], "wrclk")) {
    if(argc >= 3) {
      fpgaconf.wrhigh = strtoul(argv[1], NULL, 10);
      fpgaconf.wrlow  = strtoul(argv[2], NULL, 10);
    } else if(argc == 2) {
      uint32_t  val;
      val = strtoul(argv[1], NULL, 10);
      val = (CONFIG_CLOCK_FREQ_CPU + val - 1) / val;
      if(val < 3) val = 3;
      fpgaconf.wrlow  = val >> 1;
      fpgaconf.wrhigh = val - fpgaconf.wrlow;
    }

    //if(fpgaconf.wrhigh > 15) fpgaconf.wrhigh = 15;
    //if(fpgaconf.wrlow  > 15) fpgaconf.wrlow  = 15;
    //MainSetFmcWait(0, fpgaconf.wrlow, fpgaconf.wrhigh); // NE1
    SystemChangeBusWrAccess(0, fpgaconf.wrlow, fpgaconf.wrhigh); // NE1

  } else if(!strcmp(argv[0], "delay") && argc >= 2) {
    RtosTaskSleep(strtoul(argv[1], NULL, 10));

  } else if(!strcmp(argv[0], "progx") && argc >= 2) {
    if(       argv[1][0] == 'e') {      // enable
      GpioSetProgx_Act();
    } else if(argv[1][0] == 'd') {      // disable
      GpioSetProgx_Inact();
    }

  } else if(!strcmp(argv[0], "gpio") && argc >= 3) {
    int bit;
    bit = 1<<(argv[1][0] & 0xf);
    if(!(argv[2][0] & 1)) {
      SystemGpioReset(0, bit);
    } else {
      SystemGpioSet(  0, bit);
    }

  } else if(!strcmp(argv[0], "end")) {
    result = 1;
  }

end:
  return result;
}


static int
FpgaconfExecFpga(int argc, uint8_t *argv[])
{
  int           result = -1;
  int           num;

  fpgaconfUnit_t        *psc;

  num = strtoul(argv[1], NULL, 10);
  psc = &fpgaconf.sc[num];

  if(!strcmp(argv[2], "go")) {
    FpgaconfigExecFpgaGo(psc);

  } else if(!strcmp(argv[2], "file") && argc >= 4) {
    psc->file[0] = '0';
    psc->file[1] = ':';
    strncpy(psc->file+2, argv[3], sizeof(psc->file)-2);

  } else if(!strcmp(argv[2], "16bit")) {
    psc->bus = FPGACONF_BUS_16BIT;

  } else if(!strcmp(argv[2], "8bit")) {
    psc->bus = FPGACONF_BUS_8BIT;

  } else if(!strcmp(argv[2], "spi")) {
    psc->bus = FPGACONF_BUS_SPI;

  } else if(!strcmp(argv[2], "vendor")) {
    psc->vendor = ChAscToLong(argv[3][0], argv[3][1], argv[3][2], argv[3][3]);

  } else if(!strcmp(argv[2], "chkid")) {
    psc->id = strtoul(argv[3], NULL, 16);

  }

fail:
  return result;
}


int
FpgaconfCsControl(int unit, int active)
{
  int           result = 0;

  if(active) {
    switch(unit) {
      case  0: GpioSetSpi0Csx_Act(); break;
      case  1: GpioSetSpi1Csx_Act(); break;
      case  2: GpioSetSpi2Csx_Act(); break;
      case  3: GpioSetSpi3Csx_Act(); break;
      case  4: GpioSet8bit0Csx_Act(); break;
      case  5: GpioSet8bit1Csx_Act(); break;
      case  6: GpioSet8bit2Csx_Act(); break;
      case  7: GpioSet8bit3Csx_Act(); break;
      default: result = -1;
    }
  } else {
    switch(unit) {
      case  0: GpioSetSpi0Csx_Inact(); break;
      case  1: GpioSetSpi1Csx_Inact(); break;
      case  2: GpioSetSpi2Csx_Inact(); break;
      case  3: GpioSetSpi3Csx_Inact(); break;
      case  4: GpioSet8bit0Csx_Inact(); break;
      case  5: GpioSet8bit1Csx_Inact(); break;
      case  6: GpioSet8bit2Csx_Inact(); break;
      case  7: GpioSet8bit3Csx_Inact(); break;
      default: result = -1;
    }
  }

  return result;
}


static uint32_t      bufBitstream0[CONFIG_FPGA_DATABUF_SIZE/sizeof(uint32_t)];
static uint32_t      bufBitstream1[CONFIG_FPGA_DATABUF_SIZE/sizeof(uint32_t)];
static int
FpgaconfigExecFpgaGo(fpgaconfUnit_t *psc)
{
  int           result = -1;
  FRESULT       re = FR_OK;
  FIL           fpn;
  uint32_t      *ptrBitstream;
  int           pos = 0;
  int           size;
  int           total = 0;

  /*** pre settings ****************************/
  if(psc->bus == FPGACONF_BUS_SPI) {
    GpioSetConfSelSpi();

    FpgaconfCsControl(psc->unit, 1);
    RtosTaskSleep(1);
    FpgaconfCsControl(psc->unit, 0);
    RtosTaskSleep(1);

    if(psc->vendor == ChAscToLong('l', 'a', 't', 't')) {
      re = FpgaLatticeProgramPre(psc->unit, psc->id);
#if 0
    } else if(psc->vendor == ChAscToLong('x', 'i', 'l', 'i')) {
    } else if(psc->vendor == ChAscToLong('a', 'r', 't', 'e')) {
#endif
    } else {
      re = FR_OK;
    }
    if(re < 0) {
      printf("# fpga%d: id mismatch\n");
      goto fail;
    }
  } else {
    GpioSetConfSel8bit();
  }

  /*** open the bit stream *********************/
  if(!psc->file[0]) goto fail;
  re = f_open(&fpn, psc->file, FA_READ);
  if(re != FR_OK) goto fail;

  /*** transmit ********************************/
  fpgaconf.fChkDmaFinish = 0;
  FpgaconfCsControl(psc->unit, 1);
  while(1) {
    ptrBitstream = (pos & 1)? bufBitstream1: bufBitstream0;
    FpgaconfAccessLed(FPGACONF_ACCESS_SDMMC_SHIFT, 1);
    re = f_read(&fpn, ptrBitstream, sizeof(bufBitstream0), &size);
    FpgaconfAccessLed(FPGACONF_ACCESS_SDMMC_SHIFT, 0);
    if(re != FR_OK) {
      puts("send fail\n");
      break;
    }
    if(!size) break;

    FpgaconfAccessLed(FPGACONF_ACCESS_FPGA_SHIFT, 1);
    if(psc->bus == FPGACONF_BUS_SPI) {
      FpgaconfSpiSend(    0, (uint8_t *)ptrBitstream, size);
    } else {
      FpgaconfSend8bitBus(0, (uint8_t *)ptrBitstream, size);
    }
    FpgaconfAccessLed(FPGACONF_ACCESS_FPGA_SHIFT, 0);

    total += size;
    pos++;
  }
  f_close(&fpn);

  /*** end *************************************/
  if(psc->bus == FPGACONF_BUS_SPI) {
    FpgaconfCsControl(psc->unit, 0);
    if(psc->vendor == ChAscToLong('l', 'a', 't', 't')) {
      re = FpgaLatticeProgramEnd(psc->unit);
#if 0
    } else if(psc->vendor == ChAscToLong('x', 'i', 'l', 'i')) {
    } else if(psc->vendor == ChAscToLong('a', 'r', 't', 'e')) {
#endif
    } else {
      re = FR_OK;
    }
  } else {
    if(fpgaconf.fChkDmaFinish) {
      while(!DevDmaIsFinished(CONFIG_FPGA_8BIT_DMA_MODULE, CONFIG_FPGA_8BIT_DMA_CH));
      // delay until fifo was sent
      DevDmaIsFinished(CONFIG_FPGA_8BIT_DMA_MODULE, CONFIG_FPGA_8BIT_DMA_CH);
      DevDmaIsFinished(CONFIG_FPGA_8BIT_DMA_MODULE, CONFIG_FPGA_8BIT_DMA_CH);
    }

    FpgaconfCsControl(psc->unit, 0);
  }

  result = 0;

fail:
  return result;
}


/***************************************************************
 *  access led
 */
static void
FpgaconfAccessLed(int bit, int on)
{
  if(on) {
    fpgaconf.access |=  (1<<bit);
  } else {
    fpgaconf.access &= ~(1<<bit);
  }

  if(fpgaconf.access) {
    GpioSetUpdateLedOn();
  } else {
    GpioSetUpdateLedOff();
  }

  return;
}



/***************************************************************
 *  SPI layer
 */
int
FpgaconfSpiSend(int unit, uint8_t *ptr, int len)
{
  int           result = -1;
  //printf("spi send  %x %x\n", ptr, len);

  result = DevSpiSend(CONFIG_FPGA_SPI_DEVNUM, ptr, len);

  return result;
}
int
FpgaconfSpiSendCmd(int unit, uint8_t *ptr, int len)
{
  int           result = -1;

  result = DevSpiSend(CONFIG_FPGA_SPI_DEVNUM, ptr, len);

  return result;
}
int
FpgaconfSpiRecv(int unit, uint8_t *ptr, int len)
{
  int           result;

  result = DevSpiRecv(CONFIG_FPGA_SPI_DEVNUM, ptr, len);

  return result;
}
