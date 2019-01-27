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


/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = 0x100;
STACK_SIZE = 0x200;
VECTOR_RAM_SIZE = 0x400;

/* Specify the memory areas */
MEMORY
{
  FLASH_INTERRUPT       (rx) : ORIGIN = 0x08004000, LENGTH = 0x0040
  FLASH_INTERRUPT_USR   (rx) : ORIGIN = 0x08004040, LENGTH = 0x03c0
  FLASH_CONFIG          (rx) : ORIGIN = 0x08004400, LENGTH = 0x0020
  FLASH_TEXT            (rx) : ORIGIN = 0x08004420, LENGTH = 0x148f0
  RAM_DATA              (wr) : ORIGIN = 0x20000000, LENGTH = 0x5000
}

/* Define output sections */
SECTIONS
{

  /* pool the interrupt vectors */
  .interrupts : {
    sectVectorStart = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))        /* vectors */
  } > FLASH_INTERRUPT

  .text2 : {
    . = ALIGN(4);
    KEEP(*(.isr_vector_usr))    /* vectors */
    sectVectorEnd = .;
    *(boot1.text)
    *(.text2)
  } > FLASH_INTERRUPT_USR

  .flash_config : {
    . = ALIGN(4);
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
    . = ALIGN(4);
  } > FLASH_CONFIG

  /* the instructions and the const data */
  .text : {
    . = ALIGN(4);

    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > FLASH_TEXT

.ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH_TEXT

  sectDataRom = .; /* Symbol is used by startup for data initialization */

  /* reserve MTB memory at the beginning of RAM_DATA */
  .mtb : /* MTB buffer address as defined by the hardware */ {
    . = ALIGN(8);
    _mtb_start = .;
    KEEP(*(.mtb_buf)) /* need to KEEP Micro Trace Buffer as not referenced by application */
    . = ALIGN(8);
    _mtb_end = .;
  } > RAM_DATA


  .interruptsRam : {
    sectVectorRamStart = .;
    . = ALIGN(256);
    . += VECTOR_RAM_SIZE;
    sectVectorRamEnd = .;
  } > RAM_DATA


  /* uninitialized data */
  .bss : {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    sectBssStart = .;         /* define a global symbol at bss start */
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    sectBssEnd = .;
  } > RAM_DATA


  /* initialized data */
  .data : {
    . = ALIGN(4);
    sectDataStart = .;       /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    sectDataEnd = .;         /* define a global symbol at data end */
  } > RAM_DATA AT> FLASH_TEXT

  sectDataRomEnd = sectDataRom + (sectDataEnd - sectDataStart);
  sectTextDataSize = sectDataRomEnd - sectVectorStart;

  .heap : {
    . = ALIGN(8);
    sectHeapStart = .;
    . += HEAP_SIZE;
    sectHeapEnd = .;
  } > RAM_DATA

  .stack : {
    . = ALIGN(8);
    sectStackStart = .;
    . += STACK_SIZE;
    sectStackEnd = .;
    sectStackTop = .;
  } > RAM_DATA


  ASSERT(sectStackStart >= sectHeapEnd, "overflow (StackStart < HeapEnd")
}
