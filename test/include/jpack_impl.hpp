#include <jpack/jpack.hpp>
#include <nlohmann/json.hpp>

namespace jpack_test {

struct JsonView {
  using size_type = nlohmann::json::size_type;
  using key_type = nlohmann::json::object_t::key_type;

 public:
  JsonView(nlohmann::json& json) : json(json) {}

  bool has(size_type index) { return json.is_array() && (index < json.size()); }
  bool has(key_type key) {
    return json.is_object() && (json.find(key) != json.end());
  }

  JsonView operator[](size_type index) { return JsonView(json[index]); }
  JsonView operator[](key_type key) { return JsonView(json[key]); }

  template <typename _Tp>
  _Tp as();

 private:
  nlohmann::json& json;
};

template <>
int JsonView::as<int>() {
  return json.get<int>();
}

}  // namespace jpack_test

namespace jpack {
namespace serialization {

jpack_test::JsonView& operator>>(jpack_test::JsonView& archive, int& val) {
  val = archive.as<int>();
  return archive;
}

}  // namespace serialization
}  // namespace jpack