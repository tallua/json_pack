#include <gtest/gtest.h>

#include <string>

#include "jpack_impl.hpp"

using namespace jpack_test;
using namespace jpack::schema;
using namespace jpack::serialization;

TEST(JPackTest, unpack_simple) {
  auto int_json = nlohmann::json(3);
  auto uint64_json = nlohmann::json(30000000000ull);
  auto int64_json = nlohmann::json(100000ll);
  auto double_json = nlohmann::json(35.132);
  auto bool_json = nlohmann::json(true);
  auto string_json = nlohmann::json("foo");

  int int_val;
  uint64_t uint64_val;
  int64_t int64_val;
  double double_val;
  bool bool_val;
  std::string string_val;

  JsonView(int_json) >> int_val;
  JsonView(uint64_json) >> uint64_val;
  JsonView(int64_json) >> int64_val;
  JsonView(double_json) >> double_val;
  JsonView(bool_json) >> bool_val;
  JsonView(string_json) >> string_val;

  EXPECT_EQ(3, int_val);
  EXPECT_EQ(30000000000ull, uint64_val);
  EXPECT_EQ(100000ll, int64_val);
  EXPECT_EQ(35.132, double_val);
  EXPECT_EQ(true, bool_val);
  EXPECT_EQ("foo", string_val);
}

TEST(JPackTest, unpack_object) {
  auto json = nlohmann::json({{"int_val", -5},
                              {"uint64_val", 300ull},
                              {"int64_val", 12000ll},
                              {"double_val", 17.3},
                              {"bool_val", false},
                              {"string_val", "bar"}});

  int int_val;
  uint64_t uint64_val;
  int64_t int64_val;
  double double_val;
  bool bool_val;
  std::string string_val;

  const auto format = Formats(
      Must("int_val", Ref(int_val)), Must("uint64_val", Ref(uint64_val)),
      Must("int64_val", Ref(int64_val)), Must("double_val", Ref(double_val)),
      Must("bool_val", Ref(bool_val)), Must("string_val", Ref(string_val)));

  JsonView(json) >> format;

  EXPECT_EQ(-5, int_val);
  EXPECT_EQ(300ull, uint64_val);
  EXPECT_EQ(12000ll, int64_val);
  EXPECT_EQ(17.3, double_val);
  EXPECT_EQ(false, bool_val);
  EXPECT_EQ("bar", string_val);
}

TEST(JPackTest, unpack_array) {
  auto json = nlohmann::json::array({12, 39ull, -123, 98.0, false, "dummy"});

  int int_val;
  uint64_t uint64_val;
  int64_t int64_val;
  double double_val;
  bool bool_val;
  std::string string_val;

  const auto format = Formats(Must(0, Ref(int_val)), Must(1, Ref(uint64_val)),
                              Must(2, Ref(int64_val)), Must(3, Ref(double_val)),
                              Must(4, Ref(bool_val)), Must(5, Ref(string_val)));

  JsonView(json) >> format;

  EXPECT_EQ(12, int_val);
  EXPECT_EQ(39ull, uint64_val);
  EXPECT_EQ(-123, int64_val);
  EXPECT_EQ(98.0, double_val);
  EXPECT_EQ(false, bool_val);
  EXPECT_EQ("dummy", string_val);
}

TEST(JPackTest, unpack_nested_object) {}

TEST(JPackTest, unpack_nested_array) {}

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