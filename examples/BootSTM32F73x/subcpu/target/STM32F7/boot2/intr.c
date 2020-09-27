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

#define	_INTR_C_

/******************************************************************************
 * include:
 *  - system include
 *  - application include
 *****************************************************************************/
#include        <stdio.h>

#include        "config.h"
#include        "system.h"

#include        "intr.h"
#include        "intrvec.h"

/******************************************************************************
 * type definition (struct / enum):
 *****************************************************************************/


/******************************************************************************
 * constants & variable:
 *  - constant
 *  - global variable
 *  - static variable
 *****************************************************************************/

#define	INTR_NVIC_INTSET_REG		((unsigned long *) 0xe000e100)
#define	INTR_NVIC_INTCLR_REG		((unsigned long *) 0xe000e180)
#define	INTR_NVIC_STATSET_REG		((unsigned long *) 0xe000e200)
#define	INTR_NVIC_STATCLR_REG		((unsigned long *) 0xe000e280)
#define	INTR_NVIC_PRIORITY_REG		((unsigned long *) 0xe000e400)
#define	INTR_NVIC_INTSTATUS_REG		((unsigned long *) 0xe000ed04)

#define		NVIC_INTSTATUS_VECTACTIVE_SHIFT	(0)
#define		NVIC_INTSTATUS_VECTACTIVE_MASK	(0x1ff << (NVIC_INTSTATUS_VECTACTIVE_SHIFT))

extern CONFIG_INTERRUPT void    Reset_Handler(void);
extern CONFIG_INTERRUPT void    SystemSysTickIntr(void);

extern uint32_t         sectStackTop[];

extern void *__vector_table;
#if 0
/* vector on flash rom */
void *__vector_table[] __attribute__((section(".isr_vector"))) = {
  &sectStackTop,                        /*      0 stack                    */
  Reset_Handler,                        /* -15  1 reset vector             */
  IntrException,                        /* -14  2 nmi                      */
  IntrException,                        /* -13  3 hard fault               */
  IntrException,                        /* -12  4 memory fault             */
  IntrException,                        /* -11  5 bus fault                */
  IntrException,                        /* -10  6 usage fault              */
  0, 0, 0, 0,                           /*  -6 10 reserved                 */
  SVC_Handler,                          /*  -5 11 SVC                      */
  IntrException,                        /*  -4 12 debug                    */
  0,                                    /*  -3 13 reserved                 */
  PendSV_Handler,                       /*  -2 14 Pend System Service      */
  SysTick_Handler,                      /*  -1 15 system tick timer    FreeRTOS */
  //SystemSysTickIntr,                    /*  -1 15 system tick timer  system.c */
};
#endif

#if     CONFIG_INTERRUPT_TABLE_ON_RAM
extern uint32_t         sectVectorRamStart[];
#endif

#define INTR_SP_OFFSET          8


/*
 * int IntrInit(int unit)
 *   input:  int unit
 *   output: void
 *
 * description:
 *	interrupt initialize function
 *
 */
int
IntrInit(int unit)
{
  int                   i;
  void                  (*(*p))(void), (*(*s))(void);

#if     CONFIG_INTERRUPT_TABLE_ON_RAM
  p = (void (*(*))()) &sectVectorRamStart;
  s = (void (*(*))()) SCB->VTOR;
  for(i = 0; i < 0x10; i++) {
    p[i] = s[i];
  }
  for(i = 0x10; i < CONFIG_INTERRUPT_TABLE_LENGTH; i++) {
    p[i] = IntrException;
  }
#endif

  for(i = INTR_MASKABLE_FACTOR_START;
      i < INTR_MASKABLE_FACTOR_END; i++) {
    NVIC_SetPriority(i, INTR_PRIORITY_MIDDLE);
  }

#if     CONFIG_INTERRUPT_TABLE_ON_RAM
  SCB->VTOR = (uint32_t) &sectVectorRamStart;
#else   /* in the flash */
  SCB->VTOR = (uint32_t)&__vector_table;
#endif

  return 0;
}


/*
 * int IntrSetEntry(int unit, int irq, intr_t ptr)
 * int IntrDisableAll(int unit, int irq)
 *   input:  int unit
 *   output: void
 *
 * description:
 *	interrupt enable
 *
 */
int
IntrSetEntry(int unit, int irq, void (*ptr)(void))
{
  int                   result = -1;

  int                   off, pos;
  uint32_t              *vp;

#if     CONFIG_INTERRUPT_TABLE_ON_RAM
  vp = (uint32_t *) &sectVectorRamStart;
  //vp = (uint32_t *) 0x20000000;

  irq += INTR_MASKABLE_FACTOR_START;

  if(irq < 2 ||
     irq >= CONFIG_INTERRUPT_TABLE_LENGTH) {
    goto end;
  }

  vp[irq] = (uint32_t)ptr;

  result = irq;
#endif

end:
  return result;
}
int
IntrDisableAll(int unit)
{
  volatile unsigned long	*p;
  int				i;

  p = INTR_NVIC_INTCLR_REG;
  for(i = 0; i < 8; i++) {
    *p++ = 0xffffffff;
  }

  return 0;
}
#if 0
int
IntrShowNvicIntset(int unit)
{
  int				result = 0;

#if INTERRPUT_PRINTF
  int				i;
  volatile unsigned long	*p;

  p = INTR_NVIC_INTSET_REG;
  for(i = 0; i < 7; i++) {
    printf("NVIC.INTSET[%d]: %08x\n", i, *(p + i));
  }
#endif

  return result;
}
#endif


