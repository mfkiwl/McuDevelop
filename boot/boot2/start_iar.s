;
; Copyright (c) 2018 zhtlab
;
; Permission is hereby granted, free of charge, to any person obtaining
; a copy of this software and associated documentation files
; (the "Software"), to deal in the Software without restriction, including
; without limitation the rights to use, copy, modify, merge, publish,
; distribute, sublicense, and/or sell copies of the Software, and to permit
; persons to whom the Software is furnished to do so, subject to
; the following conditions:
;
; The above copyright notice and this permission notice shall be included
; in all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
; FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
; DEALINGS IN THE SOFTWARE.
;

;/*****************************************************************************
; * @file:    startup_MKL25Z4.s
; * @purpose: CMSIS Cortex-M0plus Core Device Startup File
; *           MKL25Z4
; * @version: 1.3
; * @date:    2012-10-4
; *----------------------------------------------------------------------------
; *
; * Copyright: 1997 - 2012 Freescale Semiconductor, Inc. All Rights Reserved.
; *
; ******************************************************************************/


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

	include	../obj/STM32L4_iar/config.inc

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        import CSTACK$$Limit
	import Boot1CopyAndJump
	import SystemJump

        EXTERN  __iar_program_start
        EXTERN  SystemInit
	EXTERN	systemVersion
	//        EXTERN  g_bootloaderTree
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

        SECTION .intvec:CODE:ROOT(2)
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     0
        DCD     0
        DCD     0
__vector_table_0x1c
	DCD	UpdateMode_Handler
__vector_table_0x20
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     0
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler
__Vectors_End

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Flash configuration field.
;;
	SECTION .boot0cfg:CODE:ROOT(2)
#ifdef BL_TARGET_FLASH
__flash_config
#if (CONFIG_HWRESET_DISABLE == 1)
__FTFA_FOPT_RESET_PIN_CFG_BIT	equ	0x0000
#else
__FTFA_FOPT_RESET_PIN_CFG_BIT	equ	0x0800
#endif

#ifdef CONFIG_SECURITY_BACKDOOR_KEY
__FTFA_FOPT             equ     CONFIG_SECURITY_FSEC | (__FTFA_FOPT_RESET_PIN_CFG_BIT)
	DC8	CONFIG_SECURITY_BACKDOOR_KEY	; 0x400
	DCD	CONFIG_SECURITY_RPROT		; 0x408
        DCB     CONFIG_SECURITY_FSEC
        DCB     CONFIG_SECURITY_FOPT | ((__FTFA_FOPT_RESET_PIN_CFG_BIT) >> 8)
	DC8	0xff
	DC8	0xff
#else
__FTFA_FOPT             equ     0xffff35fe | (__FTFA_FOPT_RESET_PIN_CFG_BIT)
	DCD     0xFFFFFFFF  ; 0x400
	DCD     0xFFFFFFFF  ; 0x404
        DCD     0xFFFFFFFF  ; 0x408
        DCD     (__FTFA_FOPT)  ; 0x40c, FOPT, FSEC=0xFE
#endif
#endif


__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

#define SCS_BASE            (0xE000E000)                            /*!< System Control Space Base Address */
#define SCB_BASE            (SCS_BASE +  0x0D00)                    /*!< System Control Block Base Address */
#define SCB_VTOR_OFFSET     (0x00000008)

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler

	// Mask interrupts
        cpsid   i

        // Set VTOR register in SCB first thing we do.
//        ldr     r0,=__vector_table
//        ldr     r1,=SCB_BASE
//        str     r0,[r1, #SCB_VTOR_OFFSET]

        // Init the rest of the registers
        ldr     r2,=0
        ldr     r3,=0
        ldr     r4,=0
        ldr     r5,=0
        ldr     r6,=0
        ldr     r7,=0
        mov     r8,r7
        mov     r9,r7
        mov     r10,r7
        mov     r11,r7
        mov     r12,r7

        // Initialize the stack pointer
        ldr     r0,=CSTACK$$Limit
        mov     r13, r0

        // Call the CMSIS system init routine
	ldr     r0,=SystemInit
        blx     r0

	ldr	r0, =0		/* normal mode */
        ldr     r7,=__iar_program_start
        blx     r7

	b	.


        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
SysTick_Handler
        B       .
	

        PUBWEAK UpdateMode_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
UpdateMode_Handler

	// Mask interrupts
        cpsid   i

        // Set VTOR register in SCB first thing we do.
//        ldr     r0,=__vector_table
//        ldr     r1,=SCB_BASE
//        str     r0,[r1, #SCB_VTOR_OFFSET]

        // Init the rest of the registers
        ldr     r2,=0
        ldr     r3,=0
        ldr     r4,=0
        ldr     r5,=0
        ldr     r6,=0
        ldr     r7,=0
        mov     r8,r7
        mov     r9,r7
        mov     r10,r7
        mov     r11,r7
        mov     r12,r7

        // Initialize the stack pointer
        ldr     r0,=CSTACK$$Limit
        mov     r13, r0

        // Call the CMSIS system init routine
        ldr     r0,=SystemInit
        blx     r0

	ldr	r0, =1		/* force update mode */
        ldr     r7,=__iar_program_start
        blx     r7

	b	.

        END
