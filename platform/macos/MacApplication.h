#pragma once

#include "core/NativeApplication.h"

namespace Neptune
{

class MacApplication : public NativeApplication
{
public:
  MacApplication();
  ~MacApplication();
  
  virtual void PollEvents() noexcept;
  virtual Float32 PollTime();
  
private:
  void* m_AppDelegate;
  
  Float64 m_InitialTime;
};

} // namespace Neptune
