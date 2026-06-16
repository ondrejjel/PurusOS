# --- Toolchain Setup ---
CC      := arm-none-eabi-gcc
AS      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE    := arm-none-eabi-size
GDB     := gdb-multiarch

# --- Project Target Name ---
TARGET  := rtos_app

# --- Directory Definitions ---
BUILD_DIR := build
APP_DIR   := app
KERN_DIR  := kernel
PORT_DIR  := portable/arch/arm/cortex-m4/port
BSP_DIR   := portable/bsp/stm32f411xx
LIBS_DIR  := libs

# --- Linker script ---
LINKER_SCRIPT := stm32f411xe
#LINKER_SCRIPT := stm32f411xc # uncomment if working with 256kb version

# --- OpenOCD Setup ---
OPENOCD           := openocd
OPENOCD_CFG       := $(BSP_DIR)/openocd.cfg
OPENOCD_INTERFACE := interface/stlink.cfg
OPENOCD_TARGET    := target/stm32f4x.cfg

# --- Architecture & Compilation Flags ---
MCU       := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
C_FLAGS   := $(MCU) -O0 -g3 -Wall -fdata-sections -ffunction-sections
AS_FLAGS  := $(MCU) -g3 -Wall
# Linking
LD_FLAGS  := $(MCU) -specs=nano.specs -T$(BSP_DIR)/$(LINKER_SCRIPT).ld -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections

# --- Include Paths ---
INC_DIRS := \
    -I$(APP_DIR) \
    -I$(KERN_DIR)/include \
    -I$(PORT_DIR) \
    -I$(BSP_DIR)/drivers/include \
    -I$(LIBS_DIR)/include

# --- Source Files ---

# Automatically grab all .c files in the specified directories
C_SOURCES := \
    $(wildcard $(APP_DIR)/*.c) \
    $(wildcard $(KERN_DIR)/src/*.c) \
    $(wildcard $(PORT_DIR)/*.c) \
    $(wildcard $(BSP_DIR)/drivers/src/*.c)

# Automatically grab all .S files in the specified directories
ASM_SOURCES := \
    $(wildcard $(PORT_DIR)/*.S) \
    $(wildcard $(BSP_DIR)/*.S)

# --- Object Files Tracking ---
OBJECTS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJECTS += $(patsubst %.S, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

# --- Build Rules ---
.PHONY: all clean openocd flash gdb rebuild

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex

# Link everything into the final ELF file
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) $(LD_FLAGS) -o $@
	$(SIZE) $@

# Generate binary image
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Generate hex image
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Compile C files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $(INC_DIRS) -c $< -o $@

# Compile Assembly files
$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) $(INC_DIRS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Rebuild option
rebuild: clean all

# --- Hardware Deployment & Debugging Targets ---

# Start OpenOCD Server
openocd:
	$(OPENOCD) -f $(OPENOCD_CFG)

# Flash the binary directly via OpenOCD and exit
flash: $(BUILD_DIR)/$(TARGET).elf
	$(OPENOCD) -f $(OPENOCD_CFG) -c "program $< verify reset exit"

# Launch gdb-multiarch, connect to local OpenOCD, program, and reset
gdb: $(BUILD_DIR)/$(TARGET).elf
	$(GDB) $< -ex "target extended-remote :3333" \
	          -ex "monitor reset halt" \
	          -ex "load" \
	          -ex "monitor reset halt"
