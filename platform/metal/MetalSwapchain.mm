#include "neptunepch.h"
#include "MetalSwapchain.h"

#include "MetalTexture.h"

namespace Neptune
{

// ----- MetalFramebuffer -----------------

MetalFramebuffer::MetalFramebuffer(id<MTLDevice> device, const FramebufferDesc& desc)
	: m_Device(device), m_Width(desc.Width), m_Height(desc.Height), m_InSwapchain(false)
{
  m_Desc = desc;
  CreateColorTexture();
  CreateDepthTexture();
}

MetalFramebuffer::MetalFramebuffer(id<MTLDevice> device, CAMetalLayer* layer, Size width, Size height)
: m_Device(device), m_Width(width), m_Height(height), m_InSwapchain(true)
{
  {
    TextureDesc colorDesc;
    colorDesc.Type = TextureType::Texture2D;
    colorDesc.Mipmapped = false;
    colorDesc.Width = m_Width;
    colorDesc.Height = m_Height;
    colorDesc.PixelFormat = PixelFormat::BGRA8Unorm;
    colorDesc.Data = nullptr;
    colorDesc.RenderTarget = true;
    
    m_ColorTexture = CreateRef<MetalSwapchainTexture>(device, layer, colorDesc);
  }
    
  CreateDepthTexture();
}

MetalFramebuffer::~MetalFramebuffer()
{
}

void MetalFramebuffer::Resize(Size width, Size height)
{
  m_Width = width > m_Width ? width : m_Width;
  m_Height = height > m_Height ? height : m_Height;
  m_Desc.Width = m_Width;
  m_Desc.Height = m_Height;
  
  if (!m_InSwapchain) CreateColorTexture();
  CreateDepthTexture();
}

void MetalFramebuffer::CreateColorTexture()
{
  TextureDesc colorDesc;
  colorDesc.Type = TextureType::Texture2D;
  colorDesc.Mipmapped = false;
  colorDesc.Width = m_Width;
  colorDesc.Height = m_Height;
  colorDesc.PixelFormat = PixelFormat::BGRA8Unorm;
  colorDesc.Data = nullptr;
  colorDesc.RenderTarget = true;
  
  m_ColorTexture = CreateRef<MetalTexture>(m_Device, colorDesc);
}

void MetalFramebuffer::CreateDepthTexture()
{
  TextureDesc depthDesc;
  depthDesc.Type = TextureType::Texture2D;
  depthDesc.Mipmapped = false;
  depthDesc.Width = m_Width;
  depthDesc.Height = m_Height;
  depthDesc.PixelFormat = PixelFormat::Depth32;
  depthDesc.Data = nullptr;
  depthDesc.RenderTarget = true;
  
  m_DepthTexture = CreateRef<MetalTexture>(m_Device, depthDesc);
}

// ----- MetalSwapchain -----------------

MetalSwapchain::MetalSwapchain(id<MTLDevice> device, CAMetalLayer* layer, Size width, Size height)
	: m_Layer([layer retain])
{
  constexpr Size numImages = 3;
  for (Size i = 0; i < numImages; i++)
  {
    // Can't use b/c it's a private constructor
    // m_Framebuffers.PushBack(CreateRef<MetalFramebuffer>(device, layer, width, height));
    m_Framebuffers.PushBack(Ref<MetalFramebuffer>(new MetalFramebuffer(device, layer, width, height)));
  }
}

MetalSwapchain::~MetalSwapchain()
{
  @autoreleasepool
  {
    [m_Layer release];
    m_Layer = nil;
  }
}

void MetalSwapchain::Resize(Size width, Size height)
{
  [m_Layer setDrawableSize: CGSizeMake((CGFloat)width, (CGFloat)height)];
	for (auto& fb : m_Framebuffers)
  {
    fb->Resize(width, height);
  }
}

Ref<Framebuffer> MetalSwapchain::GetNextFramebuffer() noexcept
{
  Size ID = m_Next;
  m_Next = (m_Next + 1) % m_Framebuffers.Size();
  
  // TODO: Wait for image to be avaiable.
  // I don't think it matters much in Metal.
  // Once the work is scheduled, then the framebuffer
  // should be available.
  return m_Framebuffers[ID];
}

void MetalSwapchain::Present(const Ref<Framebuffer>& framebuffer)
{
  StaticRefCast<MetalSwapchainTexture>(framebuffer->GetColorAttachment())->Present();
}

} // namespace Neptune
