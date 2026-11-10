# PurusOS

PurusOS is a personal bare-metal kernel development project built around one central objective:

> **Understand how a computer system works by building its fundamental components from the ground up.**

The project targets the **STM32F411CEU6 Black Pill**, based on the **ARM Cortex-M4** architecture. It aims to develop a kernel from scratch, starting with the processor's hardware capabilities and gradually building the software infrastructure required to support higher-level functionality.

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

For example, PurusOS will use **CMSIS headers** to provide standardized processor and peripheral register definitions. These headers describe the hardware interfaces; they do not replace the kernel implementation or introduce an existing hardware abstraction layer.

Similarly, existing implementations can serve as useful references, but the objective remains to understand and implement the underlying functionality independently.

Development is deliberately incremental. I prefer implementing a smaller, understandable system before introducing additional complexity. Architectural decisions should follow actual requirements rather than attempting to reproduce the feature set of a mature operating system.

## Platform and Technologies

PurusOS currently targets the **STM32F411CEU6 Black Pill**, featuring an **ARM Cortex-M4** processor.

The project uses:

- C
- ARM assembly
- CMSIS headers
- `arm-none-eabi-gcc`
- GDB
- OpenOCD
- Custom linker scripts
- A custom build system
- Bare-metal hardware access

The project does not rely on STM32 HAL, STM32CubeIDE, CubeMX, or an existing RTOS.

CMSIS is used as a standardized interface for processor and peripheral definitions, while the kernel's functionality and architecture-specific implementations are developed independently.

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

A defining characteristic of PurusOS is its bottom-up development approach.

Rather than starting with an existing operating system, the project begins with the processor and develops the software infrastructure above it.

The general architectural direction is:

```text
Hardware
↓
CMSIS / Hardware Definitions
↓
Architecture Layer
↓
Kernel
↓
Applications
```

Each layer has its own responsibilities and interfaces.

The hardware provides the underlying capabilities. CMSIS supplies standardized definitions for accessing processor and peripheral functionality. The architecture layer handles processor-specific mechanisms, while the kernel implements the general functionality required to manage the system.

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

This approach is considerably slower than building on existing infrastructure, but that trade-off is intentional.

## Current Status

The **initial kernel foundation is functionally complete**.

The project has established foundations for task management, scheduling, memory management, kernel/architecture interfaces, fault handling, context management, and kernel orchestration.

The current development direction focuses on strengthening the low-level hardware and startup infrastructure.

Particular attention is being given to implementing and understanding the ARM Cortex-M startup sequence, integrating CMSIS definitions, and validating the system through compilation, disassembly, debugging, and hardware testing.

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
