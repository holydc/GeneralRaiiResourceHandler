#pragma once

#include <utility>

// Forward declaration
namespace raii {
template<class THandle, class TCloser> class Handle;
}

// Forward declaration
template<class THandle, class TCloser>
void swap(raii::Handle<THandle, TCloser> &lhs, raii::Handle<THandle, TCloser> &rhs) noexcept;

namespace raii {
template<class THandle, class TCloser>
class Handle {
public:
  friend void ::swap(Handle &lhs, Handle &rhs) noexcept;

  Handle() = default;
  Handle(THandle const &handle, TCloser closer);

  template<class TOpener, class... TArgs>
  Handle(TOpener opener, TCloser closer, TArgs &&... args);

  Handle(Handle const &) = delete;
  Handle &operator=(Handle const &) = delete;

  Handle(Handle &&other) noexcept;
  Handle &operator=(Handle &&other) noexcept;

  ~Handle();

  operator THandle() const;

  bool operator==(THandle const &other) const;
  bool operator!=(THandle const &other) const;
  operator bool() const;

private:
  THandle handle_{};
  TCloser closer_{};
}; // class Handle

template<class TOpener, class TCloser, class... TArgs>
Handle(TOpener opener, TCloser closer, TArgs &&... args) -> Handle<decltype(opener(std::forward<TArgs>(args)...)), TCloser>;

template<class THandle, class TCloser>
Handle<THandle, TCloser>::Handle(THandle const &handle, TCloser closer)
    : handle_{handle}, closer_{closer} {
}

template<class THandle, class TCloser>
template<class TOpener, class... TArgs>
Handle<THandle, TCloser>::Handle(TOpener opener, TCloser closer, TArgs &&... args)
    : handle_{opener(std::forward<TArgs>(args)...)}, closer_{closer} {
}

template<class THandle, class TCloser>
Handle<THandle, TCloser>::Handle(Handle &&other) noexcept {
  ::swap(*this, other);
}

template<class THandle, class TCloser>
auto Handle<THandle, TCloser>::operator=(Handle &&other) noexcept -> Handle & {
  ::swap(*this, other);
  return *this;
}

template<class THandle, class TCloser>
Handle<THandle, TCloser>::~Handle() {
  if (handle_) {
    closer_(handle_);
  }
}

template<class THandle, class TCloser>
Handle<THandle, TCloser>::operator THandle() const {
  return handle_;
}

template<class THandle, class TCloser>
bool Handle<THandle, TCloser>::operator==(THandle const &other) const {
  return handle_ == other;
}

template<class THandle, class TCloser>
bool Handle<THandle, TCloser>::operator!=(THandle const &other) const {
  return handle_ != other;
}

template<class THandle, class TCloser>
Handle<THandle, TCloser>::operator bool() const {
  return handle_ != nullptr;
}
} // namespace raii

template<class THandle, class TCloser>
void swap(raii::Handle<THandle, TCloser> &lhs, raii::Handle<THandle, TCloser> &rhs) noexcept {
  std::swap(lhs.handle_, rhs.handle_);
  std::swap(lhs.closer_, rhs.closer_);
}
