#pragma once

#include "renderer/Shader.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLLibrary.h>

namespace Neptune
{

// ----- MetalShader -----------------

class MetalShader final : public Shader
{
public:
  MetalShader(id<MTLDevice> device, const ShaderDesc& desc);
  ~MetalShader();
  
  id<MTLFunction> GetVertexFunction() { return m_VertexFunc; }
  id<MTLFunction> GetFragmentFunction() { return m_FragmentFunc; }
  
private:
  id<MTLFunction> m_VertexFunc;
  id<MTLFunction> m_FragmentFunc;
};

} // namespace Neptune
