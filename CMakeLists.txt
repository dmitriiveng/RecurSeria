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

# String targets

# IR
file(GLOB_RECURSE RECURSERIA_STRING_IR_TARGET_INTERFACE_UNITS
  CONFIGURE_DEPENDS
  "src/targets/string/IR/*.ixx"
  "src/targets/string/IR/*.cppm"
)

file(GLOB_RECURSE RECURSERIA_STRING_IR_TARGET_IMPLEMENTATIONS
  CONFIGURE_DEPENDS
  "src/targets/string/IR/*.cpp"
)

add_library(recurseria_string_ir_target)

target_sources(recurseria_string_ir_target
  PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS src/targets/string/IR/
    FILES ${RECURSERIA_STRING_IR_TARGET_INTERFACE_UNITS}
  PRIVATE
    ${RECURSERIA_STRING_IR_TARGET_IMPLEMENTATIONS}
)

target_link_libraries(recurseria_string_ir_target
  PUBLIC
    recurseria_core
)

# YAML
file(GLOB_RECURSE RECURSERIA_STRING_YAML_TARGET_INTERFACE_UNITS
  CONFIGURE_DEPENDS
  "src/targets/string/YAML/*.ixx"
  "src/targets/string/YAML/*.cppm"
)

file(GLOB_RECURSE RECURSERIA_STRING_YAML_TARGET_IMPLEMENTATIONS
  CONFIGURE_DEPENDS
  "src/targets/string/YAML/*.cpp"
)

add_library(recurseria_string_yaml_target)

target_sources(recurseria_string_yaml_target
  PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS src/targets/string/YAML/
    FILES ${RECURSERIA_STRING_YAML_TARGET_INTERFACE_UNITS}
  PRIVATE
    ${RECURSERIA_STRING_YAML_TARGET_IMPLEMENTATIONS}
)

target_link_libraries(recurseria_string_yaml_target
  PUBLIC
    recurseria_core
    recurseria_string_ir_target
    yaml-cpp::yaml-cpp
)

# Tests

add_executable(recurseria_tests
  src/tests/main.cpp
)

target_sources(recurseria_tests
  PRIVATE
    FILE_SET CXX_MODULES
    BASE_DIRS src/tests
    FILES src/tests/boost_ut.ixx
)

target_include_directories(recurseria_tests
  PRIVATE
    ${ut_SOURCE_DIR}/include
)

target_link_libraries(recurseria_tests
  PRIVATE
  recurseria_core
  recurseria_string_ir_target
  recurseria_string_yaml_target
)
