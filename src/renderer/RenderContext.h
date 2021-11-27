#pragma once

#include "renderer/RenderAPI.h"
#include "renderer/RenderDevice.h"

namespace Neptune
{

class RenderContext
{
public:
  static Ref<RenderContext> Create();
  virtual ~RenderContext() noexcept;
  
  RenderAPI GetAPI() const noexcept { return m_API; }
  Ref<RenderDevice>& GetRenderDevice() noexcept { return m_Device; }
  
protected:
  RenderAPI m_API = RenderAPI::None;
  
  Ref<RenderDevice> m_Device;
};

} // namespace Neptune
