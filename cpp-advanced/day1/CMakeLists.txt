# day1/CMakeLists.txt
# Day 1: Modern C++ Standards & STL Mastery

# Find required packages
find_package(Threads REQUIRED)

# === EXAMPLES ===
# Morning Session Examples
add_executable(move_semantics_demo 
    examples/move_semantics_demo.cpp
)
target_link_libraries(move_semantics_demo PRIVATE Threads::Threads)

add_executable(stl_advanced 
    examples/stl_advanced.cpp
)
target_link_libraries(stl_advanced PRIVATE Threads::Threads)

# === EXERCISES ===
# Add exercise executables as you create them

# === PROJECT ===
# Evening Project: Resource Manager
add_executable(resource_manager_project 
    project/resource_manager_project.cpp
)
target_link_libraries(resource_manager_project PRIVATE Threads::Threads)

# === HELPER TARGETS ===
add_custom_target(run_day1_examples
    COMMAND echo "Running Day 1 Examples..."
    COMMAND $<TARGET_FILE:move_semantics_demo>
    COMMAND echo "---"
    COMMAND $<TARGET_FILE:stl_advanced>
    DEPENDS move_semantics_demo stl_advanced
    COMMENT "Running all Day 1 examples"
)

# Exercise executable
add_executable(move_semantics_exercises 
    exercises/move_semantics_exercises.cpp
)
target_link_libraries(move_semantics_exercises PRIVATE Threads::Threads)

