ENTRY(Reset_Handler)

/*
 * PurusOS linker script for STM32F411xE
 *
 * Physical RAM layout
 *
 * LOW ADDRESS
 *
 *   .data / .bss
 *   BSS guard
 *   Task memory pool
 *   Mempool guard
 *   Kernel runtime region
 *   Kernel guard
 *   General-purpose RAM
 *   General guard
 *   Startup stack
 *   Startup guard
 *   Unallocated RAM
 *
 * HIGH ADDRESS
 *
 *
 * The startup stack grows downward from _estartup.
 * The kernel stack grows downward from _ekernel.
 *
 * The task memory pool is managed by the kernel allocator.
 *
 * Guard regions are reserved RAM placed between independently
 * managed regions. They provide a physical separation that can
 * later be inspected for overflow/corruption.
 */


/* -------------------------------------------------------------------------- */
/* Memory regions                                                             */
/* -------------------------------------------------------------------------- */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}


/* -------------------------------------------------------------------------- */
/* RAM layout configuration                                                   */
/* -------------------------------------------------------------------------- */

/* Maximum combined .data + .bss allocation. */
DATA_BSS_SIZE = 64K;

/* Task memory pool. */
MEMPOOL_SIZE = 48K;

/* Kernel runtime region. */
KERNEL_SIZE = 8K;

/* General-purpose scratch/workspace region. */
GENERAL_SIZE = 4K;

/* Temporary startup stack. */
STARTUP_SIZE = 1K;

/* Minimum amount of intentionally unallocated RAM. */
MIN_FREE_RAM = 1K;

/* Guard regions. */
BSS_GUARD_SIZE     = 256;
MEMPOOL_GUARD_SIZE = 256;
KERNEL_GUARD_SIZE  = 256;
GENERAL_GUARD_SIZE = 256;
STARTUP_GUARD_SIZE = 256;


/* -------------------------------------------------------------------------- */
/* Global memory symbols                                                      */
/* -------------------------------------------------------------------------- */

/*
 * Physical end of SRAM.
 *
 * This is the highest valid RAM address + 1.
 *
 * _estack is NOT the startup stack pointer.
 * The startup code uses _estartup instead.
 */
_estack = ORIGIN(RAM) + LENGTH(RAM);


/* -------------------------------------------------------------------------- */
/* Sections                                                                   */
/* -------------------------------------------------------------------------- */

