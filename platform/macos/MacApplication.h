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
};

} // namespace Neptune
