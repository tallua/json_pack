#include <jpack/jpack.hpp>
#include <nlohmann/json.hpp>

namespace jpack_test {

struct JsonView {
  using size_type = nlohmann::json::size_type;
  using key_type = nlohmann::json::object_t::key_type;

 public:
  JsonView(nlohmann::json& json) : json(json) {}

  bool has(size_type index) const { return json.is_array() && (index < json.size()); }
  bool has(const key_type& key) const {
    return json.is_object() && (json.find(key) != json.end());
  }

  JsonView operator[](size_type index) const { return JsonView(json[index]); }
  JsonView operator[](const key_type& key) const { return JsonView(json[key]); }

  nlohmann::json& json;
};

}  // namespace jpack_test

namespace jpack {
namespace serialization {

const jpack_test::JsonView& operator>>(const jpack_test::JsonView& archive, int& val) {
  val = archive.json.get<int>();
  return archive;
}

}  // namespace serialization
}  // namespace jpack