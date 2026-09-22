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
# Cross-repo pin law (Devkit conventions: cross-repo-cmake.md; owner
# direction 2026-09-23 - full SHA-pin coverage, no revisit deferrals):
# consumption is validated against an exact commit at configure time.
set(QIVEN_FOUNDATION_PINNED_SHA "79e67895a2a02a206d07660de1a483a856bc2612")

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

    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
        WORKING_DIRECTORY "${_qiven_foundation_source}"
        RESULT_VARIABLE QIVEN_FOUNDATION_SHA_RESULT
        OUTPUT_VARIABLE QIVEN_FOUNDATION_SHA
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(NOT QIVEN_FOUNDATION_SHA_RESULT EQUAL 0)
        message(FATAL_ERROR "cannot read the qiven-foundation revision at ${_qiven_foundation_source}")
    endif()
    if(NOT QIVEN_FOUNDATION_SHA STREQUAL QIVEN_FOUNDATION_PINNED_SHA)
        message(FATAL_ERROR "qiven-foundation is pinned at ${QIVEN_FOUNDATION_PINNED_SHA} but the checkout is at ${QIVEN_FOUNDATION_SHA}; re-pin explicitly in CMakeLists.txt and re-validate (layer contract)")
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
