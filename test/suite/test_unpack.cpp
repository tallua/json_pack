#include <gtest/gtest.h>

#include "jpack_impl.hpp"

using namespace jpack_test;
using namespace jpack::schema;
using namespace jpack::serialization;

TEST(JPackTest, unpack_simple) {}

TEST(JPackTest, unpack_object) {}

TEST(JPackTest, unpack_array) {}

TEST(JPackTest, unpack_nested_object) {}

TEST(JPackTest, unpack_nested_array) {}
TEST(JPackTest, unpack_object) {}

TEST(JPackTest, unpack_copy_format) {
  auto json = nlohmann::json({{"foo", {{"bar", 2}}}});

  int val;

  const auto foo_format = Formats(Must("bar", Ref(val)));
  const auto root_format = Formats(Must("foo", foo_format));
  JsonView(json) >> root_format;

  EXPECT_EQ(2, val);

  // foo format must be copied here.
  EXPECT_NE(static_cast<const void*>(&root_format.arg0),
            static_cast<const void*>(&foo_format));
}