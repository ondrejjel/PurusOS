ENTRY(Reset_Handler)

/*
 * PurusOS linker script for stm32f411xc
 *
 * RAM layout
 *
 * Low address
 *
 *   .data / .bss
 *   BSS guard
 *   Task memory pool
 *   Mempool guard
 *   Kernel runtime region
 *   Kernel guard
 *   General-purpose RAM
 *   Startup guard
 *   Startup stack
 *
 * High address
 *
 * The startup stack begins at _estack and is abandoned once
 * Reset_Handler transfers execution to the kernel stack.
 */


/* -------------------------------------------------------------------------- */
/* Memory regions                                                             */
/* -------------------------------------------------------------------------- */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}


/* -------------------------------------------------------------------------- */
/* RAM layout configuration                                                   */
/* -------------------------------------------------------------------------- */

/* Maximum combined .data + .bss allocation. */
DATA_BSS_SIZE = 64K;

/* Reserved RAM regions. */
BSS_GUARD_SIZE     = 512;
MEMPOOL_SIZE       = 48K;
MEMPOOL_GUARD_SIZE = 512;
KERNEL_SIZE        = 8K;
KERNEL_GUARD_SIZE  = 512;
GENERAL_SIZE       = 4K;
STARTUP_GUARD_SIZE = 512;
STARTUP_SIZE       = 1K;


/* -------------------------------------------------------------------------- */
/* Global memory symbols                                                      */
/* -------------------------------------------------------------------------- */

/*
 * Top of RAM.
 *
 * Reset_Handler initially uses this address as the stack pointer.
 */
_estack = ORIGIN(RAM) + LENGTH(RAM);


/* -------------------------------------------------------------------------- */
/* Flash sections                                                             */
/* -------------------------------------------------------------------------- */

SECTIONS
{
    /*
     * Cortex-M interrupt vector table.
     *
     * This must be the first section in Flash because the processor
     * expects the vector table at the beginning of the image.
     */
    .isr_vector :
    {
        . = ALIGN(4);

        KEEP(*(.isr_vector))

        . = ALIGN(4);
    } > FLASH


    /*
     * Program code and read-only data.
     */
    .text :
    {
        . = ALIGN(4);

        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)

        . = ALIGN(4);
    } > FLASH


    /* ---------------------------------------------------------------------- */
    /* RAM data sections                                                      */
    /* ---------------------------------------------------------------------- */

    /*
     * Flash address containing the initial contents of .data.
     *
     * Reset_Handler copies this data from Flash into SRAM.
     */
    _sidata = LOADADDR(.data);


    /*
     * Initialized global/static variables.
     *
     * VMA: SRAM
     * LMA: Flash
     */
    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data)
        *(.data*)

        . = ALIGN(4);

        _edata = .;
    } > RAM AT > FLASH


    /*
     * Zero-initialized global/static variables.
     *
     * Reset_Handler clears this region before calling main().
     */
    .bss :
    {
        . = ALIGN(4);

        _sbss = .;
        __bss_start__ = _sbss;

        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;
        __bss_end__ = _ebss;
    } > RAM


    /*
     * The combined .data + .bss allocation is deliberately bounded.
     */
    ASSERT(
        (_ebss - _sdata) <= DATA_BSS_SIZE,
           "Error: Combined .data and .bss size exceeds DATA_BSS_SIZE!"
    )


    /* ---------------------------------------------------------------------- */
    /* PurusOS runtime RAM                                                    */
    /* ---------------------------------------------------------------------- */

    /*
     * Guard between .bss and the task memory pool.
     */
    .bss_guard (NOLOAD) :
    {
        . = ALIGN(8);

        _sbss_guard = .;

        . = . + BSS_GUARD_SIZE;

        . = ALIGN(8);

        _ebss_guard = .;
    } > RAM


    /*
     * Task memory pool.
     *
     * Task stacks and other task-owned memory are allocated from here.
     */
    .mempool (NOLOAD) :
    {
        . = ALIGN(8);

        _smempool = .;

        . = . + MEMPOOL_SIZE;

        . = ALIGN(8);

        _emempool = .;
    } > RAM


    /*
     * Guard between the task memory pool and kernel runtime memory.
     */
    .mempool_guard (NOLOAD) :
    {
        . = ALIGN(8);

        _smempool_guard = .;

        . = . + MEMPOOL_GUARD_SIZE;

        . = ALIGN(8);

        _emempool_guard = .;
    } > RAM


    /*
     * Kernel runtime memory.
     *
     * This region contains the permanent kernel stack and may also
     * contain scheduler state, TCB storage, and future kernel workspace.
     *
     * _ekernel is the top of the kernel region and is therefore the
     * address loaded into SP when the kernel takes over from startup.
     */
    .kernel (NOLOAD) :
    {
        . = ALIGN(8);

        _skernel = .;

        . = . + KERNEL_SIZE;

        . = ALIGN(8);

        _ekernel = .;
    } > RAM


    /*
     * Guard between kernel runtime memory and general-purpose RAM.
     */
    .kernel_guard (NOLOAD) :
    {
        . = ALIGN(8);

        _skernel_guard = .;

        . = . + KERNEL_GUARD_SIZE;

        . = ALIGN(8);

        _ekernel_guard = .;
    } > RAM


    /*
     * General-purpose RAM.
     *
     * This region is not assigned to a specific subsystem and may be
     * used by kernel/hardware code where appropriate.
     */
    .general (NOLOAD) :
    {
        . = ALIGN(8);

        _sgeneral = .;

        . = . + GENERAL_SIZE;

        . = ALIGN(8);

        _egeneral = .;
    } > RAM


    /*
     * Guard below the temporary startup stack.
     */
    .startup_guard (NOLOAD) :
    {
        . = ALIGN(8);

        _sstartup_guard = .;

        . = . + STARTUP_GUARD_SIZE;

        . = ALIGN(8);

        _estartup_guard = .;
    } > RAM


    /*
     * Temporary startup stack.
     *
     * Reset_Handler starts with:
     *
     *     SP = _estack
     *
     * and therefore uses this region while performing system
     * initialization.
     *
     * Once initialization is complete, the stack is abandoned and
     * SP is changed to _ekernel.
     */
    .startup (NOLOAD) :
    {
        . = ALIGN(8);

        _sstartup = .;

        . = . + STARTUP_SIZE;

        . = ALIGN(8);

        _estartup = .;
    } > RAM


    /* ---------------------------------------------------------------------- */
    /* Final RAM boundary                                                     */
    /* ---------------------------------------------------------------------- */

    /*
     * End of all statically assigned RAM.
     *
     * This should coincide with _estack for the current fixed layout.
     */
    . = ALIGN(8);

    _end = .;

    PROVIDE(end = .);


    /* ---------------------------------------------------------------------- */
    /* Unwanted sections                                                      */
    /* ---------------------------------------------------------------------- */

    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.ARM.extab*)
        *(.glue_7*)
        *(.glue_7t)
    }
}


/* -------------------------------------------------------------------------- */
/* Link-time safety checks                                                    */
/* -------------------------------------------------------------------------- */

/*
 * Every statically assigned RAM section must fit inside the physical RAM.
 */
ASSERT(
    _end <= _estack,
    "Error: PurusOS RAM layout exceeds available SRAM!"
)
