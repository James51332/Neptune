#include "neptunepch.h"
#include "MetalRenderDevice.h"

#include "MetalShader.h"
#include "MetalPipelineState.h"
#include "MetalRenderCommand.h"
#include "MetalCommandBuffer.h"
#include "MetalBuffer.h"
#include "MetalTexture.h"

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

void MetalRenderDevice::Submit(CommandBuffer buffer)
{
  m_Registry->Commit(buffer);
}

} // namespace Neptune
