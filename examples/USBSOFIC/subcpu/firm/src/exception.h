#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_



struct _stIntrReg {
  uint32_t              r0;
  uint32_t              r1;
  uint32_t              r2;
  uint32_t              r3;
  uint32_t              r4;
  uint32_t              r5;
  uint32_t              r6;
  uint32_t              r7;
  uint32_t              r8;
  uint32_t              r9;
  uint32_t              r10;
  uint32_t              r11;
  uint32_t              r12;
  uint32_t              sp;     /* r13 */
  uint32_t              lr;     /* r14 */
  uint32_t              pc;     /* r15 */
  uint32_t              msp;
  uint32_t              psp;
  uint32_t              stack[0x20];
};


CONFIG_INTERRUPT void   IntrException(void);


#ifdef  _EXCEPTION_C_
static void             IntrExceptionInterrupt(struct _stIntrReg *pr, char *str);
#endif

#endif
