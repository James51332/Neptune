#pragma once

#include <iostream>
#include <utility>
#include <type_traits>

namespace Neptune
{

// ----- Helper -----------

namespace Helper
{

template <typename...>
using Void = void; // Used for SFINAE

} // namespace Helper

// ----- DeclVal ----------

namespace Helper
{

template <typename T, typename Up = T&&>
Up DeclValHelper(int);

template <typename T>
T DeclValHelper(long);

} // namespace Helper

template <typename T>
auto DeclVal() noexcept -> decltype(Helper::DeclValHelper<T>(0)); // Use T&& unless T is void

// ----- Conditional ------

template <bool, typename T, typename F>
struct Conditional {};

template <typename T, typename F>
struct Conditional <true, T, F> { using Value = T; };

template <typename T, typename F>
struct Conditional <false, T, F> { using Value = F; };

// ----- RemoveReference ------

template <typename T>
struct RemoveReference { using Value = T; };

template <typename T>
struct RemoveReference <T&> { using Value = T; };

template <typename T>
struct RemoveReference <T&&> { using Value = T; };

// ----- Constructable --------

namespace Helper
{

template <typename, typename T, typename... Args>
struct ConstructableHelper { static constexpr bool Value = false; };

template <typename T, typename... Args>
struct ConstructableHelper <Helper::Void<decltype(new T(DeclVal<Args>()...))>, T, Args...> { static constexpr bool Value = true; };

} // namespace Helper

template <typename T, typename... Args>
using Constructable = Helper::ConstructableHelper<Helper::Void<>, T, Args...>;

template <typename T, typename... Args>
struct NoThrowConstructable { static constexpr bool Value = Constructable<T, Args...>::Value && noexcept(T(DeclVal<Args>()...)); };

template <typename T, typename Args>
struct NoThrowConstructable <T, Args> { static constexpr bool Value = Constructable<T, Args>::Value && noexcept(T(DeclVal<Args>())); };

template <typename T>
struct NoThrowMoveConstructable
{
  static const bool Value = NoThrowConstructable<T, T&&>::Value;
};

// ----- Move -----------------

template <typename T>
inline typename RemoveReference<T>::Value&& Move(T&& val) noexcept
{
  return static_cast<typename RemoveReference<T>::Value&&>(val);
}

template <typename T>
inline typename Neptune::Conditional<NoThrowMoveConstructable<T>::Value, typename RemoveReference<T>::Value&&, const T&>::Value MoveIfNoexcept(T& val)
{
  return Move(val);
}

// ----- Swap -----------------

template <typename T>
inline void Swap(T& a, T& b) // TODO: Noexcept if nothrow move constructable and assignable
{
  T tmp(Move(a));
  a = Move(b);
  b = Move(tmp);
}

} // namespace Neptune

// ----- Assert ---------------

// TEMPORARY
#define NEPTUNE_ASSERT(x, msg) if (!(x)) { }

