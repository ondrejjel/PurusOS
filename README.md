# PurusOS

PurusOS is a personal bare-metal kernel development project built around one central objective:

> **Understand how a computer system works by building its fundamental software components from the ground up.**

The project targets the **STM32F411CEU6 Black Pill**, based on the **ARM Cortex-M4** architecture. It aims to develop a kernel from scratch, starting with the processor's capabilities and gradually building the software infrastructure required to support higher-level functionality.

The hardware project is generated using **STM32CubeMX** and developed within **STM32CubeIDE**, providing an established development environment, toolchain integration, hardware configuration, and debugging infrastructure.

PurusOS does not aim to compete with established operating systems or become a production-ready kernel. Its primary purpose is educational: to explore kernel development, computer architecture, low-level programming, and the relationship between hardware and software through practical implementation.

## Why I Created PurusOS

I started PurusOS because I wanted to understand systems programming beyond the level of using existing libraries, frameworks, and operating-system interfaces.

Modern software relies on numerous abstractions that make development easier but often hide the mechanisms responsible for making the system work. Memory management, processor state, exception handling, scheduling, startup procedures, and hardware interaction are examples of functionality that can be used without necessarily understanding their underlying implementation.

PurusOS approaches these concepts from the opposite direction.

Rather than building applications on top of an existing kernel, I want to understand what is required to create the kernel itself. This involves exploring the hardware, designing the necessary software interfaces, implementing fundamental kernel functionality, and understanding how individual components interact.

The project provides an opportunity to encounter these problems directly, make architectural decisions, evaluate their consequences, and improve the implementation as my understanding develops.

## Development Philosophy

PurusOS is intentionally an educational project.

The primary objective is not to achieve maximum performance, portability, or feature completeness. Instead, the focus is on understanding the fundamental concepts behind operating-system and kernel development.

My general principle is:

> **Understand the abstraction before relying on it.**

This does not mean avoiding existing documentation, tools, or standardized definitions. Architecture manuals, datasheets, debugging utilities, and established specifications are essential resources.

The project uses established development tools where they provide infrastructure rather than replace the functionality being developed.

For example, **STM32CubeMX** is used to configure and generate the hardware project, while **STM32CubeIDE** provides the integrated development environment, ARM GNU toolchain, build integration, and debugging capabilities.

Similarly, **CMSIS headers** provide standardized processor and peripheral definitions. These headers describe the hardware interfaces; they do not replace the kernel implementation or introduce an existing RTOS.

The distinction is important: the development environment and hardware infrastructure are established components, while the kernel itself remains an independently developed implementation.

Development is deliberately incremental. I prefer implementing a smaller, understandable system before introducing additional complexity. Architectural decisions should follow actual requirements rather than attempting to reproduce the feature set of a mature operating system.

## Platform and Technologies

PurusOS currently targets the **STM32F411CEU6 Black Pill**, featuring an **ARM Cortex-M4** processor.

The project uses:

- C
- ARM assembly
- CMSIS
- STM32CubeMX
- STM32CubeIDE
- ARM GNU Toolchain (`arm-none-eabi-gcc`)
- GDB
- STM32CubeIDE integrated debugging and flashing tools
- STM32 device definitions and startup infrastructure
- Custom kernel and architecture-specific implementations

STM32CubeMX is responsible for hardware project configuration and code generation, while STM32CubeIDE serves as the primary development environment.

CMSIS provides standardized interfaces for processor and peripheral definitions, while the kernel's functionality and architecture-specific implementations are developed independently.

The hardware platform is an essential part of the project. It provides a real environment in which concepts such as memory layout, processor state, exception handling, startup, and context switching must work correctly.

## Areas of Exploration

PurusOS explores several interconnected areas of systems and embedded programming:

- Bare-metal development
- ARM Cortex-M architecture
- Kernel architecture and design
- Low-level C programming
- ARM assembly and C/assembly interaction
- Processor initialization and startup
- Linker scripts and memory layout
- Memory management
- Task management
- Scheduling
- Context management and context switching
- Exception and fault handling
- Hardware/software interfaces
- Kernel and architecture boundaries
- Embedded debugging
- Binary analysis and disassembly
- Designing interfaces between hardware, architecture, kernel, and application layers

