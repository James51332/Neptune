#pragma once

#include "Sync.h"
#include "Shader.h"
#include "PipelineState.h"
#include "RenderCommand.h"
#include "Buffer.h"
#include "Texture.h"

namespace Neptune
{

class RenderDevice
{
public:
  virtual ~RenderDevice() = default;
  
  virtual Ref<RenderCommandEncoder> GetEncoder() = 0;
  
  virtual Ref<Fence> CreateFence(const FenceDesc& desc) = 0;
  virtual void WaitForFence(const Ref<Fence>& fence, Float32 timeoutInMs = 0.0f) = 0;
  virtual void ResetFence(const Ref<Fence>& fence) = 0;
  
  virtual Ref<Shader> CreateShader(const ShaderDesc& desc) = 0;
  virtual Ref<PipelineState> CreatePipelineState(const PipelineStateDesc& desc) = 0;
  virtual Ref<Buffer> CreateBuffer(const BufferDesc& desc) = 0;
  virtual Ref<Framebuffer> CreateFramebuffer(const FramebufferDesc& desc) = 0;
  
  virtual Ref<Texture> CreateTexture(const TextureDesc& desc) = 0;
  virtual Ref<Texture> LoadTexture(const String& path) = 0;
  
  virtual void Submit(CommandBuffer buffer) = 0;
  virtual void Submit(CommandBuffer buffer, const Ref<Fence>& fence) = 0;
  
  virtual void WaitIdle() = 0;
};

} // namespace Neptune
