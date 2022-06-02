#pragma once

#include "utils/Types.h"
#include "utils/Utils.h"

#include <new>

namespace Neptune
{

// ----- DynamicArray ---------

template <typename T>
class DynamicArray
{
public:
  using Type = T;
  using Iterator = T*;
  using ConstIterator = const T*;
  
public:
  
  DynamicArray() noexcept;
  DynamicArray(Neptune::Size size);
  DynamicArray(const DynamicArray& other);
  DynamicArray(DynamicArray&& other) noexcept;
  
  DynamicArray& operator=(DynamicArray other) noexcept;
  
  ~DynamicArray() noexcept;
  
  T& operator[](Neptune::Size index) noexcept;
  const T& operator[](Neptune::Size index) const noexcept;
  
  bool Empty() const noexcept;
  Neptune::Size Size() const noexcept;
  Neptune::Size Capacity() const noexcept;
  void Clear() noexcept;
  
  void Reserve(Neptune::Size size);
  void Shrink(Neptune::Size size) noexcept;
  
  void PushBack(const T& val);
  void PushBack(T&& val);
  
  template <typename... Args>
  void EmplaceBack(Args&&... args);
  
  void PopBack() noexcept;
  
  Iterator begin() noexcept { return m_Array; }
  Iterator end() noexcept { return m_Array + m_Size; }
  Iterator rbegin() noexcept { return m_Array + m_Size; }
  Iterator rend() noexcept { return m_Array; }
  ConstIterator begin() const noexcept { return m_Array; }
  ConstIterator end() const noexcept { return m_Array + m_Size; }
  ConstIterator rbegin() const noexcept { return m_Array + m_Size; }
  ConstIterator rend() const noexcept { return m_Array; }
  
private:
  Neptune::Size NextSize() const noexcept;
  
private:
  Neptune::Size m_Size;
  Neptune::Size m_Capacity;
  T* m_Array;
};

template <typename T>
DynamicArray<T>::DynamicArray() noexcept
: m_Size(0), m_Capacity(0), m_Array(nullptr)
{
}

template <typename T>
DynamicArray<T>::DynamicArray(Neptune::Size size)
: m_Size(size), m_Capacity(size), m_Array(nullptr)
{
  if (size == 0) return;
  m_Array = (T*) ::operator new(m_Capacity * sizeof(T));
  
  for (Neptune::Size i = 0; i < size; i++)
    ::new (m_Array + i) T();
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
: m_Size(other.m_Size), m_Capacity(other.m_Capacity), m_Array(nullptr)
{
  m_Array = (T*) ::operator new(m_Capacity * sizeof(T));
  
  for (Neptune::Size i = 0; i < m_Size; i++)
  {
    try
    {
      ::new(&m_Array[i]) T(other[i]);
    } catch (...)
    {
      throw;
    }
  }
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) noexcept
{
  Swap(m_Size, other.m_Size);
  Swap(m_Capacity, other.m_Capacity);
  Swap(m_Array, other.m_Array);
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T> other) noexcept
{
  Swap(m_Size, other.m_Size);
  Swap(m_Capacity, other.m_Capacity);
  Swap(m_Array, other.m_Array);
  
  return (*this);
}

template <typename T>
DynamicArray<T>::~DynamicArray() noexcept
{
  Clear();
}

template <typename T>
T& DynamicArray<T>::operator[](Neptune::Size index) noexcept
{
  // TODO: Debug asserts
  return m_Array[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](Neptune::Size index) const noexcept
{
  // TODO: Debug asserts
  return m_Array[index];
}

template <typename T>
bool DynamicArray<T>::Empty() const noexcept
{
  return m_Size == 0;
}

template <typename T>
Neptune::Size DynamicArray<T>::Size() const noexcept
{
  return m_Size;
}

template <typename T>
Neptune::Size DynamicArray<T>::Capacity() const noexcept
{
  return m_Capacity;
}

template <typename T>
void DynamicArray<T>::Clear() noexcept
{
  if (m_Capacity == 0)
    return;
  
  for (Neptune::Size i = m_Size; i > 0; i--)
    m_Array[i - 1].~T();
  
  ::operator delete(m_Array, m_Capacity * sizeof(T));
  m_Capacity = 0;
  m_Array = nullptr;
}

template <typename T>
void DynamicArray<T>::Reserve(Neptune::Size size)
{
  if (size <= m_Capacity)
    return;
  
  T* tmp = (T*) ::operator new(size * sizeof(T));
  
  for (Neptune::Size i = 0; i < m_Size; i++)
  {
    try
    {
      new (&tmp[i]) T(MoveIfNoexcept(m_Array[i]));
    } catch (...)
    {
      for (Neptune::Size j = i; j > 0; j--)
        tmp[j - 1].~T();
      ::operator delete(tmp, size * sizeof(T));
      
      throw;
    }
  }
  
  for (Neptune::Size i = 0; i < m_Size; i++)
    m_Array[i].~T();
  
  ::operator delete(m_Array, m_Capacity * sizeof(T));
  
  m_Array = tmp;
  m_Capacity = size;
}

template <typename T>
void DynamicArray<T>::Shrink(Neptune::Size size) noexcept
{
  if (size >= m_Size)
    return;
  
  for (Neptune::Size i = size; i < m_Size; i++)
    m_Array[i].~T();
  
  m_Size = size;
}

template <typename T>
void DynamicArray<T>::PushBack(const T& val)
{
  EmplaceBack(val);
}

template <typename T>
void DynamicArray<T>::PushBack(T&& val)
{
  EmplaceBack(MoveIfNoexcept(val));
}

template <typename T>
template <typename... Args>
void DynamicArray<T>::EmplaceBack(Args&&... args)
{
  if (m_Size == m_Capacity)
    Reserve(NextSize());
  
  new (&m_Array[m_Size]) T(Forward<Args>(args)...);
  m_Size++;
}

template <typename T>
void DynamicArray<T>::PopBack() noexcept
{
  // TODO: Debug assert
  
  m_Array[m_Size - 1].~T();
  m_Size--;
}

template <typename T>
Neptune::Size DynamicArray<T>::NextSize() const noexcept
{
  return m_Size > 0 ? (m_Size + m_Size) : 1;
}

} // namespace Neptune
