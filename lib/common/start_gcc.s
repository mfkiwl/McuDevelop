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
/*****************************************************************************/
/* Version: GCC for ARM Embedded Processors                                  */
/*****************************************************************************/
    .syntax unified
    .arch armv6-m


/**********************************************
 * Vector table
 */
    .if (TARGETCODE==0x01 || TARGETCODE==0x02 || TARGETCODE==0x70)  /* define in config file */
    .section .isr_vector, "a"
    .align  4
    .globl  __vector_table
    .globl  __vector_table_end

__vector_table:
    .long   sectStackTop                        /* Top of Stack */
    .long   Reset_Handler                       /* Reset Handler */
    .long   NMI_Handler                         /* NMI Handler*/
    .long   HardFault_Handler                   /* Hard Fault Handler*/
    .long   0                                   /* Reserved*/
    .long   0                                   /* Reserved*/
    .long   0                                   /* Reserved*/
    .long   0                                   /* Reserved*/
    .long   0                                   /* Reserved*/
    .long   0                                   /* Reserved*/
    .long   0                                   /* Reserved*/
    .long   SVC_Handler                         /* SVCall Handler*/
    .long   0                                   /* Reserved*/
    .long   0                                   /* Reserved*/
    .long   PendSV_Handler                      /* PendSV Handler*/
    .long   SystemSysTickIntr                   /* SysTick Handler*/
__vector_table_end:

    .size   __vector_table, . - __vector_table
    .endif



/**********************************************
 * Kinetis special
 * [0x0400 -- 0x040f]: Kinetis CPU
 * [0x0410 -- 0x041f]: Firmware information
 * 0x040d FOPT
 *    [7:6] BOOTSRC_SEL   0:Flash, 1:reserved, 2:ROM 3:ROM
 *    [5]   FAST_INIT     0:Slow, 1:Fast
 *    [3]   RESET_PIN_CFG 0: RESET pin is disabled, 1: enable
 *    [2]   NMI_DIS       0: NMI disable, 1: NMI pin is enabled
 *    [1]   BOOTPIN_OPT   0: BOOTCFG0 enable, 1:disable
 *    {[4],[0]} LPBOOT    0: OUTDIV is set 7,VLPR
 *                        1: OUTDIV is set 3,VLPR
 *                        2: OUTDIV is set 1,RUN
 *                        3: OUTDIV is set 0,RUN
 */
/* Flash Configuration  for Kinetis */
    .section .FlashConfig, "a"
    .long   0xFFFFFFFF
    .long   0xFFFFFFFF
    .long   0xFFFFFFFF
    .long   0xFFFF3DFE
/* binary and version informations */
    .long   0xFFFFFFFF                          /* crc */
    .long   sectTextDataSize                    /* size of .text .data */
    .long   UpdateMode_Handler                  /* update entry */
    .long   strVersionText                      /* pointer of version */


    .text
    .thumb

/**********************************************
 * Reset Handler
 */
    .thumb_func
    .align   2
    .globl   Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    ldr     r4, =0              /* normal mode */
Reset_Init:
    cpsid   i                   /* disable interrupts */
    ldr     r0,  =0
    ldr     r1,  =0
    ldr     r2,  =0
    ldr     r3,  =0
    ldr     r5,  =0
    ldr     r6,  =0
    ldr     r7,  =0
    mov     r8,  r7
    mov     r9,  r7
    mov     r10, r7
    mov     r11, r7
    mov     r12, r7

    // Initialize the stack pointer
    ldr     r7, =sectStackTop
    mov     sp, r7              /* r13 */

    // bss, data copy
    ldr     r7, =BssDataInit
    blx     r7

    // clock init
//    ldr     r7, =SystemInit
//    blx     r7

    mov     r0, r4
    ldr     r7, =MainEntry
    blx     r7

    b       .


    .pool
    .size Reset_Handler, . - Reset_Handler


/**********************************************
 * default interrupt service routine
 * use the following entry, if the function is not existed
 */
    .align  2
    .thumb_func
    .weak   IntrException
    .type   IntrException, %function
IntrException:
    ldr     r0, =IntrException
    bx      r0
    .size   IntrException, . - IntrException

/* macro */
    .macro  def_irq_handler     handler_name
    .weak   \handler_name
    .set    \handler_name, IntrException
    .endm

/* Exception Handlers */
    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler


/**********************************************
 * update mode entry
 */
    .thumb_func
    .align   2
    .globl   Update_Handler
    .weak    Update_Handler
    .type    Update_Handler, %function
UpdateMode_Handler:
    ldr     r4, =1              /* update mode */
    b       Reset_Init


/**********************************************
 * clear BSS and copy DATA from rom
 */
    .align  2
    .thumb
    .thumb_func
BssDataInit:

DataCopyInit:
    ldr     r3, =sectDataRom
    ldr     r1, =sectDataEnd
    ldr     r2, =sectDataStart
    subs    r1, r1, r2
    b       DataCopyLoop1
DataCopyLoop:
    ldr     r0, [r3]
    str     r0, [r2]
    adds    r3, r3, #4
    adds    r2, r2, #4
    subs    r1, r1, #4
DataCopyLoop1:
    cmp     r1, #0
    bne     DataCopyLoop

BssZeroInit:
    ldr     r1, =sectBssEnd
    ldr     r2, =sectBssStart
    subs    r1, r1, r2
    movs    r0, #0
    b       BssZeroLoop1
BssZeroLoop:
    str     r0, [r2]
    adds    r2, r2, #4
    subs    r1, r1, #4
BssZeroLoop1:
    cmp     r1, #0
    bne     BssZeroLoop
    mov     pc, lr

.end