/*****************************************************************
 * interrupt entry
 */
/*
 * __interrupt void IntrException(void)
 *   input:  void
 *   output: void
 *
 * description:
 *	
 *
 */
CONFIG_INTERRUPT void
IntrException(void)
{
  struct _stIntrReg	reg;
  char			*str = NULL;
  int			num;
  
  asm("str r0, [sp, #0]");
  asm("mov %0, r1":  "+r"(reg.r1));
  asm("mov %0, r2":  "+r"(reg.r2));
  asm("mov %0, r3":  "+r"(reg.r3));
  asm("mov %0, r4":  "+r"(reg.r4));
  asm("mov %0, r5":  "+r"(reg.r5));
  asm("mov %0, r6":  "+r"(reg.r6));
  asm("mov %0, r7":  "+r"(reg.r7));
  asm("mov %0, r8":  "+r"(reg.r8));
  asm("mov %0, r9":  "+r"(reg.r9));
  asm("mov %0, r10": "+r"(reg.r10));
  asm("mov %0, r11": "+r"(reg.r11));
  asm("mov %0, r12": "+r"(reg.r12));

  asm("mov %0, sp": "+r"(reg.sp));
  asm("mov %0, lr": "+r"(reg.lr));
  asm("mov %0, pc": "+r"(reg.pc));
  reg.sp += sizeof(struct _stIntrReg) + (INTR_SP_OFFSET);

  reg.control = __get_CONTROL();
  reg.msp = __get_MSP();
  reg.psp = __get_PSP();
  reg.primask = __get_PRIMASK();
#if defined(__get_FAULTMASK)
  reg.primask = __get_FAULTMASK();
#else
  reg.primask = 0;
#endif
#if defined(__get_BASEPRI)
  reg.primask = __get_BASEPRI();
#else
  reg.primask = 0;
#endif

  IntrExceptionInterrupt(&reg);

  return;
}


static void
IntrExceptionInterrupt(struct _stIntrReg *pr)
{
  char		*str = NULL;
  int		num;
  unsigned long	*p;
  int		i;
  uint32_t      *memtop = (uint32_t *)&sectStackTop;

  num = *INTR_NVIC_INTSTATUS_REG & NVIC_INTSTATUS_VECTACTIVE_MASK;

  p = (uint32_t *) pr->sp;
#if 0
  puts("\r\n\r\n\r\n\r\n");
  printf("# IntrExceptionInterrupt() vec#%d sp:%x, lr:%x, pc:%x, level:%x\r\n",
         num-INTR_MASKABLE_FACTOR_START, pr->sp, p[5], p[6], pr->lr&0xf);

  switch(num) {
  case  2: str = "exception NMI"; break;
  case  3: str = "exception HardFault"; break;
  case  4: str = "exception MemManage"; break;
  case  5: str = "exception BusFault"; break;
  case  6: str = "exception Usage"; break;
  case 11: str = "exception SVC"; break;
  case 12: str = "exception DebugMon"; break;
  case 14: str = "exception PendSV"; break;
  case 15: str = "exception SysTick"; break;
  }
  if(str) {
    printf("# %s\r\n", str);
  }
  printf("# register info\r\n");
  printf("#  r0: %8x, r1: %8x, r2: %8x, r3: %8x\r\n", pr->r0,  pr->r1,  pr->r2,  pr->r3);
  printf("#  r4: %8x, r5: %8x, r6: %8x, r7: %8x\r\n", pr->r4,  pr->r5,  pr->r6,  pr->r7);
  printf("#  r8: %8x, r9: %8x, r10:%8x, r11:%8x\r\n", pr->r8,  pr->r9,  pr->r10, pr->r11);
  printf("#  r12:%8x, sp: %8x, lr: %8x, pc: %8x\r\n", pr->r12, pr->sp,  pr->lr,  pr->pc);
  printf("#  msp:%8x, psp:%8x\r\n", pr->msp, pr->psp);
  printf("#  control:%8x\r\n", pr->control);
  printf("#  primask:%8x, faultmask:%8x, basepri:%8x\r\n", pr->primask, pr->faultmask, pr->basepri);
  printf("#  SCB->VTOR:%x\r\n", SCB->VTOR);

  puts("#\r\n# stack info\r\n");
  p = (unsigned long *) pr->msp;
  p -= 0x10;
  for(i = 0; i < 0x10; i += 4) {
    printf("#  %8x:  %8x, %8x, %8x, %8x\r\n", p, p[0], p[1], p[2], p[3]);
    p += 4;
  }
#if 0           /* IAR */
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # r0,  r1,  r2,  r3\r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # r12, lr,  pc,  psr\r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
#endif
#if 1           /* gcc */
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # r0,  r1,  r2,  r3\r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # r4,  r5,  r6,  r7\r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # r8,  r9, r10, r11\r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # r12, sp,  lr, psr\r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # ???,msp, psp,    \r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
#endif
  for(i = 0; i < 0x40 && p < (memtop-4); i += 4) {
    printf("#  %8x:  %8x, %8x, %8x, %8x\r\n", p, p[0], p[1], p[2], p[3]);
    p += 4;
  }

  if(num < INTR_MASKABLE_FACTOR_START) {
    puts("# reboot after 5 sec\r\n");
#if 0
    CounterWaitSystemClock(5 * COUNTER_SYSTEM_1S000);
    SystemReboot(SYSTEM_BOOTFACTOR_POWERONRESET);
#endif

    /* never return */
    while(1);
  }
#endif

  return;
}


/*
 * eof
 */
