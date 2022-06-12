#include "neptunepch.h"
#include "MetalRenderDevice.h"

#include "MetalShader.h"
#include "MetalPipelineState.h"
#include "MetalRenderCommand.h"
#include "MetalCommandBuffer.h"
#include "MetalBuffer.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLCommandQueue.h>

namespace Neptune
{

MetalRenderDevice::MetalRenderDevice()
{
  @autoreleasepool
  {
  	m_Device = MTLCreateSystemDefaultDevice(); // retained
  	m_Queue = [((id<MTLDevice>)m_Device) newCommandQueue];
  	m_Registry = CreateRef<MetalCommandBufferRegistry>((id<MTLCommandQueue>)m_Queue);
  	m_Encoder = CreateRef<MetalRenderCommandEncoder>((id<MTLDevice>)m_Device, m_Registry);
  }
}

MetalRenderDevice::~MetalRenderDevice()
{
  @autoreleasepool
  {
  	[(id<MTLDevice>)m_Device release]; // retained
  	[(id<MTLCommandQueue>)m_Queue release];
  }
}

Ref<RenderCommandEncoder> MetalRenderDevice::GetEncoder() noexcept
{
  return m_Encoder;
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

Ref<Buffer> MetalRenderDevice::CreateBuffer(const BufferDesc &desc)
{
  Ref<Buffer> buffer = CreateRef<MetalBuffer>((id<MTLDevice>)m_Device, desc);
  m_Buffers.PushBack(buffer);
  return buffer;
}

void MetalRenderDevice::Submit(CommandBuffer buffer)
{
  m_Registry->Commit(buffer);
}

} // namespace Neptune
