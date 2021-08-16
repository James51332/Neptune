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
  DynamicArray(UInt32 size);
  DynamicArray(const DynamicArray& other);
  DynamicArray(DynamicArray&& other) noexcept;
  
  DynamicArray& operator=(DynamicArray other) noexcept;
  void operator=(DynamicArray&& other) noexcept;
  
  ~DynamicArray() noexcept;
  
  T& operator[](UInt32 index) noexcept
  {
    return m_Array[index];
  }
  
  const T& operator[](UInt32 index) const noexcept
  {
    return m_Array[index];
  }
  
  bool Empty() const noexcept;
  UInt32 Size() const noexcept;
  UInt32 Capacity() const noexcept;
  void Clear() noexcept;
  
  void Reserve(UInt32 size);
  void Shrink(UInt32 size) noexcept;
  
  void PushBack(const T& val);
  void PushBack(T&& val);
  
  template <typename... Args>
  void EmplaceBack(Args&&... args);
  
  void PopBack() noexcept;
  
private:
  UInt32 NextSize() const noexcept;
  
private:
  UInt32 m_Size;
  UInt32 m_Capacity;
  T* m_Array;
};

template <typename T>
DynamicArray<T>::DynamicArray() noexcept
: m_Size(0), m_Capacity(0), m_Array(nullptr)
{
}

template <typename T>
DynamicArray<T>::DynamicArray(UInt32 size)
: m_Size(0), m_Capacity(size), m_Array(nullptr)
{
  NEPTUNE_ASSERT(size >= 0, "Cannot create DynamicArray with negative size!");
  m_Array = (T*) ::operator new(m_Capacity * sizeof(T));
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
: m_Size(other.m_Size), m_Capacity(other.m_Capacity), m_Array(nullptr)
{
  m_Array = (T*) ::operator new(m_Capacity * sizeof(T));
  
  for (int i = 0; i < m_Size; i++)
    m_Array[i] = Move(other[i]);
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) noexcept
{
  Swap(m_Size, other.m_Size);
  Swap(m_Capacity, other.m_Capacity);
  Swap(m_Array, other.m_Array);
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray other) noexcept
{
  Swap(m_Size, other.m_Size);
  Swap(m_Capacity, other.m_Capacity);
  Swap(m_Array, other.m_Array);
}

template <typename T>
void DynamicArray<T>::operator=(DynamicArray<T>&& other) noexcept
{
  Swap(m_Size, other.m_Size);
  Swap(m_Capacity, other.m_Capacity);
  Swap(m_Array, other.m_Array);
}

template <typename T>
DynamicArray<T>::~DynamicArray() noexcept
{
  Clear();
}

template <typename T>
bool DynamicArray<T>::Empty() const noexcept
{
  return m_Size = 0;
}

template <typename T>
UInt32 DynamicArray<T>::Size() const noexcept
{
  return m_Size;
}

template <typename T>
UInt32 DynamicArray<T>::Capacity() const noexcept
{
  return m_Capacity;
}

template <typename T>
void DynamicArray<T>::Clear() noexcept
{
  for (int i = 0; i < m_Size; i++)
    m_Array[i].~T();
  
  ::operator delete(m_Array, m_Capacity * sizeof(T));
  m_Capacity = 0;
  m_Array = nullptr;
}

template <typename T>
void DynamicArray<T>::Reserve(UInt32 size)
{
  if (size <= m_Capacity)
    return;
  
  T* tmp = (T*) ::operator new(size * sizeof(T));
  
  for (int i = 0; i < m_Size; i++)
    new (&tmp[i]) T(MoveIfNoexcept(m_Array[i]));
  
  for (int i = 0; i < m_Size; i++)
    m_Array[i].~T();
  
  ::operator delete(m_Array, m_Capacity * sizeof(T));
  
  m_Array = tmp;
  m_Capacity = size;
}

template <typename T>
void DynamicArray<T>::Shrink(UInt32 size) noexcept
{
  if (size >= m_Size)
    return;
  
  for (int i = size; i < m_Size; i++)
    m_Array[i].~T();
  
  m_Size = size;
}

template <typename T>
void DynamicArray<T>::PushBack(const T& val)
{
  if (m_Size == m_Capacity)
    Reserve(NextSize());
  
  new (&m_Array[m_Size]) T(val);
  m_Size++;
}

template <typename T>
void DynamicArray<T>::PushBack(T&& val)
{
  if (m_Size == m_Capacity)
    Reserve(NextSize());
  
  new (&m_Array[m_Size]) T(MoveIfNoexcept(val));
  m_Size++;
}

template <typename T>
template <typename... Args>
void DynamicArray<T>::EmplaceBack(Args&&... args)
{
  if (m_Size == m_Capacity)
    Reserve(NextSize());
  
  new (&m_Array[m_Size]) T(args...);
  m_Size++;
}

template <typename T>
void DynamicArray<T>::PopBack() noexcept
{
  m_Array[m_Size - 1].~T();
  m_Size--;
}

template <typename T>
UInt32 DynamicArray<T>::NextSize() const noexcept
{
  return m_Size > 0 ? (m_Size + m_Size) : 8;
}

} // namespace Neptune
