#include "neptunepch.h"
#include "MetalRenderDevice.h"

#include "MetalShader.h"
#include "MetalPipelineState.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLCommandQueue.h>

namespace Neptune
{

MetalRenderDevice::MetalRenderDevice()
{
  m_Device = MTLCreateSystemDefaultDevice(); // retained
  m_Queue = [((id<MTLDevice>)m_Device) newCommandQueue];
}

MetalRenderDevice::~MetalRenderDevice()
{
  [(id<MTLDevice>)m_Device release]; // retained
  [(id<MTLCommandQueue>)m_Queue release];
}

Ref<Shader> MetalRenderDevice::CreateShader(const ShaderDesc& desc)
{
  return CreateRef<MetalShader>((id<MTLDevice>)m_Device, desc);
}

Ref<PipelineState> MetalRenderDevice::CreatePipelineState(const PipelineStateDesc& desc)
{
  Ref<PipelineState> ps = CreateRef<MetalPipelineState>((id<MTLDevice>)m_Device, desc);
  m_PipelineStates.PushBack(ps);
  return ps;
};


} // namespace Neptune
