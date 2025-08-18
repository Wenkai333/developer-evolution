# C++ Development Environment Setup - Mac/VS Code

## Prerequisites Check
Since you've already set up the basic environment, verify you have these installed:

```bash
# Check installed tools
brew list | grep -E "cmake|git|llvm|ninja|ccache|boost|googletest|benchmark|clang-format"

# Check VSCode
code --version

# Check compiler
clang++ --version
g++ --version  # If you installed gcc via brew
```

## 1. Additional C++ Study Tools

### Install Missing C++ Components
```bash
# Advanced C++ development tools (if not installed)
brew install gdb               # GNU debugger (requires code signing on Mac)
brew install valgrind          # Memory debugging (limited Mac support)
brew install gperftools        # Google Performance Tools
brew install tbb              # Intel Threading Building Blocks

# Static analysis and formatting
brew install cppcheck         # Static analysis
brew install include-what-you-use  # Header dependency checker
brew install astyle           # Alternative code formatter

# Documentation and visualization
brew install doxygen graphviz  # Code documentation
```

### Install C++23 Support
```bash
# For latest C++ features, use LLVM from Homebrew
brew install llvm

# Add LLVM to PATH (add to ~/.zshrc)
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc
echo 'export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"' >> ~/.zshrc
echo 'export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"' >> ~/.zshrc
source ~/.zshrc

# Verify LLVM clang++ version (should be 17+ for C++23)
/opt/homebrew/opt/llvm/bin/clang++ --version
```

## 2. VS Code Configuration for C++ Study

### Update VS Code Settings for Modern C++
Create/update `.vscode/settings.json` in your study folder:

```json
{
    // Use LLVM clang for C++23 support
    "C_Cpp.default.compilerPath": "/opt/homebrew/opt/llvm/bin/clang++",
    "C_Cpp.default.cppStandard": "c++23",
    "C_Cpp.default.intelliSenseMode": "macos-clang-arm64",
    
    // Advanced IntelliSense
    "C_Cpp.intelliSenseEngine": "default",
    "C_Cpp.intelliSenseEngineFallback": "enabled",
    "C_Cpp.autocompleteAddParentheses": true,
    "C_Cpp.suggestSnippets": true,
    
    // Code formatting
    "C_Cpp.clang_format_style": "{ BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 100, AllowShortFunctionsOnASingleLine: Empty }",
    "editor.formatOnSave": true,
    "editor.formatOnPaste": true,
    
    // Build settings
    "cmake.generator": "Ninja",
    "cmake.configureSettings": {
        "CMAKE_CXX_COMPILER": "/opt/homebrew/opt/llvm/bin/clang++",
        "CMAKE_C_COMPILER": "/opt/homebrew/opt/llvm/bin/clang",
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
    },
    
    // File associations for C++ study
    "files.associations": {
        "*.tpp": "cpp",
        "*.ipp": "cpp",
        "algorithm": "cpp",
        "memory": "cpp",
        "ranges": "cpp",
        "concepts": "cpp",
        "coroutine": "cpp"
    }
}
```

### Additional VS Code Extensions for Advanced C++
```bash
# Install via VS Code command palette (Cmd+Shift+P)
code --install-extension jeff-hykin.better-cpp-syntax
code --install-extension cschlosser.doxdocgen      # Documentation generator
code --install-extension hars.cppsnippets          # C++ snippets
code --install-extension austin.code-gnu-global    # Code navigation
code --install-extension vadimcn.vscode-lldb       # Better debugger for Mac
```

## 3. Study Project Structure

### Create C++ Study Project Template
```bash
#!/bin/bash
# Save as: setup_study_day.sh

DAY=$1  # day1, day2, or day3
mkdir -p cpp-advanced-study/$DAY/{examples,exercises,project,notes}
cd cpp-advanced-study/$DAY

# Create CMakeLists.txt with C++23 support
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.25)
project(CppStudyDay VERSION 1.0.0 LANGUAGES CXX)

# C++23 Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for VSCode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings and optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options(
        -Wall -Wextra -Wpedantic -Werror
        -Wno-unused-parameter
        -Wno-missing-field-initializers
    )
    
    # Debug vs Release flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fno-omit-frame-pointer)
        add_compile_options(-fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)
    else()
        add_compile_options(-O3 -march=native -flto)
        add_link_options(-flto)
    endif()
endif()

# Threading support
find_package(Threads REQUIRED)

# Add subdirectories
add_subdirectory(examples)
add_subdirectory(exercises)
add_subdirectory(project)

# Enable testing
enable_testing()
include(CTest)
EOF

# Create subdirectory CMakeLists
for dir in examples exercises project; do
    cat > $dir/CMakeLists.txt << 'EOF'
# Add executables here
# Example:
# add_executable(example1 example1.cpp)
# target_link_libraries(example1 PRIVATE Threads::Threads)
EOF
done

# Create compile and run scripts
cat > build.sh << 'EOF'
#!/bin/bash
cmake -B build -G Ninja \
    -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ \
    -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
EOF

cat > run.sh << 'EOF'
#!/bin/bash
if [ -z "$1" ]; then
    echo "Usage: ./run.sh <executable_name>"
    exit 1
fi
./build/$1
EOF

chmod +x build.sh run.sh

echo "Study environment for $DAY created!"
EOF

chmod +x setup_study_day.sh
```

## 4. Debugging Configuration

### LLDB Configuration for Mac
Create `~/.lldbinit`:

