#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo "🔨 Building C++ Advanced Study Project..."

# Create build directory if it doesn't exist
mkdir -p build

# Configure with CMake
echo "📦 Configuring with CMake..."
cmake -S . -B build \
    -G Ninja \
    -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ CMake configuration failed${NC}"
    exit 1
fi

# Build
echo "🔨 Building..."
cmake --build build --parallel

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Build failed${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Build successful!${NC}"

# Create symlink for compile_commands.json (for VS Code IntelliSense)
if [ -f build/compile_commands.json ]; then
    ln -sf build/compile_commands.json compile_commands.json
    echo "📝 compile_commands.json linked for VS Code"
fi
