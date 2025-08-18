# Complete C++ Development Environment Setup Guide - Mac/VS Code

## Part 1: Environment Setup

### Step 1.1: Verify and Install Development Tools

```bash
# Check if Homebrew is installed
which brew || /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Update Homebrew
brew update

# Install LLVM for C++23 support (if not already installed)
brew install llvm

# Install build tools
brew install cmake ninja ccache

# Install debugging and profiling tools
brew install gdb lldb gperftools

# Install code quality tools
brew install clang-format cppcheck doxygen

# Verify installations
echo "=== Checking installed tools ==="
brew list | grep -E "llvm|cmake|ninja|clang-format" || echo "Some tools missing"
```

### Step 1.2: Configure Shell Environment

```bash
# Add to your ~/.zshrc file
echo '# LLVM Configuration for C++23' >> ~/.zshrc
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc
echo 'export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"' >> ~/.zshrc
echo 'export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"' >> ~/.zshrc

# Reload shell configuration
source ~/.zshrc

# Verify LLVM clang++ is accessible
which clang++ && clang++ --version
```

### Step 1.3: Verify C++23 Support

```bash
# Create a test file
cat > /tmp/test_cpp23.cpp << 'ENDFILE'
#include <iostream>
#include <format>
#include <ranges>

int main() {
    std::cout << std::format("C++ version: {}\n", __cplusplus);
    auto nums = std::views::iota(1, 6);
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";
    return 0;
}
ENDFILE

# Try to compile with C++23
/opt/homebrew/opt/llvm/bin/clang++ -std=c++23 -stdlib=libc++ /tmp/test_cpp23.cpp -o /tmp/test_cpp23

# Run the test
if [ -f /tmp/test_cpp23 ]; then
    /tmp/test_cpp23
    echo "✅ C++23 support verified!"
else
    echo "❌ C++23 compilation failed. Check LLVM installation."
fi
```

## Part 2: Project Structure Setup

### Step 2.1: Create Project Directory

```bash
# Create main project directory
mkdir -p ~/Development/cpp-advanced-study
cd ~/Development/cpp-advanced-study

# Create directory structure
mkdir -p day1/examples day1/exercises day1/project day1/notes
mkdir -p day2/examples day2/exercises day2/project day2/notes
mkdir -p day3/examples day3/exercises day3/project day3/notes
mkdir -p common/include common/lib common/scripts
mkdir -p resources build

echo "✅ Directory structure created"
tree -L 2 . 2>/dev/null || ls -la
```

### Step 2.2: Create Project README

```bash
# Create README.md
cat > README.md << 'ENDFILE'
# Advanced C++ Study Plan

## Project Structure

- **day1/** - Modern C++ Standards & STL
  - examples/ - Code examples and demonstrations
  - exercises/ - Practice problems
  - project/ - Day project implementation
  - notes/ - Study notes and references

- **day2/** - Template Metaprogramming & Generic Programming
  - Same substructure as day1

- **day3/** - Memory Management & Performance
  - Same substructure as day1

- **common/** - Shared utilities
  - include/ - Common header files
  - lib/ - Common implementations
  - scripts/ - Build and utility scripts

- **resources/** - Study materials and documentation

- **build/** - Build output directory

## Getting Started

1. Install required tools (see Environment Setup)
2. Open in VS Code: `code .`
3. Build examples: `./build.sh`
4. Run examples: `./run.sh <executable_name>`

## Development Environment

- Compiler: LLVM Clang++ with C++23 support
- Build System: CMake with Ninja
- IDE: Visual Studio Code
- Debugger: LLDB
ENDFILE

echo "✅ README.md created"
```

### Step 2.3: Create Root CMakeLists.txt

```bash
# Create main CMakeLists.txt
cat > CMakeLists.txt << 'ENDFILE'
cmake_minimum_required(VERSION 3.20)
project(CppAdvancedStudy VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for VS Code IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Common compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
    )
    
    # Debug-specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fno-omit-frame-pointer)
        # Sanitizers commented out by default (enable as needed)
        # add_compile_options(-fsanitize=address,undefined)
        # add_link_options(-fsanitize=address,undefined)
    else()
        add_compile_options(-O3 -march=native)
    endif()
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common/include)

# Add subdirectories for each day (uncomment as you create them)
# add_subdirectory(day1)
# add_subdirectory(day2)
# add_subdirectory(day3)

# Enable testing
enable_testing()

# Simple example executable (remove when you have actual targets)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/day1/examples/test.cpp")
    add_executable(test_example day1/examples/test.cpp)
endif()
ENDFILE

echo "✅ CMakeLists.txt created"
```

### Step 2.4: Create Build Scripts

