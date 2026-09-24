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

# --- workspace resolution (ADR-0052, WR-3 Foundation cutover) -------------
# CMake receives the resolved Foundation root from the workspace adapter
# (QIVEN_RESOLUTION_FILE, supplied by the gate-configure bootstrap); it
# materializes the locked provider once per build graph and never resolves
# siblings or checks a consumer-local pin.
if(NOT DEFINED ENV{QIVEN_RESOLUTION_FILE} OR "$ENV{QIVEN_RESOLUTION_FILE}" STREQUAL "")
    message(FATAL_ERROR
        "QIVEN_RESOLUTION_FILE is not set - configure through the workspace bootstrap "
        "(qiven-workspace gate-configure), not a bare cmake invocation")
endif()
include("$ENV{QIVEN_RESOLUTION_FILE}")
qiven_workspace_materialize(qiven-foundation)

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
