#pragma once

#include "Shader.h"

namespace Neptune
{

class RenderDevice
{
public:
  virtual ~RenderDevice();
  
  virtual Ref<Shader> CreateShader(const ShaderDesc& desc) = 0;
};

} // namespace Neptune
