#pragma once

#include "utils/Utils.h"

namespace Neptune
{

// ----- Scope ----------------

template <typename T>
class Scope
{
  template <typename U>
  friend class Scope;
  
public:
  using Type = T;
  
public:
  Scope(T* pointer = nullptr) noexcept;
  
  Scope(const Scope& other) = delete;
  Scope(Scope&& other) noexcept;
  
  template <typename U>
  Scope(Scope<U>&& other) noexcept;
  
  Scope& operator=(const Scope& other) = delete;
  Scope& operator=(Scope&& other) noexcept;
  
  ~Scope() noexcept;
  
  T& operator*() noexcept;
  const T& operator*() const noexcept;
  T* operator->() noexcept;
  const T* operator->() const noexcept;
  
  operator bool() const noexcept { return m_Pointer; }
  
  T* Raw() noexcept;
  const T* Raw() const noexcept;
  
  T* Release() noexcept;
  
private:
  T* m_Pointer;
};

template <typename T>
Scope<T>::Scope(T* pointer) noexcept
: m_Pointer(pointer)
{
}

template <typename T>
Scope<T>::Scope(Scope<T>&& other) noexcept
: m_Pointer(other.Release())
{
}

template <typename T>
template <typename U>
Scope<T>::Scope(Scope<U>&& other) noexcept
: m_Pointer(other.Release())
{
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

template <typename T>
const T* Scope<T>::Raw() const noexcept
{
  return m_Pointer;
}

template <typename T>
T* Scope<T>::Release() noexcept
{
  T* tmp = nullptr;
  Neptune::Swap(m_Pointer, tmp);
  return tmp;
}

// ----- CreateScope ----------

template <typename T, typename... Args>
Scope<T> CreateScope(Args&&... args)
{
  return Scope<T>(new T(Forward<Args>(args)...));
}

// ----- Ref ------------------

namespace Helper
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
	
  Size References()
  {
    return m_References;
  }
	
private:
	Atomic<Size> m_References = 1;
};

} // namespace Helper

template <typename T>
class Ref
{
  template <typename U>
  friend class Ref;

public:
  using Type = T;
  
public:
  Ref(T* pointer = nullptr) noexcept;
  
  Ref(const Ref& other) noexcept;
  
  // No need to create a Move ctor because the copy constructor is only copying a pointer
  // Ref(Ref&& other);
  
  template <typename U>
  Ref(const Ref<U>& other) noexcept;
  
  Ref& operator=(Ref other) noexcept;

  ~Ref() noexcept;
  
  T& operator*() noexcept;
  const T& operator*() const noexcept;
  T* operator->() noexcept;
  const T* operator->() const noexcept;
  
  operator bool() const noexcept { return m_Pointer; }
  
  void Reset(T* ptr);
  void Reset() noexcept;
  
  T* Raw() noexcept;
  const T* Raw() const noexcept;
  
  template <typename U, typename Y>
  friend Ref<U> StaticRefCast(const Ref<Y>& other);
  
private:
  template <typename U>
  Ref(U* pointer, Helper::Counter* counter)
  	: m_Pointer(pointer), m_Counter(counter)
  {
    if (m_Counter)
      m_Counter->Increment();
  }
  
  T* m_Pointer;
  Helper::Counter* m_Counter;
};

template <typename T>
Ref<T>::Ref(T* pointer) noexcept
: m_Pointer(pointer), m_Counter(pointer ? new Helper::Counter() : nullptr)
{
}

template <typename T>
Ref<T>::Ref(const Ref<T>& other) noexcept
: m_Pointer(other.m_Pointer), m_Counter(other.m_Counter)
{
  if (m_Counter)
  	m_Counter->Increment();
}

template <typename T>
template <typename U>
Ref<T>::Ref(const Ref<U>& other) noexcept
	: m_Pointer(other.m_Pointer), m_Counter(other.m_Counter)
{
  if (m_Counter)
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
  Reset();
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
void Ref<T>::Reset(T* ptr)
{
  Reset();
  
  m_Counter = ptr ? new Helper::Counter() : nullptr;
  m_Pointer = ptr;
}

template <typename T>
void Ref<T>::Reset() noexcept
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
  
  m_Pointer = nullptr;
  m_Counter = nullptr;
}

template <typename T>
T* Ref<T>::Raw() noexcept
{
  return m_Pointer;
}

template <typename T>
const T* Ref<T>::Raw() const noexcept
{
  return m_Pointer;
}

// ----- CreateRef ------------

template <typename T, typename... Args>
Ref<T> CreateRef(Args&&... args)
{
  return Ref<T>(new T(Forward<Args>(args)...));
}

// ----- StaticRefCast ------------

template <typename U, typename Y>
Ref<U> StaticRefCast(const Ref<Y>& other)
{
  return Ref<U>(static_cast<U*>(other.m_Pointer), other.m_Counter);
}

} // namespace Neptune

