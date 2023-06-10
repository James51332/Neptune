#pragma once

#include "utils/SmartPointer.h"
#include "utils/Types.h"

namespace Neptune
{

class NativeApplication
{
public:
  static Scope<NativeApplication> Create();
  virtual ~NativeApplication();
  
  virtual void PollEvents() noexcept = 0;
  
  // Poll the time in seconds
  virtual Float32 PollTime() = 0;
};

} // namespace Neptune
