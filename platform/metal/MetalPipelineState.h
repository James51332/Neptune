#pragma once

#include "renderer/PipelineState.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLRenderPipeline.h>

namespace Neptune
{

// ----- MetalPipelineState -----------------

class MetalPipelineState final : public PipelineState
{
public:
  MetalPipelineState(id<MTLDevice> device, const PipelineStateDesc& desc);
  ~MetalPipelineState();
  
  id<MTLRenderPipelineState> GetPipelineState() { return m_PipelineState; }

private:
  id<MTLRenderPipelineState> m_PipelineState;
};

} // namespace Neptune

