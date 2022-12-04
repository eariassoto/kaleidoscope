# Copyright 2022 Emmanuel Arias Soto
file(GLOB HEADER_LIST CONFIGURE_DEPENDS
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/ast/base_expression.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/ast/binary_op.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/ast/fn_call.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/ast/fn_prototype.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/ast/fn.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/ast/number.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/ast/variable.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/jit_interpreter.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/lexer_error.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/lexer.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/parser.h"
  "${Kaleidoscope_SOURCE_DIR}/include/kaleidoscope/token.h"
)

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
  "ast/binary_op.cc"
  "ast/fn_call.cc"
  "ast/fn_prototype.cc"
  "ast/fn.cc"
  "ast/number.cc"
  "ast/variable.cc"
  "jit_interpreter.cc"
  "lexer_error.cc"
  "lexer.cc"
  "parser.cc"
  "token.cc"
)

add_library(kaleidoscope ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(kaleidoscope PUBLIC ../include)

target_compile_features(kaleidoscope PUBLIC cxx_std_17)

target_link_libraries(kaleidoscope PUBLIC fmt expected)

llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(kaleidoscope PRIVATE ${llvm_libs})

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