```bash
# Create build.sh
cat > build.sh << 'ENDFILE'
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
ENDFILE

chmod +x build.sh

# Create run.sh
cat > run.sh << 'ENDFILE'
#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: ./run.sh <executable_name>"
    echo "Available executables:"
    find build -type f -perm +111 -not -path "*/CMakeFiles/*" 2>/dev/null | sed 's|build/||'
    exit 1
fi

EXECUTABLE="build/$1"

if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ Executable '$1' not found in build directory"
    echo "Available executables:"
    find build -type f -perm +111 -not -path "*/CMakeFiles/*" 2>/dev/null | sed 's|build/||'
    exit 1
fi

echo "▶️ Running $1..."
echo "------------------------"
"$EXECUTABLE"
echo "------------------------"
echo "✅ Execution completed"
ENDFILE

chmod +x run.sh

echo "✅ Build scripts created"
```

## Part 3: VS Code Workspace Setup

### Step 3.1: Create VS Code Workspace File

```bash
# Create workspace file
cat > cpp-advanced-study.code-workspace << 'ENDFILE'
{
    "folders": [
        {
            "path": ".",
            "name": "C++ Advanced Study"
        }
    ],
    "settings": {
        "files.associations": {
            "*.cpp": "cpp",
            "*.hpp": "cpp",
            "*.h": "cpp",
            "*.cc": "cpp",
            "*.tpp": "cpp"
        }
    }
}
ENDFILE

echo "✅ Workspace file created"
```

### Step 3.2: Create VS Code Settings

```bash
# Create .vscode directory
mkdir -p .vscode

# Create settings.json
cat > .vscode/settings.json << 'ENDFILE'
{
    // Editor settings
    "editor.fontSize": 14,
    "editor.formatOnSave": true,
    "editor.rulers": [80, 100],
    "editor.bracketPairColorization.enabled": true,
    
    // C/C++ extension settings
    "C_Cpp.default.compilerPath": "/opt/homebrew/opt/llvm/bin/clang++",
    "C_Cpp.default.cppStandard": "c++23",
    "C_Cpp.default.intelliSenseMode": "macos-clang-arm64",
    "C_Cpp.default.compileCommands": "${workspaceFolder}/compile_commands.json",
    "C_Cpp.clang_format_style": "{ BasedOnStyle: Google, IndentWidth: 4 }",
    
    // CMake settings
    "cmake.generator": "Ninja",
    "cmake.configureSettings": {
        "CMAKE_CXX_COMPILER": "/opt/homebrew/opt/llvm/bin/clang++",
        "CMAKE_BUILD_TYPE": "Debug"
    },
    
    // Terminal settings
    "terminal.integrated.defaultProfile.osx": "zsh",
    "terminal.integrated.env.osx": {
        "PATH": "/opt/homebrew/opt/llvm/bin:/opt/homebrew/bin:${env:PATH}"
    }
}
ENDFILE

echo "✅ VS Code settings.json created"
```

### Step 3.3: Create VS Code Tasks

```bash
# Create tasks.json
cat > .vscode/tasks.json << 'ENDFILE'
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build",
            "type": "shell",
            "command": "./build.sh",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"],
            "detail": "Build the project using build.sh"
        },
        {
            "label": "Build Current File",
            "type": "shell",
            "command": "/opt/homebrew/opt/llvm/bin/clang++",
            "args": [
                "-std=c++23",
                "-stdlib=libc++",
                "-Wall",
                "-g",
                "${file}",
                "-o",
                "${workspaceFolder}/build/${fileBasenameNoExtension}"
            ],
            "group": "build",
            "problemMatcher": ["$gcc"],
            "detail": "Compile current file only"
        },
        {
            "label": "Run",
            "type": "shell",
            "command": "./run.sh",
            "args": ["${input:executableName}"],
            "group": {
                "kind": "test",
                "isDefault": true
            },
            "dependsOn": ["Build"],
            "detail": "Run an executable from build directory"
        },
        {
            "label": "Clean",
            "type": "shell",
            "command": "rm",
            "args": ["-rf", "build"],
            "group": "build",
            "detail": "Clean build directory"
        }
    ],
    "inputs": [
        {
            "id": "executableName",
            "type": "promptString",
            "description": "Enter executable name to run"
        }
    ]
}
ENDFILE

echo "✅ VS Code tasks.json created"
```

### Step 3.4: Create VS Code Launch Configuration

```bash
# Create launch.json for debugging
cat > .vscode/launch.json << 'ENDFILE'
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Current File",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/${fileBasenameNoExtension}",
            "args": [],
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "Build Current File"
        },
        {
            "name": "Debug Selected Program",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/${input:programName}",
            "args": [],
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "Build"
        }
    ],
    "inputs": [
        {
            "id": "programName",
            "type": "promptString",
            "description": "Enter the program name to debug"
        }
    ]
}
ENDFILE

echo "✅ VS Code launch.json created"
```

### Step 3.5: Create C++ Properties