SECTIONS
{
    /* ====================================================================== */
    /* FLASH                                                                   */
    /* ====================================================================== */

    /*
     * Interrupt vector table.
     *
     * Cortex-M expects the vector table at the beginning of the image.
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


    /* ====================================================================== */
    /* INITIALIZED / ZERO-INITIALIZED RAM                                    */
    /* ====================================================================== */

    /*
     * Flash address containing the initial contents of .data.
     *
     * Reset_Handler copies:
     *
     *     _sidata -> _sdata ... _edata
     */
    _sidata = LOADADDR(.data);


    /*
     * Initialized global and static variables.
     *
     * VMA: RAM
     * LMA: FLASH
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
     * Zero-initialized global and static variables.
     *
     * Reset_Handler clears:
     *
     *     _sbss -> _ebss
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
     * Prevent .data + .bss from consuming the RAM reserved for
     * the rest of the PurusOS runtime.
     */
    ASSERT(
        (_ebss - _sdata) <= DATA_BSS_SIZE,
           "Error: .data + .bss exceeds DATA_BSS_SIZE!"
    )


    /* ====================================================================== */
    /* PURUSOS RUNTIME RAM                                                    */
    /* ====================================================================== */

    /*
     * ------------------------------------------------------------------------
     * BSS GUARD
     * ------------------------------------------------------------------------
     *
     * Separates global/static RAM from the task memory pool.
     *
     * The task memory pool grows upward from _smempool.
     * Its lower boundary is therefore protected by this guard.
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
     * ------------------------------------------------------------------------
     * TASK MEMORY POOL
     * ------------------------------------------------------------------------
     *
     * Memory allocated to tasks, primarily for their stacks.
     *
     * The allocator operates only inside:
     *
     *     [_smempool, _emempool)
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
     * ------------------------------------------------------------------------
     * MEMPOOL GUARD
     * ------------------------------------------------------------------------
     *
     * Separates task-owned memory from kernel runtime memory.
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
     * ------------------------------------------------------------------------
     * KERNEL RUNTIME REGION
     * ------------------------------------------------------------------------
     *
     * Permanent kernel stack and kernel runtime storage.
     *
     * The kernel stack grows downward from _ekernel.
     *
     * Kernel code therefore initializes:
     *
     *     SP = _ekernel
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
     * ------------------------------------------------------------------------
     * KERNEL GUARD
     * ------------------------------------------------------------------------
     *
     * Separates the kernel runtime region from general-purpose RAM.
     *
     * Separates the kernel runtime region from general-purpose RAM
     * and provides a boundary for detecting accidental corruption.
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
     * ------------------------------------------------------------------------
     * GENERAL-PURPOSE RAM
     * ------------------------------------------------------------------------
     *
     * Temporary scratch/workspace for low-level kernel and hardware code.
     *
     * This is not a permanent execution stack.
     *
     * The region is bounded by:
     *
     *     [_sgeneral, _egeneral)
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
     * ------------------------------------------------------------------------
     * GENERAL RAM GUARD
     * ------------------------------------------------------------------------
     *
     * Separates scratch/workspace RAM from the startup stack.
     */
    .general_guard (NOLOAD) :
    {
        . = ALIGN(8);

        _sgeneral_guard = .;

        . = . + GENERAL_GUARD_SIZE;

        . = ALIGN(8);

        _egeneral_guard = .;
    } > RAM


    /*
     * ------------------------------------------------------------------------
     * STARTUP STACK
     * ------------------------------------------------------------------------
     *
     * Temporary stack used by Reset_Handler and early initialization.
     *
     * The stack grows downward.
     *
     * Initial stack pointer:
     *
     *     SP = _estartup
     *
     * The startup stack must remain inside:
     *
     *     [_sstartup, _estartup)
     *
     * Once startup has finished, SP is moved to _ekernel and this
     * stack is abandoned.
     */
    .startup (NOLOAD) :
    {
        . = ALIGN(8);

        _sstartup = .;

        . = . + STARTUP_SIZE;

        . = ALIGN(8);

        _estartup = .;
    } > RAM


    /*
     * ------------------------------------------------------------------------
     * STARTUP GUARD
     * ------------------------------------------------------------------------
     *
     * Separates the temporary startup stack from the unallocated
     * RAM at the physical top of SRAM.
     *
     * A downward-growing startup stack must never cross this boundary.
     */
    .startup_guard (NOLOAD) :
    {
        . = ALIGN(8);

        _sstartup_guard = .;

        . = . + STARTUP_GUARD_SIZE;

        . = ALIGN(8);

        _estartup_guard = .;
    } > RAM


    /* ====================================================================== */
    /* UNALLOCATED RAM                                                        */
    /* ====================================================================== */

    /*
     * Leave a small amount of SRAM completely unassigned.
     *
     * This space is intentionally not represented by a section.
     *
     * It exists as a safety margin between the fixed PurusOS layout
     * and the physical end of SRAM.
     *
     * The actual size is checked below.
     */
    _end = ALIGN(8);

    PROVIDE(end = _end);


    /* ====================================================================== */
    /* UNWANTED SECTIONS                                                      */
    /* ====================================================================== */

    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.ARM.extab*)
        *(.glue_7*)
        *(.glue_7t)
    }
}


/* -------------------------------------------------------------------------- */
/* RAM layout checks                                                          */
/* -------------------------------------------------------------------------- */

/*
 * The complete fixed layout must fit inside physical SRAM.
 */
ASSERT(
    _end <= _estack,
    "Error: PurusOS RAM layout exceeds available SRAM!"
)

/*
 * There must be at least minimum of undedicated SRAM left.
 */
ASSERT(
    _estack - _end >= MIN_FREE_RAM,
    "Error: not enough free SRAM!"
)
