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

#ifndef	_FPGA_H_
#define	_FPGA_H_



/* lattice fpga  command */
#define	FPGA_CMD_READ_IDCODE		0xe0
#define	FPGA_CMD_READ_TRACE_ID		0x19
#define	FPGA_CMD_READ_USERCODE		0xc0
#define	FPGA_CMD_REFRESH_DEVICE		0x79
#define	FPGA_CMD_SRAM_PROG_ENABLE	0xC6
#define	FPGA_CMD_SRAM_PROG_DISABLE	0x26
#define	FPGA_CMD_SRAM_ERASE		0x0e
#define	FPGA_CMD_SRAM_READ_STATUS	0x3c
#define	FPGA_CMD_SRAM_CONFIGURATION	0x7a
#define	FPGA_CMD_ERASE_FEATURES		0x0e
#define	FPGA_CMD_FEATUREROW_READ	0xe7
#define	FPGA_CMD_FEATUREROW_PROG	0xe4
#define	FPGA_CMD_FEABITS_READ		0xfb
#define	FPGA_CMD_FEABITS_PROG		0xf8
#define	FPGA_CMD_NOP			0xff


#define	FPGA_STATUS_CONFIG_MASK		0x00003100UL
#define	FPGA_STATUS_CONFIG_DONE		0x100UL
#define	FPGA_STATUS_CONFIG_NOTYET	0UL



#define	FPGA_USER_VERSION_LENGTH	8



int             FpgaLatticeProgramPre(int unit, uint32_t id);
int             FpgaLatticeProgramEnd(int unit);



#if 0
void			FpgaInit(void);
int			FpgaReset(int unit);
int			FpgaConfigurationPre(int unit);
int			FpgaConfigurationTransmit(int unit, unsigned char *ptr, int len);
int			FpgaConfigurationTransmitSpiData(int unit, uint32_t addr, int len);
int			FpgaConfigurationEnd(int unit);
#endif

int			FpgaLatticeReadDeviceID(int unit, unsigned long *pId);
int			FpgaLatticeReadTraceID(int unit, unsigned long *pId);
int			FpgaLatticeReadUsercode(int unit, unsigned long *pId);
int			FpgaLatticeResetDevice(int unit);
int			FpgaLatticeSramErase(int unit);
int			FpgaLatticeSramProgramEnable(int unit);
int			FpgaLatticeSramProgramDisable(int unit);
int			FpgaLatticeSramAccessEnable(int unit);
int			FpgaLatticeSramProgramNoCs(int unit);
int			FpgaLatticeSramReadStatus(int unit, unsigned long *pId);
int			FpgaLatticeEraseFeatures(int unit);
int			FpgaLatticeFeatureRowRead(int unit, unsigned long *pId);
int			FpgaLatticeFeabitsRead(int unit, unsigned long *pId);
int			FpgaLatticeFeabitsProgram(int unit, unsigned long *pId);
int			FpgaLatticeNop(int unit);

int			FpgaLatticeConfigRead(int unit, int page);


int			FpgaSend(int unit, unsigned char *ptr, int len);
int			FpgaSendCmd(int unit, unsigned char *ptr, int len);
int			FpgaRecv(int unit, unsigned char *ptr, int len);
int			FpgaSetSpiClkHigh(int unit);
int			FpgaSetSpiClkLow(int unit);
int			FpgaSetSpiClk(int unit, int pre, int div);

int			FpgaSetReg(int unit, unsigned short addr, unsigned short val);
int			FpgaGetReg(int unit, unsigned short addr, unsigned short *pVal);

#if CONFIG_INCLUDE_FPGA_CONFIGURATION
int			FpgaConfigurationFromPICROM();
#endif

#ifdef	_FPGA_C_
/*static int		FpgaSdEmpty(void *ptr);*/
#endif

#endif
