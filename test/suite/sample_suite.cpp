#include <gtest/gtest.h>

#include "jpack_impl.hpp"

using namespace jpack_test;
using namespace jpack::schema;
using namespace jpack::serialization;

TEST(JPackTest, unpack_example)
{
  nlohmann::json json = {
    { "val1", 3 },
    { "myobj", {
      { "val2", 2 }
    }}
  };

  JsonView view(json);

  int value1;
  int value2;
  int value3 = 8;

  auto schema = Formats(
    Must("val1", Ref(value1)),
    Must("myobj", Formats(
      Should("val2", Ref(value2))
    )),
    Might("val3", Ref(value3))
  );

  view >> schema;

  EXPECT_EQ(3, value1);
  EXPECT_EQ(2, value2);
  EXPECT_EQ(8, value3);
}