#include <gtest/gtest.h>

#include "lexer/token2.h"

class TokenTest : public ::testing::Test {

};

TEST_F(TokenTest, EofToken){

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
