#include "neptunepch.h"
#include "MetalRenderCommand.h"

#include "MetalSwapchain.h"
#include "MetalPipelineState.h"

namespace Neptune
{

MetalRenderCommandEncoder::MetalRenderCommandEncoder(id<MTLDevice> device, const Ref<MetalCommandBufferRegistry>& registry)
	: m_Device(device), m_Registry(registry), m_ActiveCommandBuffer(nil), m_ActiveRenderEncoder(nil)
{
}

MetalRenderCommandEncoder::~MetalRenderCommandEncoder()
{
  if (m_RenderPass) EndRenderPass();
  if (m_Recording) EndRecording();
}

void MetalRenderCommandEncoder::BeginRecording(CommandBuffer& commandBuffer)
{
  NEPTUNE_ASSERT(!m_Recording, "Finish recording before beginning recording again!");
  
  m_Registry->Register(commandBuffer);
  m_ActiveCommandBuffer = m_Registry->Get(commandBuffer);
  m_Recording = true;
}

void MetalRenderCommandEncoder::EndRecording()
{
  m_ActiveCommandBuffer = nil;
  m_Recording = false;
}

static MTLLoadAction MTLLoadActionFromLoadAction(LoadAction loadAction) noexcept
{
  switch (loadAction)
  {
    case LoadAction::Load: return MTLLoadActionLoad;
    case LoadAction::Clear: return MTLLoadActionClear;
    default: return MTLLoadActionDontCare;
  }
}

static MTLClearColor MTLClearColorFromFloat4(const Float4& vec) noexcept
{
  return MTLClearColorMake(vec.x, vec.y, vec.z, vec.w);
}

void MetalRenderCommandEncoder::BeginRenderPass(const RenderPass& renderPass)
{
  @autoreleasepool
  {
  	MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
  	
  	renderPassDescriptor.colorAttachments[0].texture = StaticRefCast<MetalFramebuffer>(renderPass.Framebuffer)->GetDrawable().texture;
  	renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionFromLoadAction(renderPass.LoadAction);
  	renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorFromFloat4(renderPass.ClearColor);
  	
  	m_ActiveRenderEncoder = [[m_ActiveCommandBuffer renderCommandEncoderWithDescriptor: renderPassDescriptor] retain];
  	m_RenderPass = true;
  }
}

void MetalRenderCommandEncoder::EndRenderPass()
{
  @autoreleasepool
  {
  	if (!m_RenderPass) return;
    
  	[m_ActiveRenderEncoder endEncoding];
    [m_ActiveRenderEncoder release];
  	m_ActiveRenderEncoder = nil;
  	m_RenderPass = false;
  }
}

void MetalRenderCommandEncoder::SetPipelineState(const Ref<PipelineState>& state)
{
  NEPTUNE_ASSERT(m_RenderPass, "Begin RenderPass before setting pipeline state!");
  [m_ActiveRenderEncoder setRenderPipelineState:StaticRefCast<MetalPipelineState>(state)->GetPipelineState()];
}

void MetalRenderCommandEncoder::DrawTriangles(Size start, Size count)
{
  NEPTUNE_ASSERT(m_RenderPass, "Begin RenderPass before drawing");
  [m_ActiveRenderEncoder drawPrimitives: MTLPrimitiveTypeTriangle
                            vertexStart: start
                            vertexCount: count];
}

} // namespace Neptune
