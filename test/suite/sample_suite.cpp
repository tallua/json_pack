#include <gtest/gtest.h>

#include "jpack_impl.hpp"

using namespace jpack_test;
using namespace jpack::schema;
using namespace jpack::serialization;

TEST(JPackTest, unpack_schema)
{
  nlohmann::json json = {
    { "foo", 3 }
  };

  JsonView view(json);

  int val;
  view >> Schema(
    Must("foo", val)
  );

  EXPECT_EQ(3, val);
}