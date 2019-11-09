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

#define _FPGA_LATTICE_C_

#include        <stdint.h>
#include        <string.h>

#include        "config.h"
#include        "rtos.h"

#include        "fpgaconf.h"
#include        "fpga_lattice.h"


int
FpgaLatticeProgramPre(int unit, uint32_t idExp)
{
  int           result = -1;
  uint32_t      id;

  FpgaLatticeReadDeviceID(unit, &id);
  if(id != idExp) {
    goto fail;
  }
  RtosTaskSleep(1);
  FpgaLatticeResetDevice(unit);
  RtosTaskSleep(2);

  FpgaLatticeSramProgramEnable(unit);
  FpgaLatticeSramErase(unit);
  RtosTaskSleep(10);

  for(int i = 0; i < CONFIG_FPGA_SPI_STATUS_RETRY; i++) {
    /*** read sram status */
    FpgaLatticeSramReadStatus(unit, &id);
    if((id & FPGA_STATUS_CONFIG_MASK) == FPGA_STATUS_CONFIG_NOTYET) break;
    RtosTaskSleep(1);
  }
  FpgaconfCsControl(unit, 1);
  FpgaLatticeSramProgramNoCs(unit);

fail:
  return result;
}


int
FpgaLatticeProgramEnd(int unit)
{
  int           result = -1;

  uint32_t      id;
  int           i;

  i = 0;
  while(1) {
    /*** read sram status */
    FpgaLatticeSramReadStatus(unit, &id);
    if((id & FPGA_STATUS_CONFIG_MASK) == FPGA_STATUS_CONFIG_DONE) break;
    if(i == /*CONFIG_FPGA_STATUS_RETRY*/ 1000) goto fail;
    i++;
    RtosTaskSleep(1);
  }

#if 0
  /*** read user code */
  FpgaLatticeReadUsercode(unit, &id);                // not need
  printf("user %x\n", id);
#endif

  /*** disable sram program */
  FpgaLatticeSramProgramDisable(unit);

  /*** send nop */
  FpgaLatticeNop(unit);

  result = 0;

fail:
  return result;
}




/***************************************************************
 * device command layer
 */
int
FpgaLatticeReadDeviceID(int unit, unsigned long *pId)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_READ_IDCODE, 0x00, 0x00, 0x00
  };
  uint8_t               buf[256];
  uint32_t              id;

  if(unit < 0 || !pId) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 4);
  FpgaconfSpiRecv(unit, (uint8_t *)buf, 4);
  FpgaconfCsControl(unit, 0);

  id  = (uint32_t)buf[0] << 24;
  id |= (uint32_t)buf[1] << 16;
  id |= (uint32_t)buf[2] <<  8;
  id |= (uint32_t)buf[3] <<  0;

  *pId = id;

  result = 0;

fail:
  return result;
}

int
FpgaLatticeReadTraceID(int unit, unsigned long *pId)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_READ_TRACE_ID, 0x00, 0x00, 0x00
  };
  uint8_t       buf[16];
  uint32_t      id0, id1;

  if(unit < 0 || !pId) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 8);
  FpgaconfSpiRecv(unit, buf, 8);
  FpgaconfCsControl(unit, 0);

  id0  = (unsigned long)buf[0] << 24;
  id0 |= (unsigned long)buf[1] << 16;
  id0 |= (unsigned long)buf[2] <<  8;
  id0 |= (unsigned long)buf[3];
  id1  = (unsigned long)buf[4] << 24;
  id1 |= (unsigned long)buf[5] << 16;
  id1 |= (unsigned long)buf[6] <<  8;
  id1 |= (unsigned long)buf[7];
  pId[0] = id0;
  pId[1] = id1;

  result = 0;
fail:
  return result;
}

