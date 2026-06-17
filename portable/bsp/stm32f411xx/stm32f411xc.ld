ENTRY(Reset_Handler)

/* Specify the exact memory areas for the STM32F411xC */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K  /* 256 KB True Flash */
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
        . = ALIGN(4);
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

    /* Global variable allocation cap assertion (.data + .bss <= 40KB) */
    ASSERT((_ebss - _sdata) <= 40K, "Error: Combined .data and .bss size exceeds the 40KB limit!")

    /* Custom RAM allocations with 32-bit alignment for Cortex-M4 compatibility */

    /* 1KB guard block to separate .bss from .kernel */
    .bss_guard (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss_guard = .;
        . = . + 1K;
        . = ALIGN(4);
        _ebss_guard = .;
    } > RAM

    /* Kernel Stack RAM Allocation (4KB) */
    .kernel (NOLOAD) :
    {
        . = ALIGN(4);
        _skernel = .;
        . = . + 4K;
        . = ALIGN(4);
        _ekernel = .;
    } > RAM

    /* 1KB kernel guard to separate kernel from mempool */
    .kernel_guard (NOLOAD) :
    {
        . = ALIGN(4);
        _skernel_guard = .;
        . = . + 1K;
        . = ALIGN(4);
        _ekernel_guard = .;
    } > RAM

    /* Task Stacks Memory Pool Allocation (64KB) */
    .mempool (NOLOAD) :
    {
        . = ALIGN(4);
        _smempool = .;
        . = . + 64K;
        . = ALIGN(4);
        _emempool = .;
    } > RAM

    /* 1KB mempool guard to catch big mempool/task overflows */
    .mempool_guard (NOLOAD) :
    {
        . = ALIGN(4);
        _smempool_guard = .;
        . = . + 1K;
        . = ALIGN(4);
        _emempool_guard = .;
    } > RAM

    /* End of static allocations marker */
    . = ALIGN(8);          /* 8-byte alignment required by ARM ABI */
    _end = .;
    PROVIDE(end = .);

    /* Discard unwanted compilation sections */
    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.ARM.extab*)
        *(.glue_7*)
        *(.glue_7t)
    }
}

/* * Safety RAM overflow check:
 * Ensures all allocated sections fit within the 128KB total RAM space.
 * The space between _end and _estack is left completely open for general
 * RAM access / system stack operations.
 */
ASSERT(_estack >= _end, "Error: Memory region RAM overflowed with code/data symbols!")