The objective is not simply to implement these components independently, but to understand how they interact and contribute to the operation of a complete system.

## Building a Kernel From Scratch

A defining characteristic of PurusOS is its bottom-up kernel development approach.

The project uses established hardware development infrastructure rather than attempting to recreate every aspect of the embedded development environment. However, the kernel itself is developed independently, without relying on an existing RTOS implementation.

The general architectural direction is:

```text
Hardware
    ↓
STM32CubeMX / CMSIS / Hardware Definitions
    ↓
Architecture Layer
    ↓
Kernel
    ↓
Applications
```

Each layer has its own responsibilities and interfaces.

The hardware provides the underlying capabilities. STM32CubeMX establishes the hardware configuration, while CMSIS supplies standardized definitions for accessing processor and peripheral functionality. The architecture layer handles processor-specific mechanisms, while the kernel implements the general functionality required to manage the system.

This separation is important because it distinguishes hardware-specific implementation details from kernel-level functionality.

The intention is to develop a system in which the individual layers are understandable, their responsibilities are clearly defined, and their interactions are deliberate.

Abstraction is not an objective by itself. Its usefulness should be determined by the problems it solves and the complexity it introduces.

## Why Not Use an Existing Kernel?

Established kernels and RTOS implementations are considerably more mature, reliable, and feature-complete than PurusOS.

Using one would be the appropriate choice for most practical applications.

However, PurusOS has a different objective.

Using an existing kernel primarily teaches how to interact with its interfaces and functionality. Developing one from scratch provides an opportunity to understand why those interfaces exist, what problems the underlying components solve, and how the system operates internally.

The development process itself is an important part of the project.

Implementing functionality, encountering architectural limitations, investigating unexpected behavior, and redesigning components are all valuable parts of the learning experience.

This approach is considerably slower than building on existing kernel infrastructure, but that trade-off is intentional.

## Current Status

The **initial kernel foundation is functionally complete**.

The project has established foundations for task management, scheduling, memory management, kernel/architecture interfaces, fault handling, context management, and kernel orchestration.

The current development direction focuses on strengthening the low-level hardware and startup infrastructure while transitioning the project into the STM32CubeMX and STM32CubeIDE development ecosystem.

Particular attention is being given to integrating the existing kernel with the CubeMX-generated hardware project, establishing clean boundaries between generated platform infrastructure and independently developed kernel components, and validating the system through compilation, disassembly, debugging, and hardware testing.

Additional functionality and optional hardening will be introduced incrementally as the underlying architecture develops.

PurusOS remains an active project, and its architecture is expected to evolve as new requirements and a deeper understanding emerge.

## Future Direction: VuxionOS

PurusOS is also intended to serve as a foundation for a future, more ambitious project called **VuxionOS**.

While PurusOS focuses on understanding and implementing fundamental kernel components, VuxionOS will provide an opportunity to explore more advanced operating-system concepts and architectural approaches.

Potential areas of exploration include:

- More advanced ARM architecture support
- Preemptive scheduling
- Hardware-assisted memory protection
- Advanced context management
- More sophisticated memory management
- Improved scheduling mechanisms

The knowledge gained through PurusOS will provide the foundation for making more informed architectural decisions when developing VuxionOS.

PurusOS is therefore not simply an isolated experiment, but an important step in a longer-term exploration of operating-system development.

## A Learning Project, Not a Product

PurusOS is not intended to compete with established kernels or operating systems.

It is a personal exploration of computer systems at a level where the abstractions normally taken for granted must be implemented and understood.

The project will inevitably involve mistakes, experiments, redesigns, and architectural decisions that might differ from those made in mature production systems.

These are expected parts of the development process.

The most important outcome of PurusOS is not the number of features implemented or the complexity of the resulting kernel.

It is the understanding gained by building the system and discovering **why it works**.
