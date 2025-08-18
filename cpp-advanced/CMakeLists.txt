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
add_subdirectory(day1)
# add_subdirectory(day2)
# add_subdirectory(day3)

# Enable testing
enable_testing()

# Simple example executable (remove when you have actual targets)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/day1/examples/test.cpp")
    add_executable(test_example day1/examples/test.cpp)
endif()
