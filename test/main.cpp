#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}