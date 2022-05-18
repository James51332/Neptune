#pragma once
#include "renderer/Swapchain.h"

#import <QuartzCore/CAMetalLayer.h>
#import <Metal/MTLDevice.h>

namespace Neptune
{

// ----- MetalFramebuffer -----------------

class MetalFramebuffer final : public Framebuffer
{
  friend class MetalSwapchain;
  
public:
  MetalFramebuffer(id<MTLDevice> device, CAMetalLayer* layer);
  ~MetalFramebuffer() = default;
  
  id<CAMetalDrawable> GetDrawable();
  void Present();
  
private:
  CAMetalLayer* m_Layer = nil;
  id<CAMetalDrawable> m_Drawable = nil;
  
  bool m_Available = true;
};

// ----- MetalSwapchain -----------------

class MetalSwapchain final : public Swapchain
{
public:
  MetalSwapchain(id<MTLDevice> device, CAMetalLayer* layer);
  ~MetalSwapchain();
  
  Ref<Framebuffer> GetNextFramebuffer() noexcept;
  void Present(const Ref<Framebuffer>& framebuffer);
  
private:
  CAMetalLayer* m_Layer;
  Size m_Next = 0;
  
  DynamicArray<Ref<MetalFramebuffer>> m_Framebuffers;
};

} // namespace Neptune
