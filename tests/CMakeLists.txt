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

set_target_properties(qiven-math-smoke PROPERTIES FOLDER "Tests")
