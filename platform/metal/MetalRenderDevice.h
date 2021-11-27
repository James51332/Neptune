#pragma once
#include "renderer/RenderDevice.h"

namespace Neptune
{

class MetalRenderDevice final : public RenderDevice
{
public:
  MetalRenderDevice();
  ~MetalRenderDevice();
  
  void* GetDevice() noexcept { return m_Device; }
  void* GetQueue() noexcept { return m_Queue; }
  
private:
  void* m_Device;
  
  // For now, we are only using one queue. It may eventually be a good idea to use more.
  void* m_Queue;
};

} // namespace Neptune
