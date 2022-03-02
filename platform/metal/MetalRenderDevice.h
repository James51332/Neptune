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
  
  Ref<Shader> CreateShader(const ShaderDesc& desc);
  
private:
  // For now, we are only using one queue. It may eventually be a good idea to use more.
  void* m_Queue;
  void* m_Device;
  
  // We'll cache a reference to all resources to that we don't delete them until the device is shutdown.
  DynamicArray<Ref<Shader>> m_Shaders;
};

} // namespace Neptune
