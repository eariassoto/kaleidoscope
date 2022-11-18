include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(unittests
  "lexer_unittest.cc"
  "token_unittest.cc")

target_compile_features(unittests PRIVATE cxx_std_20)

target_link_libraries(unittests PRIVATE kaleidoscope gtest)

add_test(NAME unittests COMMAND unittests)