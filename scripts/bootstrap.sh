#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

echo "========================================================================="
echo " Starting RTOS Development Environment Bootstrapper (Ubuntu/Debian)      "
echo "========================================================================="

# 1. Update package lists

echo -e "\n[1/4] Updating package repositories..."
sudo apt-get update

# 2. Install development tools, ARM cross-toolchain, debugger,
#    OpenOCD, language server, and compile_commands generator

echo -e "\n[2/4] Installing required development packages..."
sudo apt-get install -y \
build-essential \
git \
make \
gcc-arm-none-eabi \
binutils-arm-none-eabi \
libnewlib-arm-none-eabi \
gdb-multiarch \
openocd \
bear \
clangd \
kate

# 3. Create a unified .clangd config file for proper ARM indexing

echo -e "\n[3/4] Creating local .clangd configuration..."
cat << 'EOF' > .clangd
CompileFlags:
Add: [
"-target", "arm-none-eabi",
"-mthumb",
"-mcpu=cortex-m4",
"-mfloat-abi=hard",
"-mfpu=fpv4-sp-d16"
]
Compiler: arm-none-eabi-gcc
EOF

echo "--> Created '.clangd' file successfully."

# 4. Verification steps

echo -e "\n[4/4] Verifying installed toolchains..."
echo "----------------------------------------------------"

echo "ARM GCC:"
arm-none-eabi-gcc --version | head -n 1

echo "ARM binutils:"
arm-none-eabi-objcopy --version | head -n 1
arm-none-eabi-size --version | head -n 1

echo "GDB:"
gdb-multiarch --version | head -n 1

echo "OpenOCD:"
openocd --version 2>&1 | head -n 1

echo "Clangd:"
clangd --version | head -n 1

echo "Bear:"
bear --version | head -n 1

echo "Git:"
git --version

echo "----------------------------------------------------"

echo -e "\nSetup Complete!"
echo "To initialize clangd compile commands, you can now run:"
echo "  make compile_commands"
echo "========================================================================="