```bash
# Better LLDB formatting
settings set target.prefer-dynamic-value run-target
settings set target.skip-prologue true
settings set target.source-map /usr/include /opt/homebrew/opt/llvm/include

# Custom commands
command alias bfl breakpoint set -f %1 -l %2
command alias lv frame variable
command alias so thread step-out

# Pretty printing for STL
command script import /opt/homebrew/opt/llvm/share/lldb/examples/synthetic/gnu_libstdcpp.py
```

### VS Code Debug Configuration
Update `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "LLDB Debug",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/${fileBasenameNoExtension}",
            "args": [],
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "Build Current File"
        },
        {
            "name": "LLDB Debug with Input",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/${fileBasenameNoExtension}",
            "args": ["<", "${workspaceFolder}/input.txt"],
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "Build Current File"
        }
    ]
}
```

## 5. Performance Profiling Setup

### Install Profiling Tools
```bash
# Instruments (comes with Xcode)
xcode-select --install

# Alternative profilers
brew install gperftools
brew install flamegraph

# Create profiling helper script
cat > profile.sh << 'EOF'
#!/bin/bash
EXEC=$1
if [ -z "$EXEC" ]; then
    echo "Usage: ./profile.sh <executable>"
    exit 1
fi

echo "Profiling $EXEC..."

# Time execution
echo "=== Time Profile ==="
time ./$EXEC

# Memory profile (if gperftools linked)
echo "=== Memory Profile ==="
HEAPPROFILE=/tmp/heap.prof ./$EXEC
pprof --text ./$EXEC /tmp/heap.prof

# Generate flamegraph (if supported)
echo "=== Generating Flamegraph ==="
sudo dtrace -x ustackframes=100 -n "profile-997 /execname == \"$EXEC\"/ { @[ustack()] = count(); }" -o out.stacks -c ./$EXEC
flamegraph.pl out.stacks > flamegraph.svg
echo "Flamegraph saved to flamegraph.svg"
EOF

chmod +x profile.sh
```

## 6. Quick Commands Cheatsheet

### Build Commands
```bash
# Quick build with ninja
cmake -B build -G Ninja && cmake --build build

# Build with specific standard
clang++ -std=c++23 -stdlib=libc++ file.cpp -o output

# Build with all optimizations
clang++ -std=c++23 -O3 -march=native -flto file.cpp -o output

# Build with sanitizers
clang++ -std=c++23 -g -fsanitize=address,undefined file.cpp -o output

# Build with profiling
clang++ -std=c++23 -g -pg file.cpp -o output
```

### VSCode Keyboard Shortcuts (Mac)
- **⌘+Shift+B** - Build task
- **F5** - Start debugging  
- **⇧+F5** - Stop debugging
- **F9** - Toggle breakpoint
- **F10** - Step over
- **F11** - Step into
- **⇧+F11** - Step out
- **⌘+K ⌘+F** - Format selection
- **⌥+⇧+F** - Format document
- **F12** - Go to definition
- **⌥+F12** - Peek definition
- **⇧+F12** - Show references
- **⌘+.** - Quick fix
- **⌘+P** - Quick file open
- **⌘+⇧+P** - Command palette

## 7. Git Setup for Study Repository

### Initialize Study Repository
```bash
cd cpp-advanced-study
git init
git remote add origin git@github.com:yourusername/cpp-advanced-study.git

# Create .gitignore
cat > .gitignore << 'EOF'
# Build directories
build/
cmake-build-*/
*.build/

# Executables
*.out
*.exe
a.out

# Object files
*.o
*.obj

# Profiling
*.prof
gmon.out
callgrind.out.*
perf.data*

# Mac specific
.DS_Store
*.dSYM/

# VSCode
.vscode/*
!.vscode/settings.json
!.vscode/tasks.json
!.vscode/launch.json

# Compiled documentation
docs/html/
docs/latex/
EOF

git add .
git commit -m "Initial setup for advanced C++ study"
git push -u origin main
```

## 8. Verification Script

Create `verify_setup.sh`:

```bash
#!/bin/bash

echo "🔍 Verifying C++ Study Environment..."

# Check compilers
echo -e "\n📦 Compiler Versions:"
clang++ --version | head -1
if command -v /opt/homebrew/opt/llvm/bin/clang++ &> /dev/null; then
    echo "LLVM: $(/opt/homebrew/opt/llvm/bin/clang++ --version | head -1)"
fi

# Check tools
echo -e "\n🛠️ Build Tools:"
for tool in cmake ninja make git; do
    if command -v $tool &> /dev/null; then
        echo "✅ $tool: $(command -v $tool)"
    else
        echo "❌ $tool: not found"
    fi
done

# Check C++ features
echo -e "\n🚀 C++ Feature Test:"
cat > /tmp/test_cpp23.cpp << 'EOF'
#include <iostream>
#include <format>
#include <ranges>
#include <concepts>

int main() {
    std::cout << std::format("C++ {} supported!\n", __cplusplus);
    auto nums = std::views::iota(1, 5);
    for(auto n : nums) std::cout << n << " ";
    std::cout << "\n";
    return 0;
}
EOF

if /opt/homebrew/opt/llvm/bin/clang++ -std=c++23 -stdlib=libc++ /tmp/test_cpp23.cpp -o /tmp/test_cpp23 2>/dev/null; then
    echo "✅ C++23 compilation successful"
    /tmp/test_cpp23
else
    echo "⚠️ C++23 features may not be fully supported"
fi

echo -e "\n✨ Setup verification complete!"
```

## Ready to Start Learning!

Your environment is now optimized for the 3-day advanced C++ study plan with:
- C++23 support via LLVM
- Advanced debugging with LLDB
- Performance profiling tools
- Automated build system
- VSCode fully configured

Start with Day 1 by running:
```bash
./setup_study_day.sh day1
cd cpp-advanced-study/day1
code .
```