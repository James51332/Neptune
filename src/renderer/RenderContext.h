#pragma once

#include "utils/SmartPointer.h"
#include "utils/Types.h"

#include "renderer/RenderAPI.h"

namespace Neptune
{

class RenderContext
{
public:
  static Ref<RenderContext> Create();
  virtual ~RenderContext() noexcept;
  
  RenderAPI GetAPI() const noexcept
  {
    return m_API;
  }
  
protected:
  RenderAPI m_API = RenderAPI::None;
};

} // namespace Neptune