int
FpgaLatticeReadUsercode(int unit, unsigned long *pId)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_READ_USERCODE, 0x00, 0x00, 0x00
  };
  uint8_t       buf[256];
  uint32_t      id;

  if(unit < 0 || !pId) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 4);
  FpgaconfSpiRecv(unit, buf, 4);
  FpgaconfCsControl(unit, 0);

  id  = (unsigned long)buf[0] << 24;
  id |= (unsigned long)buf[1] << 16;
  id |= (unsigned long)buf[2] <<  8;
  id |= (unsigned long)buf[3];
  *pId = id;

  result = 0;

fail:
  return result;
}


int
FpgaLatticeResetDevice(int unit)
{
  int           result = -1;

  const uint8_t code1[] = {
    FPGA_CMD_REFRESH_DEVICE, 0x00, 0x00, 0x00
  };

  if(unit < 0) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)&code1[0], 4);
  FpgaconfCsControl(unit, 0);

  result = 0;

fail:
  return result;
}

int
FpgaLatticeSramErase(int unit)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_SRAM_ERASE, 0x01, 0x00, 0x00
  };

  if(unit < 0) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 4);
  FpgaconfCsControl(unit, 0);

  result = 0;

fail:
  return result;
}

int
FpgaLatticeSramProgramEnable(int unit)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_SRAM_PROG_ENABLE, 0x00, 0x00, 0x00
  };

  if(unit < 0) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 4);
  FpgaconfCsControl(unit, 0);

  result = 0;

fail:
  return result;
}

int
FpgaLatticeSramProgramDisable(int unit)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_SRAM_PROG_DISABLE, 0x00, 0x00, 0x00
  };

  if(unit < 0) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 4);
  FpgaconfCsControl(unit, 0);

  result = 0;

fail:
  return result;
}

int
FpgaLatticeSramAccessEnable(int unit)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_SRAM_PROG_ENABLE, 0x08, 0x00, 0x00
  };

  if(unit < 0) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 3);
  FpgaconfCsControl(unit, 0);

  result = 0;

fail:
  return result;
}

int
FpgaLatticeSramProgramNoCs(int unit)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_SRAM_CONFIGURATION, 0x00, 0x00, 0x00
  };

  if(unit < 0) goto fail;

  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 4);

  result = 0;

fail:
  return result;
}

int
FpgaLatticeSramReadStatus(int unit, unsigned long *pId)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_SRAM_READ_STATUS, 0x00, 0x00, 0x00
  };
  uint8_t       buf[256];
  uint32_t      id;

  if(unit < 0 || !pId) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (unsigned char *)cmd, 4);
  FpgaconfSpiRecv(unit, buf, 4);
  FpgaconfCsControl(unit, 0);

  id  = (unsigned long)buf[0] << 24;
  id |= (unsigned long)buf[1] << 16;
  id |= (unsigned long)buf[2] <<  8;
  id |= (unsigned long)buf[3];
  *pId = id;

  result = 0;

fail:
  return result;
}


int
FpgaLatticeFeatureRowRead(int unit, unsigned long *pId)
{
  int           result = -1;

  const uint8_t cmd[] = {
    FPGA_CMD_FEATUREROW_READ, 0x00, 0x00, 0x00
  };
  uint8_t       buf[8];
  uint32_t      id0, id1;

  if(unit < 0 || !pId) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (uint8_t *)cmd, 4);
  FpgaconfSpiRecv(unit, buf, 8);
  FpgaconfCsControl(unit, 0);

  id0  = (unsigned long)buf[0] << 24;
  id0 |= (unsigned long)buf[1] << 16;
  id0 |= (unsigned long)buf[2] <<  8;
  id0 |= (unsigned long)buf[3];
  id1  = (unsigned long)buf[4] << 24;
  id1 |= (unsigned long)buf[5] << 16;
  id1 |= (unsigned long)buf[6] <<  8;
  id1 |= (unsigned long)buf[7];
  *pId++ = id0;
  *pId   = id1;

  result = 0;

fail:
  return result;
}


