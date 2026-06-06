cmake_minimum_required(VERSION 4.2.3)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")

project(RecurSeria LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# --- Modules ---

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_CXX_MODULE_STD ON)

# --- Dependencies ---

# Boost
find_package(Boost REQUIRED)

# GTest
find_package(GTest REQUIRED)

# YAML cpp
include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

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

target_link_libraries(recurseria_core
  PRIVATE
    Boost::boost
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

file(GLOB_RECURSE RECURSERIA_TESTS
  CONFIGURE_DEPENDS
    "src/tests/*.cpp"
)

add_executable(recurseria_tests
  ${RECURSERIA_TESTS}
)

target_link_libraries(recurseria_tests
  PRIVATE
  recurseria_core
  recurseria_string_ir_target
  recurseria_string_yaml_target
  GTest::gtest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(recurseria_tests)
