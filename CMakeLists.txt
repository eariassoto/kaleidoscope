# Copyright 2022 Emmanuel Arias Soto
cmake_minimum_required(VERSION 3.12...3.24)

project(
    Kaleidoscope
    VERSION 1.0.0
    DESCRIPTION "Simple LLVM Frontend tutorial."
    LANGUAGES C CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  include(CTest)
endif()  # (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

include(FetchContent)

# fmt library
FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.0.0)
FetchContent_MakeAvailable(fmtlib)

# tl::expected
set(EXPECTED_BUILD_TESTS, OFF)
FetchContent_Declare(
  expected
  GIT_REPOSITORY https://github.com/TartanLlama/expected.git
  GIT_TAG b74fecd4448a1a5549402d17ddc51e39faa5020c)
FetchContent_MakeAvailable(expected)

# Configure LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_subdirectory(src)

add_subdirectory(apps)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
