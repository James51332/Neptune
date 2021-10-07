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
};

} // namespace Neptune
