#include "neptunepch.h"
#include "MetalRenderCommand.h"

#include "MetalSwapchain.h"
#include "MetalPipelineState.h"
#include "MetalBuffer.h"
#include "MetalTexture.h"

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

static MTLStoreAction MTLStoreActionFromStoreAction(StoreAction action) noexcept
{
  switch (action)
  {
    case StoreAction::Store: return MTLStoreActionStore;
    default: return MTLStoreActionDontCare;
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
    Ref<MetalFramebuffer> framebuffer = StaticRefCast<MetalFramebuffer>(renderPass.Framebuffer);
    
  	MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    
  	renderPassDescriptor.colorAttachments[0].texture = StaticRefCast<MetalTexture>(framebuffer->GetColorAttachment())->GetTexture();
  	renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionFromLoadAction(renderPass.LoadAction);
		renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionFromStoreAction(renderPass.StoreAction);
  	renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorFromFloat4(renderPass.ClearColor);
    
    renderPassDescriptor.depthAttachment.texture = StaticRefCast<MetalTexture>(framebuffer->GetDepthAttachment())->GetTexture();
    renderPassDescriptor.depthAttachment.clearDepth = 1.0f;
    
    // TODO: The user might want control over this at some point.
    renderPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
    renderPassDescriptor.depthAttachment.storeAction = MTLStoreActionDontCare;
    
  	m_ActiveRenderEncoder = [[m_ActiveCommandBuffer renderCommandEncoderWithDescriptor: renderPassDescriptor] retain];
    
    // TODO: Expose API
    [m_ActiveRenderEncoder setCullMode:MTLCullModeNone];
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
  @autoreleasepool
  {
    Ref<MetalPipelineState> ps = StaticRefCast<MetalPipelineState>(state);
    [m_ActiveRenderEncoder setRenderPipelineState: ps->GetPipelineState()];
    [m_ActiveRenderEncoder setDepthStencilState: ps->GetDepthStencilState()];
  }
}

void MetalRenderCommandEncoder::SetVertexBuffer(const Ref<Buffer> &buffer, Size index)
{
  NEPTUNE_ASSERT(m_RenderPass, "Begin RenderPass before recording!");
  
  // Disabled to allow temporary API for binding uniforms
  // NEPTUNE_ASSERT((buffer->GetType() == BufferType::Vertex), "Unable to bind buffer as vertex buffer!");
  @autoreleasepool
  {
    [m_ActiveRenderEncoder setVertexBuffer: StaticRefCast<MetalBuffer>(buffer)->GetBuffer()
                                    offset: StaticRefCast<MetalBuffer>(buffer)->GetOffset()
                                   atIndex: index];
  }
}

void MetalRenderCommandEncoder::SetClipRect(Size x, Size y, Size w, Size h)
{
  @autoreleasepool
  {
    MTLScissorRect scissorRect =
    {
      .x = NSUInteger(x),
      .y = NSUInteger(y),
      .width = NSUInteger(w),
      .height = NSUInteger(h)
    };

    [m_ActiveRenderEncoder setScissorRect: scissorRect];
  }
}

void MetalRenderCommandEncoder::BindTexture(const Ref<Texture>& texture, Size index)
{
  NEPTUNE_ASSERT(m_RenderPass, "Begin RenderPass before binding texture");
  @autoreleasepool
  {
    [m_ActiveRenderEncoder setFragmentTexture: StaticRefCast<MetalTexture>(texture)->GetTexture()
                                      atIndex: index];
  }
}

static MTLPrimitiveType MTLPrimitiveTypeFromPrimitiveType(PrimitiveType type) noexcept
{
  switch (type)
  {
    case PrimitiveType::Triangle: return MTLPrimitiveTypeTriangle;
    case PrimitiveType::TriangleStrip: return MTLPrimitiveTypeTriangleStrip;
    case PrimitiveType::Line: return MTLPrimitiveTypeLine;
    case PrimitiveType::LineStrip: return MTLPrimitiveTypeLineStrip;
    case PrimitiveType::Point: return MTLPrimitiveTypePoint;
    default: return MTLPrimitiveTypeTriangle;
  }
}

static MTLIndexType MTLIndexTypeFromIndexType(IndexType type) noexcept
{
  switch (type)
  {
    case IndexType::UInt16: return MTLIndexTypeUInt16;
    case IndexType::UInt32: return MTLIndexTypeUInt32;
    default: return MTLIndexTypeUInt32;
  }
}

void MetalRenderCommandEncoder::Submit(const DrawCommandDesc &desc)
{
  NEPTUNE_ASSERT(m_RenderPass, "Begin RenderPass before drawing");
  @autoreleasepool
  {
    if (!desc.Indexed)
    {
      [m_ActiveRenderEncoder drawPrimitives: MTLPrimitiveTypeFromPrimitiveType(desc.Type)
                                vertexStart: desc.Offset
                                vertexCount: desc.Count];
    } else
    {
      NEPTUNE_ASSERT(desc.IndexBuffer->GetType() == BufferType::Index, "Unable to use index buffer!");
      
      [m_ActiveRenderEncoder drawIndexedPrimitives: MTLPrimitiveTypeFromPrimitiveType(desc.Type)
                                        indexCount: desc.Count
                                         indexType: MTLIndexTypeFromIndexType(desc.IndexType)
                                       indexBuffer: StaticRefCast<MetalBuffer>(desc.IndexBuffer)->GetBuffer()
                                 indexBufferOffset: StaticRefCast<MetalBuffer>(desc.IndexBuffer)->GetOffset() + desc.Offset];
    }
  }
}

} // namespace Neptune
