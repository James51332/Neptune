#pragma once

#include "utils/Utils.h"

namespace Neptune
{

// ----- Scope ----------------

template <typename T>
class Scope
{
public:
  using Type = T;
  
public:
  Scope() noexcept;
  Scope(T* pointer) noexcept;
  template <typename... Args>
  Scope(Args&&... args);
  
  Scope(const Scope& other) = delete;
  Scope(Scope&& other) noexcept;
  
  Scope& operator=(const Scope& other) = delete;
  Scope& operator=(Scope&& other) noexcept;
  
  ~Scope() noexcept;
  
  T& operator*() noexcept;
  const T& operator*() const noexcept;
  T* operator->() noexcept;
  const T* operator->() const noexcept;
  
  T* Raw() noexcept;
  
private:
  T* m_Pointer;
};

template <typename T>
Scope<T>::Scope() noexcept
: m_Pointer(nullptr)
{
}

template <typename T>
Scope<T>::Scope(T* pointer) noexcept
: m_Pointer(pointer)
{
}

template <typename T>
template <typename... Args>
Scope<T>::Scope(Args&&... args)
: m_Pointer(new T(args...))
{
}

template <typename T>
Scope<T>::Scope(Scope<T>&& other) noexcept
: m_Pointer(nullptr)
{
  Neptune::Swap(m_Pointer, other.m_Pointer);
}

template <typename T>
Scope<T>& Scope<T>::operator=(Scope<T>&& other) noexcept
{
  Neptune::Swap(m_Pointer, other.m_Pointer);
  return (*this);
}

template <typename T>
Scope<T>::~Scope() noexcept
{
  if (m_Pointer)
  {
    delete m_Pointer;
  }
}

template <typename T>
T& Scope<T>::operator*() noexcept
{
  return *m_Pointer;
}

template <typename T>
const T& Scope<T>::operator*() const noexcept
{
  return *m_Pointer;
}

template <typename T>
T* Scope<T>::operator->() noexcept
{
  return m_Pointer;
}

template <typename T>
const T* Scope<T>::operator->() const noexcept
{
  return m_Pointer;
}

template <typename T>
T* Scope<T>::Raw() noexcept
{
  return m_Pointer;
}

// ----- Ref ------------------

} // namespace Neptune

