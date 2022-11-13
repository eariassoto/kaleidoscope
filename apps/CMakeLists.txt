# Copyright 2022 Emmanuel Arias Soto
cmake_minimum_required(VERSION 3.12...3.24)

add_executable(interpreter
    main.cc)

target_compile_features(interpreter PRIVATE cxx_std_17)
target_link_libraries(interpreter PRIVATE kaleidoscope)

