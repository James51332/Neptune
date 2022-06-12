#pragma once
#include "renderer/RenderDevice.h"

namespace Neptune
{

class MetalCommandBufferRegistry;
class MetalRenderCommandEncoder;

class MetalRenderDevice final : public RenderDevice
{
public:
  MetalRenderDevice();
  ~MetalRenderDevice();
  
  void* GetDevice() noexcept { return m_Device; }
  void* GetQueue() noexcept { return m_Queue; }
  Ref<RenderCommandEncoder> GetEncoder() noexcept;
  
  Ref<Shader> CreateShader(const ShaderDesc& desc);
  Ref<PipelineState> CreatePipelineState(const PipelineStateDesc& desc);
  Ref<Buffer> CreateBuffer(const BufferDesc& desc);
  
  void Submit(CommandBuffer buffer);
  
private:
  // For now, we are only using one queue. It may eventually be a good idea to use more.
  void* m_Queue;
  void* m_Device;
  
  Ref<MetalCommandBufferRegistry> m_Registry;
  Ref<MetalRenderCommandEncoder> m_Encoder;
  
  // We'll cache a reference to all resources to that we don't delete them until the device is shutdown.
  DynamicArray<Ref<PipelineState>> m_PipelineStates;
  DynamicArray<Ref<Buffer>> m_Buffers;
};

} // namespace Neptune
