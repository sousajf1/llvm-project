#ifndef ARRAY_H
#define ARRAY_H

namespace std {
using size_t = unsigned long long;
using ptrdiff_t = long long;

template <typename T>
typename T::iterator begin(T& t);
template <typename T>
typename T::iterator end(T& t);

template <typename T>
typename T::iterator rbegin(T& t);
template <typename T>
typename T::iterator rend(T& t);

template <typename T>
typename T::const_iterator cbegin(T& t);
template <typename T>
typename T::const_iterator cend(T& t);

template <typename T>
typename T::const_iterator crbegin(T& t);
template <typename T>
typename T::const_iterator crend(T& t);

template <typename T, size_t N>
struct array {
  using iterator = T *;
  using const_iterator = const T *;

  array() = default;
  iterator begin() noexcept;
  iterator end() noexcept;
  const iterator cbegin() const noexcept;
  const iterator cend() const noexcept;
  iterator rbegin() noexcept;
  iterator rend() noexcept;
  const iterator crbegin() const noexcept;
  const iterator crend() const noexcept;
};
} // namespace std

#endif /* end of include guard: ARRAY_H */
