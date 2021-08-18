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

// ----- CreateScope ----------

template <typename T, typename... Args>
Scope<T> CreateScope(Args&&... args)
{
  return Scope<T>(new T(args...));
}

// ----- Ref ------------------

template <typename T>
class Ref
{
  struct Counter
  {
  public:
    void Increment()
    {
      m_References++;
    }
    
    void Decrement()
    {
      m_References--;
    }
    
    UInt32& References()
    {
      return m_References;
    }
    
  private:
    UInt32 m_References = 1;
  };
  
public:
  using Type = T;
  
public:
  Ref() noexcept;
  Ref(T* pointer) noexcept;
  
  Ref(const Ref& other) noexcept;
  Ref(Ref&& other) = delete; // Move ctor is absolutely useless
  
  Ref& operator=(Ref other) noexcept;
  
  ~Ref() noexcept;
  
  T& operator*() noexcept;
  const T& operator*() const noexcept;
  T* operator->() noexcept;
  const T* operator->() const noexcept;
  
  T* Raw() noexcept;
  
private:
  T* m_Pointer;
  Counter* m_Counter;
};

template <typename T>
Ref<T>::Ref() noexcept
: m_Pointer(nullptr), m_Counter(nullptr)
{
}

template <typename T>
Ref<T>::Ref(T* pointer) noexcept
: m_Pointer(pointer), m_Counter(new Counter())
{
}

template <typename T>
Ref<T>::Ref(const Ref<T>& other) noexcept
: m_Pointer(other.m_Pointer), m_Counter(other.m_Counter)
{
  m_Counter->Increment();
}

template <typename T>
Ref<T>& Ref<T>::operator=(Ref<T> other) noexcept
{
  Neptune::Swap(m_Pointer, other.m_Pointer);
  Neptune::Swap(m_Counter, other.m_Counter);
  return (*this);
}

template <typename T>
Ref<T>::~Ref() noexcept
{
  if (m_Counter)
  {
    m_Counter->Decrement();
    if (m_Counter->References() == 0)
    {
      delete m_Pointer;
      delete m_Counter;
    }
  }
}

template <typename T>
T& Ref<T>::operator*() noexcept
{
  return *m_Pointer;
}

template <typename T>
const T& Ref<T>::operator*() const noexcept
{
  return *m_Pointer;
}

template <typename T>
T* Ref<T>::operator->() noexcept
{
  return m_Pointer;
}

template <typename T>
const T* Ref<T>::operator->() const noexcept
{
  return m_Pointer;
}

template <typename T>
T* Ref<T>::Raw() noexcept
{
  return m_Pointer;
}

// ----- CreateRef ------------

template <typename T, typename... Args>
Ref<T> CreateRef(Args&&... args)
{
  return Ref<T>(new T(args...));
}

} // namespace Neptune

