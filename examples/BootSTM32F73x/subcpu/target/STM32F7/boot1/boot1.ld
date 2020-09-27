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

/*********************************************************
 * ld file for STM32L0xx
 */

/*********************************************************
 * definitons
 */
szHeap        = 0x0010;
szStack       = 0x0800;

/*********************************************************
 * memory maps
 */
MEMORY
{
/* flash area */
  FLASH1                (rx) : ORIGIN = 0x08000000, LENGTH =  256K

/* ram area */
  RAM_ITCM             (xrw) : ORIGIN = 0x00000000, LENGTH =   16K

  RAM_DTCM             (xrw) : ORIGIN = 0x20000000, LENGTH =   64K
  RAM_D1               (xrw) : ORIGIN = 0x20010000, LENGTH =  172K
  RAM_D2               (xrw) : ORIGIN = 0x2003c000, LENGTH =   16K


/* firm flash area */
  FLASH_INTERRUPTS      (rx) : ORIGIN = 0x08000000, LENGTH = 0x0040
  FLASH_TEXT2           (rx) : ORIGIN = 0x08000040, LENGTH = 0x03c0
  FLASH_CONFIG          (rx) : ORIGIN = 0x08000400, LENGTH = 0x0020
  FLASH_TEXT            (rx) : ORIGIN = 0x08000420, LENGTH = 0x03e0

/* firm ram area   */
  RAM                   (rw) : ORIGIN = 0x20007000, LENGTH = 0x1000

}

/* Define output sections */
SECTIONS
{

/* The startup code goes first into internal flash */
  .interrupts : {
    __VECTOR_TABLE = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > FLASH_INTERRUPTS

  .text2 : {
    *(boot1.text)
    *(.text.Boot1Exception)
    *(.text2)
  } > FLASH_TEXT2

  .flash_config : {
    . = ALIGN(4);
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) for KINETIS */
    . = ALIGN(4);
  } > FLASH_CONFIG

  /* The program code and other data goes into internal flash */
  .text : {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > FLASH_TEXT

  .ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH_TEXT

  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > FLASH_TEXT

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */
  sectDataRom = .;


  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(FLASH_INTERRUPTS);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data : AT(__DATA_ROM) {
    . = ALIGN(4);
    __DATA_RAM = .;
    sectDataStart = .;       /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    sectDataEnd = .;        /* define a global symbol at data end */
  } > RAM

  __DATA_END = __DATA_ROM + (sectDataEnd - sectDataStart);
  sectTextDataSize = __DATA_END - __VECTOR_TABLE;

  /* Uninitialized data section */
  .bss : {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    sectBssStart = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    sectBssEnd = .;
  } > RAM

  .heap : {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += szHeap;
    __HeapLimit = .;
  } > RAM

  .stack : {
    . = ALIGN(8);
    . += szStack;
  } > RAM


/* Initializes stack on the end of block */
  sectStackTop = ORIGIN(RAM) + LENGTH(RAM);
  __StackLimit = sectStackTop - szStack;
  PROVIDE(__stack = sectStackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

/*
  ASSERT(__StackLimit >= __HeapLimit, "RAM_D1 overflow with stack and heap")
*/
}
