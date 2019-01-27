/*
 * Copyright (c) 2018,2019 zhtlab
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

#define _COMMAND_C_

#include        <stdint.h>
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>

#include        "config.h"
#include        "system.h"

#include        "kicad.h"

#include        "command.h"


#define COMMAND_WORDS_MAX       16
int             commandArgc;
uint8_t         *commandArgv[COMMAND_WORDS_MAX];
int
CommandDelimiter(uint8_t *pStr, uint8_t *av[])
{
  uint8_t       *p;
  int           ac = 0;

  p = strtok(pStr, " ");
  while(p) {
    if(ac >= COMMAND_WORDS_MAX) {
      break;
    }
    av[ac] = p;
    p = strtok(NULL, " ");
    ac++;
  }

  return ac;
}


int
CommandExec(int ac, uint8_t *av[])
{
  if(!strncmp(av[0], "getlist", 7)) {
    KicadShowKeyMapAll();

  } else if(!strncmp(av[0], "setkey", 6)) {
    //printf("XX %d  %s %s %s %s\n", ac, av[1], av[3], av[4], av[5]);
    KicadCommandSetKey(ac, av);

  } else if(!strncmp(av[0], "save", 4)) {
    KicadSaveToEeprom();

  } else if(!strncmp(av[0], "load", 4)) {
    KicadLoadFromEeprom();

  } else if(!strncmp(av[0], "erase", 5)) {
    KicadEraseEeprom();

  } else if(!strncmp(av[0], "dump", 4)) {
    KicadDumpEeprom();

  } else if(!strncmp(av[0], "reboot", 6)) {
    DevUsbDeinit(1);
    IntrReboot(INTR_REBOOT_NORMAL);

  } else if(!strncmp(av[0], "firmupdate", 10)) {
    DevUsbDeinit(1);
    IntrReboot(INTR_REBOOT_UPDATE);

  } else if(!strncmp(av[0], "systemboot", 10)) {

#if 0
    //GpioSetUsbDisable();
    UsbifCbDeInit(0, 0);
    RtosTaskSleep(2000);
    SystemDeinit();
#endif
    DevUsbDeinit(1);
    RtosTaskSleep(500);

    __disable_irq();
    SystemDeinit();
    for(int i = 0; i < 0x80; i++) {
      NVIC_DisableIRQ((IRQn_Type)i);
    }
    //IntrReboot(INTR_REBOOT_UPDATE);

    RCC_PTR->APB2ENR |= RCC_APB2ENR_SYSCFGEN_YES;
    SYSCFG_PTR->CFGR1 =  SYSCFG_CFGR1_MEM_MODE_SYSTEMFLASH;
    SYSCFG_PTR->CFGR3 =  0;

    __set_MSP(*((uint32_t*) 0x0000));
    ((void (*)(void)) *((uint32_t*) 0x0004))();

  } else if(!strncmp(av[0], "version", 7)) {
    extern const uint8_t           strVersionText[];
    puts(strVersionText);
    puts("\n");
  }

  return 0;
}


int
CommandProcess(uint8_t *pStr)
{
  commandArgc = CommandDelimiter(pStr, commandArgv);
#if 0
  printf("# Cmd ac:%d av:", commandArgc);
  for(int i = 0; i < commandArgc; i++) {
    printf("[%s]", commandArgv[i]);
  }
  puts("\n");
#endif
  if(commandArgc > 0 && commandArgv[0][0] != '#') {
    CommandExec(commandArgc, commandArgv);
  }

  return 0;
}
