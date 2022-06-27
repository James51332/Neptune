#pragma once

#include "renderer/PipelineState.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLRenderPipeline.h>
#import <Metal/MTLDepthStencil.h>

namespace Neptune
{

// ----- MetalPipelineState -----------------

class MetalPipelineState final : public PipelineState
{
public:
  MetalPipelineState(id<MTLDevice> device, const PipelineStateDesc& desc);
  ~MetalPipelineState();
  
  id<MTLRenderPipelineState> GetPipelineState() { return m_PipelineState; }
  id<MTLDepthStencilState> GetDepthStencilState() { return m_DepthStencilState; }

private:
  id<MTLRenderPipelineState> m_PipelineState;
  id<MTLDepthStencilState> m_DepthStencilState;
};

} // namespace Neptune

