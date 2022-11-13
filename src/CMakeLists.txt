# Copyright 2022 Emmanuel Arias Soto
file(GLOB HEADER_LIST CONFIGURE_DEPENDS
  "${Kaleidoscope_SOURCE_DIR}/include/ast/binary_op.h"
  "${Kaleidoscope_SOURCE_DIR}/include/ast/expression.h"
  "${Kaleidoscope_SOURCE_DIR}/include/ast/fn_call.h"
  "${Kaleidoscope_SOURCE_DIR}/include/ast/fn_prototype.h"
  "${Kaleidoscope_SOURCE_DIR}/include/ast/fn.h"
  "${Kaleidoscope_SOURCE_DIR}/include/ast/number.h"
  "${Kaleidoscope_SOURCE_DIR}/include/ast/variable.h"
  "${Kaleidoscope_SOURCE_DIR}/include/lexer/lexer.h"
  "${Kaleidoscope_SOURCE_DIR}/include/parser/parser.h"
)

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
  "ast/binary_op.cc"
  "ast/fn_call.cc"
  "ast/fn_prototype.cc"
  "ast/fn.cc"
  "ast/number.cc"
  "ast/variable.cc"
  "lexer/lexer.cc"
  "parser/parser.cc"
)

add_library(kaleidoscope ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(kaleidoscope PUBLIC ../include)

target_compile_features(kaleidoscope PUBLIC cxx_std_17)

target_link_libraries(kaleidoscope PUBLIC fmt)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
