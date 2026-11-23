/*
 * Linker script for STM32F407VET6 (512K FLASH, 128K RAM)
 * It tells the linker WHERE each part of the program goes.
 * "." = the current address.
 */

ENTRY(Reset_Handler)                 /* first function (used by the debugger) */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Stack starts at the END of RAM and grows down */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  /* Vector table MUST be first in FLASH (CPU reads SP and PC from here on reset) */
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  /* Code + constant data */
  .text :
  {
    *(.text*)
    *(.rodata*)
  } > FLASH

  /* Initialized variables (int x = 5;)
     - live in RAM when running        -> "> RAM"
     - initial values stored in FLASH  -> "AT > FLASH"
     startup.c copies them FLASH -> RAM */
  .data : ALIGN(4)
  {
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH
  _sidata = LOADADDR(.data);         /* address of the initial values in FLASH */

  /* Uninitialized variables (int y;) -> startup.c fills them with 0 */
  .bss : ALIGN(4)
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM
}
