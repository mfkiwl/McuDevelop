#define _EXCEPTION_C_

#include        <stdint.h>
#include        <stdlib.h>
#include        <stdio.h>

#include        "config.h"


#ifndef CONFIG_INTERRUPT
#define CONFIG_INTERRUPT
#endif

#include        "exception.h"

#define INTR_NVIC_INTSET_REG            ((uint32_t *) 0xe000e100)
#define INTR_NVIC_INTCLR_REG            ((uint32_t *) 0xe000e180)
#define INTR_NVIC_STATSET_REG           ((uint32_t *) 0xe000e200)
#define INTR_NVIC_STATCLR_REG           ((uint32_t *) 0xe000e280)
#define INTR_NVIC_PRIORITY_REG          ((uint32_t *) 0xe000e400)
#define INTR_NVIC_INTSTATUS_REG         ((uint32_t *) 0xe000ed04)

#define         NVIC_INTSTATUS_VECTACTIVE_SHIFT (0UL)
#define         NVIC_INTSTATUS_VECTACTIVE_MASK  (0x1ffUL << (NVIC_INTSTATUS_VECTACTIVE_SHIFT))


/*****************************************************************
 * interrupt entry
 */
/*
 * @fn __interrupt void IntrException(void)
 * @brief The entry of the exceptions
 * @param[in]
 * @param[out]
 * @return none
 * @details
 */
CONFIG_INTERRUPT void
IntrException(void)
{
  struct _stIntrReg     reg;

  IntrStoreReg(&reg);

  IntrExceptionInterrupt(&reg, NULL);

  return;
}


/*
 * @fn __interrupt void IntrException(void)
 * @brief The register values are shown
 * @param[in] pr structure of mpu registers
 * @param[in] str exception name
 * @param[out]
 * @return none
 * @details
 */
static void
IntrExceptionInterrupt(struct _stIntrReg *pr, char *str)
{
  int           num;
  uint32_t *p;
  int           i;
  uint32_t *memtop = __sfe( "CSTACK" );

  p = (uint32_t *) pr->sp;
  printf("\r\n\r\n\r\n\r\n");
  printf("# IntrExceptionInterrupt() vec#%d sp:%x, lr:%x, pc:%x, level:%x\r\n",
         *INTR_NVIC_INTSTATUS_REG & NVIC_INTSTATUS_VECTACTIVE_MASK,
         pr->sp+0x20, p[5], p[6], pr->lr&0xf);
  if(str) {
    printf("# %s\r\n", str);
  }
  printf("# register info\r\n");
  printf("#  r0: %8x, r1: %8x, r2: %8x, r3: %8x\r\n", pr->r0,  pr->r1,  pr->r2,  pr->r3);
  printf("#  r4: %8x, r5: %8x, r6: %8x, r7: %8x\r\n", pr->r4,  pr->r5,  pr->r6,  pr->r7);
  printf("#  r8: %8x, r9: %8x, r10:%8x, r11:%8x\r\n", pr->r8,  pr->r9,  pr->r10, pr->r11);
  printf("#  r12:%8x, sp: %8x, lr: %8x, pc: %8x\r\n", pr->r12, pr->sp,  pr->lr,  pr->pc);
  printf("#  msp:%8x, psp:%8x\r\n", pr->msp, pr->psp);


  printf("#\r\n# stack info\r\n");
  p = (uint32_t *) pr->sp;
  p -= 0x10;
  for(i = 0; i < 0x10; i += 4) {
    printf("#  %8x:  %8x, %8x, %8x, %8x\r\n", p, p[0], p[1], p[2], p[3]);
    p += 4;
  }
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # r0,  r1,  r2,  r3\r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
  printf("#>>%8x:  %8x, %8x, %8x, %8x  # r12, lr,  pc,  psr\r\n", p, p[0], p[1], p[2], p[3]);
  p += 4;
  for(i = 0; i < 0x40 && p < (memtop-4); i += 4) {
    printf("#  %8x:  %8x, %8x, %8x, %8x\r\n", p, p[0], p[1], p[2], p[3]);
    p += 4;
  }

  num = *INTR_NVIC_INTSTATUS_REG & NVIC_INTSTATUS_VECTACTIVE_MASK;
  if(num < /*INTR_MASKABLE_FACTOR_START*/ 16) {
    printf("# reboot after 5 sec\r\n");
    /*CounterWaitSystemClock(5 * COUNTER_SYSTEM_1S000);*/
    /*SystemReboot(SYSTEM_BOOTFACTOR_POWERONRESET);*/
    while(1);

    /* never return */
  }

  return;
}
