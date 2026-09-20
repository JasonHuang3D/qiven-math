cmake_minimum_required(VERSION 3.28)

project(
    qiven-math
    VERSION 0.1.0
    DESCRIPTION "Small deterministic spatial math vocabulary for Qiven"
    LANGUAGES CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")
endif()

set(QIVEN_FOUNDATION_ROOT "" CACHE PATH "Path to the qiven-foundation source tree")

if(NOT TARGET qiven::foundation)
    if(QIVEN_FOUNDATION_ROOT)
        set(_qiven_foundation_source "${QIVEN_FOUNDATION_ROOT}")
    else()
        get_filename_component(_qiven_foundation_source "${CMAKE_CURRENT_SOURCE_DIR}/../qiven-foundation" ABSOLUTE)
    endif()

    if(NOT EXISTS "${_qiven_foundation_source}/CMakeLists.txt" OR
       NOT EXISTS "${_qiven_foundation_source}/include/qiven/types.hpp" OR
       NOT EXISTS "${_qiven_foundation_source}/include/qiven/contracts.hpp")
        message(FATAL_ERROR "QIVEN_FOUNDATION_ROOT must identify a qiven-foundation source tree")
    endif()

    set(QIVEN_BUILD_TESTS OFF)
    add_subdirectory("${_qiven_foundation_source}" "${CMAKE_CURRENT_BINARY_DIR}/qiven-foundation" EXCLUDE_FROM_ALL)
    unset(QIVEN_BUILD_TESTS)
endif()

add_library(qiven-math INTERFACE)
add_library(qiven::math ALIAS qiven-math)

set(
    QIVEN_MATH_PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qiven/math/scalar.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qiven/math/vec.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qiven/math/vec2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qiven/math/vec3.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qiven/math/vec4.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qiven/math/vec_algorithms.hpp
)

target_sources(
    qiven-math
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES ${QIVEN_MATH_PUBLIC_HEADERS}
)

if(CMAKE_GENERATOR MATCHES "^Visual Studio")
    source_group(
        TREE "${CMAKE_CURRENT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${QIVEN_MATH_PUBLIC_HEADERS}
    )
endif()

target_compile_features(qiven-math INTERFACE cxx_std_20)
target_include_directories(
    qiven-math
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(qiven-math INTERFACE qiven::foundation)

option(QIVEN_MATH_BUILD_TESTS "Build qiven-math tests" ${PROJECT_IS_TOP_LEVEL})

if(QIVEN_MATH_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)

    if(MSVC)
        set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT qiven-math-smoke)
    endif()
endif()
