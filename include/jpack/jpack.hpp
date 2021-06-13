#pragma once

#include <utility>

namespace jpack {

namespace serialization {
class JpackSerializationException {};
}  // namespace serialization

namespace schema {
namespace detail {

template <typename... _Args>
struct schema_list;

template <>
struct schema_list<> {
  schema_list() {}

  template <typename _JsonArchive>
  void _unpack(_JsonArchive& archive) const {}
};

template <typename _Arg0, typename... _Args>
struct schema_list<_Arg0, _Args...> : private schema_list<_Args...> {
  schema_list<_Arg0, _Args...>(_Arg0&& arg0, _Args&&... args)
      : schema_list<_Args...>(std::forward<_Args>(args)...),
        arg0(std::move(arg0)) {}

  template <typename _JsonArchive>
  void _unpack(const _JsonArchive& archive) const {
    using namespace jpack::serialization;
    archive >> arg0;
    schema_list<_Args...>::_unpack(archive);
  }

  _Arg0 arg0;
};

template <typename _Ref>
struct schema_ref {
  _Ref& ref;
};

template <typename _Access, typename _ArgType>
struct schema_must {
  _Access access;
  _ArgType arg;
};

template <typename _Access, typename _ArgType>
struct schema_should {
  _Access access;
  _ArgType arg;
};

template <typename _Access, typename _ArgType>
struct schema_might {
  _Access access;
  _ArgType arg;
};

}  // namespace detail

template <typename... _Args>
auto Formats(_Args&&... args) {
  return detail::schema_list<_Args...>(std::forward<_Args>(args)...);
}

template <typename _Ref>
auto Ref(_Ref& ref) {
  return detail::schema_ref<_Ref>{ref};
}

template <typename _Access, typename _Target>
auto Must(_Access key, _Target&& target) {
  return detail::schema_must<_Access, _Target>{key, std::move(target)};
}

template <typename _Access, typename _Target>
auto Should(_Access key, _Target&& target) {
  return detail::schema_should<_Access, _Target>{key, std::move(target)};
}

template <typename _Access, typename _Target>
auto Might(_Access key, _Target&& target) {
  return detail::schema_might<_Access, _Target>{key, std::move(target)};
}

}  // namespace schema
}  // namespace jpack

namespace jpack {
namespace serialization {

template <typename _JsonArchive, typename... _Args>
static const _JsonArchive& operator>>(
    const _JsonArchive& archive,
    const jpack::schema::detail::schema_list<_Args...>& schema) {
  schema._unpack(archive);
  return archive;
}

template <typename _JsonArchive, typename _Ref>
static const _JsonArchive& operator>>(
    const _JsonArchive& archive,
    const jpack::schema::detail::schema_ref<_Ref>& schema) {
  archive >> schema.ref;
  return archive;
}

template <typename _JsonArchive, typename _Access, typename _Target>
static const _JsonArchive& operator>>(
    const _JsonArchive& archive,
    const jpack::schema::detail::schema_must<_Access, _Target>& schema) {
  archive[schema.access] >> schema.arg;
  return archive;
}

template <typename _JsonArchive, typename _Access, typename _Target>
static const _JsonArchive& operator>>(
    const _JsonArchive& archive,
    const jpack::schema::detail::schema_should<_Access, _Target>& schema) {
  if (!archive.has(schema.access)) {
    throw JpackSerializationException();
  }
  archive[schema.access] >> schema.arg;
  return archive;
}

template <typename _JsonArchive, typename _Access, typename _Target>
static const _JsonArchive& operator>>(
    const _JsonArchive& archive,
    const jpack::schema::detail::schema_might<_Access, _Target>& schema) {
  if (archive.has(schema.access)) {
    archive[schema.access] >> schema.arg;
  }
  return archive;
}

}  // namespace serialization
}  // namespace jpack