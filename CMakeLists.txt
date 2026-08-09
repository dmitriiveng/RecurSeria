cmake_minimum_required(VERSION 4.0.3)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(RecurSeria VERSION 0.1.0 LANGUAGES CXX)

# Compiler check.
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(FATAL_ERROR
    "RecurSeria requires GCC (C++26 reflection, P2996); compiler is ${CMAKE_CXX_COMPILER_ID}")
endif()

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16.2)
  message(FATAL_ERROR
    "RecurSeria requires GCC >= 16.2 (got ${CMAKE_CXX_COMPILER_VERSION}); "
    "the reflection machinery used by this project is only reliable since GCC 16.2")
endif()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Modules ---

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_CXX_MODULE_STD ON)

# --- Reflection ---
# -freflection must be in CMAKE_CXX_FLAGS (not only per-target): CMake compiles
# the synthesized `import std` module with the global flags only, and without
# -freflection GCC hides the reflection facilities of std::meta.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -freflection")

# --- Dependencies ---

# YAML cpp

find_package(yaml-cpp 0.9.0 REQUIRED)

# Boost.UT
include(FetchContent)
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
    BASE_DIRS src/tests ${ut_SOURCE_DIR}/include
    FILES ${RECURSERIA_TESTS_INTERFACE_UNITS} ${ut_SOURCE_DIR}/include/boost/ut.cppm
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

# --- Installation / packaging ---

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set_target_properties(recurseria_core PROPERTIES EXPORT_NAME core)
set_target_properties(recurseria_yaml_target PROPERTIES EXPORT_NAME yaml)
set_target_properties(recurseria_binary_target PROPERTIES EXPORT_NAME binary)

# Each target is installed into its own subdirectory.

# Core.
install(
  TARGETS recurseria_core
  EXPORT RecurSeriaTargets
  FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/recurseria/core
)

# YAML.
install(
  TARGETS recurseria_yaml_target
  EXPORT RecurSeriaTargets
  FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/recurseria/yaml
)

# Binary.
install(
  TARGETS recurseria_binary_target
  EXPORT RecurSeriaTargets
  FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/recurseria/binary
)

# Generate cmake script.
install(
  EXPORT RecurSeriaTargets
  FILE RecurSeriaTargets.cmake
  NAMESPACE RecurSeria::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RecurSeria
)

# Generate config file using template.
configure_package_config_file(
  cmake/RecurSeriaConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RecurSeriaConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RecurSeria
)

# Generate version file.
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/RecurSeriaConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion
)

# Install config and version files.
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/RecurSeriaConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/RecurSeriaConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RecurSeria
)

# Add licenses.
install(
  FILES LICENSE THIRD_PARTY_LICENSES.md
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/RecurSeria
)
