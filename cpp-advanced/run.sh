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
