#pragma once

#include "renderer/RenderDevice.h"

namespace Neptune
{

class MetalCommandBufferRegistry;
class MetalRenderCommandEncoder;
class MetalFence;

class MetalRenderDevice final : public RenderDevice
{
public:
  MetalRenderDevice();
  ~MetalRenderDevice();
  
  void* GetDevice() noexcept { return m_Device; }
  void* GetQueue() noexcept { return m_Queue; }
  Ref<RenderCommandEncoder> GetEncoder() noexcept;
  
  Ref<Fence> CreateFence(const FenceDesc& desc);
  void WaitForFence(const Ref<Fence>& fence, Float32 timeoutInMs = 0.0f);
  void ResetFence(const Ref<Fence>& fence);
  
  Ref<Shader> CreateShader(const ShaderDesc& desc);
  Ref<PipelineState> CreatePipelineState(const PipelineStateDesc& desc);
  Ref<Buffer> CreateBuffer(const BufferDesc& desc);
  Ref<Framebuffer> CreateFramebuffer(const FramebufferDesc& desc);
  
  Ref<Texture> CreateTexture(const TextureDesc& desc);
  Ref<Texture> LoadTexture(const String& path);
  
  Ref<Mesh> CreateMesh(const MeshDesc& desc);
  Ref<Material> CreateMaterial(const MaterialDesc& desc);
  Ref<Model> CreateModel(const ModelDesc& desc);
  
  void Submit(CommandBuffer buffer);
  void Submit(CommandBuffer buffer, const Ref<Fence>& fence);
  
  void WaitIdle();
  
private:
  // For now, we are only using one queue. It may eventually be a good idea to use more.
  void* m_Queue;
  void* m_Device;
  
  Ref<MetalCommandBufferRegistry> m_Registry;
  Ref<MetalRenderCommandEncoder> m_Encoder;
  
  // We'll cache a reference to all resources to that we don't delete them until the device is shutdown.
  // We might want a way to explicitly delete a resource though.
  DynamicArray<Ref<PipelineState>> m_PipelineStates;
  DynamicArray<Ref<Buffer>> m_Buffers;
  
  Ref<MetalFence> m_IdleFence;
};

} // namespace Neptune
