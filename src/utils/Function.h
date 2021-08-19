#pragma once

#include <type_traits>

namespace Neptune
{

// ----- Function -------------

template <typename T>
class Function;

template <typename Ret, typename... Args>
class Function<Ret(Args...)>
{
  class CallableBase
  {
  public:
    virtual ~CallableBase() noexcept = default;
    virtual Ret Invoke(Args... args) = 0;
    virtual CallableBase* Clone() = 0;
  };
  
  template <typename T>
  class Callable : public CallableBase
  {
  public:
    using Type = T;
  public:
    Callable(const T& t) noexcept
    : m_Function(t)
    {
    }
    
    virtual ~Callable() noexcept override = default;
    
    virtual Ret Invoke(Args... args) override
    {
      return m_Function(args...);
    }
    
    virtual CallableBase* Clone() override
    {
      return new Callable<T>(m_Function);
    }

  private:
    T m_Function;
  };
  
public:
  using Type = Ret(Args...);
  using ReturnType = Ret;
  
public:
  
  Function() noexcept;
  
  template <typename T>
  Function(T func) noexcept;
  
  Function(const Function& other);
  Function(Function&& other) noexcept;
  
  Function& operator=(Function other) noexcept;
  
  ~Function() noexcept;
  
  Ret operator()(Args... args) const;
  
private:
  CallableBase* m_Callable;
  // TODO: Consider small storage optimizations using a small buffer;
};

template <typename Ret, typename... Args>
Function<Ret(Args...)>::Function() noexcept
	: m_Callable(nullptr)
{
  
}

template <typename Ret, typename... Args>
template <typename T>
Function<Ret(Args...)>::Function(T func) noexcept
	: m_Callable(new Callable<T>(func))
{
}

template <typename Ret, typename... Args>
Function<Ret(Args...)>::Function(const Function<Ret(Args...)>& other)
	: m_Callable(new Callable<typename decltype(other.m_Callable)::Type>(other.m_Callable))
{
}

template <typename Ret, typename... Args>
Function<Ret(Args...)>& Function<Ret(Args...)>::operator=(Function<Ret(Args...)> other) noexcept
{
  Swap(m_Callable, other.m_Callable);
  return (*this);
}

template <typename Ret, typename... Args>
Function<Ret(Args...)>::~Function() noexcept
{
	delete m_Callable;
}

template <typename Ret, typename... Args>
Ret Function<Ret(Args...)>::operator()(Args... args) const
{
	if (m_Callable)
  {
    try
    {
      return m_Callable->Invoke(args...);
    } catch (...)
    {
      throw;
    }
  }
}

} // namespace Neptune
