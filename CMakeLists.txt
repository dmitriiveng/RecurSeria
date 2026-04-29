cmake_minimum_required(VERSION 4.2.3)

project(RecurSeria LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Modules
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_STD 1)

# Dependencies
find_package(Boost REQUIRED)
find_package(GTest REQUIRED)

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

file(GLOB_RECURSE RECURSERIA_STRING_TARGET_INTERFACE_UNITS
  CONFIGURE_DEPENDS
  "src/targets/string/*.ixx"
  "src/targets/string/*.cppm"
)

file(GLOB_RECURSE RECURSERIA_STRING_TARGET_IMPLEMENTATIONS
  CONFIGURE_DEPENDS
  "src/targets/string/*.cpp"
)

add_library(recurseria_string_target)

target_sources(recurseria_string_target
  PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS src/targets/string/
    FILES ${RECURSERIA_STRING_TARGET_INTERFACE_UNITS}
  PRIVATE
    ${RECURSERIA_STRING_TARGET_IMPLEMENTATIONS}
)

target_link_libraries(recurseria_string_target
  PUBLIC
    recurseria_core
)

# Tests

# Core tests

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
  recurseria_string_target
  GTest::gtest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(recurseria_tests)
