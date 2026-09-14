function(qiven_math_enable_strong_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
    endif()
endfunction()

add_executable(qiven-math-smoke smoke.cpp)
target_link_libraries(qiven-math-smoke PRIVATE qiven::math)
qiven_math_enable_strong_warnings(qiven-math-smoke)

add_test(NAME qiven-math-smoke COMMAND qiven-math-smoke)

add_executable(qiven-math-vector-core vector_core.cpp)
target_link_libraries(qiven-math-vector-core PRIVATE qiven::math)
qiven_math_enable_strong_warnings(qiven-math-vector-core)
add_test(NAME qiven-math-vector-core COMMAND qiven-math-vector-core)

add_executable(qiven-math-vector-algorithms vector_algorithms.cpp)
target_link_libraries(qiven-math-vector-algorithms PRIVATE qiven::math)
qiven_math_enable_strong_warnings(qiven-math-vector-algorithms)
add_test(NAME qiven-math-vector-algorithms COMMAND qiven-math-vector-algorithms)

add_library(
    qiven-math-header-check
    OBJECT
        header_check/scalar.cpp
        header_check/vec2.cpp
        header_check/vec3.cpp
        header_check/vec4.cpp
        header_check/vec_algorithms.cpp
        header_check/vec.cpp
)
target_link_libraries(qiven-math-header-check PRIVATE qiven::math)
qiven_math_enable_strong_warnings(qiven-math-header-check)

set_target_properties(
    qiven-math-smoke
    qiven-math-vector-core
    qiven-math-vector-algorithms
    qiven-math-header-check
    PROPERTIES FOLDER "Tests"
)
