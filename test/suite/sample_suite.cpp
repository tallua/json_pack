#include <gtest/gtest.h>

#include "jpack_impl.hpp"

using namespace jpack_test;
using namespace jpack::schema;
using namespace jpack::serialization;

TEST(JPackTest, unpack_schema)
{
  nlohmann::json json = {
    { "foo", 3 },
    { "bar", 4 },
    { "baz", 5 }
  };

  JsonView view(json);

  int val1;
  int val2;
  int val3;
  auto schema = Schema(
    Must("foo", val1),
    Must("bar", val2),
    Must("baz", val3)
  );

  view >> schema;

  EXPECT_EQ(3, val1);
}