int
FpgaLatticeFeabitsRead(int unit, unsigned long *pId)
{
  int			result = -1;

  const unsigned char	cmd[] = {
    FPGA_CMD_FEABITS_READ, 0x00, 0x00, 0x00
  };
  unsigned char		buf[2];
  unsigned long		id;

  if(unit < 0 || !pId) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (unsigned char *)cmd, 4);
  FpgaconfSpiRecv(unit, buf, 2);
  FpgaconfCsControl(unit, 0);

  id  = (unsigned long)buf[0] << 8;
  id |= (unsigned long)buf[1];
  *pId   = id;

  result = 0;

fail:
  return result;
}
int
FpgaLatticeFeabitsProgram(int unit, unsigned long *pId)
{
  int			result = -1;

  const unsigned char	cmd[] = {
    FPGA_CMD_FEABITS_PROG, 0x00, 0x00, 0x00
  };
  unsigned char		buf[2];

  if(unit < 0 || !pId) goto fail;

  buf[0] = (*pId >> 8) & 0xffUL;
  buf[1] =  *pId       & 0xffUL;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (unsigned char *)cmd, 4);
  FpgaconfSpiSend(unit, buf, 2);
  FpgaconfCsControl(unit, 0);

  result = 0;

fail:
  return result;
}


int
FpgaLatticeEraseFeatures(int unit)
{
  int			result = -1;

  const unsigned char	cmd[] = {
    FPGA_CMD_ERASE_FEATURES, 0x02, 0x00, 0x00
  };
  if(unit < 0) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (unsigned char *)&cmd, 4);
  FpgaconfCsControl(unit, 0);

  result = 0;

fail:
  return result;
}


int
FpgaLatticeNop(int unit)
{
  int			result = -1;

  const unsigned char	cmd[] = {
    FPGA_CMD_NOP, FPGA_CMD_NOP, FPGA_CMD_NOP, FPGA_CMD_NOP
  };

  if(unit < 0) goto fail;

  FpgaconfCsControl(unit, 1);
  FpgaconfSpiSendCmd(unit, (unsigned char *)cmd, 4);
  FpgaconfCsControl(unit, 0);

  result = 0;

fail:
  return result;
}


int
FpgaLatticeConfigRead(int unit, int page)
{
  int			result = -1;
  int			cmd;
  int			i;

  const unsigned char	str1[][5] = {
    {0, 0xc6, 0x00, 0x00, 0x00},
    {0, 0x0e, 0x01, 0x00, 0x00},
    {0, 0xc6, 0x08, 0x00, 0x00},
    {2, 0x00, 0x00, 0x00, 0x05},	/* wait  5 usec*/
    {0, 0x46, 0x00, 0x00, 0x00},
    {1, 0x73, 0x10, 0x00, 0x00},	/* read */
    {0, 0x26, 0x00, 0x00, 0x00},
    {0, 0xff, 0xff, 0xff, 0xff},
  };
  unsigned char		buf[256];


  for(i = 0; i < sizeof(str1)/5; i++) {
    cmd = str1[i][0];
    switch(cmd) {
    case	0:
      FpgaconfCsControl(unit, 1);
      FpgaconfSpiSendCmd(unit, (unsigned char *)&str1[i][1], 4);
      FpgaconfCsControl(unit, 0);
      break;
    case	1:
      memcpy(buf, &str1[i][1], 4);

      buf[2] = (page >> 8) & 0xff;
      buf[3] =  page       & 0xff;
      FpgaconfCsControl(unit, 1);
      FpgaconfSpiSendCmd(unit, buf, 4);
      FpgaconfSpiRecv(unit, buf, 256);
      FpgaconfSpiRecv(unit, buf, 256);
      FpgaconfSpiRecv(unit, buf, 256);
      FpgaconfSpiRecv(unit, buf, 256);
      FpgaconfCsControl(unit, 0);
      break;
    case	2:
      break;
    }
  }

  return result;
}
