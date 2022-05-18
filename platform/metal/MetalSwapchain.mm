#include "neptunepch.h"
#include "MetalSwapchain.h"

namespace Neptune
{

// ----- MetalFramebuffer -----------------

MetalFramebuffer::MetalFramebuffer(id<MTLDevice> device, CAMetalLayer* layer)
	: m_Layer(layer)
{
}

id<CAMetalDrawable> MetalFramebuffer::GetDrawable()
{
  if (m_Available)
  {
    m_Drawable = [m_Layer nextDrawable];
    m_Available = false;
  }
  
  return m_Drawable;
}

void MetalFramebuffer::Present()
{
  // TODO: Present via commandbuffer is usually better
  [m_Drawable present];
  m_Drawable = nil;
  
  m_Available = true;
}

// ----- MetalSwapchain -----------------

MetalSwapchain::MetalSwapchain(id<MTLDevice> device, CAMetalLayer* layer)
	: m_Layer([layer retain])
{
  constexpr Size numImages = 3;
  for (Size i = 0; i < numImages; i++)
  {
    m_Framebuffers.PushBack(CreateRef<MetalFramebuffer>(device, layer));
  }
}

MetalSwapchain::~MetalSwapchain()
{
  [m_Layer release];
  m_Layer = nil;
}

Ref<Framebuffer> MetalSwapchain::GetNextFramebuffer() noexcept
{
  Size ID = m_Next;
  m_Next = (m_Next + 1) % m_Framebuffers.Size();
  
  // TODO: Wait for image to be avaiable.
  return m_Framebuffers[ID];
}

void MetalSwapchain::Present(const Ref<Framebuffer>& framebuffer)
{
  StaticRefCast<MetalFramebuffer>(framebuffer)->Present();
}

} // namespace Neptune
