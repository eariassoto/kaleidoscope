# Copyright 2022 Emmanuel Arias Soto
#file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${Kaleidoscope_SOURCE_DIR}/include/lexer/lexer.h")

#file(GLOB SOURCE_LIST CONFIGURE_DEPENDS )

add_library(
    kaleidoscope
    "${Kaleidoscope_SOURCE_DIR}/include/lexer/lexer.h"
    "lexer/lexer.cc")
    #${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(kaleidoscope PUBLIC ../include)

target_compile_features(kaleidoscope PUBLIC cxx_std_17)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
