ENTRY(Reset_Handler)

/* Specify the exact memory areas for the STM32F411CE */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K  /* 512 KB True Flash */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K  /* 128 KB Total SRAM */
}

/* Define the top of the stack (End of RAM) */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Hardware Vector Table (Must be at start of Flash) */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Program Code and Constants */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)   /* ARM to Thumb interworking */
        *(.glue_7t)  /* Thumb to ARM interworking */

        . = ALIGN(4);
    } > FLASH

    /* ARM Magic Tables for Stack Unwinding & Debugging */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armextab.*)
        __exidx_end = .;
    } > FLASH

    /* Pointer used by startup to find initialization values for .data */
    _sidata = LOADADDR(.data);

    /* Initialized Variables Data Section */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* Target RAM start address marker */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;        /* Target RAM end address marker */
    } > RAM AT > FLASH

    /* Uninitialized Variables Data Section */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* Target RAM start address marker */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;         /* Target RAM end address marker */
        __bss_end__ = _ebss;
    } > RAM

    /* Heap & Stack boundaries (Dynamic Memory Allocation) */
    . = ALIGN(8);          /* 8-byte alignment required by ARM ABI */
    _end = .;
    PROVIDE(end = .);

    /* Minimum system stack requirement guard (1KB) */
    _min_stack_size = 0x400;
    .stack_space :
    {
        . = . + _min_stack_size;
        . = ALIGN(8);
    } > RAM
}

/* Safety RAM overflow check*/
ASSERT(_estack >= _end, "Error: Memory region RAM overflowed with code/data symbols!")
