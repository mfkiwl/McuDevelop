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

#ifndef _INTR_H_
#define _INTR_H_


/* definition */
#define	INTR_RESULT_SUCCESS		0
#define	INTR_RESULT_UNKNOWN		(-1)

#define	INTR_NULL				((void *)0)

#define	INTR_ENABLE()	(CONFIG_INTR_ENABLE())
#define	INTR_DISABLE()	(CONFIG_INTR_DISABLE())



/* structure */


/* prototypes */
int		IntrInit(int unit);
int		IntrSetEntry(int unit, int irq, void (*ptr)(void));
int		IntrDisableAll(int unit);
#define	INTR_PRIORITY_HIGHEST		(0x00)
#define	INTR_PRIORITY_HIGHER		(0x40)
#define	INTR_PRIORITY_MIDDLE		(0x80)
#define	INTR_PRIORITY_LOWER		(0xc0)
#define	INTR_PRIORITY_LOWEST		(0xf0)
int			IntrShowNvicIntset(int unit);

int		IntrExternalInit(int unit, int numIntr, int level, void *pIntr);
int		IntrExternalSetLevel(int unit, int numIntr, int level);
int		IntrExternalEnable(int unit, int numIntr);
int		IntrExternalDisable(int unit, int numIntr);



int		IntrGetNvicNumSerial(int unit);
int		IntrGetNvicNumDma(int unit);


void            IntrException(void);

void            SVC_Handler(void);
void            PendSV_Handler(void);
void            SysTick_Handler(void);

#ifdef	_INTR_C_
/*static void		IntrSomeFunction(void);*/

struct _stIntrReg {
  unsigned long		r0;
  unsigned long		r1;
  unsigned long		r2;
  unsigned long		r3;
  unsigned long		r4;
  unsigned long		r5;
  unsigned long		r6;
  unsigned long		r7;
  unsigned long		r8;
  unsigned long		r9;
  unsigned long		r10;
  unsigned long		r11;
  unsigned long		r12;
  unsigned long		sp;	/* r13 */
  unsigned long		lr;	/* r14 */
  unsigned long		pc;	/* r15 */
  unsigned long		fp;
  unsigned long		msp;
  unsigned long		psp;
  unsigned long		control;
  unsigned long		primask;
  unsigned long		faultmask;
  unsigned long		basepri;
  unsigned long		stack[0x20];
};

static void             IntrExceptionInterrupt(struct _stIntrReg *pr);

#endif


#endif	/* _INTR_H_ */