```bash
# Create c_cpp_properties.json
cat > .vscode/c_cpp_properties.json << 'ENDFILE'
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**",
                "/opt/homebrew/opt/llvm/include/c++/v1",
                "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include"
            ],
            "defines": [],
            "macFrameworkPath": [],
            "compilerPath": "/opt/homebrew/opt/llvm/bin/clang++",
            "cStandard": "c17",
            "cppStandard": "c++23",
            "intelliSenseMode": "macos-clang-arm64",
            "compileCommands": "${workspaceFolder}/compile_commands.json"
        }
    ],
    "version": 4
}
ENDFILE

echo "✅ VS Code c_cpp_properties.json created"
```

## Part 4: Create Test Files

### Step 4.1: Create a Simple Test File

```bash
# Create a test file in day1/examples
cat > day1/examples/hello_modern_cpp.cpp << 'ENDFILE'
#include <iostream>
#include <vector>
#include <format>
#include <ranges>
#include <algorithm>

int main() {
    std::cout << "=== Modern C++ Test ===\n";
    
    // Test C++20 format
    std::cout << std::format("C++ version: {}\n", __cplusplus);
    
    // Test ranges
    std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto even_numbers = numbers 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; });
    
    std::cout << "Even squares: ";
    for (int n : even_numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";
    
    return 0;
}
ENDFILE

echo "✅ Test file created"
```

### Step 4.2: Create Day-Specific CMakeLists.txt

```bash
# Create CMakeLists.txt for day1
cat > day1/CMakeLists.txt << 'ENDFILE'
# Day 1 Examples
file(GLOB EXAMPLE_SOURCES "examples/*.cpp")
foreach(SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(NAME ${SOURCE} NAME_WE)
    add_executable(day1_${NAME} ${SOURCE})
endforeach()

# Day 1 Exercises
file(GLOB EXERCISE_SOURCES "exercises/*.cpp")
foreach(SOURCE ${EXERCISE_SOURCES})
    get_filename_component(NAME ${SOURCE} NAME_WE)
    add_executable(day1_ex_${NAME} ${SOURCE})
endforeach()
ENDFILE

echo "✅ Day1 CMakeLists.txt created"

# Update root CMakeLists.txt to include day1
sed -i '' 's|# add_subdirectory(day1)|add_subdirectory(day1)|' CMakeLists.txt 2>/dev/null || \
sed -i 's|# add_subdirectory(day1)|add_subdirectory(day1)|' CMakeLists.txt
```

## Part 5: Install VS Code Extensions

### Step 5.1: Install Essential Extensions

```bash
# Install VS Code extensions via command line
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cmake-tools
code --install-extension vadimcn.vscode-lldb
code --install-extension jeff-hykin.better-cpp-syntax

echo "✅ VS Code extensions installed"
```

## Part 6: Verify Setup

### Step 6.1: Test Build System

```bash
echo "=== Testing Build System ==="

# Build the project
./build.sh

# Check if build was successful
if [ -f build/day1_hello_modern_cpp ]; then
    echo "✅ Build successful"
    
    # Run the test program
    ./run.sh day1_hello_modern_cpp
else
    echo "❌ Build failed. Check error messages above."
fi
```

### Step 6.2: Open in VS Code

```bash
# Open the workspace in VS Code
code cpp-advanced-study.code-workspace

echo "
✅ Setup Complete!

Your C++ development environment is ready. VS Code should now open with:
- IntelliSense configured for C++23
- Build tasks (Cmd+Shift+B to build)
- Debug configuration (F5 to debug)
- Proper syntax highlighting

Quick Commands:
- Build: ./build.sh or Cmd+Shift+B in VS Code
- Run: ./run.sh <executable_name>
- Debug: Set breakpoint and press F5 in VS Code
- Format: Option+Shift+F in VS Code

Next Steps:
1. Try building and running the test file
2. Create your own examples in day1/examples/
3. Start with the Day 1 study materials
"
```

## Part 7: Troubleshooting

### Common Issues and Solutions

```bash
# If clang++ is not found:
echo "export PATH=\"/opt/homebrew/opt/llvm/bin:\$PATH\"" >> ~/.zshrc
source ~/.zshrc

# If CMake fails:
brew reinstall cmake ninja

# If VS Code IntelliSense doesn't work:
# 1. Ensure compile_commands.json exists:
ls -la compile_commands.json
# 2. Reload VS Code window: Cmd+Shift+P -> "Developer: Reload Window"

# If debugging doesn't work:
# Install CodeLLDB extension:
code --install-extension vadimcn.vscode-lldb

# If C++23 features show errors:
# Verify LLVM version (should be 16+):
/opt/homebrew/opt/llvm/bin/clang++ --version
```

## Summary

This setup provides:
- ✅ LLVM Clang++ with C++23 support
- ✅ CMake build system with Ninja
- ✅ VS Code with IntelliSense
- ✅ LLDB debugging
- ✅ Organized project structure
- ✅ Build and run scripts
- ✅ Proper VS Code workspace configuration

The setup is now complete and tested. You can start your C++ study immediately!