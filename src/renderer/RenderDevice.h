#pragma once

#include "Shader.h"
#include "PipelineState.h"

namespace Neptune
{

class RenderDevice
{
public:
  virtual ~RenderDevice();
  
  virtual Ref<Shader> CreateShader(const ShaderDesc& desc) = 0;
  virtual Ref<PipelineState> CreatePipelineState(const PipelineStateDesc& desc) = 0;
};

} // namespace Neptune
