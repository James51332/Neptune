#include "neptunepch.h"
#include "MetalSwapchain.h"

#include "MetalTexture.h"

namespace Neptune
{

// ----- MetalFramebuffer -----------------

MetalFramebuffer::MetalFramebuffer(id<MTLDevice> device, CAMetalLayer* layer, Size width, Size height)
	: m_Layer(layer), m_Width(width), m_Height(height)
{
  CreateDepthTexture();
}

MetalFramebuffer::~MetalFramebuffer()
{
  @autoreleasepool
  {
    [m_Drawable release];
    m_Drawable = nil;
  }
}

id<CAMetalDrawable> MetalFramebuffer::GetDrawable()
{
  @autoreleasepool
  {
  	if (m_Available)
  	{
  	  m_Drawable = [[m_Layer nextDrawable] retain];
  	  m_Available = false; // TODO: Thread safety.
  	}
  
  	return m_Drawable;
  }
}

void MetalFramebuffer::Resize(Size width, Size height)
{
  m_Width = width > m_Width ? width : m_Width;
  m_Height = height > m_Height ? height : m_Height;
  
  CreateDepthTexture();
}

void MetalFramebuffer::Present()
{
  @autoreleasepool
  {
  	// TODO: Present via commandbuffer is usually better
  	[m_Drawable present];
  
    [m_Drawable release];
  	m_Drawable = nil;
  	m_Available = true;
  }
}

void MetalFramebuffer::CreateDepthTexture()
{
  @autoreleasepool
  {
    TextureDesc depthDesc;
    depthDesc.Type = TextureType::Texture2D;
    depthDesc.Mipmapped = false;
    depthDesc.Width = m_Width;
    depthDesc.Height = m_Height;
    depthDesc.PixelFormat = PixelFormat::Depth32;
    depthDesc.Data = nullptr;
    depthDesc.RenderTarget = true;
    
    m_DepthTexture = CreateRef<MetalTexture>(m_Layer.device, depthDesc);
  }
}

// ----- MetalSwapchain -----------------

MetalSwapchain::MetalSwapchain(id<MTLDevice> device, CAMetalLayer* layer, Size width, Size height)
	: m_Layer(layer)
{
  constexpr Size numImages = 3;
  for (Size i = 0; i < numImages; i++)
  {
    m_Framebuffers.PushBack(CreateRef<MetalFramebuffer>(device, layer, width, height));
  }
}

MetalSwapchain::~MetalSwapchain()
{
  m_Layer = nil;
}

void MetalSwapchain::Resize(Size width, Size height)
{
  [m_Layer setDrawableSize:CGSizeMake((CGFloat)width, (CGFloat)height)];
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
  StaticRefCast<MetalFramebuffer>(framebuffer)->Present();
}

} // namespace Neptune
