cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(RecurSeria LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# --- Modules ---

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_CXX_MODULE_STD ON)

# --- Dependencies ---

# Boost
find_package(Boost REQUIRED)

# YAML cpp
include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

# Boost.UT (header-only, has native .cppm module)
FetchContent_Declare(
  ut
  GIT_REPOSITORY https://github.com/boost-ext/ut.git
  GIT_TAG master
)
FetchContent_MakeAvailable(ut)

add_library(boost_ut)
target_sources(boost_ut
  PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${ut_SOURCE_DIR}/include
    FILES ${ut_SOURCE_DIR}/include/boost/ut.cppm
)
target_include_directories(boost_ut INTERFACE ${ut_SOURCE_DIR}/include)
target_compile_features(boost_ut INTERFACE cxx_std_20)

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

add_executable(recurseria_tests
  src/tests/main.cpp
)

target_link_libraries(recurseria_tests
  PRIVATE
  recurseria_core
  recurseria_string_ir_target
  recurseria_string_yaml_target
  boost_ut
)
