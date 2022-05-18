#pragma once

#include "renderer/RenderAPI.h"
#include "renderer/RenderDevice.h"
#include "renderer/Swapchain.h"

namespace Neptune
{

class RenderContext
{
public:
  static Ref<RenderContext> Create();
  virtual ~RenderContext() noexcept;
  
  RenderAPI GetAPI() const noexcept { return m_API; }
  Ref<RenderDevice>& GetRenderDevice() noexcept { return m_Device; }
  Ref<Swapchain>& GetSwapchain() noexcept { return m_Swapchain; }
  
protected:
  RenderAPI m_API = RenderAPI::None;
  
  Ref<RenderDevice> m_Device;
  Ref<Swapchain> m_Swapchain;
};

} // namespace Neptune
