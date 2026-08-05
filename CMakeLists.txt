cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_STANDARD_LIBRARY "libstdc++" CACHE STRING "")

project(RecurSeria LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# --- Modules ---

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_CXX_MODULE_STD ON)

# --- Reflection ---

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -freflection")

# --- Dependencies ---

# YAML cpp
include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.9.0
)
FetchContent_MakeAvailable(yaml-cpp)

# Boost.UT
FetchContent_Declare(
  ut
  GIT_REPOSITORY https://github.com/boost-ext/ut.git
  GIT_TAG v2.3.1
)
FetchContent_MakeAvailable(ut)

# --- RecurSeria library ---

# Core
file(GLOB_RECURSE RECURSERIA_CORE_INTERFACE_UNITS
  CONFIGURE_DEPENDS
  "src/core/*.ixx"
  "src/core/*.cppm"
)

file(GLOB_RECURSE RECURSERIA_CORE_IMPLEMENTATIONS
  CONFIGURE_DEPENDS
  "src/core/*.cpp"
)

add_library(recurseria_core)

target_sources(recurseria_core
  PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS src/core
    FILES ${RECURSERIA_CORE_INTERFACE_UNITS}
  PRIVATE
    ${RECURSERIA_CORE_IMPLEMENTATIONS}
)

# YAML target

file(GLOB_RECURSE RECURSERIA_YAML_TARGET_INTERFACE_UNITS
  CONFIGURE_DEPENDS
  "src/targets/YAML/*.ixx"
  "src/targets/YAML/*.cppm"
)

file(GLOB_RECURSE RECURSERIA_YAML_TARGET_IMPLEMENTATIONS
  CONFIGURE_DEPENDS
  "src/targets/YAML/*.cpp"
)

add_library(recurseria_yaml_target)

target_sources(recurseria_yaml_target
  PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS src/targets/YAML/
    FILES ${RECURSERIA_YAML_TARGET_INTERFACE_UNITS}
  PRIVATE
    ${RECURSERIA_YAML_TARGET_IMPLEMENTATIONS}
)

target_link_libraries(recurseria_yaml_target
  PUBLIC
    recurseria_core
    yaml-cpp::yaml-cpp
)

# Binary target

file(GLOB_RECURSE RECURSERIA_BINARY_TARGET_INTERFACE_UNITS
  CONFIGURE_DEPENDS
  "src/targets/binary/*.ixx"
  "src/targets/binary/*.cppm"
)

file(GLOB_RECURSE RECURSERIA_BINARY_TARGET_IMPLEMENTATIONS
  CONFIGURE_DEPENDS
  "src/targets/binary/*.cpp"
)

add_library(recurseria_binary_target)

target_sources(recurseria_binary_target
  PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS src/targets/binary/
    FILES ${RECURSERIA_BINARY_TARGET_INTERFACE_UNITS}
  PRIVATE
    ${RECURSERIA_BINARY_TARGET_IMPLEMENTATIONS}
)

target_link_libraries(recurseria_binary_target
  PUBLIC
    recurseria_core
)

# Tests

file(GLOB_RECURSE RECURSERIA_TESTS_INTERFACE_UNITS
  CONFIGURE_DEPENDS
  "src/tests/*.ixx"
  "src/tests/*.cppm"
)

file(GLOB_RECURSE RECURSERIA_TESTS_IMPLEMENTATIONS
  CONFIGURE_DEPENDS
  "src/tests/*.cpp"
)

add_executable(recurseria_tests
  src/tests/main.cpp
)

target_sources(recurseria_tests
  PRIVATE
    FILE_SET CXX_MODULES
    BASE_DIRS src/tests
    FILES ${RECURSERIA_TESTS_INTERFACE_UNITS}
)

target_include_directories(recurseria_tests
  PRIVATE
    ${ut_SOURCE_DIR}/include
)

target_link_libraries(recurseria_tests
  PRIVATE
  recurseria_core
  recurseria_yaml_target
  recurseria_binary_target
)
