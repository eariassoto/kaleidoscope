include(FetchContent)

set(BUILD_GMOCK, on)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(unittests
  "mock_lexer.h"
  "lexer_unittest.cc"
)

target_compile_features(unittests PRIVATE cxx_std_17)

target_link_libraries(unittests PRIVATE kaleidoscope gmock gtest fmt)

add_test(NAME unittests COMMAND unittests)