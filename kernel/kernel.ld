OUTPUT_ARCH("riscv")
ENTRY(_entry)

SECTIONS
{
  /* Start at an appropriate 32-bit address */
  . = 0x80000000;  /* Typical start address for RISC-V 32-bit */

  .text : {
    *(.text .text.*)
    . = ALIGN(4);  /* Align to 4 bytes for 32-bit architecture */
    PROVIDE(etext = .);
  }

  .rodata : {
    . = ALIGN(4);  /* Align to 4 bytes */
    *(.rodata .rodata.*)
  }

  .data : {
    . = ALIGN(4);  /* Align to 4 bytes */
    *(.data .data.*)
  }

  .bss : {
    . = ALIGN(4);  /* Align to 4 bytes */
    *(.bss .bss.*)
  }

  PROVIDE(end = .);
}
