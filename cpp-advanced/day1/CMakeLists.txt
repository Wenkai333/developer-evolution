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
