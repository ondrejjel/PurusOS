.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.global Reset_Handler
.global g_pfnVectors

/* Linker script symbols for memory initialization */
.word _sidata
.word _sdata
.word _edata
.word _sbss
.word _ebss

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
/* Set Stack Pointer */
ldr r0, =_estack
mov sp, r0

/* Copy .data section from Flash to SRAM */
ldr r0, =_sdata
ldr r1, =_edata
ldr r2, =_sidata
movs r3, #0
b LoopCopyDataInit

CopyDataInit:
ldr r4, [r2, r3]
str r4, [r0, r3]
adds r3, r3, #4

LoopCopyDataInit:
adds r4, r0, r3
cmp r4, r1
bcc CopyDataInit

/* Zero fill the .bss section in SRAM */
ldr r2, =_sbss
ldr r4, =_ebss
movs r3, #0
b LoopFillZerobss

FillZerobss:
str r3, [r2]
adds r2, r2, #4

LoopFillZerobss:
cmp r2, r4
bcc FillZerobss

/* Call main function */
bl main

/* Infinite loop if main returns */
Default_Loop:
b Default_Loop

.size Reset_Handler, .-Reset_Handler

/* Vector Table */
.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
.word _estack         /* Top of Stack */
.word Reset_Handler   /* Reset Handler */
