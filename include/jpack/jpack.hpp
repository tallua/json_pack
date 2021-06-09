#pragma once

#include <utility>

namespace jpack {
namespace serialization {}

namespace schema {
namespace detail {

template <typename... _Args>
struct JsonSchemas;

template <typename _Arg0>
struct JsonSchemas<_Arg0> {
  JsonSchemas<_Arg0>(_Arg0&& arg0) : arg0(arg0) {}

  template <typename _JsonArchive>
  void _unpack(_JsonArchive& archive) {
    using namespace jpack::serialization;
    archive >> arg0;
  }

  _Arg0 arg0;
};

template <typename _Arg0, typename... _Args>
struct JsonSchemas<_Arg0, _Args...> : public JsonSchemas<_Args...> {
  JsonSchemas<_Arg0, _Args...>(_Arg0&& arg0, _Args&&... args)
      : JsonSchemas<_Args...>(std::forward<_Args>(args)...),
        arg0(std::move(arg0)) {}

  template <typename _JsonArchive>
  void _unpack(_JsonArchive& archive) {
    using namespace jpack::serialization;
    archive >> arg0;
    JsonSchemas<_Args...>::_unpack(archive);
  }

  _Arg0 arg0;
};

template <typename _Access, typename _ArgType>
struct JsonSchemaMust {
  _Access access;
  _ArgType arg;
};

}  // namespace detail

template <typename... _Args>
auto Schema(_Args&&... args) {
  return detail::JsonSchemas<_Args...>(std::forward<_Args>(args)...);
}

template <typename _Access, typename _Ref>
auto Must(_Access key, _Ref& ref) {
  return detail::JsonSchemaMust<_Access, _Ref&>{key, ref};
}

template <typename _Access, typename _RVal>
auto Must(_Access key, _RVal&& ref) {
  return detail::JsonSchemaMust<_Access, _RVal>{key, std::move(ref)};
}


}  // namespace schema
}  // namespace jpack

namespace jpack {
namespace serialization {

template <typename _JsonArchive, typename... _Args>
_JsonArchive& operator>>(_JsonArchive& archive,
                         jpack::schema::detail::JsonSchemas<_Args...>& schema) {
  schema._unpack(archive);
  return archive;
}

template <typename _JsonArchive, typename _Access, typename _ArgType>
_JsonArchive& operator>>(
    _JsonArchive& archive,
    jpack::schema::detail::JsonSchemaMust<_Access, _ArgType>& schema) {
  archive[schema.access] >> schema.arg;
  return archive;
}

}  // namespace serialization
}  // namespace jpack