# PurusOS

PurusOS is a personal bare-metal RTOS project built around one main idea:

> **If I want to understand how something works, I should try building it myself.**

It targets the **STM32F411CEU6 Black Pill**, using an **ARM Cortex-M4**, and is built from the hardware level upward without relying on an existing RTOS or large hardware abstraction framework.

PurusOS is not intended to compete with established RTOS projects or become a production-ready operating system. Its purpose is to explore systems programming by building the pieces myself and understanding what is happening underneath the abstractions that normally hide the hardware.

## Why I Created PurusOS

I started PurusOS because I wanted to learn systems programming by actually building a system rather than only using existing ones.

It is possible to use an RTOS, scheduler, memory manager, hardware abstraction layer, or operating-system component without ever needing to understand what is happening underneath. PurusOS is my way of approaching the problem from the opposite direction.

Rather than simply reading about tasks, memory, context switching, scheduling, startup code, and hardware interfaces, I want to encounter the problems involved in building them myself.

The project has become a way for me to explore how software interacts with a microcontroller at a much lower level than I had previously worked with.

More importantly, I want to be able to look at a feature and understand what it needs from the hardware, how the different layers communicate, and why the system behaves the way it does.

## Learning Philosophy

PurusOS is intentionally a learning project.

The goal is not to produce the most efficient scheduler, the most sophisticated allocator, or the most portable RTOS. The goal is to understand the concepts behind them well enough to make informed design decisions.

My general principle is:

> **Build it yourself first, so you can understand what the abstraction is actually doing.**

If something normally comes from a library, framework, HAL, or existing operating-system component, I want to understand what would actually be required to build that layer myself.

This does not mean avoiding existing software or documentation. Datasheets, architecture manuals, debugging tools, and existing implementations are valuable learning resources. The important distinction is that I do not want to treat an existing implementation as a black box simply because it already works.

PurusOS is also deliberately incremental. Some parts are simpler than they would be in a mature RTOS, and that is intentional. I would rather understand a smaller system completely before introducing additional complexity.

## Platform and Technologies

PurusOS currently targets the **STM32F411CEU6 Black Pill**, based on an **ARM Cortex-M4** processor.

The project uses:

* C
* ARM assembly
* `arm-none-eabi-gcc`
* GDB
* OpenOCD
* Custom linker scripts
* A custom build system
* Bare-metal hardware access

The project does not use STM32CubeIDE, CubeMX, or the STM32 HAL, and the kernel is not built on top of an existing RTOS.

The hardware is an important part of the project because it gives the software somewhere real to run. Concepts such as memory layout, stacks, processor state, startup, exceptions, and context switching become much less abstract when they have to work on an actual microcontroller.

## What I Am Exploring

PurusOS brings together many areas of systems and embedded programming, including:

* Bare-metal embedded development
* ARM Cortex-M architecture
* C programming at a low level
* ARM assembly and C/assembly interaction
* Startup and low-level system initialization
* Linker scripts and memory layout
* Memory management
* Task management
* Scheduling
* Context management and context switching
* Fault handling
* Hardware/software interfaces
* Embedded debugging
* Binary and disassembly analysis
* Designing boundaries between hardware, architecture, kernel, and application code

The project is intentionally broad. One of its main purposes is to understand how these areas connect rather than studying each one as an isolated topic.

## Building From the Hardware Up

One of the defining aspects of PurusOS is the direction in which I approach the system.

Instead of starting with an existing RTOS and learning its API, I start with the processor and build the software layers above it.

The general idea is:

```text
Hardware
↓
Architecture
↓
Kernel
↓
Application
```

This makes the boundaries between the different parts of the system something I have to understand rather than something that is simply given to me.

It also means that abstractions have to be justified by the problems they solve. PurusOS deliberately separates hardware-specific and architecture-specific behavior from the kernel where doing so makes the system easier to understand and reason about.

At the same time, I am not treating abstraction as an end in itself. Part of the learning process is discovering where an abstraction is useful, where it becomes unnecessary complexity, and what trade-offs it introduces.

## Why Not Just Use an Existing RTOS?

Existing RTOS projects are much more mature and capable than PurusOS, and using one would make sense for an actual application.

That is not the purpose here.

Using an existing RTOS would teach me how to **use** an RTOS. Building PurusOS teaches me more about **why an RTOS needs the things it has**.

The development process itself becomes part of the learning material. I get to encounter the problems that an RTOS actually has to solve, make design decisions, discover where those decisions fail, and redesign parts of the system when my understanding changes.

That makes PurusOS considerably slower and less practical than using an established RTOS, but that is intentional.

## Current Status

The **core PurusOS kernel is functionally complete**.

The project currently has foundations for task management, scheduling, memory management, kernel/architecture interfaces, fault handling, context management, and kernel orchestration.

The project is now moving deeper into the low-level hardware and startup layers. The immediate focus is on replacing the temporary startup implementation with a properly understood ARM Cortex-M startup sequence and validating the complete system through compilation, disassembly, debugging, and hardware testing.

Some optional hardening and more advanced features are intentionally being left for later rather than being added simply for completeness.

PurusOS is still an active learning project, so its architecture is expected to continue evolving as my understanding improves.

## What Comes Next

PurusOS is intended to remain a long-term learning project while also providing a foundation for a future, more ambitious project called **VuxionOS**.

VuxionOS will allow me to explore ideas that go beyond the deliberately simple design of PurusOS, potentially including a more ARM-focused architecture, preemptive scheduling, hardware-assisted protection, more advanced context management, and more sophisticated memory and scheduling systems.

The important part is that VuxionOS will be approached with the knowledge gained from building PurusOS.

PurusOS is where I am learning what the fundamental pieces of an RTOS actually are and what problems they have to solve. VuxionOS will be an opportunity to take that understanding further and make more informed architectural choices.

## A Learning Project, Not a Product

PurusOS is not intended to compete with established operating systems or RTOS projects.

It is a personal experiment in understanding how computer systems work at a level where abstractions eventually disappear.

The project will contain mistakes, redesigns, experiments, and decisions that would probably be made differently in a mature production system. Those are not failures of the project; they are part of the reason I am building it.

The most important result of PurusOS is not the final RTOS.

It is understanding **why it works**.
