# Copyright 2022 Emmanuel Arias Soto
cmake_minimum_required(VERSION 3.12...3.24)

project(
    Kaleidoscope
    VERSION 1.0.0
    DESCRIPTION "Simple LLVM Frontend tutorial."
    LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()  # (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

add_subdirectory(src)
add_subdirectory(apps)
