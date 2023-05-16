#include "neptunepch.h"
#include "MetalRenderDevice.h"

#include "MetalSync.h"
#include "MetalShader.h"
#include "MetalPipelineState.h"
#include "MetalRenderCommand.h"
#include "MetalCommandBuffer.h"
#include "MetalBuffer.h"
#include "MetalTexture.h"
#include "MetalSwapchain.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLCommandQueue.h>

#include <stb_image/stb_image.h>

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
    
    FenceDesc fenceDesc;
    fenceDesc.Signaled = true;
    m_IdleFence = CreateRef<MetalFence>((id<MTLDevice>)m_Device, fenceDesc);
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

Ref<Fence> MetalRenderDevice::CreateFence(const FenceDesc& desc)
{
  return CreateRef<MetalFence>((id<MTLDevice>)m_Device, desc);
}

void MetalRenderDevice::WaitForFence(const Ref<Fence>& fence, Float32 timeoutInMs)
{
  StaticRefCast<MetalFence>(fence)->Wait(timeoutInMs);
}

void MetalRenderDevice::ResetFence(const Ref<Fence>& fence)
{
  StaticRefCast<MetalFence>(fence)->Reset();
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

Ref<Framebuffer> MetalRenderDevice::CreateFramebuffer(const FramebufferDesc &desc)
{
  Ref<Framebuffer> framebuffer = CreateRef<MetalFramebuffer>((id<MTLDevice>)m_Device, desc);
  return framebuffer;
}

Ref<Texture> MetalRenderDevice::CreateTexture(const TextureDesc& desc)
{
  Ref<Texture> texture = CreateRef<MetalTexture>((id<MTLDevice>)m_Device, desc);
  //m_Textures.PushBack(texture); // We probably don't want to cache all of the textures.
  return texture;
}

Ref<Texture> MetalRenderDevice::LoadTexture(const String &path)
{
  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(path.Raw(), &width, &height, &channels, 4);
  NEPTUNE_ASSERT(data, "Failed to load image");
  
  TextureDesc desc;
  {
  	desc.Type = TextureType::Texture2D;
  	desc.Width = (Size)width;
  	desc.Height = (Size)height;
  	desc.Data = (void*)data;
    desc.PixelFormat = PixelFormat::RGBA8Unorm;
    desc.Mipmapped = true;
  }
  
  return CreateTexture(desc);
}

Ref<Mesh> MetalRenderDevice::CreateMesh(const MeshDesc &desc)
{
  BufferDesc vbDesc;
  vbDesc.Size = desc.NumVertices * sizeof(MeshVertex);
  vbDesc.Type = BufferType::Vertex;
  vbDesc.Usage = BufferUsage::Static; // TODO: May change eventually
  vbDesc.Data = desc.Vertices;
  Ref<Buffer> vb = CreateBuffer(vbDesc);
  
  BufferDesc ibDesc;
  ibDesc.Size = desc.NumIndices * sizeof(MeshIndex);
  ibDesc.Type = BufferType::Index;
  ibDesc.Usage = BufferUsage::Static;
  ibDesc.Data = desc.Indices;
  Ref<Buffer> ib = CreateBuffer(ibDesc);
  
  return Ref<Mesh>(new Mesh(vb, ib));
}

Ref<Material> MetalRenderDevice::CreateMaterial(const MaterialDesc& desc)
{
  return Ref<Material>(new Material(desc));
}

void MetalRenderDevice::Submit(CommandBuffer buffer)
{
  if (m_IdleFence->Status())
    m_IdleFence->Reset();
  
  m_Registry->Commit(buffer, m_IdleFence);
}

void MetalRenderDevice::Submit(CommandBuffer buffer, const Ref<Fence>& fence)
{
  if (m_IdleFence->Status())
    m_IdleFence->Reset();
  
  m_Registry->Commit(buffer, fence, m_IdleFence);
}

void MetalRenderDevice::WaitIdle()
{
  m_IdleFence->Wait(1000.0f);
}

} // namespace Neptune
