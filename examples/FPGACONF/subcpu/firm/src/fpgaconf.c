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
  while(1) {
    // program, if the card is inserted
    if(GpioIsSdmmc1CardInserted()) {
      FpgaconfProgram();
    }

    // wait, if the power sw is pushed
    while(GpioIsPowerSwPushed()) {
      RtosTaskSleep(10);
    }
    RtosTaskSleep(1000);

    // re-program, if the power sw is pushed
    while(1) {
      if(GpioIsPowerSwPushed()) break;
      RtosTaskSleep(10);
    }

  }
}

void
FpgaconfProgram(void)
{
  FATFS         fatfs;
  FIL           fp;
  FRESULT       re;
  uint8_t       str[128];
  UINT          size;
#define FPGACONF_DELIMIT_ENT    20
  uint8_t       *argv[FPGACONF_DELIMIT_ENT];
  int           argc;

  FpgaconfInitSdmmc();

  RtosTaskSleep(10);

  // the sdmmc entry is registered to fatfs diskio
  FatfsdioRegsiterFunc(0, (fatfsdioFunc_t *)&func);

  // read init.txt file
  re = f_mount(&fatfs, "", 1);               // 0: delay mount, 1: mount now
  //puts("# XXX FpgaconfMain(): call disk_initialize() twice for done\n");
  re = f_open(&fp, "0:init.txt", FA_READ);
  if(re != FR_OK) {
    //puts("# XXX FpgaconfMain(): fatfs fail, halt\n");
    while(1) RtosTaskSleep(10);
  }
  // halt, if init.txt can not be opened
  if(re != FR_OK) {
    //puts("# XXX FpgaconfMain(): fatfs fail, halt\n");
    while(1) RtosTaskSleep(10);
  }

  while(1) {
    //FpgaconfAccessLed(FPGACONF_ACCESS_SDMMC_SHIFT, 1);
    f_gets(str, sizeof(str), &fp);
    //FpgaconfAccessLed(FPGACONF_ACCESS_SDMMC_SHIFT, 0);
    size = strlen(str);
    size = FpgaconfChomp(str, size);

    if(size > 0) {
      argc = FpgaconfDelimit(argv, FPGACONF_DELIMIT_ENT, str, size);
      re = FpgaconfParse(argc, argv);
      if(re > 0) break;
    }

    if(f_eof(&fp)) break;
    if(f_error(&fp)) break;
  }

  f_close(&fp);

  return;
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
  param.clk = 400000;

  DevSdmmcInit(SDMMC1_NUM, &param);

#if 0
  NVIC_SetPriority(SDMMC1_IRQn, 0);
  NVIC_EnableIRQ(SDMMC1_IRQn);
#endif

  return 0;
}


static int
FpgaconfSend8bitBus(int unit, uint8_t *ptr, int size)
{
  for(int i = 0; i < size; i++) {
    *(uint8_t *)FMC_BANK1SRAM0_PTR = *ptr++;
  }

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

  if(argv[0][0] == '#') goto end;

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
    fpgaconf.spiclk = atoi(argv[1]);

    // i/f settings
    DevSpiSetSpeed(CONFIG_FPGA_SPI_DEVNUM, fpgaconf.spiclk);

  } else if(!strcmp(argv[0], "wrclk")) {
    fpgaconf.wrhigh = atoi(argv[1]);
    fpgaconf.wrlow  = atoi(argv[2]);
    for(int i = 0; i < STM32FMC_NUMBEROF_BANK; i++) {
      //SystemChangeBusWrAccess(i, fpgaconf.wrlow, fpgaconf.wrhigh);
    }

  } else if(!strcmp(argv[0], "delay") && argc >= 2) {
    RtosTaskSleep(atoi(argv[1]));

  } else if(!strcmp(argv[0], "progx") && argc >= 2) {
    if(       argv[1][0] == 'e') {      // enable
      GpioSetProgx_Act();
    } else if(argv[1][0] == 'd') {      // disable
      GpioSetProgx_Inact();
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
  FRESULT       re;
  FIL           fpn;
  uint32_t      buf[512/4];
  int           size;
  int           total = 0;
  int           num;
  uint32_t            id;

  fpgaconfUnit_t        *psc;

  num = atoi(argv[1]);
  //psc = &fpgaconf.sc[num];
  psc = &fpgaconf.sc[0];

  if(!strcmp(argv[2], "go")) {

    if(psc->bus == FPGACONF_BUS_SPI) {
      GpioSetConfSelSpi();

      FpgaconfCsControl(num, 1);
      RtosTaskSleep(1);
      FpgaconfCsControl(num, 0);

      RtosTaskSleep(1);

      if(psc->vendor == ChAscToLong('l', 'a', 't', 't')) {
        re = FpgaLatticeProgramPre(num, psc->id);
      } else if(psc->vendor == ChAscToLong('x', 'i', 'l', 'i')) {
      } else if(psc->vendor == ChAscToLong('a', 'r', 't', 'e')) {
      }
      if(re < 0) goto fail;
    } else {
      GpioSetConfSel8bit();
    }


    // open the bit stream
    if(!psc->file[0]) goto fail;
    re = f_open(&fpn, psc->file, FA_READ);
    if(re != FR_OK) goto fail;

    // transmit
    FpgaconfCsControl(num, 1);
    while(1) {
      //FpgaconfAccessLed(FPGACONF_ACCESS_SDMMC_SHIFT, 1);
      re = f_read(&fpn, buf, sizeof(buf), &size);
      //FpgaconfAccessLed(FPGACONF_ACCESS_SDMMC_SHIFT, 0);
      if(re != FR_OK || !size) break;

      //FpgaconfAccessLed(FPGACONF_ACCESS_FPGA_SHIFT, 1);
      if(psc->bus == FPGACONF_BUS_SPI) {
        FpgaconfSpiSend(0, (uint8_t *)buf, size);
      } else {
        FpgaconfSend8bitBus(0, (uint8_t *)buf, size);
      }
      //FpgaconfAccessLed(FPGACONF_ACCESS_FPGA_SHIFT, 0);

      total += size;
    }
    FpgaconfCsControl(num, 0);
    f_close(&fpn);


    // end
    if(psc->bus == FPGACONF_BUS_SPI) {
      if(psc->vendor == ChAscToLong('l', 'a', 't', 't')) {
        FpgaLatticeProgramEnd(num);
      } else if(psc->vendor == ChAscToLong('x', 'i', 'l', 'i')) {
      } else if(psc->vendor == ChAscToLong('a', 'r', 't', 'e')) {
      }
    }

  } else if(!strcmp(argv[2], "file") && argc >= 4) {
    psc->file[0] = '0';
    psc->file[1] = ':';
    strncpy(psc->file+2, argv[3], sizeof(psc->file)-2);

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
FpgaconfCsControl(int num, int active)
{
  int           result = 0;

  if(active) {
    switch(num) {
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
    switch(num) {
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
  //printf("spi send cmd %x %x\n", ptr, len);

  result = DevSpiSend(CONFIG_FPGA_SPI_DEVNUM, ptr, len);

  return result;
}
int
FpgaconfSpiRecv(int unit, uint8_t *ptr, int len)
{
  int           result;
  //printf("spi recv cmd %x %x\n", ptr, len);

  result = DevSpiRecv(CONFIG_FPGA_SPI_DEVNUM, ptr, len);

  return result;
}
