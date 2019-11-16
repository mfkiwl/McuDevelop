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
 * ld file for STM32H7XX
 */

/*********************************************************
 * definitons
 */
szHeap        = 0x100;
szStack       = 0x1000;
VECTOR_RAM_SIZE = 0x400;


/*********************************************************
 * memory maps
 */
MEMORY {
  /* flash area */
  FLASH1                (rx) : ORIGIN = 0x08000000, LENGTH =   64K
  FLASH_INTERRUPTS      (rx) : ORIGIN = 0x08000000, LENGTH = 0x00040
  FLASH_INTERRUPTS_USR  (rx) : ORIGIN = 0x08000040, LENGTH = 0x003c0
  FLASH_CONFIG          (rx) : ORIGIN = 0x08000400, LENGTH = 0x00020
  FLASH_TEXT            (rx) : ORIGIN = 0x08000420, LENGTH = 0xbfbe0

  /* ram area */
  RAM_DTCM             (xrw) : ORIGIN = 0x20000000, LENGTH =   64K
  RAM_D1               (xrw) : ORIGIN = 0x20010000, LENGTH =  176K
  RAM_D2               (xrw) : ORIGIN = 0x2003c000, LENGTH =   16K
  RAM_ITCM             (xrw) : ORIGIN = 0x00000000, LENGTH =   16K
}

/*********************************************************
 * section definitions
 */
SECTIONS {
  /***
   * interrupt vector
   */
  .intr_vector : {
    __VECTOR_TABLE = .;
    sectVectorStart = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))    /* vectors system */
    . = ALIGN(4);
  } > FLASH_INTERRUPTS

  .text2 : {
    . = ALIGN(4);
    KEEP(*(.isr_vector_usr))    /* vectors user */
    sectVectorEnd = .;
    *(boot1.text)
    *(.text2)
  } > FLASH_INTERRUPTS_USR

  /***
   * Flash Configuration Field (FCF) for Kinetis
   */
  .flash_config : {
    . = ALIGN(4);
    KEEP(*(.FlashConfig))
    . = ALIGN(4);
  } > FLASH_CONFIG

  /***
   * TEXT, main codes
   */
  .text : {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
  } >FLASH_TEXT

  /* Constant data goes into FLASH */
  .rodata : {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH_TEXT

  .ARM.extab : {
      *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH_TEXT

  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH_TEXT

  .preinit_array : {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH_TEXT

  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  /* used by the startup to initialize data */
  sectDataRom = LOADADDR(.data);

  /* interrupt in ram area */
  .interruptsRam : {
    sectVectorRamStart = .;
    . = ALIGN(256);
    . += VECTOR_RAM_SIZE;
    sectVectorRamEnd = .;
  } > RAM_DTCM


  /***
   * DATA, Initialized data section
   *       data is copied from FLASH
   */
  .data : {
    . = ALIGN(4);
    sectDataStart = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    sectDataEnd = .;        /* define a global symbol at data end */
  } >RAM_D1 AT> FLASH_TEXT

  __DATA_END = __DATA_ROM + (sectDataEnd - sectDataStart);
  __TEXTDATA_SIZE = __DATA_END - __VECTOR_TABLE;

  sectDataRomEnd = sectDataRom + (sectDataEnd - sectDataStart);
  sectTextDataSize = sectDataRomEnd - sectVectorStart;


  /***
   * BSS, Uninitialized data section
   */
  .bss : {
    . = ALIGN(8);
    /* This is used by the startup in order to initialize the .bss secion */
    sectBssStart = .;         /* define a global symbol at bss start */
    __bss_start__ = sectBssStart;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(8);
    sectBssEnd = .;
    __bss_end__ = sectBssEnd;
  } >RAM_D1

  /***
   * memory reserved for heap and stack
   */
  .heap : {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    sectHeapStart = .;
    . = . + szHeap;
    sectHeapEnd = .;
  } >RAM_D1

  .stack : {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );

    . = . + szStack;

    . = ALIGN(8);

    sectStackTop = .;
    sectStackEnd = .;
  } >RAM_DTCM


  /***
   * Remove the information of the folloings files
   */
}
