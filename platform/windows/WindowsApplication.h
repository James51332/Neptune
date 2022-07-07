#pragma once

#include "core/NativeApplication.h"

namespace Neptune
{

// ----- WindowsApplication -----------------

class WindowsApplication final : public NativeApplication
{
public:
  WindowsApplication();
  ~WindowsApplication();

  virtual void PollEvents();
};

} // namespace Neptune