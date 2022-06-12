#pragma once

#include "Shader.h"
#include "PipelineState.h"
#include "RenderCommand.h"
#include "Buffer.h"

namespace Neptune
{

class RenderDevice
{
public:
  virtual ~RenderDevice();
  
  virtual Ref<RenderCommandEncoder> GetEncoder() = 0;
  
  virtual Ref<Shader> CreateShader(const ShaderDesc& desc) = 0;
  virtual Ref<PipelineState> CreatePipelineState(const PipelineStateDesc& desc) = 0;
  virtual Ref<Buffer> CreateBuffer(const BufferDesc& desc) = 0;
  
  virtual void Submit(CommandBuffer buffer) = 0;
};

} // namespace Neptune
