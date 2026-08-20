################################################################################
# Toolchain
################################################################################

CC      := arm-none-eabi-gcc
AS      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE    := arm-none-eabi-size
GDB     := gdb-multiarch

################################################################################
# Project
################################################################################

TARGET := rtos_app

################################################################################
# Build Directories
################################################################################

BUILD_DIR  := build
APP_DIR    := app
KERNEL_DIR := kernel
LIBS_DIR   := libs

################################################################################
# Board Selection
################################################################################

BOARD := stm32-blackpill

################################################################################
# Board Definitions
################################################################################

ifeq ($(BOARD),stm32-blackpill)

CPU             := cortex-m4
FLOAT_ABI       := hard
FPU             := fpv4-sp-d16

CORE_DIR        := arch/arm/cortex-m4/core
SOC_DIR         := soc/stm32/f411
BSP_DIR         := bsp/stm32-blackpill

LINKER_SCRIPT   := xe

OPENOCD_CFG     := $(BSP_DIR)/openocd.cfg

endif

################################################################################
# OpenOCD
################################################################################

OPENOCD := openocd

################################################################################
# Compiler & Linker Flags
################################################################################

CPU_FLAGS := \
	-mcpu=$(CPU) \
	-mthumb \
	-mfloat-abi=$(FLOAT_ABI) \
	-mfpu=$(FPU)

C_FLAGS := \
	$(CPU_FLAGS) \
	-O0 \
	-g3 \
	-Wall \
	-fdata-sections \
	-ffunction-sections

AS_FLAGS := \
	$(CPU_FLAGS) \
	-g3 \
	-Wall

LD_FLAGS := \
	$(CPU_FLAGS) \
	-specs=nano.specs \
	--specs=nosys.specs \
	-T$(SOC_DIR)/linker/$(LINKER_SCRIPT).ld \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections

################################################################################
# Include Paths
################################################################################

INC_DIRS := \
	-I$(APP_DIR) \
	-I$(KERNEL_DIR)/include \
	-I$(CORE_DIR) \
	-I$(SOC_DIR)/drivers/include \
	-I$(BSP_DIR) \
	-I$(LIBS_DIR)/include

################################################################################
# Source Files
################################################################################

C_SOURCES := \
	$(wildcard $(APP_DIR)/*.c) \
	$(wildcard $(KERNEL_DIR)/src/*.c) \
	$(wildcard $(CORE_DIR)/*.c) \
	$(wildcard $(SOC_DIR)/drivers/src/*.c) \
	$(wildcard $(BSP_DIR)/*.c) \
	$(wildcard $(LIBS_DIR)/src/*.c)

ASM_SOURCES := \
	$(wildcard $(CORE_DIR)/*.S) \
	$(SOC_DIR)/startup.S

################################################################################
# Object Files
################################################################################

OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
OBJECTS += $(patsubst %.S,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

################################################################################
# Build Targets
################################################################################

.PHONY: all clean rebuild compile_commands flash openocd gdb

all: $(BUILD_DIR)/$(TARGET).elf \
     $(BUILD_DIR)/$(TARGET).bin \
     $(BUILD_DIR)/$(TARGET).hex

################################################################################
# Linking
################################################################################

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) $(LD_FLAGS) -o $@
	$(SIZE) $@

################################################################################
# Binary Generation
################################################################################

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

################################################################################
# Compilation
################################################################################

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $(INC_DIRS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) $(INC_DIRS) -c $< -o $@

################################################################################
# Utility Targets
################################################################################

clean:
	rm -rf $(BUILD_DIR)/*
	touch $(BUILD_DIR)/.gitkeep

rebuild: clean all

compile_commands: clean
	bear -- make

################################################################################
# Hardware Targets
################################################################################

openocd:
	$(OPENOCD) -f $(OPENOCD_CFG)

flash: $(BUILD_DIR)/$(TARGET).elf
	$(OPENOCD) -f $(OPENOCD_CFG) \
	-c "program $< verify reset exit"

gdb: $(BUILD_DIR)/$(TARGET).elf
	$(GDB) $< \
	-ex "target extended-remote :3333" \
	-ex "monitor reset halt" \
	-ex "load" \
	-ex "monitor reset halt"
