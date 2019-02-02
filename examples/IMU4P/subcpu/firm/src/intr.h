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

#ifndef	_INTR_H_
#define	_INTR_H_


/* definition */
#define	INTR_RESULT_SUCCESS		0
#define	INTR_RESULT_UNKNOWN		(-1)

#define	INTR_NULL				((void *)0)

#define	INTR_ENABLE()	(CONFIG_INTR_ENABLE())
#define	INTR_DISABLE()	(CONFIG_INTR_DISABLE())

typedef void	(*intrFunc_t)(void);
typedef union {
  intrFunc_t	__func;
  void		*__ptr;
} intr_t;

typedef void	(*intrExtFunc_t)(int);
typedef union {
  intrExtFunc_t	__func;
  void		*__ptr;
} intrExt_t;


/* structure */
struct _stIntr {
  int		unit;
};


/* prototypes */
int			IntrInit(int unit);
int			IntrSetEntry(int unit, int irq, void (*ptr)(void));
int			IntrEnable(int unit, int irq);
int			IntrDisable(int unit, int irq);
int			IntrDisableAll(int unit);
int			IntrGetMask(int unit, unsigned long *ptr, int cnt);
int			IntrSetMask(int unit, unsigned long *ptr, int cnt);
int			IntrClear(int unit, int irq);
int			IntrSetPriority(int unit, int irq, int pri);
int			IntrGetPriority(int unit, int irq, int *pPri);
#define	INTR_PRIORITY_HIGHEST		(0x00)
#define	INTR_PRIORITY_HIGHER		(0x40)
#define	INTR_PRIORITY_MIDDLE		(0x80)
#define	INTR_PRIORITY_LOWER		(0xc0)
#define	INTR_PRIORITY_LOWEST		(0xf0)
int			IntrShowNvicIntset(int unit);

int			IntrExternalInit(int unit, int numIntr, int level, void *pIntr);
int			IntrExternalSetLevel(int unit, int numIntr, int level);
int			IntrExternalEnable(int unit, int numIntr);
int			IntrExternalDisable(int unit, int numIntr);

void                    IntrReboot(int factor);
#define INTR_REBOOT_NORMAL      0
#define INTR_REBOOT_UPDATE      1

enum {
  INTR_EXTFACTOR_NONE = 0,

  INTR_EXTFACTOR_RFWOL,
  INTR_EXTFACTOR_WLANWOL,

  INTR_EXTFACTOR_REMOCON = 5,
  INTR_EXTFACTOR_CEC = 9,
  INTR_EXTFACTOR_ETHWOL = 23,
  INTR_EXTFACTOR_POWERSW = 31,


  INTR_EXTFACTOR_MAX
};

enum interruptLines {
  INTR_MAIN_STACK =            0, /* Main Stack                   */
  INTR_RESETI =                1, /* Reset                        */
  INTR_NMII =                  2, /* Non-maskable Intr       */
  INTR_HFI =                   3, /* Hard Fault                   */
  INTR_MMI =                   4, /* Memory Management            */
  INTR_BFI =                   5, /* Bus Fault                    */
  INTR_UFI =                   6, /* Usage Fault                  */
  INTR_SVCI =                 11, /* SVCall                       */
  INTR_DMI =                  12, /* Debug Monitor                */
  INTR_PSI =                  14, /* PendSV                       */
  INTR_STI =                  15, /* SysTick                      */

  INTR_MASKABLE_FACTOR_START = 16,
  INTR_NVIC_DMA0 =		16, /* DMA 0			*/
  INTR_NVIC_DMA1 =		17, /* DMA 1			*/
  INTR_NVIC_DMA2 =		18, /* DMA 2			*/
  INTR_NVIC_DMA3 =		19, /* DMA 3			*/
  INTR_NVIC_20 =		20, /*				*/
  INTR_NVIC_FTFA =		21, /* Command completion	*/
  INTR_NVIC_PMC =		22, /* Low voltage detect	*/
  INTR_NVIC_LLWU =		23, /* Wakeup			*/
  INTR_NVIC_I2C0 =		24, /* I2C ch0			*/
  INTR_NVIC_I2C1 =		25, /* I2C ch1			*/
  INTR_NVIC_SPI0 =		26, /* SPI ch0			*/
  INTR_NVIC_SPI1 =		27, /* SPI ch1			*/
  INTR_NVIC_LPUART0 =		28, /* LPUART ch0		*/
  INTR_NVIC_LPUART1 =		29, /* LPUART ch1		*/
  INTR_NVIC_UART2FLEXIO =	30, /* UART2 or FLEXIO		*/
  INTR_NVIC_ADC0 =		31, /* ADC ch0			*/
  INTR_NVIC_CMP0 =		32, /* Comparator ch0		*/
  INTR_NVIC_TPM0 =		33, /* Timer module ch0		*/
  INTR_NVIC_TPM1 =		34, /* Timer module ch1		*/
  INTR_NVIC_TPM2 =		35, /* Timer module ch2		*/
  INTR_NVIC_RTC_ALARM =		36, /* Alarm interrupt		*/
  INTR_NVIC_RTC_SECONDS =	37, /* Seconds interrupt	*/
  INTR_NVIC_PIT =		38, /* 				*/
  INTR_NVIC_I2S0 =		49, /* I2S			*/
  INTR_NVIC_USB =		40, /* 				*/
  INTR_NVIC_DAC0 =		41, /* DAC ch0			*/
  INTR_NVIC_42 =		42, /*				*/
  INTR_NVIC_43 =		43, /*				*/
  INTR_NVIC_LPTMR0 =		44, /* LP Timer			*/
  INTR_NVIC_45 =		45, /*				*/
  INTR_NVIC_PORTA =		46, /* Pin detect Port A	*/
  INTR_NVIC_PORTCD =		47, /* Pin detect Port C and D	*/

  INTR_MASKABLE_FACTOR_END = 48,
};

#if 0
int			IntrUnInit(int unit);
void			IntrLoop();

int			IntrCommand(int ac, char *av[]);
#endif
int			IntrGetNvicNumSerial(int unit);
int			IntrGetNvicNumDma(int unit);


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

static void		IntrExceptionInterrupt(struct _stIntrReg *pr);

#endif


#endif	/* __INTR_H__ */
