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
  auto schema = Format(
    Must("foo", Ref(val1)),
    Must("bar", Ref(val2)),
    Must("baz", Ref(val3))
  );

  view >> schema;

  EXPECT_EQ(3, val1);
  EXPECT_EQ(4, val2);
  EXPECT_EQ(5, val3);
}