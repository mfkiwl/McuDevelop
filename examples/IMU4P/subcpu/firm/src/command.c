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
#include        <stdlib.h>
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>

#include        "config.h"
#include        "system.h"
#include        "imu.h"
#include        "main.h"
#include        "devFlash.h"

#include        "command.h"


extern mainSetting_t            setting;

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
  int           re;

  if(!strncmp(av[0], "reset", 5)) {
    MainResetTimCounter();

  } else if(!strncmp(av[0], "start", 5)) {
    MainEnableTim();

  } else if(!strncmp(av[0], "stop", 4)) {
    MainDisableTim();

  } else if(!strncmp(av[0], "init", 4)) {
    for(int i = 0; i < CONFIG_NUM_OF_IMUS; i++) {
      ImuInit(i);
    }

  } else if(!strncmp(av[0], "format", 6)) {
    setting.format = strtoul(av[1], NULL, 10);

  } else if(!strncmp(av[0], "imuno", 5)) {
    uint32_t    addr = EEPROM_BASE;
    uint8_t     no;
    addr = EEPROM_BASE + MAIN_SETTING_IMUNO_POS;

    if(ac >= 2) {
      no = strtoul(av[1], NULL, 10);
      setting.imuno = no;
      DevFlashProgram(0, addr, MAIN_SETTING_IMUNO_LEN, &no);
    } else {
      printf("imuno %d\n", *(uint8_t *) addr);
    }

  } else if(!strncmp(av[0], "imu", 3)) {
    int         unit;
    imuSetting_t settings;
    unit = strtoul(av[1], NULL, 10);
    if(!strncmp(av[2], "get", 3)) {
      re = ImuGetSettings(unit, &settings);
      printf("%dHz %d0mG %d0DPS\n",
             settings.freq, settings.acc_fs, settings.gyro_fs);
    } else if(!strncmp(av[2], "set", 3)) {
      if(ac >= 4) {
        memset(&settings, 0xff, sizeof(settings));
        settings.freq = strtoul(av[3], NULL, 10);
        if(ac >= 5) settings.acc_fs  = strtoul(av[4], NULL, 10);
        if(ac >= 6) settings.gyro_fs = strtoul(av[5], NULL, 10);
        ImuSetSettings(unit, &settings);
      }
    } else if(!strncmp(av[2], "regset", 6)) {
      if(ac >= 5) {
        uint8_t         reg, val;
        reg = strtoul(av[3], NULL, 16);
        val = strtoul(av[4], NULL, 16);
        ImuSetValueStandard(unit, reg, val);
      }
    }

  } else if(!strncmp(av[0], "id", 2)) {
    uint8_t     id[MAIN_SETTING_ID_LEN+1];
    int         addr = EEPROM_BASE;
    if(ac >= 3 && !strncmp(av[1], "set", 3)) {
      strncpy(id, av[2], MAIN_SETTING_ID_LEN);
      id[MAIN_SETTING_ID_LEN] = '\0';
      addr = EEPROM_BASE + MAIN_SETTING_ID_POS;
      DevFlashProgram(0, addr, MAIN_SETTING_ID_LEN, &id[0]);
    } else if(!strncmp(av[1], "get", 3)) {
      addr = EEPROM_BASE + MAIN_SETTING_ID_POS;
      puts((uint8_t *)addr);
      puts("\n");
    }

  } else if(!strncmp(av[0], "reboot", 6)) {
    IntrReboot(INTR_REBOOT_NORMAL);

  } else if(!strncmp(av[0], "firmupdate", 10)) {
    IntrReboot(INTR_REBOOT_UPDATE);

  } else if(!strncmp(av[0], "systemboot", 10)) {


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
    extern const uint8_t        strVersionText[];
    uint8_t                     **ptr = (uint8_t **)CONFIG_BOOTLOADER_INFO_VERSION_POS;
    puts(strVersionText);
    puts("\n");
    puts(*ptr);
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
