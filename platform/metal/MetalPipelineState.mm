#include "neptunepch.h"
#include "MetalPipelineState.h"

#include "MetalShader.h"

namespace Neptune
{

// ----- MetalPipelineState -----------------

MetalPipelineState::MetalPipelineState(id<MTLDevice> device, const PipelineStateDesc& desc)
{
  MTLRenderPipelineDescriptor *pipelineDesc = [[MTLRenderPipelineDescriptor alloc] init];
  pipelineDesc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
  
  // obtain the functions from the shader.
  Ref<MetalShader> metalShader = StaticRefCast<MetalShader>(desc.Shader);
  pipelineDesc.vertexFunction = metalShader->GetVertexFunction();
  pipelineDesc.fragmentFunction = metalShader->GetFragmentFunction();
  
  NSError* err;
  m_PipelineState = [device newRenderPipelineStateWithDescriptor: pipelineDesc
                                                           error: &err];
  NEPTUNE_ASSERT(m_PipelineState, [err.description UTF8String]);
  
  [pipelineDesc release];
  pipelineDesc = nil;
}

MetalPipelineState::~MetalPipelineState()
{
  [m_PipelineState release];
  m_PipelineState = nil;
}

} // namespace Neptune
